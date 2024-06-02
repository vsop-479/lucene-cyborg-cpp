#include <bit>
#include "SegmentTermsEnum.hpp"
#include "FieldReader.hpp"
#include "SegmentTermsEnumFrame.hpp"
#include "Lucene90BlockTreeTermsReader.hpp"
#include "../../../util/BytesFstEnumerator.hpp"

using lucene::cyborg::codecs::v90::block_tree::BlockTreeTermsStats;
using lucene::cyborg::codecs::v90::block_tree::BlockTreeTermsStatsPtr;
using lucene::cyborg::codecs::v90::block_tree::FieldReader;
using lucene::cyborg::codecs::v90::block_tree::SegmentTermsEnum;
using lucene::cyborg::codecs::v90::block_tree::SegmentTermsEnumFrame;
using lucene::cyborg::index::ImpactsEnum;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::BytesFstEnumerator;
using lucene::cyborg::util::FstArc;
using lucene::cyborg::util::FstBytesOutput;
using lucene::cyborg::util::FstReverseBytes;
using lucene::cyborg::util::FstReverseBytesReader;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MemoryUtils;

void accumulate_output(uint8_t *&buffer,
                       int32_t &buffer_idx,
                       int32_t &len,
                       FstReverseBytes &bytes,
                       LinearAllocator *allocator) {
  if (bytes.len > 0) {
    if ((buffer_idx + bytes.len) > len) {
      // doubling buffer
      const int32_t new_len = std::bit_ceil<uint32_t>(buffer_idx + bytes.len);
      uint8_t *new_buffer = allocator->allocate(new_len);
      MemoryUtils::memcpy(new_buffer, buffer, len);
      buffer = new_buffer;
      len = new_len;
    }  // End if
    FstReverseBytesReader::copy_to_dest((char *) (buffer + buffer_idx), bytes.bytes, bytes.len);
    buffer_idx += bytes.len;
  }  // End if
}

//
// SegmentTermsEnum
//

SegmentTermsEnum::SegmentTermsEnum(ManagedPtr<FieldReader> _field_reader, ManagedPtr<LinearAllocator> _allocator)
    : field_reader(_field_reader),
      reverse_bytes_reader{field_reader->index.get_bytes_reader()},
      allocator(_allocator),
      current_frame(),
      term_state() {
}

std::string_view SegmentTermsEnum::term() {
  // TODO : Impl
  throw 13;
}

bool SegmentTermsEnum::seek_exact(std::string_view target) {
  if (field_reader->size() > 0 && (target < field_reader->get_min() || target > field_reader->get_max())) {
    return false;
  }

  FstArc<FstReverseBytes> arc0;
  FstArc<FstReverseBytes> arc1;
  FstArc<FstReverseBytes> *arc = &arc0;
  FstArc<FstReverseBytes> *next_arc = &arc1;
  field_reader->index.get_first_arc(*arc);
  assert(arc->is_final());
  int32_t buffer_len = 32;
  uint8_t *buffer = allocator->allocate(buffer_len);
  int32_t buffer_idx = 0;
  accumulate_output(buffer, buffer_idx, buffer_len, arc->output, allocator);
  int32_t final_buffer_mark = buffer_idx;
  FstReverseBytes final_outputs = arc->next_final_output;
  int32_t prefix = 0;

  for (int32_t i = 0; i < target.size(); ++i) {
    const int32_t target_label = target[i] & 0xFFU;

    const bool found =
        field_reader->index.find_target_arc(target_label, *arc, *next_arc, reverse_bytes_reader);

    if (!found) {
      // append the last final outputs
      buffer_idx = final_buffer_mark;
      accumulate_output(buffer, buffer_idx, buffer_len, final_outputs, allocator);

      // Index is exhausted
      auto frame =
          allocator->allocate_object<SegmentTermsEnumFrame>(
              buffer,
              buffer_idx,
              allocator,
              DataInput<ReadOnlyDataInput>{field_reader->suffix_trie_input.to_read_only()});

      frame->scan_to_floor_frame(target[prefix]);

      if (!frame->has_terms) {
        return false;
      }

      frame->load_block();
      auto suffix = target.substr(prefix);
      const auto result =
          frame->scan_to_term(suffix, true);
      if (result == SeekStatus::FOUND) {
        current_frame = frame;
        field_reader->postings_reader->format_term_state(field_reader->field_info.get(), term_state);
        return true;
      }

      return false;
    } else {
      accumulate_output(buffer, buffer_idx, buffer_len, next_arc->output, allocator);
      if (next_arc->is_final()) {
        final_outputs = next_arc->next_final_output;
        final_buffer_mark = buffer_idx;
        prefix = i + 1;
      }
      std::swap(arc, next_arc);
    }  // End if
  }  // End for

  // append final outputs
  accumulate_output(buffer, final_buffer_mark, buffer_len, final_outputs, allocator);

  auto frame =
      allocator->allocate_object<SegmentTermsEnumFrame>(
          buffer,
          final_buffer_mark,
          allocator,
          DataInput<ReadOnlyDataInput>{field_reader->suffix_trie_input.to_read_only()});

  if (prefix < target.size()) {
    frame->scan_to_floor_frame(target[prefix]);
  }

  // Target term is entirely contained in the index:
  if (!frame->has_terms) {
    return false;
  }

  frame->load_block();

  const SeekStatus result = frame->scan_to_term(target.substr(prefix), true);
  if (result == SeekStatus::FOUND) {
    current_frame = frame;
    field_reader->postings_reader->format_term_state(field_reader->field_info.get(), term_state);
    return true;
  }

  return false;
}

