#ifndef LUCENE_CYBORG_CPP_SRC_STORE_BUFFEREDCHECKSUMMEMORYDATAINPUT_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_BUFFEREDCHECKSUMMEMORYDATAINPUT_HPP_

#include "CloseableMemoryDataInput.hpp"
#include "../util/BufferedChecksum.hpp"

namespace lucene::cyborg::store {

struct BufferedChecksumMemoryDataInput {
  util::ReadOnlyPtr<uint8_t> org_data = nullptr;
  util::ReadOnlyPtr<uint8_t> data = nullptr;
  int64_t offset = 0;
  int64_t length = 0;
  util::BufferedChecksum buffered_checksum{};

  BufferedChecksumMemoryDataInput(util::ReadOnlyPtr<uint8_t> data,
                                  int64_t offset,
                                  int64_t length,
                                  util::BufferedChecksum buffered_checksum);

  BufferedChecksumMemoryDataInput() = default;

  BufferedChecksumMemoryDataInput(const BufferedChecksumMemoryDataInput &other);

  BufferedChecksumMemoryDataInput(BufferedChecksumMemoryDataInput &&other) noexcept;

  BufferedChecksumMemoryDataInput &operator=(BufferedChecksumMemoryDataInput &&other) noexcept;

  BufferedChecksumMemoryDataInput &operator=(const BufferedChecksumMemoryDataInput &other);

  int64_t actual_length() const noexcept {
    return offset + length;
  }

  void update(uint8_t byte);

  void update(util::ReadOnlyPtr<uint8_t> bytes, int32_t len);
};  // BufferedChecksumMemoryDataInput



}  // lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_BUFFEREDCHECKSUMMEMORYDATAINPUT_HPP_