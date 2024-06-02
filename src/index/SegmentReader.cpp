#include <cassert>
#include "SegmentReader.hpp"

using lucene::cyborg::index::FieldInfosPtr;
using lucene::cyborg::index::NumericDocValues;
using lucene::cyborg::index::SegmentCommitInfoPtr;
using lucene::cyborg::index::SegmentReader;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

SegmentReader::SegmentReader(SegmentCommitInfoPtr _si,
                             int32_t _created_version_major)
    : si(_si->clone()),
      org_si(_si),
      segment_core_readers(),
      field_infos(),
      leaf_meta_data(_created_version_major, _si->info->min_version),
      num_docs(),
    // We pull liveDocs/DV updates from disk:
      is_nrt(false) {
  segment_core_readers = std::make_shared<SegmentCoreReaders>(_si->info->dir, _si);

  if (_si->has_deletions()) {
    // TODO
    throw 13;
  } else {
    assert(_si->del_count == 0);
    // TODO
    // hard_live_docs = live_docs = null;
  }
  num_docs = _si->info->max_doc - _si->del_count;
  field_infos = init_field_infos();
}

FieldInfosPtr SegmentReader::init_field_infos() {
  if (!si->has_field_updates()) {
    return segment_core_readers->core_field_infos;
  } else {
    // TODO : Impl
    throw 13;
  }
}

ManagedPtr<Bits> SegmentReader::get_live_docs() {
  // TODO :
  return nullptr;
}

ManagedPtr<NumericDocValues> SegmentReader::get_norm_values(
    ManagedPtr<LinearAllocator> allocator, std::string_view field) {
  auto field_info = field_infos->field_info(field).get();
  if (!field_info || !field_info->has_norms()) {
    // Field does not exist or does not index norms
    return nullptr;
  }

  return segment_core_readers->norms_producer->get_norms(allocator, field_info);
}
