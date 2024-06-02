#include "../Weight.hpp"
#include "BooleanReqExclScorer.hpp"

using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::BooleanReqExclScorer;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::UseOnlyPtr;

float get_match_cost(UseOnlyPtr<DocIdSetIterator> req_approx,
                     UseOnlyPtr<TwoPhaseIterator> req_two_phase_iterator,
                     UseOnlyPtr<DocIdSetIterator> excl_approx,
                     UseOnlyPtr<TwoPhaseIterator> excl_two_phase_iterator) {
  float match_cost = 2; // we perform 2 comparisons to advance exclApproximation
  if (req_two_phase_iterator) {
    // this two-phase iterator must always be matched
    match_cost += req_two_phase_iterator->get_match_cost();
  }

  // match get_cost of the prohibited clause: we need to advance the approximation
  // and match the two-phased iterator

  /**
   * Estimation of the number of operations required to call DISI.advance. This is likely completely
   * wrong, especially given that the get_cost of this method usually depends on how far you want to
   * advance, but it's probably better than nothing.
   */
  static constexpr int32_t ADVANCE_COST = 10;

  const float excl_match_cost =
      ADVANCE_COST + (excl_two_phase_iterator ? excl_two_phase_iterator->get_match_cost() : 0);

  // upper value for the ratio of documents that req_approximation matches that
  // excl_approximation also matches
  float ratio;
  if (req_approx->get_cost() <= 0) {
    ratio = 1;
  } else if (excl_approx->get_cost() <= 0) {
    ratio = 0;
  } else {
    ratio = (float) std::min(req_approx->get_cost(), excl_approx->get_cost()) / req_approx->get_cost();
  }
  match_cost += ratio * excl_match_cost;

  return match_cost;
}

//
// BooleanReqExclScorer
//

BooleanReqExclScorer::BooleanReqExclScorer(ManagedPtr<Scorer> _req_scorer,
                                           ManagedPtr<Scorer> _excl_scorer)
    : Scorer(_req_scorer->get_weight()),
      req_scorer(_req_scorer),
      req_two_phase_iterator(_req_scorer->two_phase_iterator()),
      excl_two_phase_iterator(_excl_scorer->two_phase_iterator()),
      req_approximation(req_two_phase_iterator ? req_two_phase_iterator->approximation : _req_scorer->get_iterator()),
      excl_approximation(excl_two_phase_iterator ? excl_two_phase_iterator->approximation : _excl_scorer
          ->get_iterator()) {
}

ManagedPtr<DocIdSetIterator> BooleanReqExclScorer::get_iterator() {
  return TwoPhaseIterator::as_doc_id_set_iterator(two_phase_iterator(), weight->srch_context->linear_allocator);
}

ManagedPtr<TwoPhaseIterator> BooleanReqExclScorer::two_phase_iterator() {
  const float match_cost = get_match_cost(req_approximation,
                                          req_two_phase_iterator,
                                          excl_approximation,
                                          excl_two_phase_iterator);

  if (req_two_phase_iterator == nullptr
      || (excl_two_phase_iterator
          && req_two_phase_iterator->get_match_cost() <= excl_two_phase_iterator->get_match_cost())) {
    return weight->srch_context->linear_allocator->allocate_object < BooleanReqExclScorerTwoPhaseIterator < true >> (
        req_approximation,
            excl_approximation,
            req_two_phase_iterator,
            excl_two_phase_iterator,
            match_cost);
  } else {
    return weight->srch_context->linear_allocator->allocate_object < BooleanReqExclScorerTwoPhaseIterator < false >> (
        req_approximation,
            excl_approximation,
            req_two_phase_iterator,
            excl_two_phase_iterator,
            match_cost);
  }  // End if
}
