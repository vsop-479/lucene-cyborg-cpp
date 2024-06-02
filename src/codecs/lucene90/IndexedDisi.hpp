#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_INDEXEDDISI_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_INDEXEDDISI_HPP_

#include "../../store/DataInput.hpp"
#include "../../store/ReadOnlyDataInput.hpp"
#include "../../index/FieldInfo.hpp"
#include "Lucene90NormsEntry.hpp"
#include "../../store/CloseableMemoryDataInput.hpp"
#include "../../search/DocIdSetIterator.hpp"
#include "../../util/LinearAllocator.hpp"

namespace lucene::cyborg::codecs::v90 {

struct IndexedDISI final : public search::DocIdSetIterator {
  static store::DataInput<store::ReadOnlyDataInput> create_block_slice(
      store::CloseableMemoryDataInput &data,
      const Lucene90NormsEntry &entry);

  static store::DataInput<store::ReadOnlyDataInput> create_jump_table(
      store::CloseableMemoryDataInput &data,
      const Lucene90NormsEntry &entry);

  IndexedDISI(util::ManagedPtr<util::LinearAllocator> allocator,
              const store::DataInput<store::ReadOnlyDataInput> &block_slice,
              const store::DataInput<store::ReadOnlyDataInput> &jump_table,
              const Lucene90NormsEntry &entry);

  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final;

  int32_t next_doc() final;

  void advance_block(int32_t target_block);

  void read_block_header();

  bool advance_exact(int32_t target);

  int32_t block_end;
  int32_t dense_bitmap_offset;
  int32_t next_block_index;
  int32_t index;
  int32_t gap;
  int32_t doc;
  int32_t block;
  bool (*advance_within_block)(IndexedDISI *, int32_t);
  bool (*advance_exact_within_block)(IndexedDISI *, int32_t);
  bool exists;
  int32_t next_exist_doc_in_block;
  uint64_t word;
  int32_t word_index;
  int32_t number_of_ones;
  int32_t dense_origo_index;
  util::TypedVector<char> dense_rank_table;

  store::DataInput<store::ReadOnlyDataInput> slice;
  int32_t jump_table_entry_count;
  uint8_t dense_rank_power;
  store::DataInput<store::ReadOnlyDataInput> jump_table;
  int64_t cost;
};  // IndexedDISI



}  // namespace lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_INDEXEDDISI_HPP_
