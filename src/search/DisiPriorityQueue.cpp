#include "DisiPriorityQueue.hpp"

using lucene::cyborg::search::DisiPriorityQueue;
using lucene::cyborg::util::UseOnlyPtr;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::search::DisiWrapper;

//
// DisiPriorityQueue
//

DisiPriorityQueue::DisiPriorityQueue(int32_t _max_size,
                                     UseOnlyPtr<LinearAllocator> _allocator)
    : heap(_allocator->allocate_objects<DisiWrapper *>(_max_size)),
      tmp_top_list(LinearAllocatorAdapterForSTL<ManagedPtr<DisiWrapper>>{_allocator}),
      heap_size(_max_size),
      size() {
}

ManagedPtr<DisiWrapper> DisiPriorityQueue::update_top() noexcept {
  down_heap();
  return heap[0];
}

ManagedPtr<DisiWrapper> DisiPriorityQueue::update_top(
    ManagedPtr<DisiWrapper> top_replacement) noexcept {
  heap[0] = top_replacement;
  return update_top();
}

ManagedPtr<DisiWrapper> DisiPriorityQueue::pop() noexcept {
  auto result = heap[0];
  const int32_t i = --size;
  heap[0] = heap[i];
  down_heap();
  return result;
}

ManagedPtr<DisiWrapper> DisiPriorityQueue::top2() noexcept {
  switch (size) {
    case 0:
    case 1:return nullptr;
    case 2:return heap[1];
    default:
      if (heap[1]->doc <= heap[2]->doc) {
        return heap[1];
      } else {
        return heap[2];
      }
  }  // End switch
}

TypedVector<ManagedPtr<DisiWrapper>> DisiPriorityQueue::top_list() {
  tmp_top_list.clear();
  const auto doc_id = heap[0]->doc;
  tmp_top_list.emplace_back(heap[0]);
  if (size >= 3) {
    top_list(doc_id, 1);
    top_list(doc_id, 2);
  } else if (size == 2 && heap[1]->doc == doc_id) {
    tmp_top_list.emplace_back(heap[1]);
  }

  return TypedVector<ManagedPtr<DisiWrapper>>{
      tmp_top_list.data(),
      (int32_t) tmp_top_list.size()
  };
}

void DisiPriorityQueue::top_list(const int32_t doc_id, const int32_t i) {
  if (heap[i]->doc == doc_id) {
    tmp_top_list.emplace_back(heap[i]);
    const auto left = left_node(i);
    const auto right = right_node(i);

    if (right < size) {
      top_list(doc_id, left);
      top_list(doc_id, right);
    } else if (left < size && heap[left]->doc == doc_id) {
      tmp_top_list.emplace_back(heap[left]);
    }
  }  // End if
}

ManagedPtr<DisiWrapper> DisiPriorityQueue::add(ManagedPtr<DisiWrapper> entry) {
  heap[size] = entry;
  up_heap(size);
  ++size;
  return heap[0];
}

void DisiPriorityQueue::add_all(ManagedPtr<DisiWrapper> *entries, int32_t len) {
  if (len == 0) {
    return;
  }

  if ((size + len) > heap_size) {
    // TODO
    throw 13;
  }

  for (int32_t i = size, until = size + len; i < until; ++i) {
    heap[i] = entries[i];
  }
  size += len;

  // Heapify in bulk:
  int32_t first_leaf_index = size >> 1;
  for (int32_t root_index = first_leaf_index - 1; root_index >= 0; --root_index) {
    int32_t parent_index = root_index;
    auto parent = heap[parent_index];
    while (parent_index < first_leaf_index) {
      auto child_index = left_node(parent_index);
      const auto right_child_index = right_node(child_index);
      auto child = heap[child_index];
      if (right_child_index < size && heap[right_child_index]->doc < child->doc) {
        child = heap[right_child_index];
        child_index = right_child_index;
      }
      if (child->doc >= parent->doc) {
        break;
      }
      heap[parent_index] = child;
      parent_index = child_index;
    }  // End while
    heap[parent_index] = parent;
  }  // End for
}

void DisiPriorityQueue::up_heap(int32_t i) {
  auto node = heap[i];
  const int32_t node_doc = node->doc;
  int32_t j = parent_node(i);
  while (j >= 0 && node_doc < heap[j]->doc) {
    heap[i] = heap[j];
    i = j;
    j = parent_node(j);
  }  // End while
  heap[i] = node;
}

void DisiPriorityQueue::down_heap() {
  int32_t i = 0;
  auto node = heap[0];
  int32_t j = left_node(i);
  if (j < size) {
    int32_t k = right_node(j);
    if (k < size && heap[k]->doc < heap[j]->doc) {
      j = k;
    }
    if (heap[j]->doc < node->doc) {
      do {
        heap[i] = heap[j];
        i = j;
        j = left_node(i);
        k = right_node(j);
        if (k < size && heap[k]->doc < heap[j]->doc) {
          j = k;
        }
      } while (j < size && heap[j]->doc < node->doc);
      heap[i] = node;
    }
  }  // End if
}