int32_t SegmentTermsEnum::doc_freq() {
  auto postings_reader = field_reader->postings_reader.get();
  current_frame->decode_meta_data(postings_reader, term_state);
  return term_state.doc_freq;
}

int64_t SegmentTermsEnum::total_term_freq() {
  auto postings_reader = field_reader->postings_reader.get();
  current_frame->decode_meta_data(postings_reader, term_state);
  return term_state.total_term_freq;
}

ManagedPtr<PostingsEnum> SegmentTermsEnum::postings(PostingsEnum::Flags flags) {
  // Must've called seek_exact, and found the term.
  assert(current_frame);

  auto field_info = field_reader->field_info.get();
  auto postings_reader = field_reader->postings_reader.get();
  current_frame->decode_meta_data(postings_reader, term_state);
  return postings_reader->postings(allocator, field_info, term_state, flags);
}

ManagedPtr<ImpactsEnum> SegmentTermsEnum::impacts(PostingsEnum::Flags flags) {
  auto postings_reader = field_reader->postings_reader.get();
  auto field_info = field_reader->field_info.get();
  current_frame->decode_meta_data(postings_reader, term_state);
  return postings_reader->impacts(allocator, field_info, term_state, flags);
}

BlockTreeTermsStatsPtr SegmentTermsEnum::compute_block_stats() {
  BlockTreeTermsStatsPtr stats = std::make_unique<BlockTreeTermsStats>(
      field_reader->parent->segment, field_reader->field_info->name);

  stats->index_num_bytes = field_reader->index.fst_in.to_read_only().length;

  FstArc<FstReverseBytes> arc;
  field_reader->index.get_first_arc(arc);
  assert(arc.is_final());

  DataInput<ReadOnlyDataInput> terms_in{field_reader->suffix_trie_input.to_read_only()};
  int32_t buffer_len = 16;
  uint8_t *buffer = allocator->allocate(buffer_len);
  int32_t buffer_idx = 0;
  accumulate_output(buffer, buffer_idx, buffer_len, arc.output, allocator);
  accumulate_output(buffer, buffer_idx, buffer_len, arc.next_final_output, allocator);

  auto current_frame =
      allocator->allocate_object<SegmentTermsEnumFrame>(
          buffer,
          buffer_idx,
          allocator,
          terms_in);
  current_frame->load_block();

  stats->start_block(current_frame, !current_frame->is_last_in_floor);

  int32_t frames_to_visit_size = 16;
  auto frames_to_visit = allocator->allocate_objects<SegmentTermsEnumFrame *>(
      frames_to_visit_size);
  int32_t idx_of_frame_to_visit = -1;

  int32_t frames_to_reuse_size = 16;
  auto frames_to_reuse = allocator->allocate_objects<SegmentTermsEnumFrame *>(
      frames_to_reuse_size);
  int32_t idx_of_frame_to_reuse = -1;

  while (true) {
    while (current_frame->next_ent == current_frame->num_entries) {
      stats->end_block(current_frame);
      if (!current_frame->is_last_in_floor) {
        // Advance to next floor block
        current_frame->load_next_floor_block();
        stats->start_block(current_frame, true);
        break;
      } else {
        if (idx_of_frame_to_visit < 0) {
          stats->finish();
          return stats;
        }

        // Save current frame to the pool
        if (++idx_of_frame_to_reuse >= frames_to_reuse_size) {
          // Doubling the array
          const int32_t new_frames_to_reuse_size = frames_to_reuse_size * 2;
          auto new_frames_to_reuse = allocator->allocate_objects<
              SegmentTermsEnumFrame *>(new_frames_to_reuse_size);
          for (int32_t i = 0; i < frames_to_reuse_size; ++i) {
            new_frames_to_reuse[i] = frames_to_reuse[i];
          }
          frames_to_reuse_size = new_frames_to_reuse_size;
          frames_to_reuse = new_frames_to_reuse;
        }  // End if

        frames_to_reuse[idx_of_frame_to_reuse] = current_frame;

        // Pop the previous frame before the current
        current_frame = frames_to_visit[idx_of_frame_to_visit--];
      }  // End if
    }  // End while

    while (current_frame->next_entry()) {
      // It's a sub block entry

      // Push current frame to the stack
      if (++idx_of_frame_to_visit >= frames_to_visit_size) {
        // Doubling the array
        const int32_t new_frames_to_visit_size = frames_to_visit_size * 2;
        auto new_frames_to_visit = allocator->allocate_objects<
            SegmentTermsEnumFrame *>(new_frames_to_visit_size);
        for (int32_t i = 0; i < frames_to_visit_size; ++i) {
          new_frames_to_visit[i] = frames_to_visit[i];
        }
        frames_to_visit_size = new_frames_to_visit_size;
        frames_to_visit = new_frames_to_visit;
      }  // End if

      frames_to_visit[idx_of_frame_to_visit] = current_frame;

      // Try reuse previously instantiated frame object
      if (idx_of_frame_to_reuse >= 0) {
        frames_to_reuse[idx_of_frame_to_reuse]->reset_for_traversal(current_frame->sub_block_fp);
        current_frame = frames_to_reuse[idx_of_frame_to_reuse--];
      } else {
        current_frame =
            allocator->allocate_object<SegmentTermsEnumFrame>(
                current_frame->sub_block_fp,
                allocator,
                terms_in);
      }  // End if

      // This is a "next" frame -- even if it's
      // floor'd we must pretend it isn't so we don't
      // try to scan to the right floor frame:
      current_frame->load_block();
      stats->start_block(current_frame, !current_frame->is_last_in_floor);
    }  // End while

    // It's a term entry
    stats->term(current_frame->suffix);
  }  // End while
}

