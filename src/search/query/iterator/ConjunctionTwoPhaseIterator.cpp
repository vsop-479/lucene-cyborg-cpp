#include <algorithm>
#include "ConjunctionTwoPhaseIterator.hpp"

using lucene::cyborg::search::query::ConjunctionTwoPhaseIterator;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;

//
// ConjunctionTwoPhaseIterator
//

const TypeInfoPtr ConjunctionTwoPhaseIterator::TYPE_INFO =
    TypeInfo::get_type_info<ConjunctionTwoPhaseIterator>("ConjunctionTwoPhaseIterator");

ConjunctionTwoPhaseIterator::ConjunctionTwoPhaseIterator(ManagedPtr<DocIdSetIterator> _approximation,
                                                         LVector<ManagedPtr<TwoPhaseIterator>> &&_two_phase_iterators)
    : TwoPhaseIterator(_approximation, ConjunctionTwoPhaseIterator::TYPE_INFO),
      two_phase_iterators(std::move(_two_phase_iterators)) {
  std::sort(two_phase_iterators.begin(), two_phase_iterators.end(), [](auto tpi_0, auto tpi_1) {
    return tpi_0->get_match_cost() < tpi_1->get_match_cost();
  });
  float total_match_cost = 0;
  for (auto two_phase_iterator : two_phase_iterators) {
    total_match_cost += two_phase_iterator->get_match_cost();
  }
  match_cost = total_match_cost;
}

bool ConjunctionTwoPhaseIterator::matches() {
  for (auto two_phase_iterator : two_phase_iterators) {
    if (!two_phase_iterator->matches()) {
      return false;
    }
  }
  return true;
}

float ConjunctionTwoPhaseIterator::get_match_cost() {
  return match_cost;
}
