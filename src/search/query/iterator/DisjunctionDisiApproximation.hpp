#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_DISJUNCTIONDISIAPPROXIMATION_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_DISJUNCTIONDISIAPPROXIMATION_HPP_

#include "../../DocIdSetIterator.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../DisiPriorityQueue.hpp"

namespace lucene::cyborg::search::query {

class DisjunctionDisiApproximation final : public DocIdSetIterator {
 public:
  explicit DisjunctionDisiApproximation(util::ManagedPtr<DisiPriorityQueue> sub_iterators);

  int64_t get_cost() final {
    return cost;
  }

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final {
    return sub_iterators->top()->doc;
  }

  int32_t next_doc() final;

 private:
  util::ManagedPtr<DisiPriorityQueue> sub_iterators;
  int64_t cost;
};  // DisjunctionDisiApproximation



}  // namespace lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_DISJUNCTIONDISIAPPROXIMATION_HPP_