ManagedPtr<SegmentTermsEnumFrame> SegmentTermsEnum::compute_the_first_block(
    ManagedPtr<LinearAllocator> allocator) {
  FstArc<FstReverseBytes> arc;
  field_reader->index.get_first_arc(arc);
  assert(arc.is_final());

  DataInput<ReadOnlyDataInput> terms_in{field_reader->suffix_trie_input.to_read_only()};
  int32_t buffer_len = 16;
  uint8_t *buffer = allocator->allocate(buffer_len);
  int32_t buffer_idx = 0;
  accumulate_output(buffer, buffer_idx, buffer_len, arc.output, allocator);
  accumulate_output(buffer, buffer_idx, buffer_len, arc.next_final_output, allocator);

  auto current_frame =
      allocator->allocate_object<SegmentTermsEnumFrame>(
          buffer,
          buffer_idx,
          allocator,
          terms_in);
  current_frame->load_block();

  int32_t frames_to_visit_size = 16;
  auto frames_to_visit = allocator->allocate_objects<SegmentTermsEnumFrame *>(
      frames_to_visit_size);
  int32_t idx_of_frame_to_visit = -1;

  while (true) {
    while (current_frame->next_ent == current_frame->num_entries) {
      if (!current_frame->is_last_in_floor) {
        // Advance to next floor block
        current_frame->load_next_floor_block();
        break;
      } else {
        if (idx_of_frame_to_visit < 0) {
          // WTF! No leaf block??
          throw 13;
        }
      }  // End if
    }  // End while

    while (current_frame->next_entry()) {
      // It's a sub block entry

      // Push current frame to the stack
      if (++idx_of_frame_to_visit >= frames_to_visit_size) {
        // Doubling the array
        const int32_t new_frames_to_visit_size = frames_to_visit_size * 2;
        auto new_frames_to_visit = allocator->allocate_objects<
            SegmentTermsEnumFrame *>(new_frames_to_visit_size);
        for (int32_t i = 0; i < frames_to_visit_size; ++i) {
          new_frames_to_visit[i] = frames_to_visit[i];
        }
        frames_to_visit_size = new_frames_to_visit_size;
        frames_to_visit = new_frames_to_visit;
      }  // End if

      frames_to_visit[idx_of_frame_to_visit] = current_frame;

      current_frame =
          allocator->allocate_object<SegmentTermsEnumFrame>(
              current_frame->sub_block_fp,
              allocator,
              terms_in);

      // This is a "next" frame -- even if it's
      // floor'd we must pretend it isn't so we don't
      // try to scan to the right floor frame:
      current_frame->load_block();
    }  // End while

    return current_frame;
  }  // End while
}

