#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSFORMAT_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSFORMAT_HPP_

#include <string_view>
#include "Lucene90NormsProducer.hpp"

namespace lucene::cyborg::codecs::v90 {

struct Lucene90NormsFormat {
  static constexpr int VERSION_START = 0;
  static constexpr int VERSION_CURRENT = VERSION_START;
  static constexpr std::string_view DATA_CODEC = "Lucene90NormsData";
  static constexpr std::string_view DATA_EXTENSION = "nvd";
  static constexpr std::string_view METADATA_CODEC = "Lucene90NormsMetadata";
  static constexpr std::string_view METADATA_EXTENSION = "nvm";

  Lucene90NormsProducerPtr create_norms_producer(util::ReadOnlyPtr<index::SegmentReadState> read_state);
};  // Lucene90NormsFormat



}  // namespace lucene::cyborg::codec::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSFORMAT_HPP_
