#ifndef LUCENE_CYBORG_CPP_SRC_STORE_DATA_INPUT_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_DATA_INPUT_HPP_

#include <cassert>
#include <cstdint>
#include <unordered_map>
#include <unordered_set>
#include <string>
#include "../util/PointerTypes.hpp"
#include "../util/MemoryUtils.hpp"

namespace lucene::cyborg::store {

struct DataDecoding {
  DataDecoding() = delete;

  static uint64_t zig_zag_decode(uint64_t l) noexcept {
    return ((l >> 1) ^ (~(l & 1U) + 1U));
  }

  template<typename T>
  static uint32_t read_byte(util::UseOnlyPtr<T> &data) noexcept {
    return (uint32_t) *data++;
  }

  template<typename T>
  static uint32_t read_short(util::UseOnlyPtr<T> &data) noexcept {
    const auto b1 = read_byte(data);
    const auto b2 = read_byte(data);
    return b1 | (b2 << 8U);
  }

  template<typename T>
  static uint32_t read_int(util::UseOnlyPtr<T> &data) noexcept {
    const auto b1 = read_byte(data);
    const auto b2 = read_byte(data);
    const auto b3 = read_byte(data);
    const auto b4 = read_byte(data);
    return b1 | (b2 << 8U) | (b3 << 16U) | (b4 << 24U);
  }

  template<typename T>
  static uint64_t read_long(util::UseOnlyPtr<T> &data) noexcept {
    const uint64_t b1 = read_byte(data);
    const uint64_t b2 = read_byte(data);
    const uint64_t b3 = read_byte(data);
    const uint64_t b4 = read_byte(data);
    const uint64_t b5 = read_byte(data);
    const uint64_t b6 = read_byte(data);
    const uint64_t b7 = read_byte(data);
    const uint64_t b8 = read_byte(data);

    return b1 | (b2 << 8U) | (b3 << 16U) | (b4 << 24U)
        | (b5 << 32U) | (b6 << 40U) | (b7 << 48U) | (b8 << 56U);
  }

  template<typename T>
  static uint32_t read_be_int(util::ReadOnlyPtr<T> &data) noexcept {
    uint32_t res = read_byte(data) << 24U;
    res |= read_byte(data) << 16U;
    res |= read_byte(data) << 8U;
    res |= read_byte(data);
    return res;
  }

  template<typename T>
  static uint64_t read_be_long(util::ReadOnlyPtr<T> &data) noexcept {
    uint64_t upper = read_be_int(data);
    upper <<= 32U;
    const uint64_t lower = read_be_int(data) & 0xFFFFFFFFLU;
    return upper | lower;
  }

  template<typename T>
  static uint32_t read_vint(util::ReadOnlyPtr<T> &data) {
    auto b = read_byte(data);
    if (b < 128U) return b;
    uint32_t i = b & 0x7FU;

    b = read_byte(data);
    i |= (b & 0x7FU) << 7U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FU) << 14U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FU) << 21U;
    if (b < 128U) return i;

    b = read_byte(data);
    // Warning: the next ands use 0x0F / 0xF0 - beware copy/paste errors:
    i |= (b & 0x0FU) << 28U;
    if ((b & 0xF0U) == 0) return i;
    // TODO : Throw
    throw 13;
  }

  template<typename T>
  static uint64_t read_zlong(util::UseOnlyPtr<T> &data) {
    return zig_zag_decode(read_vlong(data, true));
  }

  template<typename T>
  static uint64_t read_vlong(util::UseOnlyPtr<T> &data, bool allow_negative = false) {
    uint64_t b = read_byte(data);
    if (b < 128U) return b;
    uint64_t i = b & 0x7FLU;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 7U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 14U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 21;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 28U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 35U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 42U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 49U;
    if (b < 128U) return i;

    b = read_byte(data);
    i |= (b & 0x7FLLU) << 56U;
    if (b < 128U) return i;

    if (allow_negative) {
      b = read_byte(data);
      i |= (b & 0x7FLLU) << 63U;
      if (b == 0 || b == 1) return i;
      // TODO
      throw 13;
      // throw new IOException("Invalid vLong detected (more than 64 bits)");
    } else {
      // TODO
      throw 13;
      // throw new IOException("Invalid vLong detected (negative values disallowed)");
    }
  }

  template<typename T>
  static void skip_variant_integer(util::UseOnlyPtr<T> &data) {
    while ((*data++) & 0x80U);
  }

  template<typename T>
  static void skip_variant_integer(util::UseOnlyPtr<T> &data, int32_t num_skips) {
    while (num_skips > 0) {
      skip_variant_integer(data);
      --num_skips;
    }
  }
};  // DataDecoding


