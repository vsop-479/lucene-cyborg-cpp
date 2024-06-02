#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_LINEARALLOCATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_LINEARALLOCATOR_HPP_

#include <cstddef>
#include <cassert>
#include <cstdint>
#include <utility>
#include <new>
#include <iostream>

namespace lucene::cyborg::util {

class LinearAllocator {
 private:
  struct MemoryPage {
    MemoryPage *next_page = nullptr;
    uint8_t *next_allocate = nullptr;
    uint8_t failed = 0;
  };  // MemoryPage

  static constexpr int32_t MEMORY_PAGE_STRUCT_SIZE = sizeof(MemoryPage);

  struct LargeMemoryList {
    LargeMemoryList *next = nullptr;
    uint8_t *ptr = nullptr;
  };  // LargeMemoryList


 public:
  class MarkingPoint {
   public:
    MarkingPoint() = default;

   private:
    friend class LinearAllocator;

    MarkingPoint(MemoryPage *_page, int32_t _position, int32_t _marking_id, int32_t _failed)
        : page(_page),
          marking_id(_marking_id),
          failed(_failed),
          position(_position) {
    }

    MemoryPage *page;
    int32_t marking_id;
    int16_t failed;
    int16_t position;
  };  // MarkingPoint

  LinearAllocator(const LinearAllocator &) = delete;

  LinearAllocator(LinearAllocator &&) noexcept;

  LinearAllocator();

  ~LinearAllocator();

  template<typename T>
  T *allocate_objects(int32_t num_elements) {
    auto memory = aligned_allocate(num_elements * sizeof(T));
    auto base = (T *) memory;
    for (int32_t i = 0; i < num_elements; ++i) {
      new(&base[i]) T();
    }

    return base;
  }

  template<typename T>
  T *allocate_object() {
    auto memory = aligned_allocate(sizeof(T));
    auto ret = new(memory) T();
    return ret;
  }

  template<typename T, typename ... Args>
  T *allocate_object(Args &&... args) {
    auto memory = aligned_allocate(sizeof(T));
    return new(memory) T(std::forward<Args>(args)...);
  }

  LinearAllocator &operator=(const LinearAllocator &) = delete;

  LinearAllocator &operator=(LinearAllocator &&) noexcept;

  uint8_t *allocate(int32_t size);

  uint8_t *aligned_allocate(int32_t size);

  MarkingPoint mark() noexcept;

  void gc(const MarkingPoint &marked_point);

  void clear();

  static uint64_t get_page_size() noexcept;

 private:
  template<uint64_t alignment>
  uint8_t *allocate_from_pool(int32_t size) {
    assert(pool);
    assert(curr_page);
    assert(last_page);

    const uint64_t page_size = get_page_size();

    uint8_t *next_allocate;
    for (auto page = curr_page; page; page = page->next_page) {
      int64_t available_space;

      if constexpr (alignment == 0) {
        next_allocate = page->next_allocate;
      } else {
        next_allocate = (uint8_t *) (((uint64_t) (page->next_allocate)
            + (alignment - 1)) & ~(alignment - 1));
      }
      available_space = (((uint64_t) page) + page_size) - ((uint64_t) next_allocate);

      if (size <= available_space) {
        page->next_allocate = next_allocate + size;
        return next_allocate;
      }
    }  // End for

    // failed to allocate, advance curr_page
    for (auto page = curr_page; page; page = page->next_page) {
      page->failed++;
      if (page->failed > 3) {
        curr_page = page->next_page;
      }
    }

    // create a new page
    auto raw_ptr = new uint8_t[page_size];
    auto new_page = new(raw_ptr) MemoryPage();
    if constexpr (alignment == 0) {
      next_allocate = raw_ptr + MEMORY_PAGE_STRUCT_SIZE;
    } else {
      next_allocate = (uint8_t *) (((uint64_t) (raw_ptr + MEMORY_PAGE_STRUCT_SIZE)
          + (alignment - 1)) & ~(alignment - 1));
    }

    // update last page
    last_page->next_page = new_page;
    last_page = new_page;

    // reset curr_page if it's required
    if (!curr_page) {
      curr_page = last_page;
    }

    new_page->next_allocate = next_allocate + size;
    return next_allocate;
  }

  void init_pool();

  uint8_t *prepend_large_memory(int32_t size);

  static int32_t get_max_available_size_for_aligned_in_page();

  static int32_t get_max_available_size_in_page();

  MemoryPage *pool;
  MemoryPage *curr_page;
  MemoryPage *last_page;
  LargeMemoryList *large_memory_list;
  int32_t next_mark_id;
};  // LinearAllocator



}  // namespace lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_LINEARALLOCATOR_HPP_