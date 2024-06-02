#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_LINEARALLOCATORADAPTORFORSTL_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_LINEARALLOCATORADAPTORFORSTL_HPP_

#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "LinearAllocator.hpp"

namespace lucene::cyborg::util {

template<typename T>
struct LinearAllocatorAdapterPointerTraits {
  using reference = T &;
  using const_reference = const T &;
};

// Avoid declaring a reference to void with an empty specialization
template<>
struct LinearAllocatorAdapterPointerTraits<void> {
};

template<typename T = void>
struct LinearAllocatorAdapterForSTL : public LinearAllocatorAdapterPointerTraits<T> {
  using value_type = T;
  using size_type = std::size_t;
  using pointer = T *;
  using const_pointer = const T *;
  using difference_type = typename std::pointer_traits<pointer>::difference_type;
  using propagate_on_container_copy_assignment = std::true_type;
  using propagate_on_container_move_assignment = std::true_type;

  template<typename U>
  struct rebind {
    typedef LinearAllocatorAdapterForSTL<U> other;
  };

  explicit LinearAllocatorAdapterForSTL(LinearAllocator *_linear_allocator = nullptr)
      : linear_allocator(_linear_allocator) {
  }

  template<typename U>
  explicit LinearAllocatorAdapterForSTL(const LinearAllocatorAdapterForSTL<U> &other) noexcept
      : linear_allocator(other.linear_allocator) {
  }

  T *allocate(size_t size) {
    return (T *) linear_allocator->aligned_allocate(sizeof(T) * size);
  }

  void deallocate(T *ptr, size_t size) {
    // no-op
  }

  LinearAllocator *linear_allocator;
};  // LinearAllocatorAdapterForSTL

template<typename T, typename U>
constexpr bool operator==(const LinearAllocatorAdapterForSTL<T> &a1,
                          const LinearAllocatorAdapterForSTL<U> &a2) noexcept {
  return a1.linear_allocator == a2.linear_allocator;
}

template<typename T, typename U>
constexpr bool operator!=(const LinearAllocatorAdapterForSTL<T> &a1,
                          const LinearAllocatorAdapterForSTL<U> &a2) noexcept {
  return a1.linear_allocator != a2.linear_allocator;
}

template<typename T>
using LVector = std::vector<T, LinearAllocatorAdapterForSTL<T>>;

template<typename Key, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
using LUnorderedSet = std::unordered_set<Key, Hash, KeyEqual, LinearAllocatorAdapterForSTL<Key>>;

template<typename Key, typename T, typename Hash = std::hash<Key>, typename KeyEqual = std::equal_to<Key>>
using LUnorderedMap = std::unordered_map<Key, T, Hash, KeyEqual, LinearAllocatorAdapterForSTL<std::pair<const Key, T>>>;

template<typename T>
struct LinearAllocatorNoOpDelete {
  void operator()(T *) const {
    // No-op
  }
};  // LinearAllocatorNoOpDelete

template<typename T>
using LUniquePtr = std::unique_ptr<T, LinearAllocatorNoOpDelete<T>>;

}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_LINEARALLOCATORADAPTORFORSTL_HPP_
