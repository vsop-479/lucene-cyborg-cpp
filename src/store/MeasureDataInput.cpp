#include "MeasureDataInput.hpp"

using lucene::cyborg::store::MeasureDataInput;
using lucene::cyborg::util::ReadOnlyPtr;

//
// MeasureDataInput
//

MeasureDataInput::MeasureDataInput(ReadOnlyPtr<uint8_t> _data, int32_t _length) noexcept
    : org_data(_data),
      end_data(_data + _length),
      data(_data),
      length(_length) {
}

std::optional<uint32_t> MeasureDataInput::read_byte() noexcept {
  if (data >= end_data) {
    return std::nullopt;
  }
  return *data++;
}

std::optional<int32_t> MeasureDataInput::read_vint() {
  auto b = read_byte();
  if (!b) return std::nullopt;
  if (*b < 128U) return *b;
  uint32_t i = *b & 0x7FU;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FU) << 7U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FU) << 14U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FU) << 21U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  // Warning: the next ands use 0x0F / 0xF0 - beware copy/paste errors:
  i |= (*b & 0x0FU) << 28U;
  if ((*b & 0xF0U) == 0) return i;
  // TODO : Throw
  throw 13;
}

std::optional<int64_t> MeasureDataInput::read_vlong(bool allow_negative/* = false*/) {
  auto b = read_byte();
  if (!b) return std::nullopt;
  if (*b < 128U) return b;
  uint64_t i = *b & 0x7FLU;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 7U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 14U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 21;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 28U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 35U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 42U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 49U;
  if (*b < 128U) return i;

  b = read_byte();
  if (!b) return std::nullopt;
  i |= (*b & 0x7FLLU) << 56U;
  if (*b < 128U) return i;

  if (allow_negative) {
    b = read_byte();
    if (!b) return std::nullopt;
    i |= (*b & 0x7FLLU) << 63U;
    if (*b == 0 || *b == 1) return i;
    // TODO
    // throw new IOException("Invalid vLong detected (more than 64 bits)");
    throw 13;
  } else {
    // TODO
    // throw new IOException("Invalid vLong detected (negative values disallowed)");
    throw 13;
  }
}

bool MeasureDataInput::skip_bytes(int32_t len) noexcept {
  if ((data + len) <= end_data) {
    data += len;
    return true;
  }
  return false;
}

void MeasureDataInput::seek(int64_t fp) noexcept {
  data = org_data + fp;
}
