#include <cmath>
#include "../Weight.hpp"
#include "ConjunctionScorer.hpp"
#include "iterator/ConjunctionUtils.hpp"

using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::ConjunctionScorer;
using lucene::cyborg::search::query::ConjunctionUtils;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::ManagedPtr;

//
// ConjunctionScorer
//

ConjunctionScorer::ConjunctionScorer(ManagedPtr<Weight> _weight,
                                     LVector<ManagedPtr<Scorer>> &&_required_scorers,
                                     LVector<ManagedPtr<Scorer>> &&_scoring_scorers)
    : Scorer(_weight),
      conjunction_iterator(ConjunctionUtils::intersect_scorers(
          _required_scorers.data(), _required_scorers.size(), _weight->srch_context->linear_allocator)),
      required_scorers(std::move(_required_scorers)),
      scoring_scorers(std::move(_scoring_scorers)) {
}

float ConjunctionScorer::get_score() {
  double score_sum = 0;
  for (auto scorer : scoring_scorers) {
    score_sum += scorer->get_score();
  }
  return (float) score_sum;
}

int32_t ConjunctionScorer::get_doc_id() {
  return conjunction_iterator->get_doc_id();
}

ManagedPtr<DocIdSetIterator> ConjunctionScorer::get_iterator() {
  return conjunction_iterator;
}

float ConjunctionScorer::get_max_score(int32_t upto) {
  switch (scoring_scorers.size()) {
    case 0:return 0;
    case 1:return scoring_scorers[0]->get_max_score(upto);
    default:return INFINITY;
  }
}

ManagedPtr<TwoPhaseIterator> ConjunctionScorer::two_phase_iterator() {
  return TwoPhaseIterator::unwrap(conjunction_iterator);
}
