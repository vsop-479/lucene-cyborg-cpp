#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTINFO_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTINFO_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "../util/Version.hpp"
#include "../store/MMapDirectory.hpp"

namespace lucene::cyborg::index {

class SegmentInfo {
 public:
  static constexpr int32_t NO = -1; // e.g. no norms; no deletes;

  // Used by some member fields to mean present (e.g., norms, deletions). */
  static constexpr int32_t YES = 1; // e.g. have norms; have deletes;

  SegmentInfo(store::MMapDirectoryPtr _dir,
              const util::Version &_version,
              const util::Version &_min_version,
              const std::string &_name,
              int32_t _max_doc,
              bool _is_compound_file,
              const std::string &_codec_name,
              std::unordered_map<std::string, std::string> _diagnostics,
              const std::string &_id,
              std::unordered_map<std::string, std::string> _attributes);

  std::string named_for_this_segment(const std::string &file) const;

  void set_files(const std::unordered_set<std::string> &files);

  /** Unique segment name fst_in the directory. */
  std::string name;

  int32_t max_doc; // number of docs fst_in seg

  /** Where this segment resides. */
  store::MMapDirectoryPtr dir;

  bool is_compound_file;

  /** Id that uniquely identifies this segment. */
  std::string id;

  std::unordered_map<std::string, std::string> diagnostics;

  std::unordered_map<std::string, std::string> attributes;

  // Tracks the Lucene version this segment was created with, since 3.1. Null
  // indicates an older than 3.0 index, and it's used to detect a too old index.
  // The format expected is "x.y" - "2.x" for pre-3.0 indexes (or null), and
  // specific versions afterward ("3.0.0", "3.1.0" etc.).
  // see o.a.l.util.Version.
  util::Version version;

  // Tracks the minimum version that contributed documents to a segment. For
  // flush segments, that is the version that wrote it. For merged segments,
  // this is the minimum minVersion of all the segments that have been merged
  // into this segment
  util::Version min_version = util::Version::UNKNOWN;

  std::string codec_name;

  std::unordered_set<std::string> files;
};  // SegmentInfo

using SegmentInfoPtr = std::shared_ptr<SegmentInfo>;

}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTINFO_HPP_
