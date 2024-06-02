#include <filesystem>
#include <sys/stat.h>
#include "MMapDirectory.hpp"
#include "MappedMemory.hpp"

using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MappedMemory;

//
// MMapDirectory
//
MMapDirectory::MMapDirectory(const std::string &path)
    : path(path) {
}

CloseableMemoryDataInput MMapDirectory::open_input_with_region(const std::string &file_name,
                                                               int64_t offset,
                                                               int64_t length) {
  auto file_path = path / file_name;
  auto mapped_memory = MappedMemory::open(file_path.string(), offset, length);
  return CloseableMemoryDataInput{std::move(mapped_memory)};
}

CloseableMemoryDataInput MMapDirectory::open_input(const std::string &file_name) {
  auto file_path = path / file_name;
  auto mapped_memory = MappedMemory::open(file_path.string());
  return CloseableMemoryDataInput{std::move(mapped_memory)};
}

std::vector<std::string> MMapDirectory::list_all() {
  struct stat sb{};
  std::vector<std::string> files;

  for (const auto &entry : std::filesystem::directory_iterator(path)) {
    const std::filesystem::path &file_path = entry.path();
    std::string file_path_string = file_path.string();
    const char *path_c_str = file_path_string.c_str();

    if (stat(path_c_str, &sb) == 0
        && !(sb.st_mode & S_IFDIR)) {
      files.emplace_back(file_path.filename().string());
    }
  }

  return files;
}

int64_t MMapDirectory::file_length(const std::string &file_name) {
  namespace fs = std::filesystem;
  auto file_path = path / file_name;
  try {
    if (fs::exists(file_path)) {
      return fs::file_size(file_path);
    } else {
      // TODO
      throw 13;
    }
  } catch (const std::filesystem::filesystem_error &ex) {
    // TODO
    throw 13;
  }
}

