#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCORER_HPP_

#include "Scorable.hpp"
#include "DocIdSetIterator.hpp"
#include "TwoPhaseIterator.hpp"
#include "../util/PointerTypes.hpp"

namespace lucene::cyborg::search {

class Weight;

class Scorer : public Scorable {
 public:
  explicit Scorer(util::ManagedPtr<Weight> _weight)
      : Scorable(),
        weight(_weight) {
  }

  util::ManagedPtr<Weight> get_weight() noexcept {
    return weight;
  }

  virtual util::ManagedPtr<DocIdSetIterator> get_iterator() = 0;

  virtual util::ManagedPtr<TwoPhaseIterator> two_phase_iterator() {
    return nullptr;
  }

  virtual int32_t advance_shallow(int32_t target) {
    return DocIdSetIterator::NO_MORE_DOCS;
  }

  virtual float get_max_score(int32_t upto) = 0;

 protected:
  util::ManagedPtr<Weight> weight;
};  // Scorer



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCORER_HPP_
