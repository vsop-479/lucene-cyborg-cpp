#include "BytesFstEnumerator.hpp"

using lucene::cyborg::util::BytesFstEnumerator;
using lucene::cyborg::util::Fst;
using lucene::cyborg::util::FstBytesOutput;
using lucene::cyborg::util::FstReverseBytes;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::UseOnlyPtr;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::MemoryUtils;

void ensure_buffer(ManagedPtr<char> &buffer,
                   int32_t &buffer_size,
                   const int32_t required_size,
                   ManagedPtr<LinearAllocator> allocator) {
  if (buffer_size >= required_size) {
    return;
  }

  const int32_t new_buffer_size = MemoryUtils::oversize(required_size);
  const auto new_buffer = allocator->allocate(new_buffer_size);
  if (buffer) {
    MemoryUtils::memcpy(new_buffer, buffer, buffer_size);
  }

  buffer = (char *) new_buffer;
  buffer_size = new_buffer_size;
}

//
// BytesFstEnumerator
//

BytesFstEnumerator::BytesFstEnumerator(ManagedPtr<Fst<1, FstReverseBytes, FstBytesOutput>> _fst,
                                       ManagedPtr<LinearAllocator> _allocator)
    : fst(_fst),
      reverse_bytes_reader(_fst->get_bytes_reader()),
      allocator(_allocator),
      current_arc(),
      key_buffer(),
      key_buffer_size(),
      value_buffer(),
      value_buffer_size(),
      term_value(),
      encoded_data_value() {
}

bool BytesFstEnumerator::next() {
  if (do_next()) {
    set_result();
    return true;
  }

  return false;
}

bool BytesFstEnumerator::do_next() {
  if (current_arc) {
    while (current_arc->arc.is_last()) {
      current_arc = current_arc->prev;
      if (current_arc->index == 0) {
        return false;
      }
    }
    fst->read_next_arc(current_arc->arc, reverse_bytes_reader);
  } else {
    current_arc = allocator->allocate_object<ArcNode>();
    fst->get_first_arc(current_arc->arc);
    auto next_arc = create_new_node_of_current();

    fst->read_first_target_arc(current_arc->arc, next_arc->arc, reverse_bytes_reader);
    current_arc = next_arc;
  }  // End if

  while (true) {
    // Accumulate encoded data value
    const int32_t required_value_len = current_arc->prev->value_buffer_next_write + current_arc->arc.output.len;
    ensure_buffer(value_buffer, value_buffer_size, required_value_len, allocator);
    FstReverseBytesReader::copy_to_dest(value_buffer + current_arc->prev->value_buffer_next_write,
                                        current_arc->arc.output.bytes,
                                        current_arc->arc.output.len);
    current_arc->value_buffer_next_write = required_value_len;

    if (current_arc->arc.label == FST_END_LABEL) {
      // It's final node, stop here.
      return true;
    }

    // Append label
    ensure_buffer(key_buffer, key_buffer_size, current_arc->index, allocator);
    key_buffer[current_arc->index - 1] = current_arc->arc.label;

    // Keep traversal until we find a final node.
    auto next_arc = create_new_node_of_current();
    fst->read_first_target_arc(current_arc->arc, next_arc->arc, reverse_bytes_reader);
    current_arc = next_arc;
  }  // End while
}

void BytesFstEnumerator::set_result() {
  if (current_arc->index > 0) {
    term_value = std::string_view{key_buffer, (uint64_t) current_arc->index - 1};
    encoded_data_value = std::string_view{value_buffer, (uint64_t) current_arc->value_buffer_next_write};
  } else {
    // Exhausted
    encoded_data_value = term_value = {};
  }
}

ManagedPtr<BytesFstEnumerator::ArcNode> BytesFstEnumerator::create_new_node_of_current() {
  if (current_arc->next) {
    // Already allocated, reuse it.
    return current_arc->next;
  } else {
    // This leaves us no choice but creating a new one.
    auto next_arc = allocator->allocate_object<ArcNode>();
    current_arc->next = next_arc;
    next_arc->prev = current_arc;
    next_arc->index = current_arc->index + 1;
    return next_arc;
  }  // End if
}
