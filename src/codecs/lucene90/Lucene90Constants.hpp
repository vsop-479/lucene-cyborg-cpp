#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90CONSTANTS_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90CONSTANTS_HPP_

#include <cstdint>

namespace lucene::cyborg::codecs::v90 {

struct Lucene90Constants {
  static constexpr int32_t MAX_SKIP_LEVELS = 10;
  static constexpr int32_t SKIP_MULTIPLIER = 8;
};  // Lucene90Constants



}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90CONSTANTS_HPP_
