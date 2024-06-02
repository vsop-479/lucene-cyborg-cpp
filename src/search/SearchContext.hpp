#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SEARCHCONTEXT_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SEARCHCONTEXT_HPP_

#include "../util/PointerTypes.hpp"
#include "../util/LinearAllocator.hpp"

namespace lucene::cyborg::search {

struct SearchContext {
  util::UseOnlyPtr<util::LinearAllocator> linear_allocator;
};  // SearchContext



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SEARCHCONTEXT_HPP_
