#include <algorithm>
#include <utility>
#include "../Weight.hpp"
#include "BlockMaxConjunctionScorer.hpp"
#include "../../pco/factory/PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco.hpp"

using lucene::cyborg::pco::PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco;
using lucene::cyborg::search::BlockMaxConjunctionScorer;
using lucene::cyborg::search::BlockMaxConjunctionScorerIterator;
using lucene::cyborg::search::BlockMaxConjunctionScorerTwoPhaseIterator;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::search::Weight;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::TypeInfo;
using lucene::cyborg::util::TypeInfoPtr;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;

//
// BlockMaxConjunctionScorer
//

BlockMaxConjunctionScorer::BlockMaxConjunctionScorer(ManagedPtr<Weight> _weight,
                                                     LVector<ManagedPtr<Scorer>> &&_scorers_list)
    : Scorer(_weight),
      scorers(std::move(_scorers_list)),
      two_phases(LinearAllocatorAdapterForSTL<ManagedPtr<TwoPhaseIterator>>{
          _weight->srch_context->linear_allocator}),
      approximations(LinearAllocatorAdapterForSTL<ManagedPtr<DocIdSetIterator>>{
          _weight->srch_context->linear_allocator}),
      min_score() {
  LVector<std::pair<ManagedPtr<Scorer>, ManagedPtr<DocIdSetIterator>>> scorer_and_iterators{
      LinearAllocatorAdapterForSTL<std::pair<ManagedPtr<Scorer>, ManagedPtr<DocIdSetIterator>>>{
          _weight->srch_context->linear_allocator}};
  scorer_and_iterators.reserve(scorers.size());

  for (auto scorer : scorers) {
    scorer_and_iterators.emplace_back(std::make_pair(scorer, scorer->get_iterator()));
  }

  std::sort(scorer_and_iterators.begin(), scorer_and_iterators.end(), [](const auto &s0, const auto &s1) noexcept {
    return s0.second->get_cost() < s1.second->get_cost();
  });

  approximations.reserve(scorers.size());
  for (int32_t i = 0; i < scorers.size(); ++i) {
    auto scorer = scorers[i] = scorer_and_iterators[i].first;
    approximations.emplace_back(scorer_and_iterators[i].second);
    if (auto two_phase = scorers[i]->two_phase_iterator(); two_phase) {
      two_phases.emplace_back(two_phase);
      approximations[i] = two_phase->approximation;
    }
    scorer->advance_shallow(0);
  }  // End for

  if (two_phases.size() > 1) {
    std::sort(two_phases.begin(), two_phases.end(), [](auto tp0, auto tp1) {
      return tp0->get_match_cost() < tp1->get_match_cost();
    });
  }  // End if
}

