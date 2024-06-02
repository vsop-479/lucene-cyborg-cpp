#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTINFOS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTINFOS_HPP_

#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include "../util/PointerTypes.hpp"
#include "../store/MMapDirectory.hpp"
#include "../store/CloseableMemoryDataInput.hpp"
#include "../util/Version.hpp"
#include "SegmentCommitInfo.hpp"
#include "../store/DataInput.hpp"
#include "../store/BufferedChecksumMemoryDataInput.hpp"

namespace lucene::cyborg::index {

class SegmentInfos;
using SegmentInfosPtr = std::shared_ptr<SegmentInfos>;

class SegmentInfos {
 public:
  static constexpr int32_t VERSION_70 = 7;

  static constexpr int32_t VERSION_72 = 8;

  static constexpr int32_t VERSION_74 = 9;

  static constexpr int32_t VERSION_86 = 10;

  static constexpr int32_t VERSION_CURRENT = VERSION_86;

  static const std::string OLD_SEGMENTS_GEN;

  static int64_t get_last_commit_generation(const std::vector<std::string> &files);

  static int64_t generation_from_segments_file_name(const std::string &file_name);

  static SegmentInfosPtr read_commit(store::MMapDirectoryPtr directory,
                                     const std::string &segment_file_name,
                                     int32_t min_supported_major_version);

  explicit SegmentInfos(int32_t index_created_version_major);

  int32_t size() const noexcept {
    return segments.size();
  }

  SegmentCommitInfoPtr &info(int32_t idx) {
    return segments[idx];
  }

  int32_t get_index_created_version_major() const noexcept {
    return index_created_version_major;
  }

  void add(SegmentCommitInfoPtr si);

 private:
  static SegmentInfosPtr read_commit_with_input(store::MMapDirectoryPtr directory,
                                                store::CloseableMemoryDataInput input,
                                                int64_t generation,
                                                int32_t min_supported_major_version);

  static void parse_segment_infos(
      store::MMapDirectoryPtr directory,
      store::DataInput<store::BufferedChecksumMemoryDataInput> &input,
      SegmentInfosPtr infos,
      int32_t format);

  std::vector<SegmentCommitInfoPtr> segments;
  // The Lucene version major that was used to create the index.
  int32_t index_created_version_major;
  std::string id;
  int64_t generation;
  int64_t last_generation;
  util::Version lucene_version = util::Version::UNKNOWN;
  int64_t version;
  int64_t counter;
  util::Version min_segment_lucene_version = util::Version::UNKNOWN;
  std::unordered_map<std::string, std::string> user_data;
};  // SegmentInfos



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTINFOS_HPP_
