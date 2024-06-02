#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_FIXEDBITSET_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_FIXEDBITSET_HPP_

#include <cstdint>

namespace lucene::cyborg::util {

class FixedBitSet {
 public:
  static constexpr int32_t bits_to_words(const int32_t num_bits) noexcept {
    return ((num_bits - 1) >> 6) + 1;
  }
};  // FixedBitSet



}  // namespace lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_FIXEDBITSET_HPP_
