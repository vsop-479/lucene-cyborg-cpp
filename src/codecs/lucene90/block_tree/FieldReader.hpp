#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_FIELDREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_FIELDREADER_HPP_

#include <memory>
#include "../../../index/Terms.hpp"
#include "../../../index/FieldInfo.hpp"
#include "SegmentTermsEnum.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../../store/DataInput.hpp"
#include "../../../store/BufferedChecksumMemoryDataInput.hpp"
#include "../../../util/Fst.hpp"
#include "../../../util/FstBytesOutput.hpp"
#include "../Lucene90PostingsReader.hpp"
#include "../DocPosPayFilePointers.hpp"

namespace lucene::cyborg::codecs::v90::block_tree {

class Lucene90BlockTreeTermsReader;

class FieldReader final : public index::Terms {
 public:
  FieldReader(util::ReadOnlyPtr<Lucene90BlockTreeTermsReader> _parent,
              index::FieldInfoPtr _field_info,
              int64_t _num_terms,
              std::string _root_code,
              int64_t _sum_total_term_freq,
              int64_t _sum_doc_freq,
              int32_t _doc_count,
              store::DataInput<store::BufferedChecksumMemoryDataInput> &_meta_in,
              store::CloseableMemoryDataInput _index_input,
              store::CloseableMemoryDataInput _suffix_trie_input,
              const std::string &_min_term,
              const std::string &_max_term);

  util::ManagedPtr<SegmentTermsEnum> iterator(util::UseOnlyPtr<util::LinearAllocator> allocator) final;

  int32_t size() final;

  int64_t get_sum_total_term_freq() final;

  int64_t get_sum_doc_freq() final;

  int32_t get_doc_count() final;

  bool has_freq() final;

  bool has_offset() final;

  bool has_positions() final;

  bool has_payloads() final;

  std::string_view get_min() final;

  std::string_view get_max() final;

  index::TermsStatsPtr get_stats(util::UseOnlyPtr<util::LinearAllocator> allocator) final;

  DocPosPayFilePointers set_posting_reader(
      const DocPosPayFilePointers &end_file_fps,
      util::ManagedPtr<Lucene90PostingsReader> parent_postings_reader,
      util::ManagedPtr<store::MMapDirectory> directory);

  util::Fst<1, util::FstReverseBytes, util::FstBytesOutput> index;
  util::ReadOnlyPtr<Lucene90BlockTreeTermsReader> parent;
  index::FieldInfoPtr field_info;
  int64_t num_terms;
  std::string root_code;
  int64_t sum_total_term_freq;
  int64_t sum_doc_freq;
  int32_t doc_count;
  store::CloseableMemoryDataInput suffix_trie_input;
  std::string min_term;
  std::string max_term;
  int64_t root_block_fp;
  Lucene90PostingsReaderPtr postings_reader;
};  // FieldReader

using FieldReaderPtr = std::shared_ptr<FieldReader>;

}  // lucene::cyborg::codecs::v90::block_tree

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_BLOCK_TREE_FIELDREADER_HPP_