//
// BlockTreeTermsStats
//
BlockTreeTermsStats::BlockTreeTermsStats(std::string _segment,
                                         std::string _field)
    : index_num_bytes(),
      total_term_count(),
      total_term_bytes(),
      non_floor_block_count(),
      floor_block_count(),
      floor_sub_block_count(),
      mixed_block_count(),
      terms_only_block_count(),
      sub_blocks_only_block_count(),
      total_block_count(),
      block_count_by_prefix_len(),
      total_block_suffix_bytes(),
      compression_algorithms{0},
      total_uncompressed_block_suffix_bytes(),
      total_block_stats_bytes(),
      total_block_other_bytes(),
      segment(std::move(_segment)),
      field(std::move(_field)),
      start_block_count(),
      end_block_count(),
      cached_to_string() {
}

std::string BlockTreeTermsStats::to_string() {
  return "";
}

void BlockTreeTermsStats::start_block(SegmentTermsEnumFrame *frame,
                                      bool is_floor) {
  ++total_block_count;
  if (is_floor) {
    if (frame->block_fp == frame->org_block_fp) {
      ++floor_block_count;
    }
    ++floor_sub_block_count;
  } else {
    ++non_floor_block_count;
  }

//  if (blockCountByPrefixLen.length <= frame.prefix) {
//    blockCountByPrefixLen = ArrayUtil.grow(blockCountByPrefixLen, 1 + frame.prefix);
//  }
//  blockCountByPrefixLen[frame.prefix]++;
//  startBlockCount++;
//  totalBlockSuffixBytes += frame.totalSuffixBytes;
//  totalUncompressedBlockSuffixBytes += frame.suffixesReader.length();
//  if (frame.suffixesReader != frame.suffixLengthsReader) {
//    totalUncompressedBlockSuffixBytes += frame.suffixLengthsReader.length();
//  }
//  totalBlockStatsBytes += frame.statsReader.length();
//  compressionAlgorithms[frame.compressionAlg.code]++;
}

void BlockTreeTermsStats::end_block(SegmentTermsEnumFrame *frame) {

}

void BlockTreeTermsStats::term(std::string_view term) {

}

void BlockTreeTermsStats::finish() {

}
