#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONUTILS_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONUTILS_HPP_

#include "../../../util/LinearAllocator.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../DocIdSetIterator.hpp"
#include "../../Scorer.hpp"

namespace lucene::cyborg::search::query {

struct ConjunctionUtils {
  static util::ManagedPtr<DocIdSetIterator> intersect_scorers(
      util::ManagedPtr<Scorer> scorers[],
      int32_t num_scorers,
      util::UseOnlyPtr<util::LinearAllocator> allocator);

 private:
  ConjunctionUtils() = default;
};  // ConjunctionUtils



}  // lucene::cyborg::search::query


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONUTILS_HPP_
