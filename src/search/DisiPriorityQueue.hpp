#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DISIPRIORITYQUEUE_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DISIPRIORITYQUEUE_HPP_

#include "DisiWrapper.hpp"
#include "../util/LinearAllocatorAdaptorForSTL.hpp"

namespace lucene::cyborg::search {

class DisiPriorityQueue final {

 public:
  DisiPriorityQueue(int32_t max_size, util::UseOnlyPtr<util::LinearAllocator> allocator);

  int32_t get_size() const noexcept {
    return size;
  }

  auto get_internal_array() noexcept {
    return util::TypedVector<DisiWrapper *>{heap, heap_size};
  }

  util::ManagedPtr<DisiWrapper> update_top() noexcept;

  util::ManagedPtr<DisiWrapper> update_top(util::ManagedPtr<DisiWrapper> top_replacement) noexcept;

  void clear() noexcept {
    size = 0;
  }

  util::ManagedPtr<DisiWrapper> pop() noexcept;

  util::ManagedPtr<DisiWrapper> top() noexcept {
    return heap[0];
  }

  util::ManagedPtr<DisiWrapper> top2() noexcept;

  util::TypedVector<util::ManagedPtr<DisiWrapper>> top_list();

  util::ManagedPtr<DisiWrapper> add(util::ManagedPtr<DisiWrapper> entry);

  void add_all(util::ManagedPtr<DisiWrapper> entries[], int32_t len);

 private:
  static int32_t left_node(uint32_t node) noexcept {
    return ((node + 1U) << 1U) - 1;
  }

  static int32_t right_node(int32_t left_node) noexcept {
    return left_node + 1;
  }

  static int32_t parent_node(uint32_t node) noexcept {
    return ((node + 1U) >> 1U) - 1U;
  }

  void top_list(int32_t doc_id, int32_t i);

  void up_heap(int32_t i);

  void down_heap();

  util::ManagedPtr<DisiWrapper *> heap;
  util::LVector<util::ManagedPtr<DisiWrapper>> tmp_top_list;
  int32_t heap_size;
  int32_t size;
};  // DisiPriorityQueue



}  // lucene::cyborg::search


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DISIPRIORITYQUEUE_HPP_
