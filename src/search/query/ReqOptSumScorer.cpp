#include <limits>
#include "ReqOptSumScorer.hpp"
#include "../Weight.hpp"

using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::ReqOptSumScorer;
using lucene::cyborg::search::query::ReqOptSumScorerIterator;
using lucene::cyborg::search::query::ReqOptSumScorerTwoPhaseIterator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;

//
// ReqOptSumScorer
//

ReqOptSumScorer::ReqOptSumScorer(ManagedPtr<Scorer> _req_scorer,
                                 ManagedPtr<Scorer> _opt_scorer,
                                 ScoreMode _score_mode)
    : Scorer(_req_scorer->get_weight()),
      req_scorer(_req_scorer),
      opt_scorer(_opt_scorer),
      req_approximation(),
      opt_approximation(),
      opt_two_phase(),
      approximation(),
      two_phase(),
      min_score(),
      req_max_score(),
      opt_is_required() {
  assert(_req_scorer);
  assert(_opt_scorer);

  auto req_two_phase = req_scorer->two_phase_iterator();
  opt_two_phase = opt_scorer->two_phase_iterator();
  if (req_two_phase) {
    req_approximation = req_two_phase->approximation;
  } else {
    req_approximation = req_scorer->get_iterator();
  }
  if (opt_two_phase) {
    opt_approximation = opt_two_phase->approximation;
  } else {
    opt_approximation = opt_scorer->get_iterator();
  }

  if (_score_mode != ScoreMode::TOP_SCORES) {
    approximation = req_approximation;
    req_max_score = std::numeric_limits<float>::infinity();
  } else {
    req_scorer->advance_shallow(0);
    opt_scorer->advance_shallow(0);
    req_max_score = req_scorer->get_max_score(DocIdSetIterator::NO_MORE_DOCS);
    approximation = weight->srch_context->linear_allocator->allocate_object<ReqOptSumScorerIterator>(this);
  }

  if (req_two_phase || opt_two_phase) {
    two_phase = weight->srch_context->linear_allocator->allocate_object<ReqOptSumScorerTwoPhaseIterator>(
        this, req_two_phase);
  }  // End if
}

float ReqOptSumScorer::get_score() {
  // TODO: sum into a double and cast to float if we ever send required clauses to BS1
  const int32_t cur_doc = req_scorer->get_doc_id();
  float score = req_scorer->get_score();

  int32_t opt_scorer_doc = opt_approximation->get_doc_id();
  if (opt_scorer_doc < cur_doc) {
    opt_scorer_doc = opt_approximation->advance(cur_doc);
    if (opt_two_phase && opt_scorer_doc == cur_doc && !opt_two_phase->matches()) {
      opt_scorer_doc = opt_approximation->next_doc();
    }
  }
  if (opt_scorer_doc == cur_doc) {
    score += opt_scorer->get_score();
  }

  return score;
}

ManagedPtr<DocIdSetIterator> ReqOptSumScorer::get_iterator() {
  if (!two_phase) {
    return approximation;
  } else {
    return TwoPhaseIterator::as_doc_id_set_iterator(two_phase, weight->srch_context->linear_allocator);
  }
}

int32_t ReqOptSumScorer::advance_shallow(int32_t target) {
  const int32_t up_to = req_scorer->advance_shallow(target);
  const int32_t opt_doc_id = opt_scorer->get_doc_id();
  if (opt_doc_id <= target) {
    return std::min(up_to, opt_scorer->advance_shallow(target));
  } else if (opt_doc_id != DocIdSetIterator::NO_MORE_DOCS) {
    return std::min(up_to, opt_doc_id - 1);
  }
  return up_to;
}

float ReqOptSumScorer::get_max_score(const int32_t upto) {
  float max_score = req_scorer->get_max_score(upto);
  if (opt_scorer->get_doc_id() <= upto) {
    max_score += opt_scorer->get_max_score(upto);
  }
  return max_score;
}

//
// ReqOptSumScorerTwoPhaseIterator
//

ReqOptSumScorerTwoPhaseIterator::ReqOptSumScorerTwoPhaseIterator(ManagedPtr<ReqOptSumScorer> _req_opt_sum_scorer,
                                                                 ManagedPtr<TwoPhaseIterator> _req_two_phase)
    : TwoPhaseIterator(_req_opt_sum_scorer->approximation),
      req_two_phase(_req_two_phase),
      opt_two_phase(_req_opt_sum_scorer->opt_two_phase),
      opt_is_required(&(_req_opt_sum_scorer->opt_is_required)),
      opt_approximation(_req_opt_sum_scorer->opt_approximation),
      req_scorer(_req_opt_sum_scorer->req_scorer) {
  assert(_req_opt_sum_scorer->opt_is_required);
}

