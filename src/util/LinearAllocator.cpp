#include "LinearAllocator.hpp"

static constexpr uint64_t PAGE_SIZE = 64 * 1024;

using lucene::cyborg::util::LinearAllocator;



//
// LinearAllocator
//

LinearAllocator::LinearAllocator()
    : pool(),
      curr_page(),
      last_page(),
      large_memory_list(),
      next_mark_id() {
}

LinearAllocator::~LinearAllocator() {
  clear();
}

LinearAllocator::LinearAllocator(LinearAllocator &&other) noexcept
    : pool(other.pool),
      curr_page(other.curr_page),
      last_page(other.last_page),
      large_memory_list(other.large_memory_list),
      next_mark_id(other.next_mark_id) {
  other.pool = nullptr;
}

LinearAllocator &LinearAllocator::operator=(LinearAllocator &&other) noexcept {
  if (this != &other) {
    pool = other.pool;
    curr_page = other.curr_page;
    last_page = other.last_page;
    large_memory_list = other.large_memory_list;
    next_mark_id = other.next_mark_id;

    other.pool = nullptr;
  }

  return *this;
}

int32_t LinearAllocator::get_max_available_size_for_aligned_in_page() {
  static constexpr auto MAX_ALIGN = alignof(std::max_align_t);
  static const auto MAX_SIZE = PAGE_SIZE - ((
      MEMORY_PAGE_STRUCT_SIZE + MAX_ALIGN - 1) & ~(MAX_ALIGN - 1));
  return MAX_SIZE;
}

int32_t LinearAllocator::get_max_available_size_in_page() {
  static constexpr auto MAX_SIZE = PAGE_SIZE - MEMORY_PAGE_STRUCT_SIZE;
  return MAX_SIZE;
}

uint8_t *LinearAllocator::allocate(int32_t size) {
  if (!pool) {
    init_pool();
  }

  if (size <= get_max_available_size_in_page()) {
    return allocate_from_pool<0>(size);
  } else {
    return prepend_large_memory(size);
  }
}

uint8_t *LinearAllocator::aligned_allocate(int32_t size) {
  if (!pool) {
    init_pool();
  }

  if (size <= get_max_available_size_for_aligned_in_page()) {
    return allocate_from_pool<alignof(std::max_align_t)>(size);
  } else {
    return prepend_large_memory(size);
  }
}

LinearAllocator::MarkingPoint LinearAllocator::mark() noexcept {
  if (pool) {
    return MarkingPoint{last_page,
                        (int32_t) ((uint64_t) (last_page->next_allocate) - (uint64_t) last_page),
                        next_mark_id++,
                        last_page->failed};
  } else {
    return MarkingPoint{};
  }
}

void LinearAllocator::gc(const LinearAllocator::MarkingPoint &marked_point) {
  if (marked_point.page) {
    const int32_t expected_marking_id = next_mark_id - 1;
    if (marked_point.marking_id == expected_marking_id) {
      --next_mark_id;

      // init the first page
      auto page = marked_point.page;
      page->next_allocate = (uint8_t *) page + marked_point.position;
      page->failed = marked_point.failed;
      curr_page = page;

      page = page->next_page;
      while (page) {
        page->next_allocate = (uint8_t *) page + MEMORY_PAGE_STRUCT_SIZE;
        page->failed = 0;
        page = page->next_page;
      }
    } else {
      // TODO
      throw 13;
    }
  }  // End if
}

uint64_t LinearAllocator::get_page_size() noexcept {
  return PAGE_SIZE;
}

void LinearAllocator::init_pool() {
  auto raw_ptr = new uint8_t[PAGE_SIZE];
  auto new_page = new(raw_ptr) MemoryPage();
  new_page->next_allocate = raw_ptr + MEMORY_PAGE_STRUCT_SIZE;

  curr_page = last_page = pool = new_page;
}

uint8_t *LinearAllocator::prepend_large_memory(int32_t size) {
  static constexpr auto CALIBRATED_HEADER_SIZE = (sizeof(LargeMemoryList)
      + alignof(std::max_align_t) - 1) & (~(alignof(std::max_align_t) - 1));

  auto large_memory = new uint8_t[CALIBRATED_HEADER_SIZE + size];
  auto large_memory_holder = new(large_memory) LargeMemoryList();
  large_memory_holder->ptr = large_memory + CALIBRATED_HEADER_SIZE;

  large_memory_holder->next = large_memory_list;
  large_memory_list = large_memory_holder;
  return large_memory_holder->ptr;
}

void LinearAllocator::clear() {
  if (pool) {
    // cleaning up large memory blocks
    for (auto large_memory = large_memory_list; large_memory;) {
      auto next_large_memory = large_memory->next;
      delete[] (uint8_t *) large_memory;
      large_memory = next_large_memory;
    }
    large_memory_list = nullptr;

    // cleaning up memory pages
    for (auto page = pool; page;) {
      auto next_page = page->next_page;
      delete[] (uint8_t *) page;
      page = next_page;
    }

    last_page = curr_page = pool = nullptr;
    next_mark_id = 0;
  }  // End if
}
