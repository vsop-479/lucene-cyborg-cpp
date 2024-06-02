#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTCOREREADERS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTCOREREADERS_HPP_

#include <memory>
#include "FieldInfos.hpp"
#include "../store/MMapDirectory.hpp"
#include "SegmentCommitInfo.hpp"
#include "Fields.hpp"
#include "../codecs/NormsProducer.hpp"

namespace lucene::cyborg::index {

class SegmentCoreReaders {
 public:
  SegmentCoreReaders(store::MMapDirectoryPtr _dir, SegmentCommitInfoPtr _si);

  std::string segment;
  FieldInfosPtr core_field_infos;
  FieldsPtr fields;
  codecs::NormsProducerPtr norms_producer;
};  // SegmentCoreReaders

using SegmentCoreReadersPtr = std::shared_ptr<SegmentCoreReaders>;

}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SEGMENTCOREREADERS_HPP_
