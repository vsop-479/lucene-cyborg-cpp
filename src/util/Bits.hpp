#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_BITS_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_BITS_HPP_

#include <cstdint>

namespace lucene::cyborg::util {

struct Bits {
  virtual bool get(int32_t index) = 0;

  int32_t get_length() const noexcept {
    return len;
  }

 private:
  int32_t len;
};  // Bits



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_BITS_HPP_
