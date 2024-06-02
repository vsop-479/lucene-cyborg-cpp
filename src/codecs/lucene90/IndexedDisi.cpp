#include <bit>
#include "IndexedDisi.hpp"

using lucene::cyborg::codecs::v90::IndexedDISI;
using lucene::cyborg::codecs::v90::Lucene90NormsEntry;
using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

void rank_skip(IndexedDISI *disi, int32_t target_in_block) {
  assert(disi->dense_rank_power >= 0);
  // Resolve the rank as close to target_in_block as possible (maximum distance is 8 longs)
  // Note: rank_origo_offset is tracked on block open, so it is absolute (e.g. don't add origo)
  const int32_t rank_index =
      target_in_block >> disi->dense_rank_power; // Default is 9 (8 longs: 2^3 * 2^6 = 512 docIDs)

  const int32_t rank =
      (disi->dense_rank_table.vec[rank_index << 1] & 0xFF) << 8
          | (disi->dense_rank_table.vec[(rank_index << 1) + 1] & 0xFF);

  // Position the counting logic just after the rank point
  int32_t rank_aligned_word_index = rank_index << disi->dense_rank_power >> 6;
  disi->slice.seek(disi->dense_bitmap_offset + rank_aligned_word_index * (int64_t) sizeof(int64_t));
  uint64_t rank_word = disi->slice.read_long();
  int32_t dense_noo = rank + std::popcount(rank_word);

  disi->word_index = rank_aligned_word_index;
  disi->word = rank_word;
  disi->number_of_ones = disi->dense_origo_index + dense_noo;
}

bool sparse_advance_within_block(IndexedDISI *disi, int32_t target) {
  const uint32_t target_in_block = target & 0xFFFFU;
  // TODO: binary search
  while (disi->index < disi->next_block_index) {
    const uint32_t doc = disi->slice.read_short();
    disi->index++;
    if (doc >= target_in_block) {
      disi->doc = disi->block | doc;
      disi->exists = true;
      disi->next_exist_doc_in_block = doc;
      return true;
    }
  }  // End while

  return false;
}

bool sparse_advance_exact_within_block(IndexedDISI *disi, int32_t target) {
  const uint32_t target_in_block = target & 0xFFFFU;
  // TODO: binary search
  if (disi->next_exist_doc_in_block > target_in_block) {
    assert(!disi->exists);
    return false;
  }
  if (target == disi->doc) {
    return disi->exists;
  }
  while (disi->index < disi->next_block_index) {
    const uint32_t doc = disi->slice.read_short();
    disi->index++;
    if (doc >= target_in_block) {
      disi->next_exist_doc_in_block = doc;
      if (doc != target_in_block) {
        disi->index--;
        disi->slice.seek(disi->slice.get_file_pointer() - sizeof(int16_t));
        break;
      }
      return disi->exists = true;
    }  // End if
  }  // End while

  return disi->exists = false;
}

bool dense_advance_within_block(IndexedDISI *disi, int32_t target) {
  const uint32_t target_in_block = target & 0xFFFFU;
  const int32_t target_word_index = target_in_block >> 6U;

  // If possible, skip ahead using the rank cache
  // If the distance between the current position and the target is < rank-longs
  // there is no sense in using rank
  if (disi->dense_rank_power != 127
      && target_word_index - disi->word_index >= (1 << (disi->dense_rank_power - 6))) {
    rank_skip(disi, target_in_block);
  }

  for (int32_t i = disi->word_index + 1; i <= target_word_index; ++i) {
    disi->word = disi->slice.read_long();
    disi->number_of_ones += std::popcount(disi->word);
  }
  disi->word_index = target_word_index;

  uint64_t left_bits = disi->word >> target;
  if (left_bits != 0) {
    disi->doc = target + std::countr_zero(left_bits);
    disi->index = disi->number_of_ones - std::popcount(left_bits);
    return true;
  }

  // There were no set bits at the wanted position. Move forward until one is reached
  while (++disi->word_index < 1024) {
    // This could use the rank cache to skip empty spaces >= 512 bits, but it seems
    // unrealistic
    // that such blocks would be DENSE
    disi->word = disi->slice.read_long();
    if (disi->word != 0) {
      disi->index = disi->number_of_ones;
      disi->number_of_ones += std::popcount(disi->word);
      disi->doc = disi->block | (disi->word_index << 6) | std::countr_zero(disi->word);
      return true;
    }
  }

  // No set bits in the block at or after the wanted position.
  return false;
}

