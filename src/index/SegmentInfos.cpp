#include <unordered_map>
#include <unordered_set>
#include "SegmentInfos.hpp"
#include "IndexFileNames.hpp"
#include "IndexWriterConstants.hpp"
#include "../java/JavaLong.hpp"
#include "../codecs/CodecUtil.hpp"
#include "../util/StringHelper.hpp"
#include "../codecs/EffectiveCodecs.hpp"
#include "SegmentInfo.hpp"

using lucene::cyborg::codecs::CodecUtil;
using lucene::cyborg::codecs::CurrentCodec;
using lucene::cyborg::codecs::PreviousCodec;
using lucene::cyborg::index::IndexWriterConstants;
using lucene::cyborg::index::SegmentInfo;
using lucene::cyborg::index::SegmentInfos;
using lucene::cyborg::index::SegmentInfosPtr;
using lucene::cyborg::java::JavaLong;
using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::DataInput;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::StringHelper;
using lucene::cyborg::util::UseOnlyPtr;
using lucene::cyborg::util::Version;
using lucene::cyborg::util::BufferedChecksum;

//
// SegmentInfos
//

const std::string SegmentInfos::OLD_SEGMENTS_GEN = "segments.gen";

SegmentInfos::SegmentInfos(int32_t _index_created_version_major)
    : segments(),
      index_created_version_major(_index_created_version_major),
      id(),
      generation(),
      last_generation(),
      lucene_version(Version::UNKNOWN),
      version(),
      counter(),
      min_segment_lucene_version(Version::UNKNOWN),
      user_data() {
  if (_index_created_version_major > Version::LATEST->major) [[unlikely]] {
    // TODO
    throw 13;
  }
  if (_index_created_version_major < 6) [[unlikely]] {
    // TODO
    throw 13;
  }
}

int64_t SegmentInfos::get_last_commit_generation(const std::vector<std::string> &files) {
  int64_t max = -1;
  for (const auto &file : files) {
    if (file.starts_with(IndexFileNames::SEGMENTS)
        && // skipping this file here helps deliver the right exception when opening an old index
            !file.starts_with(OLD_SEGMENTS_GEN)) {
      int64_t gen = SegmentInfos::generation_from_segments_file_name(file);
      if (gen > max) {
        max = gen;
      }
    }
  }  // End for

  return max;
}

int64_t SegmentInfos::generation_from_segments_file_name(const std::string &file_name) {
  if (file_name == OLD_SEGMENTS_GEN) {
    // TODO
    // throw new IllegalArgumentException(
    //     "\"" + OLD_SEGMENTS_GEN + "\" is not a valid segment file name since 4.0");
    throw 13;
  } else if (file_name == IndexFileNames::SEGMENTS) {
    return 0;
  } else if (file_name.starts_with(IndexFileNames::SEGMENTS)) {
    return JavaLong::parse_long(
        std::string_view(file_name.data() + (1 + IndexFileNames::SEGMENTS.length()),
                         file_name.size() - (1 + IndexFileNames::SEGMENTS.length())),
        JavaLong::MAX_RADIX);
  } else {
    // TODO
    // throw new IllegalArgumentException("fileName \"" + fileName + "\" is not a segments file");
    throw 13;
  }
}

