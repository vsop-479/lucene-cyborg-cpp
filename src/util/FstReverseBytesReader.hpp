#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_FSTREVERSEBYTESREADER_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_FSTREVERSEBYTESREADER_HPP_

#include <string>
#include <unordered_map>
#include <unordered_set>
#include "PointerTypes.hpp"
#include "MemoryUtils.hpp"

namespace lucene::cyborg::util {

struct FstReverseBytesReader {
  static void copy_to_dest(OverwriteBytes target,
                           ReadOnlyPtr<uint8_t> source,
                           int32_t len,
                           int32_t target_offset = 0) noexcept;

  FstReverseBytesReader(ReadOnlyPtr<uint8_t> data, int32_t length);

  uint32_t read_byte() noexcept;

  int64_t get_file_pointer() const noexcept;

  void seek(int64_t fp) noexcept;

  int32_t read_int() noexcept;

  int16_t read_short() noexcept;

  int64_t read_long() noexcept;

  uint32_t read_be_int() noexcept;

  uint64_t read_be_long() noexcept;

  void skip_bytes(int32_t len);

  void read_bytes(OverwriteBytes target,
                  int32_t len,
                  int32_t offset = 0) noexcept;

  int32_t read_vint();

  int64_t read_vlong(bool allow_negative = false);

  std::string read_string_slow();

  std::unordered_map<std::string, std::string> read_map_of_strings_slow();

  std::unordered_set<std::string> read_set_of_strings_slow();

  ReadOnlyPtr<uint8_t> start_data;
  ReadOnlyPtr<uint8_t> data;
  int32_t length;
};  // FstReverseBytesReader



}  // lucene::cyborg::util


#endif //LUCENE_CYBORG_CPP_SRC_UTIL_FSTREVERSEBYTESREADER_HPP_
