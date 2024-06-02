#include "CloseableMemoryDataInput.hpp"

using lucene::cyborg::store::CloseableMemoryDataInput;
using lucene::cyborg::store::MappedMemory;

//
// CloseableMemoryDataInput
//

CloseableMemoryDataInput::CloseableMemoryDataInput(
    MappedMemory &&_mapped_memory)
    : mapped_memory(std::forward<MappedMemory>(_mapped_memory)),
      data(mapped_memory.data) {
}

CloseableMemoryDataInput::CloseableMemoryDataInput(CloseableMemoryDataInput &&_other) noexcept
    : mapped_memory(std::move(_other.mapped_memory)),
      data(this->mapped_memory.data) {
}

CloseableMemoryDataInput &CloseableMemoryDataInput::operator=(
    CloseableMemoryDataInput &&other) noexcept {
  if (this != &other) {
    mapped_memory = std::move(other.mapped_memory);
    data = other.data;
  }
  return *this;
}

void CloseableMemoryDataInput::close() {
  mapped_memory.close();
}
