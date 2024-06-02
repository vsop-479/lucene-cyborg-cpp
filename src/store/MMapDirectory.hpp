#ifndef LUCENE_CYBORG_CPP_SRC_STORE_MMAPDIRECTORY_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_MMAPDIRECTORY_HPP_

#include <cstdint>
#include <memory>
#include <vector>
#include <string>
#include <filesystem>

#include "CloseableMemoryDataInput.hpp"

namespace lucene::cyborg::store {

class MMapDirectory {
 public:
  explicit MMapDirectory(const std::string &path);

  std::vector<std::string> list_all();

  int64_t file_length(const std::string &file_name);

  CloseableMemoryDataInput open_input(const std::string &file_name);

  CloseableMemoryDataInput open_input_with_region(const std::string &file_name,
                                                  int64_t offset,
                                                  int64_t length);

  [[nodiscard]] const std::filesystem::path &get_path() const noexcept {
    return path;
  }

 private:
  std::filesystem::path path;
};  // MMapDirectory

using MMapDirectoryPtr = std::shared_ptr<MMapDirectory>;

}  // lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_MMAPDIRECTORY_HPP_
