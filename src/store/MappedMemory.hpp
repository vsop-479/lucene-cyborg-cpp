#ifndef LUCENE_CYBORG_CPP_SRC_STORE_MAPPEDMEMORY_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_MAPPEDMEMORY_HPP_

#include <cstdint>
#include <string>
#include "../util/PointerTypes.hpp"

#if defined(_WIN64)
#include <windows.h>
#endif

namespace lucene::cyborg::store {

class MappedMemory {
 public:
#if defined(_WIN64)
  friend void windows_mmap(MappedMemory &, TCHAR *, int64_t, int64_t);
#endif
#if defined(__linux__)
  friend void linux_mmap(MappedMemory &, const char*, int64_t, int64_t);
#endif

  static MappedMemory open(const std::string &file_path, int64_t offset, int64_t length);

  static MappedMemory open(const std::string &file_path);

  MappedMemory() = default;

  MappedMemory(const MappedMemory &) = delete;

  MappedMemory(MappedMemory &&other) noexcept;

  MappedMemory &operator=(const MappedMemory &) = delete;

  MappedMemory &operator=(MappedMemory &&) noexcept;

  ~MappedMemory();

  void close();

  util::ReadOnlyPtr<uint8_t> mapped_data_point;
  util::ReadOnlyPtr<uint8_t> data;
  int64_t offset;
  int64_t length;

 private:
#if defined(_WIN64)
  HANDLE h_file;
  HANDLE h_map;
#endif

#if defined(__linux__)
  uint64_t actual_mapped_size;
#endif
};  // MappedMemory



}  // namespace lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_MAPPEDMEMORY_HPP_
