#include <functional>
#include "Lucene90BlockTreeTermsReader.hpp"
#include "SegmentTermsEnumFrame.hpp"
#include "BlockTreeConstants.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::codecs::v90::DocPosPayFilePointers;
using lucene::cyborg::codecs::v90::block_tree::BlockTreeConstants;
using lucene::cyborg::codecs::v90::block_tree::FieldReader;
using lucene::cyborg::codecs::v90::block_tree::FieldReaderPtr;
using lucene::cyborg::codecs::v90::block_tree::Lucene90BlockTreeTermsReader;
using lucene::cyborg::codecs::v90::block_tree::SegmentTermsEnumFrame;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::index::SegmentReadState;
using lucene::cyborg::index::Terms;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::MeasureDataInput;
using lucene::cyborg::store::ReadOnlyDataInput;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::skip_meta_input_in_fst_during_loading;

constexpr int32_t OUTPUT_FLAGS_NUM_BITS = 2;
constexpr int32_t OUTPUT_FLAGS_MASK = 0x3;

constexpr int32_t COLLECT_FILE_POINTS_MODE = 0;
constexpr int32_t CREATE_FIELD_READERS_MODE = 1;

/** extension of terms file */
static const std::string TERMS_EXTENSION = "tim";
static const std::string TERMS_CODEC_NAME = "BlockTreeTermsDict";

/** initial terms format. */
static constexpr int32_t VERSION_START = 0;

/** current terms format. */
static constexpr int32_t VERSION_CURRENT = VERSION_START;

/** extension of terms index file */
static const std::string TERMS_INDEX_EXTENSION = "tip";

static const std::string TERMS_INDEX_CODEC_NAME = "BlockTreeTermsIndex";

/** extension of terms meta file */
static const std::string TERMS_META_EXTENSION = "tmd";

static const std::string TERMS_META_CODEC_NAME = "BlockTreeTermsMeta";

template<typename DataInputType>
std::string read_bytes_ref(DataInputType &in) {
  const int32_t num_bytes = in.read_vint();
  if (num_bytes < 0) {
    // throw new CorruptIndexException("invalid bytes length: " + numBytes, fst_in);
    // TODO
    throw 13;
  }

  std::string bytes(num_bytes, '\0');
  in.read_bytes(bytes.data(), num_bytes);
  return bytes;
}

