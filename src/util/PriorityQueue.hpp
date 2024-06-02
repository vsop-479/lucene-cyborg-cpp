#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_PRIORITYQUEUE_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_PRIORITYQUEUE_HPP_

#include <optional>
#include "PointerTypes.hpp"

namespace lucene::cyborg::util {

struct PriorityQueueConstants {
  static constexpr int32_t MINIMUM_REQUIRED_SIZE_OF_ARRAY = 2;
  static constexpr int32_t START_INDEX = 1;

 private:
  PriorityQueueConstants() = default;
};  // PriorityQueueConstants

template<typename T, typename Impl>
class PriorityQueue {
 public:
  PriorityQueue() = default;

  explicit PriorityQueue(const TypedVector<T> &_heap_array)
      : heap_array(_heap_array),
        size(_heap_array.len) {
    if (_heap_array.len < PriorityQueueConstants::MINIMUM_REQUIRED_SIZE_OF_ARRAY) [[unlikely]] {
      // TODO
      throw 13;
    }
  }

  void inplace_sort() {
    for (int32_t i = size; i > 1; --i) {
      std::swap(heap_array.vec[1], heap_array.vec[i]);
      --size;
      update_top();
    }
  }

  // Support lazy init
  void init(const TypedVector<T> &_heap_array) {
    if (_heap_array.len < PriorityQueueConstants::MINIMUM_REQUIRED_SIZE_OF_ARRAY) [[unlikely]] {
      // TODO
      throw 13;
    }

    heap_array = _heap_array;
  }

  void init_with_pre_built_heap(const TypedVector<T> &_heap_array) {
    if (_heap_array.len < PriorityQueueConstants::MINIMUM_REQUIRED_SIZE_OF_ARRAY) {
      // TODO
      throw 13;
    }

    heap_array = _heap_array;
    size = _heap_array.len - 1;  // 1-based.
  }

  T add(T element) {
    int32_t index = size + 1;
    heap_array.vec[index] = element;
    size = index;
    up_heap(index);
    return heap_array.vec[1];
  }

  std::optional<T> insert_with_overflow(T element) {
    auto self = (Impl *) this;

    if (size < heap_array.len) {
      add(element);
      return std::nullopt;
    } else if (size > 0 && self->less_than(heap_array.vec[1], element)) {
      auto ret = heap_array.vec[1];
      heap_array.vec[1] = element;
      update_top();
      return ret;
    } else {
      return element;
    }  // End if
  }

  TypedVector<T> &get_heap_array() noexcept {
    return heap_array;
  }

  T top() noexcept {
    return heap_array.vec[1];
  }

  void pop() {
    if (size) {
      auto result = heap_array.vec[1];
      heap_array.vec[1] = heap_array.vec[size--];
      down_heap(1);
    }  // End if
  }

  void update_top() {
    down_heap(1);
  }

  void update_top(T new_top) {
    heap_array.vec[1] = new_top;
    update_top();
  }

  int32_t get_size() const noexcept {
    return size;
  }

  void clear() noexcept {
    size = 0;
  }

 private:
  static int32_t left_child(int32_t idx) noexcept {
    return idx * 2;
  }

  static int32_t right_child(int32_t idx) noexcept {
    return idx * 2 + 1;
  }

  static int32_t parent(int32_t idx) noexcept {
    return idx / 2;
  }

  bool up_heap(int32_t orig_pos) {
    int32_t i = orig_pos;
    auto node = heap_array.vec[i]; // save bottom node
    int32_t j = parent(i);
    auto self = (Impl *) this;
    while (j > 0 && self->less_than(node, heap_array.vec[j])) {
      heap_array.vec[i] = heap_array.vec[j]; // shift parents down
      i = j;
      j = parent(j);
    }
    heap_array.vec[i] = node; // install saved node
    return i != orig_pos;
  }

  void down_heap(int32_t i) {
    auto node = heap_array.vec[i]; // save top node
    auto self = (Impl *) this;

    int32_t j = left_child(i);
    int32_t k = right_child(i);

    if (k <= size && self->less_than(heap_array.vec[k], heap_array.vec[j])) {
      j = k;
    }

    while (j <= size && self->less_than(heap_array.vec[j], node)) {
      heap_array.vec[i] = heap_array.vec[j]; // shift up child
      i = j;
      j = left_child(i);
      k = right_child(i);
      if (k <= size && self->less_than(heap_array.vec[k], heap_array.vec[j])) {
        j = k;
      }
    }  // End while

    heap_array.vec[i] = node; // install saved node
  }

  TypedVector<T> heap_array;
  int32_t size;
};  // PriorityQueue



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_PRIORITYQUEUE_HPP_
