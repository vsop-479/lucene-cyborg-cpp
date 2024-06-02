#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_SEGMENTTERMSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_SEGMENTTERMSENUM_HPP_

#include <vector>
#include "CompressionAlgorithm.hpp"
#include "../../../index/BaseTermsEnum.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../../index/TermsStats.hpp"
#include "../../../util/LinearAllocator.hpp"
#include "../../../util/FstReverseBytesReader.hpp"
#include "../../../util/FstBytesOutput.hpp"
#include "../Lucene90TermState.hpp"

namespace lucene::cyborg::codecs::v90::block_tree {

class SegmentTermsEnumFrame;

struct BlockTreeTermsStats : public index::TermsStats {
  BlockTreeTermsStats(std::string segment, std::string field);

  std::string to_string() final;

  void start_block(SegmentTermsEnumFrame *frame, bool is_floor);

  void end_block(SegmentTermsEnumFrame *frame);

  void term(std::string_view term);

  void finish();

  /** Byte size of the index. */
  int64_t index_num_bytes;

  /** Total number of terms in the field. */
  int64_t total_term_count;

  /** Total number of bytes (sum of term lengths) across all terms in the field. */
  int64_t total_term_bytes;

  /** The number of normal (non-floor) blocks in the terms file. */
  int32_t non_floor_block_count;

  /**
   * The number of floor blocks (meta-blocks larger than the allowed {@code maxItemsPerBlock}) in
   * the terms file.
   */
  int32_t floor_block_count;

  /** The number of sub-blocks within the floor blocks. */
  int32_t floor_sub_block_count;

  /** The number of "internal" blocks (that have both terms and sub-blocks). */
  int32_t mixed_block_count;

  /** The number of "leaf" blocks (blocks that have only terms). */
  int32_t terms_only_block_count;

  /** The number of "internal" blocks that do not contain terms (have only sub-blocks). */
  int32_t sub_blocks_only_block_count;

  /** Total number of blocks. */
  int32_t total_block_count;

  /** Number of blocks at each prefix depth. */
  std::vector<int32_t> block_count_by_prefix_len;

  /** Total number of bytes used to store term suffixes. */
  int64_t total_block_suffix_bytes;

  /**
   * Number of times each compression method has been used. 0 = uncompressed 1 = lowercase_ascii 2 =
   * LZ4
   */
  int64_t compression_algorithms[NumSupportedCompressionAlgorithms];

  /** Total number of suffix bytes before compression. */
  int64_t total_uncompressed_block_suffix_bytes;

  /**
   * Total number of bytes used to store term stats (not including what the {@link
   * PostingsReaderBase} stores.
   */
  int64_t total_block_stats_bytes;

  /**
   * Total bytes stored by the {@link PostingsReaderBase}, plus the other few vInts stored in the
   * frame.
   */
  int64_t total_block_other_bytes;

  /** Segment name. */
  std::string segment;

  /** Field name. */
  std::string field;

 private:
  int32_t start_block_count;
  int32_t end_block_count;
  std::string cached_to_string;
};  // BlockTreeTermsStats

using BlockTreeTermsStatsPtr = std::unique_ptr<BlockTreeTermsStats>;

class FieldReader;

class SegmentTermsEnum final : public index::BaseTermsEnum {
 public:
  SegmentTermsEnum(util::ManagedPtr<FieldReader> field_reader, util::ManagedPtr<util::LinearAllocator> allocator);

  std::string_view term() final;

  bool seek_exact(std::string_view target) final;

  int32_t doc_freq() final;

  int64_t total_term_freq() final;

  util::ManagedPtr<index::PostingsEnum> postings(index::PostingsEnum::Flags flags) final;

  util::ManagedPtr<index::ImpactsEnum> impacts(index::PostingsEnum::Flags flags) final;

  BlockTreeTermsStatsPtr compute_block_stats();

  util::ManagedPtr<SegmentTermsEnumFrame> compute_the_first_block(util::ManagedPtr<util::LinearAllocator> allocator);

 private:
  util::ManagedPtr<FieldReader> field_reader;
  util::FstReverseBytesReader reverse_bytes_reader;
  util::ManagedPtr<util::LinearAllocator> allocator;
  util::ManagedPtr<SegmentTermsEnumFrame> current_frame;
  Lucene90TermState term_state;
};  // SegmentTermsEnum



}  // lucene::cyborg::codecs::v90::block_tree

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_SEGMENTTERMSENUM_HPP_