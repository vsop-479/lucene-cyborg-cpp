#ifndef LUCENE_CYBORG_CPP_SRC_STORE_CLOSEABLEMEMORYDATAINPUT_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_CLOSEABLEMEMORYDATAINPUT_HPP_

#include "MappedMemory.hpp"
#include "ReadOnlyDataInput.hpp"

namespace lucene::cyborg::store {

struct CloseableMemoryDataInput {
 public:
  explicit CloseableMemoryDataInput(MappedMemory &&_mapped_memory);

  CloseableMemoryDataInput() = default;

  CloseableMemoryDataInput(const CloseableMemoryDataInput &other) = delete;

  CloseableMemoryDataInput(CloseableMemoryDataInput &&_other) noexcept;

  CloseableMemoryDataInput &operator=(CloseableMemoryDataInput &&other) noexcept;

  CloseableMemoryDataInput &operator=(const CloseableMemoryDataInput &other) = delete;

  [[nodiscard]] ReadOnlyDataInput to_read_only() const noexcept {
    return ReadOnlyDataInput{mapped_memory.data, mapped_memory.data, mapped_memory.offset, mapped_memory.length};
  }

  [[nodiscard]] ReadOnlyDataInput slice(int64_t offset, int64_t length) const noexcept {
    return ReadOnlyDataInput{mapped_memory.data + offset,
                             mapped_memory.data + offset,
                             0, length};
  }

  void close();

 private:
  MappedMemory mapped_memory;
  util::ReadOnlyPtr<uint8_t> data;
};  // CloseableMemoryDataInput



}  // lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_CLOSEABLEMEMORYDATAINPUT_HPP_
