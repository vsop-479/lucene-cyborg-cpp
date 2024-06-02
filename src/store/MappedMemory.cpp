#include "MappedMemory.hpp"
#if defined(__linux__)
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

using lucene::cyborg::store::MappedMemory;
using lucene::cyborg::util::ReadOnlyPtr;

constexpr int32_t MAP_ALL_FILE = -1;

//
// Windows
//

#if defined(_WIN64)

static const auto MEMORY_GRANULARITY = []() {
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  return system_info.dwAllocationGranularity;
}();

namespace lucene::cyborg::store {

void windows_mmap(
    MappedMemory &mapped_memory,
    TCHAR *file_path,
    int64_t offset,
    int64_t length) {
  if (offset < 0) {
    throw 13;
  }

  // file handle
  HANDLE h_file;
  // mapping handle
  HANDLE h_map;
  // mapped memory
  LPVOID lp_base_ptr;
  // holding the mapping size
  LARGE_INTEGER li_file_size;

  h_file = CreateFile(file_path,
                      GENERIC_READ,               // dwDesiredAccess
                      FILE_SHARE_READ,            // dwShareMode, Enables subsequent open operations on a file or device to request read access.
                      nullptr,                    // lpSecurityAttributes, use the default
                      OPEN_EXISTING,              // dwCreationDisposition, opens a file or device, only if it exists.
                      FILE_ATTRIBUTE_NORMAL,      // dwFlagsAndAttributes
                      nullptr);                   // hTemplateFile, no attributes.

  if (h_file == INVALID_HANDLE_VALUE) {
    // TODO: Error
    throw 13;
  }

  if (length >= 0) {
    li_file_size.QuadPart = length;
  } else if (length == -1) {
    if (!GetFileSizeEx(h_file, &li_file_size)) {
      // TODO: Error
      throw 13;
    }
  } else {
    throw 13;
  }

  h_map = CreateFileMapping(
      h_file,
      nullptr,                       // Mapping attributes
      PAGE_READONLY,                 // Protection flags
      0,                             // MaximumSizeHigh
      0,          // MaximumSizeLow
      nullptr);                      // Name

  if (h_map == nullptr) {
    // TODO : Error
    CloseHandle(h_file);
    throw 13;
  }

  uint64_t calibrated_offset = offset;
  int32_t calibrated_distance = 0;
  if (calibrated_offset % MEMORY_GRANULARITY) {
    calibrated_offset = (offset / MEMORY_GRANULARITY) * MEMORY_GRANULARITY;
    calibrated_distance = offset - calibrated_offset;
    li_file_size.QuadPart += calibrated_distance;
  }

  lp_base_ptr = MapViewOfFile(
      h_map,
      FILE_MAP_READ,              // dwDesiredAccess
      (calibrated_offset >> 32U),                          // dwFileOffsetHigh
      calibrated_offset & 0xFFFFFFFFULL,                     // dwFileOffsetLow
      li_file_size.QuadPart);      // dwNumberOfBytesToMap

  if (lp_base_ptr == nullptr) {
    // TODO : Error
    CloseHandle(h_map);
    CloseHandle(h_file);
    throw 13;
  }

  mapped_memory.mapped_data_point = (ReadOnlyPtr<uint8_t>) lp_base_ptr;
  mapped_memory.data = mapped_memory.mapped_data_point + calibrated_distance;
  mapped_memory.offset = offset;
  mapped_memory.length = li_file_size.QuadPart - calibrated_distance;
  mapped_memory.h_file = h_file;
  mapped_memory.h_map = h_map;
}

}  // namespace lucene::cyborg::store


#endif  // End Windows

//
// Linux
//

#if defined(__linux__)

namespace lucene::cyborg::store {

void linux_mmap(
    MappedMemory &mapped_memory,
    const char *file_path,
    int64_t offset,
    int64_t length) {
  int fd = open(file_path, O_RDONLY);
  if (fd == -1) {
    // TODO
    throw 13;
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1) {
    // TODO
    throw 13;
  }

  const off_t pa_offset = ((off_t) offset) & ~(sysconf(_SC_PAGE_SIZE) - 1);
  if (offset >= sb.st_size) {
    // TODO : offset is past end of file
    throw 13;
  }

  if (length == MAP_ALL_FILE) {
    length = sb.st_size;
  }

  if ((offset + length) > sb.st_size) {
    length = sb.st_size - offset;
  }

  const auto calibrated_distance = offset - pa_offset;
  const auto actual_mapped_size = length + calibrated_distance;
  void *addr = mmap(NULL, actual_mapped_size, PROT_READ, MAP_PRIVATE, fd, pa_offset);
  close(fd);
  if (addr == MAP_FAILED) {
    // TODO
    throw 13;
  }

  //    <----- calibrated distance ----->
  // <pa_offset>                     <offset> <----- Length ----->
  //    ^-- addr

  mapped_memory.mapped_data_point = (ReadOnlyPtr<uint8_t>) addr;
  mapped_memory.data = mapped_memory.mapped_data_point + calibrated_distance;
  mapped_memory.offset = offset;
  mapped_memory.length = length;
  mapped_memory.actual_mapped_size = actual_mapped_size;
}

}  // namespace lucene::cyborg::store

#endif


//
// MappedMemory
//

MappedMemory MappedMemory::open(const std::string &file_path, int64_t offset, int64_t length) {
  MappedMemory mapped_memory{};
#if defined(_WIN64)
  windows_mmap(mapped_memory, (TCHAR *) file_path.c_str(), offset, length);
#endif
#if defined(__linux__)
  linux_mmap(mapped_memory, file_path.c_str(), offset, length);
#endif
  return mapped_memory;
}

MappedMemory MappedMemory::open(const std::string &file_path) {
  MappedMemory mapped_memory{};
#if defined(_WIN64)
  windows_mmap(mapped_memory, (TCHAR *) file_path.c_str(), 0, MAP_ALL_FILE);
#endif
#if defined(__linux__)
  linux_mmap(mapped_memory, file_path.c_str(), 0, MAP_ALL_FILE);
#endif
  return mapped_memory;
}

MappedMemory::MappedMemory(MappedMemory &&other) noexcept
    : mapped_data_point(other.mapped_data_point),
      data(other.data),
      offset(other.offset),
      length(other.length)
#if defined(_WIN64)
    , h_file(other.h_file),
      h_map(other.h_map)
#endif
#if defined(__linux__)
, actual_mapped_size(other.actual_mapped_size)
#endif
{
  other = MappedMemory{};
}

MappedMemory::~MappedMemory() {
  close();
}

MappedMemory &MappedMemory::operator=(MappedMemory &&other) noexcept {
  if (this != &other) {
    mapped_data_point = other.mapped_data_point;
    other.mapped_data_point = {};
    data = other.data;
    other.data = {};
    offset = other.offset;
    other.offset = {};
    length = other.length;
    other.length = {};
#if defined(_WIN64)
    h_file = other.h_file;
    other.h_file = {};
    h_map = other.h_map;
    other.h_map = {};
#endif
#if defined(__linux__)
    actual_mapped_size = other.actual_mapped_size;
    other.actual_mapped_size = {};
#endif
  }

  return *this;
}

void MappedMemory::close() {
  if (mapped_data_point) {
#if defined(_WIN64)
    UnmapViewOfFile((LPVOID) mapped_data_point);
    CloseHandle(h_map);
    CloseHandle(h_file);
#endif
#if defined(__linux__)
    munmap((void*) mapped_data_point, actual_mapped_size);
#endif

    *this = MappedMemory{};
  }
}
