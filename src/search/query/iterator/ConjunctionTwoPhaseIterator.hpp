#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONTWOPHASEITERATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONTWOPHASEITERATOR_HPP_

#include "../../TwoPhaseIterator.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../../util/TypeInfo.hpp"

namespace lucene::cyborg::search::query {

struct ConjunctionTwoPhaseIterator final : public TwoPhaseIterator {
  static const util::TypeInfoPtr TYPE_INFO;

  ConjunctionTwoPhaseIterator(util::ManagedPtr<DocIdSetIterator> approximation,
                              util::LVector<util::ManagedPtr<TwoPhaseIterator>> &&two_phase_iterators);

  bool matches() final;

  float get_match_cost() final;

  util::LVector<util::ManagedPtr<TwoPhaseIterator>> two_phase_iterators;
  float match_cost;
}; // ConjunctionTwoPhaseIterator



}  // lucene::cyborg::search::query


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONTWOPHASEITERATOR_HPP_
