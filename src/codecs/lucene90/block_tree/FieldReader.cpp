#include "FieldReader.hpp"
#include "BlockTreeConstants.hpp"
#include "SegmentTermsEnumFrame.hpp"

using lucene::cyborg::codecs::v90::DocPosPayFilePointers;
using lucene::cyborg::codecs::v90::block_tree::FieldReader;
using lucene::cyborg::codecs::v90::block_tree::Lucene90BlockTreeTermsReader;
using lucene::cyborg::codecs::v90::block_tree::SegmentTermsEnum;
using lucene::cyborg::codecs::v90::block_tree::SegmentTermsEnumFrame;
using lucene::cyborg::index::FieldInfoPtr;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::index::TermsStats;
using lucene::cyborg::index::TermsStatsPtr;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::FstBytesOutput;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// FieldReader
//

FieldReader::FieldReader(ReadOnlyPtr<Lucene90BlockTreeTermsReader> _parent,
                         FieldInfoPtr _field_info,
                         int64_t _num_terms,
                         std::string _root_code,
                         int64_t _sum_total_term_freq,
                         int64_t _sum_doc_freq,
                         int32_t _doc_count,
                         DataInput<BufferedChecksumMemoryDataInput> &_meta_in,
                         CloseableMemoryDataInput _index_input,
                         CloseableMemoryDataInput _suffix_trie_input,
                         const std::string &_min_term,
                         const std::string &_max_term)
    : index(_meta_in, std::move(_index_input), FstBytesOutput{}),
      parent(_parent),
      field_info(std::move(_field_info)),
      num_terms(_num_terms),
      root_code(std::move(_root_code)),
      sum_total_term_freq(_sum_total_term_freq),
      sum_doc_freq(_sum_doc_freq),
      doc_count(_doc_count),
      suffix_trie_input(std::move(_suffix_trie_input)),
      min_term(_min_term),
      max_term(_max_term),
      root_block_fp(),
      postings_reader() {
  DataInput<ReadOnlyDataInput> root_code_input{
      ReadOnlyDataInput{
          (ReadOnlyPtr<uint8_t>) this->root_code.data(),
          (ReadOnlyPtr<uint8_t>) this->root_code.data(),
          0,
          (int32_t) this->root_code.size()
      }
  };

  const auto root_block_header = root_code_input.read_vlong();
  root_block_fp = root_block_header >> BlockTreeConstants::OUTPUT_FLAGS_NUM_BITS;
}

ManagedPtr<SegmentTermsEnum> FieldReader::iterator(UseOnlyPtr<LinearAllocator> allocator) {
  return allocator->allocate_object<SegmentTermsEnum>(this, allocator);
}

int32_t FieldReader::size() {
  return num_terms;
}

int64_t FieldReader::get_sum_total_term_freq() {
  return sum_total_term_freq;
}

int64_t FieldReader::get_sum_doc_freq() {
  return sum_doc_freq;
}

int32_t FieldReader::get_doc_count() {
  return doc_count;
}

bool FieldReader::has_freq() {
  return field_info->index_options >= IndexOptions::DOCS_AND_FREQS;
}

bool FieldReader::has_offset() {
  return field_info->index_options >= IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS;
}

bool FieldReader::has_positions() {
  return field_info->index_options >= IndexOptions::DOCS_AND_FREQS_AND_POSITIONS;
}

bool FieldReader::has_payloads() {
  return field_info->has_payloads();
}

std::string_view FieldReader::get_min() {
  return min_term;
}

std::string_view FieldReader::get_max() {
  return max_term;
}

TermsStatsPtr FieldReader::get_stats(UseOnlyPtr<LinearAllocator> allocator) {
  auto segment_terms_enum =
      allocator->allocate_object<SegmentTermsEnum>(this, allocator);
  return segment_terms_enum->compute_block_stats();
}

DocPosPayFilePointers FieldReader::set_posting_reader(const DocPosPayFilePointers &end_file_fps,
                                                      ManagedPtr<Lucene90PostingsReader> parent_postings_reader,
                                                      ManagedPtr<MMapDirectory> directory) {
  LinearAllocator allocator{};
  auto segment_terms_enum =
      allocator.allocate_object<SegmentTermsEnum>(this, &allocator);
  auto first_entry_in_first_block = segment_terms_enum->compute_the_first_block(&allocator);

  Lucene90TermState term_state{};
  parent_postings_reader->format_term_state(field_info.get(), term_state);

  first_entry_in_first_block->decode_meta_data(parent_postings_reader, term_state);

  this->postings_reader = parent_postings_reader->slice(
      directory,
      term_state.doc_start_fp, end_file_fps.doc_start_fp - term_state.doc_start_fp,
      term_state.pos_start_fp, end_file_fps.pos_start_fp - term_state.pos_start_fp,
      term_state.pay_start_fp, end_file_fps.pay_start_fp - term_state.pay_start_fp);

  auto ret = end_file_fps;
  if (term_state.index_options & 1U) {
    // has doc
    ret.doc_start_fp = term_state.doc_start_fp;
  }
  if (term_state.index_options & 4U) {
    // has pos
    ret.pos_start_fp = term_state.pos_start_fp;
  }
  if ((term_state.index_options & 8U) | (term_state.index_options & 16U)) {
    // has pay
    ret.pay_start_fp = term_state.pay_start_fp;
  }

  return ret;
}

