#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTREADER_HPP_

#include <cstdint>
#include <memory>
#include "SegmentCommitInfo.hpp"
#include "SegmentCoreReaders.hpp"
#include "FieldInfos.hpp"
#include "LeafMetaData.hpp"
#include "../util/Bits.hpp"
#include "../index/NumericDocValues.hpp"

namespace lucene::cyborg::index {

class SegmentReader {
 public:
  SegmentReader(SegmentCommitInfoPtr _si, int32_t _created_version_major);

  int32_t get_max_doc() const noexcept {
    return si->info->max_doc;
  }

  FieldInfosPtr init_field_infos();

  util::ManagedPtr<util::Bits> get_live_docs();

  util::ManagedPtr<index::NumericDocValues> get_norm_values(
      util::ManagedPtr<util::LinearAllocator> allocator,
      std::string_view field);

  SegmentCommitInfoPtr si;
  SegmentCommitInfoPtr org_si;
  SegmentCoreReadersPtr segment_core_readers;
  FieldInfosPtr field_infos;
  LeafMetaData leaf_meta_data;
  int32_t num_docs;
  bool is_nrt;
};  // SegmentReader

using SegmentReaderPtr = std::shared_ptr<SegmentReader>;

}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTREADER_HPP_
