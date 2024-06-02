#include <vector>
#include <unordered_map>
#include <cassert>
#include "Lucene94FieldInfosFormat.hpp"
#include "../../store/BufferedChecksumMemoryDataInput.hpp"
#include "../../codecs/CodecUtil.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::codecs::v94::Lucene94FieldInfosFormat;
using lucene::cyborg::index::DocValuesType;
using lucene::cyborg::index::FieldInfo;
using lucene::cyborg::index::FieldInfoPtr;
using lucene::cyborg::index::FieldInfos;
using lucene::cyborg::index::FieldInfosPtr;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::index::SegmentInfoPtr;
using lucene::cyborg::index::VectorEncoding;
using lucene::cyborg::index::VectorSimilarityFunction;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::BufferedChecksum;

const std::string EXTENSION = "fnm";
const std::string CODEC_NAME = "Lucene94FieldInfos";
constexpr int32_t FORMAT_START = 0;
constexpr int32_t FORMAT_CURRENT = 0;
constexpr uint32_t STORE_TERM_VECTOR = 0x1U;
constexpr uint32_t OMIT_NORMS = 0x2U;
constexpr uint32_t STORE_PAYLOADS = 0x4U;
constexpr uint32_t SOFT_DELETES_FIELD = 0x8U;

IndexOptions get_index_options(uint32_t b) {
  switch (b) {
    case 0:return IndexOptions::NONE;
    case 1U:return IndexOptions::DOCS;
    case 2U:return IndexOptions::DOCS_AND_FREQS;
    case 3U:return IndexOptions::DOCS_AND_FREQS_AND_POSITIONS;
    case 4U:return IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS;
    default:
      // TODO
      throw 13;
  }  // End switch
}

DocValuesType get_doc_values_type(uint32_t b) {
  switch (b) {
    case 0U:return DocValuesType::NONE;
    case 1U:return DocValuesType::NUMERIC;
    case 2U:return DocValuesType::BINARY;
    case 3U:return DocValuesType::SORTED;
    case 4U:return DocValuesType::SORTED_SET;
    case 5U:return DocValuesType::SORTED_NUMERIC;
    default:
      // TODO
      throw 13;
      // throw new CorruptIndexException("invalid docvalues byte: " + b, input);
  }
}

VectorEncoding get_vector_encoding(int32_t b) {
  if (b < 0 || b >= lucene::cyborg::index::NUM_VECTOR_ENCODING_TYPES) {
    // TODO
    // throw new CorruptIndexException("invalid vector encoding: " + b, input);
    throw 13;
  }

  if (b == 0) {
    return VectorEncoding::BYTE;
  } else {
    assert(b == 1);
    return VectorEncoding::FLOAT32;
  }
}

VectorSimilarityFunction get_distance_func(int32_t b) {
  if (b < 0 || b >= VectorSimilarityFunction::NUM_VECTOR_SIMILARITY_FUNCTIONS) {
    // TODO
    throw 13;
  }

  // TODO
  return VectorSimilarityFunction{};
}

//
// Lucene94FieldInfosFormat
//

FieldInfosPtr Lucene94FieldInfosFormat::read(MMapDirectoryPtr directory,
                                             SegmentInfoPtr segment_info,
                                             const std::string &segment_suffix) {
  const auto file_name = IndexFileNames::segment_file_name(
      segment_info->name, segment_suffix, EXTENSION);
  auto input = directory->open_input(file_name);
  auto read_only_input = input.to_read_only();

  DataInput<BufferedChecksumMemoryDataInput> checksum_input{
      BufferedChecksumMemoryDataInput{
          read_only_input.data,
          read_only_input.offset,
          read_only_input.length,
          BufferedChecksum{}
      }
  };

  CodecUtil::check_index_header(checksum_input,
                                CODEC_NAME,
                                FORMAT_START,
                                FORMAT_CURRENT,
                                segment_info->id,
                                segment_suffix);

  const int32_t size = checksum_input.read_vint();
  std::vector<FieldInfoPtr> field_infos(size);
  std::unordered_map<std::string, std::string> last_attributes;

  for (int32_t i = 0; i < size; ++i) {
    auto name = checksum_input.read_string_slow();
    const int32_t field_number = checksum_input.read_vint();

    if (field_number < 0) {
      // TODO
      // throw new CorruptIndexException(
      //     "invalid field number for field: " + name + ", fieldNumber=" + fieldNumber, input);
      throw 13;
    }
    const auto bits = checksum_input.read_byte();
    const bool store_term_vector = (bits & STORE_TERM_VECTOR);
    const bool omit_norms = (bits & OMIT_NORMS);
    const bool store_payloads = (bits & STORE_PAYLOADS);
    const bool is_soft_deletes_field = (bits & SOFT_DELETES_FIELD);

    const IndexOptions index_options = get_index_options(checksum_input.read_byte());

    // DV Types are packed fst_in one byte
    const DocValuesType doc_values_type = get_doc_values_type(checksum_input.read_byte());
    const int64_t dv_gen = checksum_input.read_long();
    std::unordered_map<std::string, std::string> attributes = checksum_input.read_map_of_strings_slow();
    last_attributes = attributes;
    const int32_t point_data_dimension_count = checksum_input.read_vint();
    int32_t point_num_bytes = 0;
    int32_t point_index_dimension_count = point_data_dimension_count;
    if (point_data_dimension_count != 0) {
      point_index_dimension_count = checksum_input.read_vint();
      point_num_bytes = checksum_input.read_vint();
    }
    const int32_t vector_dimension = checksum_input.read_vint();
    const VectorEncoding vector_encoding = get_vector_encoding(checksum_input.read_byte());
    const VectorSimilarityFunction vector_dist_func = get_distance_func(checksum_input.read_byte());

    field_infos[i] =
        std::make_shared<FieldInfo>(
            std::move(name),
            field_number,
            store_term_vector,
            omit_norms,
            store_payloads,
            index_options,
            doc_values_type,
            dv_gen,
            attributes,
            point_data_dimension_count,
            point_index_dimension_count,
            point_num_bytes,
            vector_dimension,
            vector_encoding,
            vector_dist_func,
            is_soft_deletes_field);
  }  // End for

  CodecUtil::check_footer(checksum_input);
  return std::make_shared<FieldInfos>(std::move(field_infos));
}
