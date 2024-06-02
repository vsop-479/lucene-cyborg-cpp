#include "BufferedChecksum.hpp"
#include "ZLibCrc32.h"
#include "MemoryUtils.hpp"

using lucene::cyborg::util::Crc32;
using lucene::cyborg::util::BufferedChecksum;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::MemoryUtils;

//
// Crc32
//
Crc32::Crc32()
    : crc(0) {
}

void Crc32::update(const uint8_t b) {
  update(&b, 0, 1);
}

void Crc32::update(ReadOnlyPtr<uint8_t> bytes, const int32_t off, const int32_t len) {
  crc = _l_crc32(crc,
                 reinterpret_cast<const unsigned char *>(bytes + off),
                 len);
}

int64_t Crc32::get_value() const noexcept {
  return (static_cast<int64_t>(crc)) & 0xFFFFFFFFL;
}

void Crc32::reset() noexcept {
  crc = 0;
}

//
// BufferedChecksum
//

BufferedChecksum::BufferedChecksum()
    : crc_32(),
      buffer(std::make_unique<uint8_t[]>(BufferedChecksum::DEFAULT_BUFFER_SIZE)),
      buffer_size(BufferedChecksum::DEFAULT_BUFFER_SIZE),
      upto(0) {
}

BufferedChecksum::BufferedChecksum(const int32_t buffer_size)
    : crc_32(),
      buffer(std::make_unique<uint8_t[]>(buffer_size)),
      buffer_size(buffer_size),
      upto(0) {
}

BufferedChecksum::BufferedChecksum(const BufferedChecksum &other)
    : crc_32(other.crc_32),
      buffer(),
      buffer_size(other.buffer_size),
      upto(other.upto) {
  buffer = std::make_unique<uint8_t[]>(buffer_size);
  MemoryUtils::memcpy(buffer.get(), other.buffer.get(), upto);
}

BufferedChecksum::BufferedChecksum(BufferedChecksum &&other)
    : crc_32(other.crc_32),
      buffer(std::move(other.buffer)),
      buffer_size(other.buffer_size),
      upto(other.upto) {
}

void BufferedChecksum::flush() {
  if (upto > 0) {
    crc_32.update(buffer.get(), 0, upto);
    upto = 0;
  }
}

void BufferedChecksum::update(const uint8_t b) {
  if (upto == buffer_size) {
    flush();
  }

  buffer[upto++] = b;
}

void BufferedChecksum::update(ReadOnlyPtr<uint8_t> bytes, const int32_t off, const int32_t len) {
  if (len >= buffer_size) {
    flush();
    crc_32.update(bytes, off, len);
  } else {
    if ((upto + len) > buffer_size) {
      flush();
    }

    MemoryUtils::memcpy(buffer.get() + upto, bytes + off, len);
    upto += len;
  }
}

int64_t BufferedChecksum::get_value() {
  flush();
  return crc_32.get_value();
}

void BufferedChecksum::reset() {
  upto = 0;
  crc_32.reset();
}

BufferedChecksum &BufferedChecksum::operator=(const BufferedChecksum &other) {
  if (this != &other) {
    crc_32 = other.crc_32;
    buffer_size = other.buffer_size;
    upto = other.upto;
    buffer = std::make_unique<uint8_t[]>(buffer_size);
    MemoryUtils::memcpy(buffer.get(), other.buffer.get(), upto);
  }
  return *this;
}

BufferedChecksum &BufferedChecksum::operator=(BufferedChecksum &&other) noexcept {
  if (this != &other) {
    crc_32 = other.crc_32;
    other.crc_32 = Crc32{};
    buffer = std::move(other.buffer);
    other.buffer = {};
    buffer_size = other.buffer_size;
    other.buffer_size = {};
    upto = other.upto;
    other.upto = {};
  }
  return *this;
}
