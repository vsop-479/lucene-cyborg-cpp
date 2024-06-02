#include <cassert>
#include "FstReverseBytesReader.hpp"

using lucene::cyborg::util::FstReverseBytesReader;
using lucene::cyborg::util::ReadOnlyPtr;

//
// FstReverseBytesReader
//

FstReverseBytesReader::FstReverseBytesReader(ReadOnlyPtr<uint8_t> data, int32_t length)
    : start_data(data),
      data(data),
      length(length) {
}

uint32_t FstReverseBytesReader::read_byte() noexcept {
  assert(data >= start_data);
  assert(data < (start_data + length));
  return *data--;
}

int64_t FstReverseBytesReader::get_file_pointer() const noexcept {
  return data - start_data;
}

void FstReverseBytesReader::seek(int64_t fp) noexcept {
  assert(fp >= 0);
  assert(fp < length);
  data = start_data + fp;
}

int32_t FstReverseBytesReader::read_int() noexcept {
  const auto b1 = read_byte();
  const auto b2 = read_byte();
  const auto b3 = read_byte();
  const auto b4 = read_byte();
  return b1 | (b2 << 8U) | (b3 << 16U) | (b4 << 24U);
}

int16_t FstReverseBytesReader::read_short() noexcept {
  const auto b1 = read_byte();
  const auto b2 = read_byte();
  return b1 | (b2 << 8U);
}

int64_t FstReverseBytesReader::read_long() noexcept {
  const uint64_t b1 = read_byte();
  const uint64_t b2 = read_byte();
  const uint64_t b3 = read_byte();
  const uint64_t b4 = read_byte();
  const uint64_t b5 = read_byte();
  const uint64_t b6 = read_byte();
  const uint64_t b7 = read_byte();
  const uint64_t b8 = read_byte();

  return b1 | (b2 << 8U) | (b3 << 16U) | (b4 << 24U)
      | (b5 << 32U) | (b6 << 40U) | (b7 << 48U) | (b8 << 56U);
}

uint32_t FstReverseBytesReader::read_be_int() noexcept {
  uint32_t res = read_byte() << 24U;
  res |= read_byte() << 16U;
  res |= read_byte() << 8U;
  res |= read_byte();
  return res;
}

uint64_t FstReverseBytesReader::read_be_long() noexcept {
  uint64_t upper = read_be_int();
  upper <<= 32;
  const uint64_t lower = read_be_int() & 0xFFFFFFFFLU;
  return upper | lower;
}

void FstReverseBytesReader::skip_bytes(int32_t len) {
  data -= len;
}

void FstReverseBytesReader::read_bytes(OverwriteBytes target,
                                       int32_t len,
                                       int32_t offset/* = 0*/) noexcept {
  assert((data - len) >= start_data);
  copy_to_dest(target, data, len);
  data -= len;
}

int32_t FstReverseBytesReader::read_vint() {
  auto b = read_byte();
  if (b < 128U) return b;
  uint32_t i = b & 0x7FU;

  b = read_byte();
  i |= (b & 0x7FU) << 7U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FU) << 14U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FU) << 21U;
  if (b < 128U) return i;

  b = read_byte();
  // Warning: the next ands use 0x0F / 0xF0 - beware copy/paste errors:
  i |= (b & 0x0FU) << 28U;
  if ((b & 0xF0U) == 0) return i;
  // TODO : Throw
  throw 13;
}

int64_t FstReverseBytesReader::read_vlong(bool allow_negative/* = false*/) {
  uint64_t b = read_byte();
  if (b < 128U) return b;
  uint64_t i = b & 0x7FLU;

  b = read_byte();
  i |= (b & 0x7FLLU) << 7U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 14U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 21;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 28U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 35U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 42U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 49U;
  if (b < 128U) return i;

  b = read_byte();
  i |= (b & 0x7FLLU) << 56U;
  if (b < 128U) return i;

  if (allow_negative) {
    b = read_byte();
    i |= (b & 0x7FLLU) << 63U;
    if (b == 0 || b == 1) return i;
    // TODO
    // throw new IOException("Invalid vLong detected (more than 64 bits)");
    throw 13;
  } else {
    // TODO
    // throw new IOException("Invalid vLong detected (negative values disallowed)");
    throw 13;
  }  // End if
}

std::string FstReverseBytesReader::read_string_slow() {
  const int32_t length = read_vint();
  std::string string(length, '\0');
  read_bytes(string.data(), length);
  return string;
}

std::unordered_map<std::string, std::string> FstReverseBytesReader::read_map_of_strings_slow() {
  const int32_t count = read_vint();
  std::unordered_map<std::string, std::string> map(count);
  for (int32_t i = 0; i < count; ++i) {
    auto key = read_string_slow();
    auto value = read_string_slow();
    map.emplace(std::move(key), std::move(value));
  }
  return map;
}

std::unordered_set<std::string> FstReverseBytesReader::read_set_of_strings_slow() {
  const int32_t count = read_vint();
  std::unordered_set<std::string> set(count);
  for (int32_t i = 0; i < count; ++i) {
    set.emplace(read_string_slow());
  }
  return set;
}

void FstReverseBytesReader::copy_to_dest(OverwriteBytes target,
                                         ReadOnlyPtr<uint8_t> source,
                                         int32_t len,
                                         int32_t target_offset/*=0*/) noexcept {
  if (len > 0) {
    auto src = source - (len - 1);
    auto dest = target + target_offset + len - 1;
    auto until = source + 1;
    while (src != until) {
      *dest = *src;
      --dest;
      ++src;
    }
  }  // End if
}
