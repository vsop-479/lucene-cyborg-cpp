#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_BYTESFSTENUMERATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_BYTESFSTENUMERATOR_HPP_

#include "FstBytesOutput.hpp"
#include "Fst.hpp"
#include "LinearAllocator.hpp"

namespace lucene::cyborg::util {

class BytesFstEnumerator {
 public:
  BytesFstEnumerator(ManagedPtr<Fst<1, FstReverseBytes, FstBytesOutput>> fst, ManagedPtr<LinearAllocator> allocator);

  bool next();

  [[nodiscard]] const std::string_view &term() const noexcept {
    return term_value;
  }

  [[nodiscard]] const std::string_view &encoded_data() const noexcept {
    return encoded_data_value;
  }

 private:
  struct ArcNode {
    FstArc<FstReverseBytes> arc;
    ManagedPtr<ArcNode> next;
    ManagedPtr<ArcNode> prev;
    int32_t index;
    int32_t value_buffer_next_write;
  };  // ArcNode

  bool do_next();

  void set_result();

  ManagedPtr<ArcNode> create_new_node_of_current();

  ManagedPtr<Fst<1, FstReverseBytes, FstBytesOutput>> fst;
  util::FstReverseBytesReader reverse_bytes_reader;
  ManagedPtr<LinearAllocator> allocator;
  ManagedPtr<ArcNode> current_arc;
  ManagedPtr<char> key_buffer;
  int32_t key_buffer_size;
  ManagedPtr<char> value_buffer;
  int32_t value_buffer_size;
  std::string_view term_value;
  std::string_view encoded_data_value;
};  // BytesFstEnumerator



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_BYTESFSTENUMERATOR_HPP_