bool dense_advance_exact_within_block(IndexedDISI *disi, int32_t target) {
  const uint32_t target_in_block = ((uint32_t) target) & 0xFFFFU;
  const int32_t target_word_index = target_in_block >> 6U;

  // If possible, skip ahead using the rank cache
  // If the distance between the current position and the target is < rank-longs
  // there is no sense in using rank
  if (disi->dense_rank_power != 127
      && target_word_index - disi->word_index >= (1 << (disi->dense_rank_power - 6))) {
    rank_skip(disi, target_in_block);
  }

  for (int32_t i = disi->word_index + 1; i <= target_word_index; ++i) {
    disi->word = disi->slice.read_long();
    disi->number_of_ones += std::popcount(disi->word);
  }
  disi->word_index = target_word_index;

  const uint64_t left_bits = disi->word >> target;
  disi->index = disi->number_of_ones - std::popcount(left_bits);
  return (left_bits & 1ULL) != 0;
}

bool all_advance_within_block(IndexedDISI *disi, int32_t target) noexcept {
  disi->doc = target;
  disi->index = target - disi->gap;
  return true;
}

bool all_advance_exact_within_block(IndexedDISI *disi, int32_t target) noexcept {
  disi->index = target - disi->gap;
  return true;
}



//
// IndexedDISI
//

static constexpr int32_t DENSE_BLOCK_LONGS = 1024;
static constexpr int32_t MAX_ARRAY_LENGTH = (1 << 12) - 1;
static constexpr int32_t BLOCK_SIZE = 65536;

IndexedDISI::IndexedDISI(ManagedPtr<LinearAllocator> _allocator,
                         const DataInput<ReadOnlyDataInput> &_block_slice,
                         const DataInput<ReadOnlyDataInput> &_jump_table,
                         const Lucene90NormsEntry &_entry)
    : DocIdSetIterator(),
      block_end(),
      dense_bitmap_offset(-1),
      next_block_index(-1),
      index(-1),
      gap(),
      doc(-1),
      block(-1),
      advance_within_block(),
      advance_exact_within_block(),
      exists(),
      next_exist_doc_in_block(-1),
      word(),
      word_index(-1),
      number_of_ones(),
      dense_origo_index(),
      dense_rank_table(),
      slice(_block_slice),
      jump_table_entry_count(_entry.jump_table_entry_count),
      dense_rank_power(_entry.dense_rank_power),
      jump_table(_jump_table),
      cost(_entry.num_docs_with_field) {
  if ((dense_rank_power < 7 || dense_rank_power > 15) && dense_rank_power != 127) {
    // TODO
    throw 13;
    // throw new IllegalArgumentException(
    //     "Acceptable values for denseRankPower are 7-15 (every 128-32768 docIDs). "
    //         + "The provided power was "
    //         + denseRankPower
    //         + " (every "
    //         + (int) Math.pow(2, denseRankPower)
    //         + " docIDs). ");
  }  // End if

  const auto rank_index_shift = dense_rank_power - 7;
  if (dense_rank_power != 127) {
    dense_rank_table.len = DENSE_BLOCK_LONGS >> rank_index_shift;
    dense_rank_table.vec = _allocator->allocate_object<char>(dense_rank_table.len);
  }
}

DataInput<ReadOnlyDataInput> IndexedDISI::create_block_slice(
    CloseableMemoryDataInput &data,
    const Lucene90NormsEntry &entry) {
  const int64_t jump_table_bytes =
      entry.jump_table_entry_count < 0 ? 0 : entry.jump_table_entry_count * (sizeof(int32_t) * 2);
  return DataInput<ReadOnlyDataInput>{
      data.slice(entry.docs_with_field_offset, entry.docs_with_field_length - jump_table_bytes)
  };
}

