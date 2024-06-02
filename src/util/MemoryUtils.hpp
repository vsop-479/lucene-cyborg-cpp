#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_MEMORYUTILS_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_MEMORYUTILS_HPP_

#include <cstdint>
#include <cstring>
#include <bit>

namespace lucene::cyborg::util {

class MemoryUtils {
 public:
  static void memcpy(void *dest, const void *src, int32_t len) noexcept {
    if (len > 0) {
      std::memcpy(dest, src, len);
    }
  }

  static void memset(void *dest, const int32_t val, const int32_t size) {
    if (size > 0) {
      std::memset(dest, val, size);
    }
  }

  static int32_t oversize(int32_t size) noexcept {
    return std::bit_ceil<uint32_t>(size);
  }

 private:
  MemoryUtils() = default;
};  // MemoryUtils

}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_MEMORYUTILS_HPP_