template<typename Input>
class DataInput {
 private:
  static constexpr bool has_byte_update = requires(Input input) {
    input.update((uint8_t) 0);
  };

  static constexpr bool has_bytes_update = requires(Input input) {
    input.update((util::ReadOnlyPtr<uint8_t>) nullptr, (int32_t) 0);
  };

 public:
  uint32_t read_byte() noexcept {
    if constexpr (has_byte_update) {
      const auto ret = DataDecoding::read_byte(input.data);
      input.update(ret);
      return ret;
    } else {
      return DataDecoding::read_byte(input.data);
    }
  }

  uint64_t get_file_pointer() const noexcept {
    return (((uint64_t) input.data) - ((uint64_t) input.org_data)) + input.offset;
  }

  void seek(int64_t fp) noexcept {
    assert(fp >= input.offset);
    assert((fp - input.offset) < input.length);
    input.data = input.org_data + (fp - input.offset);
  }

  uint32_t read_short() noexcept {
    if constexpr (has_byte_update) {
      const auto org_data = input.data;
      const auto ret = DataDecoding::read_short(input.data);
      input.update(org_data, sizeof(int16_t));
      return ret;
    } else {
      return DataDecoding::read_short(input.data);
    }
  }

  uint32_t read_int() noexcept {
    if constexpr (has_byte_update) {
      const auto org_data = input.data;
      const auto ret = DataDecoding::read_int(input.data);
      input.update(org_data, sizeof(int32_t));
      return ret;
    } else {
      return DataDecoding::read_int(input.data);
    }
  }

  uint64_t read_long() noexcept {
    if constexpr (has_byte_update) {
      const auto org_data = input.data;
      const auto ret = DataDecoding::read_long(input.data);
      input.update(org_data, sizeof(int64_t));
      return ret;
    } else {
      return DataDecoding::read_long(input.data);
    }
  }

  template<int32_t N>
  void read_longs(uint64_t dest[]) {
    for (int32_t i = 0; i < N; ++i) {
      *dest++ = read_long();
    }
  }

  void read_longs(uint64_t dest[], const int32_t length) {
    for (int32_t i = 0; i < length; ++i) {
      *dest++ = read_long();
    }
  }

  uint64_t read_zlong() {
    if constexpr (has_byte_update) {
      const auto org_data = input.data;
      const auto ret = DataDecoding::read_zlong(input.data);
      input.update(org_data, sizeof(int64_t));
      return ret;
    } else {
      return DataDecoding::read_zlong(input.data);
    }
  }

  uint32_t read_be_int() noexcept {
    if constexpr (has_byte_update) {
      const auto org_data = input.data;
      const auto ret = DataDecoding::read_be_int(input.data);
      input.update(org_data, sizeof(int32_t));
      return ret;
    } else {
      return DataDecoding::read_be_int(input.data);
    }
  }

  uint64_t read_be_long() noexcept {
    if constexpr (has_byte_update) {
      const auto org_data = input.data;
      const auto ret = DataDecoding::read_be_long(input.data);
      input.update(org_data, sizeof(int64_t));
      return ret;
    } else {
      return DataDecoding::read_be_long(input.data);
    }
  }

  void skip_bytes(int32_t len) {
    input.data += len;
  }

  void skip_variant_integer() {
    DataDecoding::skip_variant_integer(input.data);
  }

  void read_bytes(util::OverwriteBytes dest,
                  int32_t len,
                  int32_t offset = 0) noexcept {
    util::MemoryUtils::memcpy(dest + offset, input.data, len);
    if constexpr (has_bytes_update) {
      input.update(input.data, len);
    }
    input.data += len;
  }

  uint32_t read_vint() {
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

  uint64_t read_vlong(bool allow_negative = false) {
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
    }
  }

  /**
   * A combination of ReadVInt + ReadBytes.
   * @return
   */
  std::string read_string_slow() {
    const int32_t length = read_vint();
    std::string string(length, '\0');
    read_bytes(string.data(), length);
    return string;
  }

  std::unordered_map<std::string, std::string> read_map_of_strings_slow() {
    const int32_t count = read_vint();
    std::unordered_map<std::string, std::string> map(count);
    for (int32_t i = 0; i < count; ++i) {
      auto key = read_string_slow();
      auto value = read_string_slow();
      map.emplace(std::move(key), std::move(value));
    }
    return map;
  }

  std::unordered_set<std::string> read_set_of_strings_slow() {
    const int32_t count = read_vint();
    std::unordered_set<std::string> set(count);
    for (int32_t i = 0; i < count; ++i) {
      set.emplace(read_string_slow());
    }
    return set;
  }

  Input input;
};  // DataInput



}  // lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_DATA_INPUT_HPP_
