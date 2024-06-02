#ifndef LUCENE_CYBORG_CPP_SRC_STORE_MEASUREDATAINPUT_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_MEASUREDATAINPUT_HPP_

#include <optional>
#include <cstdint>
#include "../util/PointerTypes.hpp"

namespace lucene::cyborg::store {

struct MeasureDataInput {
  MeasureDataInput(util::ReadOnlyPtr<uint8_t> _data, int32_t _length) noexcept;

  std::optional<uint32_t> read_byte() noexcept;

  std::optional<int32_t> read_vint();

  std::optional<int64_t> read_vlong(bool allow_negative = false);

  bool skip_bytes(int32_t len) noexcept;

  void seek(int64_t fp) noexcept;

  util::ReadOnlyPtr<uint8_t> org_data;
  util::ReadOnlyPtr<uint8_t> end_data;
  util::ReadOnlyPtr<uint8_t> data;
  int32_t length;
};  // MeasureDataInput



}  // lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_MEASUREDATAINPUT_HPP_