bool ReqOptSumScorerTwoPhaseIterator::matches() {
  if (req_two_phase && req_two_phase->matches()) {
    return false;
  }

  if (opt_two_phase) {
    if (!(*opt_is_required)) {
      if (opt_approximation->get_doc_id() == req_scorer->get_doc_id()
          && !opt_two_phase->matches()) {
        // Advance the iterator to make it clear it doesn't match the current doc id
        opt_approximation->next_doc();
      }
    } else {
      // The below condition is rare and can only happen if we transitioned to
      // opt_is_required = true
      // after the opt approximation was advanced and before it was confirmed.
      if (req_scorer->get_doc_id() != opt_approximation->get_doc_id()) {
        if (opt_approximation->get_doc_id() < req_scorer->get_doc_id()) {
          opt_approximation->advance(req_scorer->get_doc_id());
        }
        if (req_scorer->get_doc_id() != opt_approximation->get_doc_id()) {
          return false;
        }
      }
      if (!opt_two_phase->matches()) {
        // Advance the iterator to make it clear it doesn't match the current doc id
        opt_approximation->next_doc();
        return false;
      }
    }  // End if
  }  // End if

  return true;
}

float ReqOptSumScorerTwoPhaseIterator::get_match_cost() {
  float match_cost = 1;

  if (req_two_phase) {
    match_cost += req_two_phase->get_match_cost();
  }
  if (opt_two_phase) {
    match_cost += opt_two_phase->get_match_cost();
  }
  return match_cost;
}

//
// ReqOptSumScorerIterator
//

ReqOptSumScorerIterator::ReqOptSumScorerIterator(ManagedPtr<ReqOptSumScorer> _req_opt_sum_scorer)
    : DocIdSetIterator(),
      req_opt_sum_scorer(_req_opt_sum_scorer),
      up_to(-1),
      max_score(),
      req_scorer(_req_opt_sum_scorer->req_scorer),
      opt_is_required(&(_req_opt_sum_scorer->opt_is_required)),
      min_score(&(_req_opt_sum_scorer->min_score)),
      req_max_score(&(_req_opt_sum_scorer->req_max_score)),
      req_approximation(_req_opt_sum_scorer->req_approximation),
      opt_approximation(_req_opt_sum_scorer->opt_approximation) {
}

int64_t ReqOptSumScorerIterator::get_cost() {
  return req_approximation->get_cost();
}

int32_t ReqOptSumScorerIterator::advance(int32_t target) {
  return advance_internal(req_approximation->get_doc_id() + 1);
}

int32_t ReqOptSumScorerIterator::get_doc_id() {
  return req_approximation->get_doc_id();
}

int32_t ReqOptSumScorerIterator::next_doc() {
  return advance_internal(req_approximation->get_doc_id() + 1);
}

int32_t ReqOptSumScorerIterator::advance_internal(int32_t target) {
  if (target != NO_MORE_DOCS) {
    auto req_doc = target;
    while (true) {
      ADVANCE_HEAD:
      if (*min_score != 0) {
        req_doc = advance_impacts(req_doc);
      }
      if (req_approximation->get_doc_id() < req_doc) {
        req_doc = req_approximation->advance(req_doc);
      }
      if (req_doc == NO_MORE_DOCS || !(*opt_is_required)) {
        return req_doc;
      }
      int32_t upper_bound = (*req_max_score) < (*min_score) ? NO_MORE_DOCS : up_to;
      if (req_doc > upper_bound) {
        continue;
      }

      // Find the next common doc within the current block
      while (true) { // invariant: reqDoc >= optDoc
        int32_t opt_doc = opt_approximation->get_doc_id();
        if (opt_doc < req_doc) {
          opt_doc = opt_approximation->advance(req_doc);
        }
        if (opt_doc > upper_bound) {
          req_doc = upper_bound + 1;
          goto ADVANCE_HEAD;
        }

        if (opt_doc != req_doc) {
          req_doc = req_approximation->advance(opt_doc);
          if (req_doc > upper_bound) {
            goto ADVANCE_HEAD;
          }
        }

        if (req_doc == NO_MORE_DOCS || opt_doc == req_doc) {
          return req_doc;
        }
      }  // End while
    }  // End while
  } else {
    req_approximation->advance(NO_MORE_DOCS);
    return NO_MORE_DOCS;
  }  // End if
}

int32_t ReqOptSumScorerIterator::advance_impacts(int32_t target) {
  if (target > up_to) {
    move_to_next_block(target);
  }

  assert(min_score);
  while (true) {
    if (max_score >= *min_score) {
      return target;
    }

    if (up_to == NO_MORE_DOCS) {
      return NO_MORE_DOCS;
    }

    target = up_to + 1;

    move_to_next_block(target);
  }  // End while
}

void ReqOptSumScorerIterator::move_to_next_block(int32_t target) {
  up_to = req_opt_sum_scorer->advance_shallow(target);
  float req_max_score_block = req_scorer->get_max_score(up_to);
  max_score = req_opt_sum_scorer->get_max_score(up_to);

  // Potentially move to a conjunction
  assert(min_score);
  *opt_is_required = req_max_score_block < *min_score;
}