void collect_mapping_area_info(
    DataInput<ReadOnlyDataInput> &meta_in,
    ReadOnlyPtr<SegmentReadState> state,
    std::vector<int64_t> &index_fps,
    std::vector<int64_t> &suffix_block_fps,
    std::vector<int64_t> &meta_start_fps,
    const std::string &terms_name,
    int64_t &index_length,
    int64_t &terms_length,
    int64_t max_fp_in_terms_file) {
  const int32_t num_fields = meta_in.read_vint();
  if (num_fields < 0) {
    // todo
    throw 13;
  }

  for (int32_t i = 0; i < num_fields; ++i) {
    meta_start_fps.emplace_back(meta_in.get_file_pointer());
    const int32_t field_no = meta_in.read_vint();
    const int64_t num_terms = meta_in.read_vlong();
    if (num_terms <= 0) {
      // todo
      throw 13;
    }
    std::string root_code = read_bytes_ref(meta_in);
    auto field_info = state->field_infos->field_info(field_no);
    if (!field_info) {
      // todo
      throw 13;
    }
    const int64_t sum_total_term_freq = meta_in.read_vlong();
    const int64_t sum_doc_freq =
        field_info->index_options == IndexOptions::DOCS
        ? sum_total_term_freq
        : meta_in.read_vlong();
    const int32_t doc_count = meta_in.read_vint();
    std::string min_term = read_bytes_ref(meta_in);
    std::string max_term = read_bytes_ref(meta_in);
    if (doc_count < 0
        || doc_count > state->segment_info->max_doc) {
      // todo
      throw 13;
    }
    if (sum_doc_freq < doc_count) {
      // todo
      throw 13;
    }
    if (sum_total_term_freq < sum_doc_freq) {
      // todo
      throw 13;
    }

    const int64_t index_start_fp = meta_in.read_vlong();

    index_fps.emplace_back(index_start_fp);

    DataInput<ReadOnlyDataInput> root_code_input{
        ReadOnlyDataInput{
            (ReadOnlyPtr<uint8_t>) root_code.data(),
            (ReadOnlyPtr<uint8_t>) root_code.data(),
            0,
            (int32_t) root_code.size()
        }
    };

    const auto root_block_header = root_code_input.read_vlong();
    int64_t suffix_block_fp = root_block_header >> OUTPUT_FLAGS_NUM_BITS;
    if (root_block_header & BlockTreeConstants::OUTPUT_FLAG_IS_FLOOR) {
      // Root block is a floor block
      const auto num_sub_blocks = root_code_input.read_vint();
      assert(num_sub_blocks > 0);

      if (num_sub_blocks <= 0) {
        // TODO
        throw 13;
      }

      int32_t sub_block_fp = 0;
      for (int32_t i = 0; i < num_sub_blocks; ++i) {
        // Consume lead label
        root_code_input.read_byte();
        sub_block_fp = root_code_input.read_vlong();
        sub_block_fp >>= 1;
        sub_block_fp += suffix_block_fp;
      }
      // Make it point to the last sub block
      suffix_block_fp = sub_block_fp;
    }

    // fst_in suffix trie file, identify the end boundary for each field.
    for (int32_t map_size = 4096, is_last_trial = 0;; map_size *= 2) {
      if (is_last_trial) {
        // todo: failed
        throw 13;
      }

      const int32_t actual_size = std::min((int64_t) map_size, max_fp_in_terms_file - suffix_block_fp);
      is_last_trial = (actual_size < map_size ? 1 : 0);

      auto terms_input = state->directory->open_input_with_region(
          terms_name, suffix_block_fp, actual_size);
      auto terms_read_only_input = terms_input.to_read_only();
      MeasureDataInput measure_data_input{terms_read_only_input.data, (int32_t) terms_read_only_input.length};

      std::optional<int32_t> root_block_size =
          SegmentTermsEnumFrame::measure_block_size(measure_data_input);

      if (root_block_size) {
        suffix_block_fps.emplace_back(suffix_block_fp + *root_block_size);
        break;
      }
    }  // end for

    skip_meta_input_in_fst_during_loading(meta_in);
  }  // end for

  meta_start_fps.emplace_back(meta_in.get_file_pointer());
  index_length = meta_in.read_long();
  terms_length = meta_in.read_long();
  meta_start_fps.emplace_back(meta_in.get_file_pointer());

  index_fps.emplace_back(index_length - CodecUtil::footer_length());
}

void create_field_infos_and_field_readers(
    Lucene90BlockTreeTermsReader *block_tree_terms_reader,
    DataInput<BufferedChecksumMemoryDataInput> &meta_in,
    ReadOnlyPtr<SegmentReadState> state,
    std::vector<int64_t> &index_fps,
    std::vector<int64_t> &suffix_block_end_fps,
    std::vector<int64_t> &meta_start_fps,
    std::unordered_map<std::string, FieldReaderPtr> &field_map,
    const std::string &index_name,
    const std::string &terms_name) {
  const int32_t num_fields = meta_in.read_vint();
  DocPosPayFilePointers doc_pos_pay_file_pointers{
      block_tree_terms_reader->postings_reader->get_doc_file_size(),
      block_tree_terms_reader->postings_reader->get_pos_file_size(),
      block_tree_terms_reader->postings_reader->get_pay_file_size(),
  };

  for (int32_t i = num_fields - 1; i >= 0; --i) {
    meta_in.seek(meta_start_fps[i]);
    const int32_t field_no = meta_in.read_vint();
    const int64_t num_terms = meta_in.read_vlong();
    std::string root_code = read_bytes_ref(meta_in);
    auto field_info = state->field_infos->field_info(field_no);
    const int64_t sum_total_term_freq = meta_in.read_vlong();
    const int64_t sum_doc_freq =
        field_info->index_options == IndexOptions::DOCS
        ? sum_total_term_freq
        : meta_in.read_vlong();
    const int32_t doc_count = meta_in.read_vint();
    std::string min_term = read_bytes_ref(meta_in);
    std::string max_term = read_bytes_ref(meta_in);

    const int64_t index_start_fp = meta_in.read_vlong();

    const auto index_size = index_fps[i + 1] - index_fps[i];
    auto index_input = state->directory->open_input_with_region(
        index_name, index_start_fp, index_size);

    const auto suffix_trie_size = suffix_block_end_fps[i + 1] - suffix_block_end_fps[i];
    auto suffix_trie_input = state->directory->open_input_with_region(
        terms_name, suffix_block_end_fps[i], suffix_trie_size);

    // avoid using `shared_from_this` as it leads circular references, can cause a potential memory leak.
    // also given that the lifecycle for both terms reader and field reader is the same,
    // it's safe to pass the raw pointer here.
    auto parent = block_tree_terms_reader;

    auto field_reader = std::make_shared<FieldReader>(
        parent,
        field_info,
        num_terms,
        root_code,
        sum_total_term_freq,
        sum_doc_freq,
        doc_count,
        meta_in,
        std::move(index_input),
        std::move(suffix_trie_input),
        min_term,
        max_term);

    doc_pos_pay_file_pointers = field_reader->set_posting_reader(
        doc_pos_pay_file_pointers,
        block_tree_terms_reader->postings_reader.get(),
        state->directory.get());

    field_map.emplace(
        field_info->name,
        std::move(field_reader));
  }  // End for

  // consume length information
  // go to the position right before consuming length info.
  // the last fp in the vec is the point after consumed them.
  meta_in.seek(meta_start_fps[meta_start_fps.size() - 2]);
  meta_in.read_long();
  meta_in.read_long();
  assert(meta_in.get_file_pointer() == meta_start_fps.back());
}

