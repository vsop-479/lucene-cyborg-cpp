#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE94_LUCENE94FIELDINFOSFORMAT_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE94_LUCENE94FIELDINFOSFORMAT_HPP_

#include "../../index/FieldInfos.hpp"
#include "../../store/MMapDirectory.hpp"
#include "../../index/SegmentInfo.hpp"

namespace lucene::cyborg::codecs::v94 {

class Lucene94FieldInfosFormat final {
 public:
  index::FieldInfosPtr read(store::MMapDirectoryPtr directory,
                            index::SegmentInfoPtr segment_info,
                            const std::string &segment_suffix);
};  // Lucene94FieldInfosFormat



}  // lucene::cyborg::codecs::lucene94


#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE94_LUCENE94FIELDINFOSFORMAT_HPP_
