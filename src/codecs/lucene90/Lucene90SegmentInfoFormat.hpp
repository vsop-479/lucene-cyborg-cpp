#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SEGMENTINFOFORMAT_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SEGMENTINFOFORMAT_HPP_

#include <string>
#include <cstdint>
#include "../../store/MMapDirectory.hpp"
#include "../../index/SegmentInfo.hpp"

namespace lucene::cyborg::codecs::v90 {

struct Lucene90SegmentInfoFormat {
  // File extension used to store SegmentInfo.
  static const std::string SI_EXTENSION;
  static const std::string CODEC_NAME;
  static constexpr int32_t VERSION_START = 0;
  static constexpr int32_t VERSION_CURRENT = VERSION_START;

  index::SegmentInfoPtr read(store::MMapDirectoryPtr dir,
                             const std::string &segment,
                             const std::string &segment_id);
};  // Lucene90SegmentInfoFormat



}  // lucene::cyborg::codecs::v90


#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90SEGMENTINFOFORMAT_HPP_
