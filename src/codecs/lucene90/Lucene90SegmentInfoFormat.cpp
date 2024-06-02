#include "Lucene90SegmentInfoFormat.hpp"
#include "../../index/IndexFileNames.hpp"
#include "../../store/BufferedChecksumMemoryDataInput.hpp"
#include "../../store/DataInput.hpp"
#include "../../util/BufferedChecksum.hpp"
#include "../../codecs/CodecUtil.hpp"
#include "../../index/SegmentInfo.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::codecs::v90::Lucene90SegmentInfoFormat;
using lucene::cyborg::index::IndexFileNames;
using lucene::cyborg::index::SegmentInfo;
using lucene::cyborg::index::SegmentInfoPtr;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::Version;

//
// Lucene90SegmentInfoFormat
//

const std::string Lucene90SegmentInfoFormat::SI_EXTENSION = "si";
const std::string Lucene90SegmentInfoFormat::CODEC_NAME = "Lucene90SegmentInfo";

SegmentInfoPtr parse_segment_info(MMapDirectoryPtr dir,
                                  DataInput<BufferedChecksumMemoryDataInput> &input,
                                  const std::string &segment,
                                  const std::string &segment_id) {
  const int32_t major = input.read_int();
  const int32_t minor = input.read_int();
  const int32_t bugfix = input.read_int();
  const Version version = Version::from_bits(major, minor, bugfix);

  const auto has_min_version = input.read_byte();
  Version min_version = Version::UNKNOWN;
  if (has_min_version == 0) {
    // no-op
  } else if (has_min_version == 1) {
    const int32_t major = input.read_int();
    const int32_t minor = input.read_int();
    const int32_t bugfix = input.read_int();
    min_version = Version::from_bits(major, minor, bugfix);
  } else {
    // TODO
    throw 13;
  }

  const int32_t doc_count = input.read_int();
  if (doc_count < 0) {
    // TODO
    throw 13;
  }
  const bool is_compound_file = (input.read_byte() == SegmentInfo::YES);

  auto diagnostics = input.read_map_of_strings_slow();
  auto files = input.read_set_of_strings_slow();
  auto attributes = input.read_map_of_strings_slow();

  const int32_t num_sort_fields = input.read_vint();
  if (num_sort_fields > 0) {
    // TODO : Impl
    throw 13;
  } else if (num_sort_fields < 0) {
    // TODO : Impl
    throw 13;
  }

  SegmentInfoPtr segment_info = std::make_shared<SegmentInfo>(
      dir,
      version,
      min_version,
      segment,
      doc_count,
      is_compound_file,
      "",
      diagnostics,
      segment_id,
      attributes);
  segment_info->set_files(files);
  return segment_info;
}

SegmentInfoPtr Lucene90SegmentInfoFormat::read(MMapDirectoryPtr dir,
                                               const std::string &segment,
                                               const std::string &segment_id) {
  const std::string file_name = IndexFileNames::segment_file_name(segment, "", SI_EXTENSION);
  auto input = dir->open_input(file_name);
  auto read_only_input = input.to_read_only();
  DataInput<BufferedChecksumMemoryDataInput> checksum_input{
      BufferedChecksumMemoryDataInput{read_only_input.data,
                                      read_only_input.offset,
                                      read_only_input.length,
                                      BufferedChecksum{}}};

  CodecUtil::check_index_header(checksum_input,
                                CODEC_NAME,
                                VERSION_START,
                                VERSION_CURRENT,
                                segment_id,
                                "");
  SegmentInfoPtr si = parse_segment_info(dir, checksum_input, segment, segment_id);
  CodecUtil::check_footer(checksum_input);
  return si;
}