SegmentInfosPtr SegmentInfos::read_commit_with_input(MMapDirectoryPtr directory,
                                                     CloseableMemoryDataInput input,
                                                     int64_t generation,
                                                     int32_t min_supported_major_version) {
  auto read_only_input = input.to_read_only();
  DataInput<BufferedChecksumMemoryDataInput> checksum_input{
      BufferedChecksumMemoryDataInput{read_only_input.data,
                                      read_only_input.offset,
                                      read_only_input.length,
                                      BufferedChecksum{}}};

  int32_t format = -1;
  const int32_t magic = checksum_input.read_be_int();
  if (magic != CodecUtil::CODEC_MAGIC) {
    // TODO
    // throw new IndexFormatTooOldException(
    //     input, magic, CodecUtil.CODEC_MAGIC, CodecUtil.CODEC_MAGIC);
    throw 13;
  }

  format = CodecUtil::check_header_no_magic(checksum_input, "segments", VERSION_70, VERSION_CURRENT);
  std::string id(StringHelper::ID_LENGTH, '\0');
  // TODO : check whether it's safe to read id.size() bytes.
  checksum_input.read_bytes(id.data(), StringHelper::ID_LENGTH);
  CodecUtil::check_index_header_suffix(checksum_input, JavaLong::to_string(generation, JavaLong::MAX_RADIX));

  const auto major = checksum_input.read_vint();
  const auto minor = checksum_input.read_vint();
  const auto bugfix = checksum_input.read_vint();

  Version lucene_version = Version::from_bits(major, minor, bugfix);
  const int32_t index_created_version = checksum_input.read_vint();
  if (lucene_version.major < index_created_version) {
    // TODO
    // throw new CorruptIndexException(
    //     "Creation version ["
    //         + indexCreatedVersion
    //         + ".x] can't be greater than the version that wrote the segment infos: ["
    //         + luceneVersion
    //         + "]",
    //     input);
    throw 13;
  }

  if (index_created_version < min_supported_major_version) {
    // TODO
    // throw new IndexFormatTooOldException(
    //     input,
    //     "This index was initially created with Lucene "
    //         + indexCreatedVersion
    //         + ".x while the current version is "
    //         + Version.LATEST
    //         + " and Lucene only supports reading"
    //         + (minSupportedMajorVersion == Version.MIN_SUPPORTED_MAJOR
    //            ? " the current and previous major versions"
    //            : " from version " + minSupportedMajorVersion + " upwards"));
    throw 13;
  }

  auto infos = std::make_shared<SegmentInfos>(index_created_version);
  infos->id = id;
  infos->generation = generation;
  infos->last_generation = generation;
  infos->lucene_version = lucene_version;
  parse_segment_infos(std::move(directory), checksum_input, infos, format);

  if (format >= VERSION_70) { // oldest supported version
    CodecUtil::check_footer(checksum_input);
  }

  return infos;
}

SegmentInfosPtr SegmentInfos::read_commit(MMapDirectoryPtr directory,
                                          const std::string &segment_file_name,
                                          int32_t min_supported_major_version) {
  const int64_t generation = generation_from_segments_file_name(segment_file_name);
  auto input = directory->open_input(segment_file_name);

  try {
    return read_commit_with_input(std::move(directory),
                                  std::move(input),
                                  generation,
                                  min_supported_major_version);
  } catch (...) {
    // TODO
    // throw new CorruptIndexException(
    //     "Unexpected file read error while reading index.", input, e);
    throw 13;
  }
}

