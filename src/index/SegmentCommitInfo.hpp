#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTCOMMITINFO_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTCOMMITINFO_HPP_

#include <atomic>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include "SegmentInfo.hpp"

namespace lucene::cyborg::index {

struct SegmentCommitInfo;
using SegmentCommitInfoPtr = std::shared_ptr<SegmentCommitInfo>;

struct SegmentCommitInfo {
  SegmentCommitInfo(
      SegmentInfoPtr _info,
      int32_t _del_count,
      int32_t _soft_del_count,
      int64_t _del_gen,
      int64_t _field_infos_gen,
      int64_t _doc_values_gen,
      std::string _id);

  void set_field_infos_files(std::unordered_set<std::string> _field_infos_files);

  void set_doc_values_updates_files(std::unordered_map<int32_t, std::unordered_set<std::string>> _dv_updates_files);

  SegmentCommitInfoPtr clone();

  bool has_deletions() const noexcept {
    return del_gen != -1;
  }

  bool has_field_updates() const noexcept {
    return field_infos_gen != -1;
  }

  // The {@link SegmentInfo} that we wrap. */
  SegmentInfoPtr info;

  /** Id that uniquely identifies this segment commit. */
  std::string id;

  // How many deleted docs fst_in the segment:
  int32_t del_count;

  // How many soft-deleted docs fst_in the segment that are not also hard-deleted:
  int32_t soft_del_count;

  // Generation number of the live docs file (-1 if there
  // are no deletes yet):
  int64_t del_gen;

  // Normally 1+delGen, unless an exception was hit on last
  // attempt to write:
  int64_t next_write_del_gen;

  // Generation number of the FieldInfos (-1 if there are no updates)
  int64_t field_infos_gen;

  // Normally 1+fieldInfosGen, unless an exception was hit on last attempt to
  // write
  int64_t next_write_field_infos_gen;

  // generation number of the docvalues (-1 if there are no updates)
  int64_t doc_values_gen;

  // normally 1+dvgen, unless an exception was hit on last attempt to
  // write
  int64_t next_write_doc_values_gen;

  // track the per-field docvalues update files
  std::unordered_map<int32_t, std::unordered_set<std::string>> dv_updates_files;

  std::unordered_set<std::string> field_infos_files;

  std::atomic<int64_t> size_in_bytes = -1;

  // note: only used fst_in-ram by iw to track buffered deletes;
  // this is never written to/read from the directory
  int64_t buffered_deletes_gen = -1;
};  // SegmentCommitInfo



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTCOMMITINFO_HPP_
