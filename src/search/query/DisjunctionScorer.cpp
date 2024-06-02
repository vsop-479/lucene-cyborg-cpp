#include "DisjunctionScorer.hpp"
#include "../Weight.hpp"

using lucene::cyborg::search::DisiWrapper;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::DisjunctionScorer;
using lucene::cyborg::search::query::DisjunctionScorerTwoPhaseIterator;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::TypedVector;

//
// DisjunctionScorer
//

DisjunctionScorer::DisjunctionScorer(ManagedPtr<Weight> _weight,
                                     LVector<ManagedPtr<Scorer>> &&_sub_scorers,
                                     ScoreMode _score_mode)
    : Scorer(_weight),
      approximation(),
      two_phase(),
      sub_scorers(_sub_scorers.size(), _weight->srch_context->linear_allocator),
      needs_scores(_score_mode != ScoreMode::COMPETE_NO_SCORES) {
  if (_sub_scorers.size() <= 1) [[unlikely]] {
    // TODO
    throw 13;
  }
  auto allocator = _weight->srch_context->linear_allocator;
  bool has_approximation = false;
  float sum_match_cost = 0;
  int64_t sum_approx_cost = 0;
  // Compute match_cost as the average over the get_match_cost of the sub_scorers.
  // This is weighted by the get_cost, which is an expected number of freqs documents.
  for (int32_t i = 0; i < _sub_scorers.size(); ++i) {
    auto disi_wrapper = allocator->allocate_object<DisiWrapper>(_sub_scorers[i]);
    sub_scorers.add(disi_wrapper);
    const int64_t cost_weight = (disi_wrapper->cost <= 1) ? 1 : disi_wrapper->cost;
    sum_approx_cost += cost_weight;
    if (disi_wrapper->two_phase_iterator) {
      has_approximation = true;
      sum_match_cost += disi_wrapper->match_cost * cost_weight;
    }
  }  // End for
  approximation = allocator->allocate_object<DisjunctionDisiApproximation>(&(sub_scorers));

  if (has_approximation) {
    const float match_cost = sum_match_cost / (float) sum_approx_cost;
    two_phase = allocator->allocate_object<DisjunctionScorerTwoPhaseIterator>(
        approximation,
        &sub_scorers,
        allocator,
        needs_scores,
        match_cost);
  }  // End if
}

ManagedPtr<DocIdSetIterator> DisjunctionScorer::get_iterator() {
  if (two_phase) {
    return TwoPhaseIterator::as_doc_id_set_iterator(two_phase, weight->srch_context->linear_allocator);
  } else {
    return approximation;
  }
}

ManagedPtr<TwoPhaseIterator> DisjunctionScorer::two_phase_iterator() {
  return two_phase;
}

float DisjunctionScorer::get_score() {
  return do_score(two_phase == nullptr ? sub_scorers.top_list() : two_phase->get_sub_matches());
}

int32_t DisjunctionScorer::get_doc_id() {
  return sub_scorers.top()->doc;
}

//
// DisjunctionScorerTwoPhaseIterator
//

DisjunctionScorerTwoPhaseIterator::DisjunctionScorerTwoPhaseIterator(
    ManagedPtr<DisjunctionDisiApproximation> _approximation,
    ManagedPtr<DisiPriorityQueue> _sub_scorers,
    ManagedPtr<LinearAllocator> _allocator,
    float _match_cost,
    bool _needs_scores)
    : TwoPhaseIterator(_approximation),
      sub_scorers(_sub_scorers),
      unverified_matches(),
      verified_matches(),
      sub_matches(LinearAllocatorAdapterForSTL<ManagedPtr<DisiWrapper>>(_allocator)),
      match_cost(_match_cost),
      needs_scores(_needs_scores) {
  assert(_sub_scorers);

  TypedVector<ManagedPtr<DisiWrapper>> heap_array;
  heap_array.len = _sub_scorers->get_size();
  heap_array.vec = _allocator->allocate_objects<ManagedPtr<DisiWrapper>>(heap_array.len);
  unverified_matches.init(heap_array);
}

bool DisjunctionScorerTwoPhaseIterator::matches() {
  auto top_list = sub_scorers->top_list();
  bool verified_matches_exist = false;
  unverified_matches.clear();

  for (int32_t i = 0; i < top_list.len; ++i) {
    auto disi_wrapper = top_list.vec[i];
    if (disi_wrapper->two_phase_iterator == nullptr) {
      if (!needs_scores) {
        // we can stop here
        return true;
      }

      // implicitly verified
      verified_matches_exist = true;
    } else {
      unverified_matches.add(disi_wrapper);
    }  // End if
  }  // End for

  if (verified_matches_exist) {
    return true;
  }

  if (unverified_matches.get_size()) {
    auto w = unverified_matches.top();
    unverified_matches.pop();

    while (w->two_phase_iterator->matches()) {
      w->next = nullptr;
      verified_matches = w;
      return true;
    }
  }  // End if

  return false;
}

TypedVector<ManagedPtr<DisiWrapper>> DisjunctionScorerTwoPhaseIterator::get_sub_matches() {
  sub_matches.clear();

  auto sub_matches_vec = unverified_matches.get_heap_array();
  for (int32_t i = 0; i < sub_matches_vec.len; ++i) {
    if (sub_matches_vec.vec[i]->two_phase_iterator->matches()) {
      sub_matches.emplace_back(sub_matches_vec.vec[i]);
    }
  }
  unverified_matches.clear();
  return TypedVector<ManagedPtr<DisiWrapper>>{
      .vec = sub_matches.data(),
      .len = (int32_t) sub_matches.size()
  };
}