//
// Lucene90BlockTreeTermsReader
//

Lucene90BlockTreeTermsReader::Lucene90BlockTreeTermsReader(Lucene90PostingsReaderPtr postings_reader,
                                                           ReadOnlyPtr<SegmentReadState> state)
    : postings_reader(std::move(postings_reader)),
      field_map(),
      field_name_bytes(),
      field_list(),
      segment(state->segment_info->name),
      version() {
  // retrieve the suffix name

  // suffix trie file
  const std::string terms_name =
      IndexFileNames::segment_file_name(segment, state->segment_suffix, TERMS_EXTENSION);
  const int64_t terms_file_size = state->directory->file_length(terms_name);
  auto terms_input = state->directory->open_input_with_region(
      terms_name, 0,
      std::min((int64_t) CodecUtil::maximum_size_for_checking_index_header(
          TERMS_CODEC_NAME), terms_file_size));
  auto terms_read_only_input = terms_input.to_read_only();
  DataInput<BufferedChecksumMemoryDataInput> terms_in{
      BufferedChecksumMemoryDataInput{
          terms_read_only_input.data,
          terms_read_only_input.offset,
          terms_read_only_input.length,
          BufferedChecksum{}
      }
  };

  version = CodecUtil::check_index_header(terms_in,
                                          TERMS_CODEC_NAME,
                                          VERSION_START,
                                          VERSION_CURRENT,
                                          state->segment_info->id,
                                          state->segment_suffix);

  const int64_t actual_header_size_of_terms_file = terms_in.get_file_pointer();

  // index file
  const std::string index_name =
      IndexFileNames::segment_file_name(segment, state->segment_suffix, TERMS_INDEX_EXTENSION);
  const int64_t index_file_size = state->directory->file_length(index_name);
  auto index_input = state->directory->open_input_with_region(
      index_name, 0,
      std::min((int64_t) CodecUtil::maximum_size_for_checking_index_header(
          TERMS_INDEX_EXTENSION), index_file_size));
  auto index_read_only_input = index_input.to_read_only();
  DataInput<BufferedChecksumMemoryDataInput> index_in{
      BufferedChecksumMemoryDataInput{
          index_read_only_input.data,
          index_read_only_input.offset,
          index_read_only_input.length,
          BufferedChecksum{}
      }
  };

  CodecUtil::check_index_header(index_in,
                                TERMS_INDEX_CODEC_NAME,
                                version,
                                version,
                                state->segment_info->id,
                                state->segment_suffix);

  // meta file
  const std::string meta_name =
      IndexFileNames::segment_file_name(segment, state->segment_suffix, TERMS_META_EXTENSION);
  std::unordered_map<std::string, FieldReaderPtr> field_map;
  int64_t index_length = -1;
  int64_t terms_length = -1;

  auto meta_input = state->directory->open_input(meta_name);
  auto meta_read_only_input = meta_input.to_read_only();
  DataInput<BufferedChecksumMemoryDataInput> meta_in{
      BufferedChecksumMemoryDataInput{
          meta_read_only_input.data,
          meta_read_only_input.offset,
          meta_read_only_input.length,
          BufferedChecksum{}
      }
  };

  CodecUtil::check_index_header(meta_in,
                                TERMS_META_CODEC_NAME,
                                version,
                                version,
                                state->segment_info->id,
                                state->segment_suffix);
  this->postings_reader->init(meta_in, state);

  // file pointers
  std::vector<int64_t> index_start_fps;
  std::vector<int64_t> suffix_blocks_end_fps{actual_header_size_of_terms_file};
  std::vector<int64_t> meta_start_fps;
  DataInput<ReadOnlyDataInput> measure_meta_in{meta_read_only_input};
  const auto meta_start_fp = meta_in.get_file_pointer();
  measure_meta_in.seek(meta_start_fp);

  collect_mapping_area_info(
      measure_meta_in,
      state,
      index_start_fps,
      suffix_blocks_end_fps,
      meta_start_fps,
      terms_name,
      index_length,
      terms_length,
      terms_file_size - CodecUtil::footer_length());
  index_start_fps.emplace_back(index_length - CodecUtil::footer_length());

  // CRC check
  {
    int32_t left = meta_start_fps.back() - meta_in.get_file_pointer();
    for (int32_t i = left / 8; i > 0; --i) {
      meta_in.read_long();
    }
    left = left % 8;
    for (int32_t i = left / 4; i > 0; --i) {
      meta_in.read_int();
    }
    left = left % 4;
    for (int32_t i = left; i > 0; --i) {
      meta_in.read_byte();
    }
  }
  CodecUtil::check_footer(meta_in);
  meta_in.seek(meta_start_fp);

  create_field_infos_and_field_readers(
      this,
      meta_in,
      state,
      index_start_fps,
      suffix_blocks_end_fps,
      meta_start_fps,
      field_map,
      index_name,
      terms_name);

  // index file checksum
  // at this point the checksum of the meta file has been verified so the lengths are likely
  // correct
  index_input = state->directory->open_input_with_region(
      index_name, index_length - CodecUtil::footer_length(), CodecUtil::footer_length());
  index_read_only_input = index_input.to_read_only();
  index_in = DataInput<BufferedChecksumMemoryDataInput>{
      BufferedChecksumMemoryDataInput{
          index_read_only_input.data,
          index_read_only_input.offset,
          index_read_only_input.length,
          BufferedChecksum{}
      }
  };

  CodecUtil::retrieve_checksum(index_in);

  // terms file checksum
  terms_input = state->directory->open_input_with_region(
      terms_name, terms_length - CodecUtil::footer_length(), CodecUtil::footer_length());
  terms_read_only_input = terms_input.to_read_only();
  terms_in = DataInput<BufferedChecksumMemoryDataInput>{
      BufferedChecksumMemoryDataInput{
          terms_read_only_input.data,
          terms_read_only_input.offset,
          terms_read_only_input.length,
          BufferedChecksum{}
      }
  };
  CodecUtil::retrieve_checksum(terms_in);

  // finalizing fields
  field_list.reserve(field_map.size());
  int32_t field_name_total_size = 0;
  for (auto &p : field_map) {
    field_list.emplace_back(p.first);
    field_name_total_size += p.first.size();
  }
  std::sort(field_list.begin(), field_list.end());

  if (field_name_total_size > 0) {
    this->field_map = std::unordered_map<std::string_view, FieldReaderPtr>(field_map.size() * 2);
    field_name_bytes = std::make_unique<char[]>(field_name_total_size);
    int32_t field_name_bytes_upto = 0;
    for (auto &p : field_map) {
      std::memcpy(&field_name_bytes[field_name_bytes_upto], p.first.data(), p.first.size());
      this->field_map.emplace(std::string_view(&field_name_bytes[field_name_bytes_upto], p.first.size()), p.second);
      field_name_bytes_upto += p.first.size();
    }
  }  // End if
}

void Lucene90BlockTreeTermsReader::close() {
  // TODO
}

void Lucene90BlockTreeTermsReader::check_integrity() {
  // TODO
}

int32_t Lucene90BlockTreeTermsReader::size() {
  return field_map.size();
}

ManagedPtr<FieldReader> Lucene90BlockTreeTermsReader::terms(
    std::string_view field_name,
    ManagedPtr<SearchContext> search_context) {
  auto it = field_map.find(field_name);
  if (it != field_map.end()) {
    return it->second.get();
  } else {
    return nullptr;
  }  // End if
}