void SegmentInfos::parse_segment_infos(MMapDirectoryPtr directory,
                                       DataInput<BufferedChecksumMemoryDataInput> &input,
                                       SegmentInfosPtr infos,
                                       int32_t format) {
  infos->version = input.read_be_long();

  if (format > VERSION_70) {
    infos->counter = input.read_vlong();
  } else {
    infos->counter = input.read_be_int();
  }
  const int32_t num_segments = input.read_be_int();
  if (num_segments < 0) {
    // TODO
    // throw new CorruptIndexException("invalid segment count: " + numSegments, input);
    throw 13;
  }

  if (num_segments > 0) {
    const auto major = input.read_vint();
    const auto minor = input.read_vint();
    const auto bugfix = input.read_vint();
    infos->min_segment_lucene_version = Version::from_bits(major, minor, bugfix);
  } else {
    // else leave as null: no segments
  }

  int64_t total_docs = 0;
  for (int32_t seg = 0; seg < num_segments; ++seg) {
    const std::string seg_name = input.read_string_slow();
    std::string segment_id(StringHelper::ID_LENGTH, '\0');
    input.read_bytes(segment_id.data(), StringHelper::ID_LENGTH);
    const std::string codec_name = input.read_string_slow();

    SegmentInfoPtr info;
    if (codec_name == PreviousCodec::CODE_NAME) {
      info = PreviousCodec::SegmentInfoFormat().read(directory, seg_name, segment_id);
    } else {
      info = CurrentCodec::SegmentInfoFormat().read(directory, seg_name, segment_id);
    }  // End if

    info->codec_name = codec_name;
    total_docs += info->max_doc;
    int64_t del_gen = input.read_be_long();
    int32_t del_count = input.read_be_int();
    if (del_count < 0 || del_count > info->max_doc) {
      // TODO
      // throw new CorruptIndexException(
      //     "invalid deletion count: " + delCount + " vs maxDoc=" + info.maxDoc(), input);
      throw 13;
    }
    const int64_t field_infos_gen = input.read_be_long();
    const int64_t dv_gen = input.read_be_long();
    const int32_t soft_del_count = format > VERSION_72 ? input.read_be_int() : 0;
    if (soft_del_count < 0 || soft_del_count > info->max_doc) {
      // TODO
      // throw new CorruptIndexException(
      //     "invalid deletion count: " + softDelCount + " vs maxDoc=" + info.maxDoc(), input);
      throw 13;
    }
    if (soft_del_count + del_count > info->max_doc) {
      // TODO
      // throw new CorruptIndexException(
      //     "invalid deletion count: " + (softDelCount + delCount) + " vs maxDoc=" + info.maxDoc(),
      //     input);
      throw 13;
    }
    std::string sci_id;
    if (format > VERSION_74) {
      const auto marker = input.read_byte();
      switch (marker) {
        case 1: {
          sci_id = std::string(StringHelper::ID_LENGTH, '\0');
          input.read_bytes(sci_id.data(), StringHelper::ID_LENGTH);
          break;
        }
        case 0: {
          break;
        }
        default:
          // TODO
          // throw new CorruptIndexException(
          //     "invalid SegmentCommitInfo ID marker: " + marker, input);
          throw 13;
      }
    }

    SegmentCommitInfoPtr si_per_commit =
        std::make_shared<SegmentCommitInfo>(
            info,
            del_count,
            soft_del_count,
            del_gen,
            field_infos_gen,
            dv_gen,
            sci_id);
    si_per_commit->set_field_infos_files(input.read_set_of_strings_slow());
    std::unordered_map<int32_t, std::unordered_set<std::string>> dv_update_files;
    const int32_t num_dv_fields = input.read_be_int();
    if (num_dv_fields != 0) {
      std::unordered_map<int32_t, std::unordered_set<std::string>> map(num_dv_fields / 0.75);
      for (int32_t i = 0; i < num_dv_fields; i++) {
        const int32_t key = input.read_be_int();
        auto value = input.read_set_of_strings_slow();
        map[key] = std::move(value);
      }
    }

    si_per_commit->set_doc_values_updates_files(std::move(dv_update_files));
    infos->add(si_per_commit);

    const Version segment_version = info->version;

    if (!segment_version.on_or_after(infos->min_segment_lucene_version)) {
      // TODO
      // throw new CorruptIndexException(
      //     "segments file recorded minSegmentLuceneVersion="
      //         + infos.minSegmentLuceneVersion
      //         + " but segment="
      //         + info
      //         + " has older version="
      //         + segmentVersion,
      //     input);
      throw 13;
    }

    if (infos->index_created_version_major >= 7
        && segment_version.major < infos->index_created_version_major) {
      // TODO
      // throw new CorruptIndexException(
      //     "segments file recorded indexCreatedVersionMajor="
      //         + infos.indexCreatedVersionMajor
      //         + " but segment="
      //         + info
      //         + " has older version="
      //         + segmentVersion,
      //     input);
      throw 13;
    }

    if (infos->index_created_version_major >= 7 && info->min_version == Version::UNKNOWN) {
      // throw new CorruptIndexException(
      //     "segments infos must record minVersion with indexCreatedVersionMajor="
      //         + infos.indexCreatedVersionMajor,
      //     input);
      throw 13;
    }
  }

  infos->user_data = input.read_map_of_strings_slow();

  // LUCENE-6299: check we are fst_in bounds
  if (total_docs > IndexWriterConstants::ACTUAL_MAX_DOCS) {
    // TODO
    // throw new CorruptIndexException(
    //     "Too many documents: an index cannot exceed "
    //         + IndexWriterConstants.getActualMaxDocs()
    //         + " but readers have total maxDoc="
    //         + totalDocs,
    //     input);
    throw 13;
  }
}

void SegmentInfos::add(SegmentCommitInfoPtr si) {
  if (index_created_version_major >= 7 && si->info->min_version == Version::UNKNOWN) {
    // TODO
    // throw new IllegalArgumentException(
    //     "All segments must record the minVersion for indices created on or after Lucene 7");
  }

  segments.emplace_back(std::move(si));
}

