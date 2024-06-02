#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_BUFFEREDCHECKSUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_BUFFEREDCHECKSUM_HPP_

#include <memory>
#include <cstdint>
#include "PointerTypes.hpp"

namespace lucene::cyborg::util {

class Crc32 {
 private:
  int32_t crc;

 public:
  explicit Crc32();

  void update(uint8_t b);

  void update(ReadOnlyPtr<uint8_t> bytes, int32_t off, int32_t len);

  [[nodiscard]] int64_t get_value() const noexcept;

  void reset() noexcept;
};  // Crc32




class BufferedChecksum {
 public:
  static constexpr int32_t DEFAULT_BUFFER_SIZE = 1024;

  explicit BufferedChecksum();

  explicit BufferedChecksum(int32_t buffer_size);

  BufferedChecksum(const BufferedChecksum &other);

  BufferedChecksum(BufferedChecksum &&other);

  BufferedChecksum &operator=(const BufferedChecksum &other);

  BufferedChecksum &operator=(BufferedChecksum &&other) noexcept;

  void update(uint8_t b);

  void update(ReadOnlyPtr<uint8_t> bytes, int32_t off, int32_t len);

  [[nodiscard]] int64_t get_value();

  void reset();

 private:
  void flush();

  Crc32 crc_32;
  std::unique_ptr<uint8_t[]> buffer;
  uint32_t buffer_size;
  uint32_t upto;
};  // BufferedChecksum



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_BUFFEREDCHECKSUM_HPP_
