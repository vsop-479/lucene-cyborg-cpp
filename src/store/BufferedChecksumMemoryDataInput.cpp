#include "BufferedChecksumMemoryDataInput.hpp"

using lucene::cyborg::store::BufferedChecksumMemoryDataInput;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::ReadOnlyPtr;

//
// BufferedChecksumMemoryDataInput
//

BufferedChecksumMemoryDataInput::BufferedChecksumMemoryDataInput(
    ReadOnlyPtr<uint8_t> data,
    int64_t offset,
    int64_t length,
    BufferedChecksum buffered_checksum)
    : org_data(data),
      data(data),
      offset(offset),
      length(length),
      buffered_checksum(std::move(buffered_checksum)) {
}

BufferedChecksumMemoryDataInput::BufferedChecksumMemoryDataInput(const BufferedChecksumMemoryDataInput &other)
    : org_data(),
      data(),
      offset(),
      length(),
      buffered_checksum(other.buffered_checksum) {
}

BufferedChecksumMemoryDataInput::BufferedChecksumMemoryDataInput(BufferedChecksumMemoryDataInput &&other) noexcept
    : org_data(),
      data(),
      offset(),
      length(),
      buffered_checksum(std::forward<BufferedChecksum>(other.buffered_checksum)) {
}

BufferedChecksumMemoryDataInput &BufferedChecksumMemoryDataInput::operator=(
    BufferedChecksumMemoryDataInput &&other) noexcept {
  if (this != &other) {
    org_data = other.org_data;
    data = other.data;
    offset = other.offset;
    length = other.length;
    buffered_checksum = std::move(other.buffered_checksum);
  }
  return *this;
}

BufferedChecksumMemoryDataInput &BufferedChecksumMemoryDataInput::operator=(
    const BufferedChecksumMemoryDataInput &other) {
  if (this != &other) {
    org_data = other.org_data;
    data = other.data;
    offset = other.offset;
    length = other.length;
    buffered_checksum = other.buffered_checksum;
  }
  return *this;
}

void BufferedChecksumMemoryDataInput::update(uint8_t byte) {
  buffered_checksum.update(byte);
}

void BufferedChecksumMemoryDataInput::update(ReadOnlyPtr<uint8_t> bytes, int32_t len) {
  buffered_checksum.update(bytes, 0, len);
}