ManagedPtr<BlockMaxConjunctionScorerTwoPhaseIterator> BlockMaxConjunctionScorer::two_phase_iterator() {
  if (two_phases.empty()) {
    return nullptr;
  }

  float match_cost = 0;
  for (auto two_phase : two_phases) {
    match_cost += two_phase->get_match_cost();
  }

  return weight->srch_context->linear_allocator->allocate_object<BlockMaxConjunctionScorerTwoPhaseIterator>(
      approximation(),
      match_cost,
      this);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorer::get_iterator() {
  return two_phases.empty() ?
         approximation() : TwoPhaseIterator::as_doc_id_set_iterator(two_phase_iterator(),
                                                                    weight->srch_context->linear_allocator);
}

ManagedPtr<DocIdSetIterator> BlockMaxConjunctionScorer::approximation() {
//  return weight->srch_context->linear_allocator->allocate_object<BlockMaxConjunctionScorerIterator>(
//      approximations[0], this);

  return PcoTargetClassFactoryOfLuceneCyborgSearchBlockMaxConjunctionScorerIteratorPco::create_for_search(
      approximations[0], this, weight->srch_context->linear_allocator);
}

int32_t BlockMaxConjunctionScorer::get_doc_id() {
  return scorers.front()->get_doc_id();
}

float BlockMaxConjunctionScorer::get_score() {
  double score = 0;
  for (auto scorer : scorers) {
    score += scorer->get_score();
  }
  return (float) score;
}

int32_t BlockMaxConjunctionScorer::advance_shallow(int32_t target) {
  // We use block boundaries of the lead scorer.
  // It is tempting to fold in other clauses as well to have better bounds of
  // the score, but then there is a risk of not progressing fast enough.
  const int32_t result = scorers.front()->advance_shallow(target);
  // But we still need to shallow-advance other clauses, in order to have
  // better score upper bounds
  for (int32_t i = 1; i < scorers.size(); ++i) {
    scorers[i]->advance_shallow(target);
  }
  return result;
}

float BlockMaxConjunctionScorer::get_max_score(int32_t up_to) {
  double sum = 0;
  for (auto scorer : scorers) {
    sum += scorer->get_max_score(up_to);
  }
  return (float) sum;
}

void BlockMaxConjunctionScorer::set_min_competitive_score(float score) {
  min_score = score;
}

//
// BlockMaxConjunctionScorerTwoPhaseIterator
//

BlockMaxConjunctionScorerTwoPhaseIterator::BlockMaxConjunctionScorerTwoPhaseIterator(DocIdSetIterator *_approximation,
                                                                                     float _match_cost,
                                                                                     ManagedPtr<
                                                                                         BlockMaxConjunctionScorer> _scorer)
    : TwoPhaseIterator(_approximation,
                       TypeInfo::get_type_info<BlockMaxConjunctionScorerTwoPhaseIterator>(
                           "BlockMaxConjunctionScorerTwoPhaseIterator")),
      match_cost(_match_cost),
      scorer(_scorer) {
}

bool BlockMaxConjunctionScorerTwoPhaseIterator::matches() {
  for (auto two_phase : scorer->two_phases) {
    assert(two_phase->approximation->get_doc_id() == scorer->get_doc_id());
    if (!two_phase->matches()) {
      return false;
    }
  }  // End for
  return true;
}

float BlockMaxConjunctionScorerTwoPhaseIterator::get_match_cost() {
  return match_cost;
}

//
// BlockMaxConjunctionScorerIterator
//

BlockMaxConjunctionScorerIterator::BlockMaxConjunctionScorerIterator(
    ManagedPtr<DocIdSetIterator> _lead,
    ManagedPtr<BlockMaxConjunctionScorer> _scorer)
    : DocIdSetIterator(),
      max_score(),
      up_to(-1),
      lead(_lead),
      scorer(_scorer) {
}

int64_t BlockMaxConjunctionScorerIterator::get_cost() {
  return lead->get_cost();
}

int32_t BlockMaxConjunctionScorerIterator::advance(int32_t target) {
  return do_next(lead->advance(advance_target(target)));
}

int32_t BlockMaxConjunctionScorerIterator::get_doc_id() {
  return lead->get_doc_id();
}

int32_t BlockMaxConjunctionScorerIterator::next_doc() {
  return advance(get_doc_id() + 1);
}

void BlockMaxConjunctionScorerIterator::move_to_next_block(int32_t target) {
  up_to = scorer->advance_shallow(target);
  max_score = scorer->get_max_score(up_to);
}

int32_t BlockMaxConjunctionScorerIterator::advance_target(int32_t target) {
  if (target > up_to) {
    move_to_next_block(target);
  }

  const auto min_score = scorer->min_score;

  while (true) {
    assert(up_to >= target);

    if (max_score >= min_score) {
      return target;
    }

    if (up_to == NO_MORE_DOCS) {
      return NO_MORE_DOCS;
    }

    target = up_to + 1;

    move_to_next_block(target);
  }  // End while
}

int32_t BlockMaxConjunctionScorerIterator::do_next(int32_t target) {
  while (true) {
    LOOP_START:
    assert(target == lead->get_doc_id());

    if (target == NO_MORE_DOCS) {
      return NO_MORE_DOCS;
    }

    if (target > up_to) {
      // This check is useful when scorers return information about blocks
      // that do not actually have any matches. Otherwise `target` will always
      // be in the current block already since it is always the result of
      // lead.advance(advance_target(some_doc_id))
      const int32_t next_target = advance_target(target);
      if (next_target != target) {
        target = lead->advance(next_target);
        continue;
      }
    }  // End if

    assert(target <= up_to);

    // then find agreement with other iterators
    for (int32_t i = 1; i < scorer->approximations.size(); ++i) {
      auto other = scorer->approximations[i];
      // other.doc may already be equal to doc if we "continued advanceHead"
      // on the previous iteration and the advance on the lead scorer exactly matched.
      if (other->get_doc_id() < target) {
        const int32_t next = other->advance(target);

        if (next > target) {
          // iterator beyond the current doc - advance lead and continue to the new highest doc.
          target = lead->advance(advance_target(next));
          goto LOOP_START;
        }
      }

      assert(other->get_doc_id() == target);
    }  // End for

    // success - all iterators are on the same doc and the score is competitive
    return target;
  }  // End while
}
