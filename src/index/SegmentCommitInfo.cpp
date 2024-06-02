#include "SegmentCommitInfo.hpp"
#include "../util/StringHelper.hpp"

using lucene::cyborg::index::SegmentCommitInfo;
using lucene::cyborg::index::SegmentCommitInfoPtr;
using lucene::cyborg::index::SegmentInfoPtr;
using lucene::cyborg::util::StringHelper;

//
// SegmentCommitInfo
//

SegmentCommitInfo::SegmentCommitInfo(SegmentInfoPtr _info,
                                     int32_t _del_count,
                                     int32_t _soft_del_count,
                                     int64_t _del_gen,
                                     int64_t _field_infos_gen,
                                     int64_t _doc_values_gen,
                                     std::string _id)
    : info(std::move(_info)),
      id(std::move(_id)),
      del_count(_del_count),
      soft_del_count(_soft_del_count),
      del_gen(_del_gen),
      next_write_del_gen(_del_gen == -1 ? 1 : _del_gen + 1),
      field_infos_gen(_field_infos_gen),
      next_write_field_infos_gen(_field_infos_gen == -1 ? 1 : _field_infos_gen + 1),
      doc_values_gen(_doc_values_gen),
      next_write_doc_values_gen(_doc_values_gen == -1 ? 1 : _doc_values_gen + 1),
      dv_updates_files(),
      field_infos_files(),
      size_in_bytes(-1),
      buffered_deletes_gen(-1) {
  if (!id.empty() && id.length() != StringHelper::ID_LENGTH) [[unlikely]] {
    // TODO
    throw 13;
  }
}

void SegmentCommitInfo::set_field_infos_files(std::unordered_set<std::string> _field_infos_files) {
  field_infos_files.clear();
  for (auto &file : _field_infos_files) {
    field_infos_files.emplace(info->named_for_this_segment(file));
  }
}

void SegmentCommitInfo::set_doc_values_updates_files(std::unordered_map<int32_t,
                                                                        std::unordered_set<std::string>> _dv_updates_files) {
  dv_updates_files.clear();
  for (auto &entry : _dv_updates_files) {
    // rename the set
    std::unordered_set<std::string> set;
    for (auto &file : entry.second) {
      set.emplace(info->named_for_this_segment(file));
    }
    dv_updates_files.emplace(entry.first, std::move(set));
  }
}

SegmentCommitInfoPtr SegmentCommitInfo::clone() {
  auto other =
      std::make_shared<SegmentCommitInfo>(
          info, del_count, soft_del_count, del_gen, field_infos_gen, doc_values_gen, id);
  // Not clear that we need to carry over nextWriteDelGen
  // (i.e. do we ever clone after a failed write and
  // before the next successful write?), but just do it to
  // be safe:
  other->next_write_del_gen = next_write_del_gen;
  other->next_write_field_infos_gen = next_write_field_infos_gen;
  other->next_write_doc_values_gen = next_write_doc_values_gen;

  // deep clone
  other->dv_updates_files.insert(dv_updates_files.begin(), dv_updates_files.end());
  other->field_infos_files.insert(field_infos_files.begin(), field_infos_files.end());

  return other;
}