DataInput<ReadOnlyDataInput> IndexedDISI::create_jump_table(
    CloseableMemoryDataInput &data,
    const Lucene90NormsEntry &entry) {
  if (entry.jump_table_entry_count <= 0) {
    return {};
  } else {
    const int32_t jump_table_bytes = entry.jump_table_entry_count * (sizeof(int32_t) * 2);
    return DataInput<ReadOnlyDataInput>{
        data.slice(entry.docs_with_field_offset + entry.docs_with_field_length - jump_table_bytes,
                   jump_table_bytes)
    };
  }
}

int64_t IndexedDISI::get_cost() {
  return doc;
}

int32_t IndexedDISI::advance(int32_t target) {
  const uint32_t target_block = ((uint32_t) target) & 0xFFFF0000U;
  if (block < target_block) {
    advance_block(target_block);
  }
  if (block == target_block) {
    if (advance_within_block(this, target)) {
      return doc;
    }
    read_block_header();
  }
  const bool found = advance_within_block(this, block);
  assert(found);
  return doc;
}

int32_t IndexedDISI::get_doc_id() {
  return doc;
}

int32_t IndexedDISI::next_doc() {
  return advance(doc + 1);
}

void IndexedDISI::advance_block(int32_t target_block) {
  const uint32_t block_index = target_block >> 16U;
  // If the destination block is 2 blocks or more ahead, we use the jump-table.
  if (jump_table.input.data && block_index >= (block >> 16) + 2) {
    // If the jump_table_entry_count is exceeded, there are no further bits.
    // Last entry is always NO_MORE_DOCS
    const int32_t in_range_block_index =
        block_index < jump_table_entry_count ? block_index : jump_table_entry_count - 1;
    jump_table.seek(in_range_block_index * ((int64_t) (sizeof(int32_t) * 2)));
    const int32_t index = jump_table.read_int();
    jump_table.seek(in_range_block_index * ((int64_t) sizeof(int32_t) * 2 + sizeof(int32_t)));
    const int32_t offset = jump_table.read_int();
    next_block_index = index - 1; // -1 to compensate for the always-added 1 in read_block_header
    slice.seek(offset);
    read_block_header();
    return;
  }  // End if

  // Fallback to iteration of blocks
  do {
    slice.seek(block_end);
    read_block_header();
  } while (block < target_block);
}

void IndexedDISI::read_block_header() {
  block = slice.read_short() << 16U;
  assert(block >= 0);
  // cardinality
  const int32_t num_values = 1 + slice.read_short();
  index = next_block_index;
  next_block_index = index + num_values;
  if (num_values <= MAX_ARRAY_LENGTH) {
    advance_within_block = sparse_advance_within_block;
    advance_exact_within_block = sparse_advance_exact_within_block;
    block_end = slice.get_file_pointer() + (num_values << 1);
    next_exist_doc_in_block = -1;
  } else if (num_values == BLOCK_SIZE) {
    advance_within_block = all_advance_within_block;
    advance_exact_within_block = all_advance_exact_within_block;
    block_end = slice.get_file_pointer();
    gap = block - index - 1;
  } else {
    advance_within_block = dense_advance_within_block;
    advance_exact_within_block = dense_advance_exact_within_block;
    dense_bitmap_offset = slice.get_file_pointer() + dense_rank_table.len;
    block_end = dense_bitmap_offset + (1 << 13);
    // Performance consideration: All rank (default 128 * 16 bits) are loaded up front. This
    // should be fast with the reusable byte[] buffer, but it is still wasted if the DENSE block is iterated in small
    // steps.
    // If this results in too great a performance regression, a heuristic strategy might work
    // where the rank data are loaded on first in-block advance, if said advance is > X docIDs. The hope being that a
    // small first advance means that subsequent advances will be small too.
    // Another alternative is to maintain an extra slice for DENSE rank, but IndexedDISI is already slice-heavy.
    if (dense_rank_power != 127) {
      slice.read_bytes(dense_rank_table.vec, dense_rank_table.len);
    }
    word_index = -1;
    number_of_ones = index + 1;
    dense_origo_index = number_of_ones;
  }  // End if
}

bool IndexedDISI::advance_exact(int32_t target) {
  const uint32_t target_block = target & 0xFFFF0000U;
  if (block < target_block) {
    advance_block(target_block);
  }
  const bool found = (block == target_block && advance_exact_within_block(this, target));
  this->doc = target;
  return found;
}
