#include <cmath>
#include "WandScorer.hpp"
#include "Weight.hpp"
#include "ScorerUtil.hpp"
#include "../util/MathUtil.hpp"

using lucene::cyborg::search::DisiWrapper;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::TwoPhaseIterator;
using lucene::cyborg::search::WANDScorer;
using lucene::cyborg::search::WANDScorerIterator;
using lucene::cyborg::search::WANDScorerPriorityQueue;
using lucene::cyborg::search::WANDScorerTwoPhaseIterator;
using lucene::cyborg::search::Weight;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MathUtil;
using lucene::cyborg::util::ReadOnlyPtr;

constexpr int32_t FLOAT_MANTISSA_BITS = 24;
constexpr int64_t MAX_SCALED_SCORE = (1L << 24) - 1;

int64_t get_scale_max_score(float max_score, const int32_t scaling_factor) {
  assert(std::isnan(max_score) == false);
  assert(max_score >= 0);

  // NOTE: because doubles have more amplitude than floats for the
  // exponent, the scalb call produces an accurate value.
  const double scaled = std::scalbn((double) max_score, scaling_factor);

  if (scaled > MAX_SCALED_SCORE) {
    // This happens if one scorer returns +Infty as a max score, or if the scorer returns greater
    // max scores locally than globally - which shouldn't happen with well-behaved scorers
    return MAX_SCALED_SCORE;
  }

  return (long) std::ceil(scaled); // round up, cast is accurate since value is < 2^24
}

int64_t get_scale_min_score(float min_score, int32_t scaling_factor) {
  assert(std::isfinite(min_score));
  assert(min_score >= 0);

  // like for scale_max_score, this scalb call is accurate
  double scaled = std::scalbn((double) min_score, scaling_factor);
  // round down, cast might lower the value again if scaled > Long.MAX_VALUE, which is fine
  return (long) std::floor(scaled);
}

int32_t get_exponent(double d) noexcept {
  if (d == 0) {
    return -1023;
  } else if (std::isnan(d)) {
    return 1024;
  }

  int32_t exp;
  std::frexp(d, &exp);
  return exp - 1;
}

int32_t get_scaling_factor(float f) {
  constexpr int32_t DOUBLE_MIN_EXPONENT = -1022;

  if (f < 0) {
    // TODO :
    throw 13;
    // throw new IllegalArgumentException("Scores must be positive or null");
  } else if (f == 0) {
    return get_scaling_factor(std::numeric_limits<float>::min()) + 1;
  } else if (std::isinf(f)) {
    return get_scaling_factor(std::numeric_limits<float>::max()) - 1;
  } else {
    double d = f;
    // Since doubles have more amplitude than floats for the
    // exponent, the cast produces a normal value.
    assert(d == 0 || get_exponent(d) >= DOUBLE_MIN_EXPONENT); // normal double
    return FLOAT_MANTISSA_BITS - 1 - get_exponent(d);
  }
}

//
// WANDScorer
//

WANDScorer::WANDScorer(ManagedPtr<Weight> _weight,
                       LVector<ManagedPtr<Scorer>> &&_scorers,
                       int32_t _min_should_match,
                       ScoreMode _score_mode)
    : Scorer(_weight),
      doc(-1),
      upto(-1),  // will be computed on the first call to next_doc/advance
      scaling_factor(),
      lead_max_score(),
      cost(),
      min_competitive_score(),
      head(_scorers.size(), _weight->srch_context->linear_allocator),
      min_should_match(_min_should_match),
      score_mode(_score_mode),
      all_scorers(std::move(_scorers)),
      tail_priority_queue(_weight->srch_context->linear_allocator, this),
      leads(LinearAllocatorAdapterForSTL<ManagedPtr<DisiWrapper>>(_weight->srch_context->linear_allocator)) {
  if (_min_should_match >= all_scorers.size()) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException("minShouldMatch should be < the number of scorers");
  }
  assert(min_should_match >= 0);

  // there can be at most num_scorers - 1 scorers beyond the current position

  if (score_mode == ScoreMode::TOP_SCORES) {
    // To avoid accuracy issues with floating-point numbers, this scorer operates on scaled longs.
    // How do you choose the scaling factor? The thing is that we want to retain as many
    // significant bits as possible, but not too many, otherwise operations on longs would be more
    // precise than the equivalent operations on their unscaled counterparts and we might skip too
    // many hits. So we compute the maximum possible score produced by this scorer, which is the
    // sum of the maximum scores of each clause, and compute a scaling factor that would preserve
    // 24 bits of accuracy - the number of mantissa bits of single-precision floating-point
    // numbers.
    double max_score_sum_double = 0;
    for (auto scorer : all_scorers) {
      scorer->advance_shallow(0);
      float maxScore = scorer->get_max_score(DocIdSetIterator::NO_MORE_DOCS);
      max_score_sum_double += maxScore;
    }
    const auto max_score_sum = (float) MathUtil::sum_upper_bound(max_score_sum_double, all_scorers.size());
    scaling_factor = get_scaling_factor(max_score_sum);
  }  // End if

  leads.reserve(all_scorers.size());
  for (auto scorer : all_scorers) {
    add_lead(_weight->srch_context->linear_allocator->allocate_object<DisiWrapper>(scorer));
  }

  LVector<int64_t> costs{LinearAllocatorAdapterForSTL<int64_t>{_weight->srch_context->linear_allocator}};
  costs.reserve(all_scorers.size());
  for (auto scorer : all_scorers) {
    costs.emplace_back(scorer->get_iterator()->get_cost());
  }

  cost = ScorerUtil::cost_with_min_should_match(costs.data(), costs.size(), min_should_match);
}

void WANDScorer::set_min_competitive_score(float min_score) {
  // Let this disjunction know about the new min score so that it can skip
  // over clauses that produce low scores.
  assert(score_mode == ScoreMode::TOP_SCORES);
  assert(min_score >= 0);
  int64_t scaled_min_score = get_scale_min_score(min_score, scaling_factor);
  assert(scaled_min_score >= min_competitive_score);
  min_competitive_score = scaled_min_score;
}

ManagedPtr<DocIdSetIterator> WANDScorer::get_iterator() {
  return TwoPhaseIterator::as_doc_id_set_iterator(two_phase_iterator(), weight->srch_context->linear_allocator);
}

ManagedPtr<TwoPhaseIterator> WANDScorer::two_phase_iterator() {
  auto approximation = weight->srch_context->linear_allocator->allocate_object<WANDScorerIterator>(this);

  return weight->srch_context->linear_allocator->allocate_object<WANDScorerTwoPhaseIterator>(
      approximation, this);
}

float WANDScorer::get_score() {
  // We need to know about all matches
  advance_all_tail();

  double score = 0;
  for (auto lead_disi : leads) {
    score += lead_disi->scorer->get_score();
  }

  return (float) score;
}

int32_t WANDScorer::advance_shallow(int32_t target) {
  // Propagate to improve score bounds
  for (auto scorer : all_scorers) {
    if (scorer->get_doc_id() < target) {
      scorer->advance_shallow(target);
    }
  }

  if (target <= upto) {
    return upto;
  }

  return DocIdSetIterator::NO_MORE_DOCS;
}

float WANDScorer::get_max_score(int32_t upto) {
  double max_score_sum = 0;
  for (auto scorer : all_scorers) {
    if (scorer->get_doc_id() <= upto) {
      max_score_sum += scorer->get_max_score(upto);
    }
  }
  return (float) MathUtil::sum_upper_bound(max_score_sum, all_scorers.size());
}

int32_t WANDScorer::get_doc_id() {
  return doc;
}

bool WANDScorer::ensure_consistent() {
#ifndef NDEBUG
  if (score_mode == ScoreMode::TOP_SCORES) {
    uint64_t max_score_sum = 0;
    for (int32_t i = 0; i < tail_priority_queue.tail_size; ++i) {
      assert(tail_priority_queue.tail[i]->doc < doc);
      uint64_t tmp_max_score_sum = max_score_sum + tail_priority_queue.tail[i]->scaled_max_score;
      if ((int64_t) ((tmp_max_score_sum ^ max_score_sum)
          & (tmp_max_score_sum ^ ((uint64_t) tail_priority_queue.tail[i]->scaled_max_score))) < 0) {
        // TODO : overflow
        throw 13;
      }
      max_score_sum = tmp_max_score_sum;
    }
    assert(max_score_sum == tail_priority_queue.tail_max_score);

    max_score_sum = 0;
    for (auto lead_disi : leads) {
      assert(lead_disi->doc == doc);
      uint64_t tmp_max_score_sum = max_score_sum + lead_disi->scaled_max_score;
      if ((int64_t) ((tmp_max_score_sum ^ max_score_sum)
          & (tmp_max_score_sum ^ ((uint64_t) lead_disi->scaled_max_score))) < 0) {
        // TODO : overflow
        throw 13;
      }
      max_score_sum = tmp_max_score_sum;
    }
    assert(max_score_sum == lead_max_score);

    assert(min_competitive_score == 0
               || tail_priority_queue.tail_max_score < min_competitive_score
               || tail_priority_queue.tail_size < min_should_match);
    assert(doc <= upto);
  }  // End if

  auto head_inner_array = head.get_internal_array();
  for (int32_t i = 0; i < head.get_size(); ++i) {
    assert(head_inner_array.vec[i]->doc > doc);
  }

  return true;
#endif
}

void WANDScorer::add_lead(ManagedPtr<DisiWrapper> lead) {
  leads.emplace_back(lead);
  lead_max_score += lead->scaled_max_score;
}

void WANDScorer::push_back_leads(int32_t target) {
  for (auto lead_disi : leads) {
    auto evicted = tail_priority_queue.insert_tail_with_over_flow(lead_disi);
    if (evicted) {
      evicted->doc = evicted->iterator->advance(target);
      head.add(evicted);
    }
  }  // End for

  leads.clear();
  lead_max_score = 0;
}

void WANDScorer::advance_head(int32_t target) {
  auto head_top = head.top();
  while (head_top && head_top->doc < target) {
    auto evicted = tail_priority_queue.insert_tail_with_over_flow(head_top);
    if (evicted) {
      evicted->doc = evicted->iterator->advance(target);
      head_top = head.update_top(evicted);
    } else {
      head.pop();
      head_top = head.top();
    }
  }  // End while
}

void WANDScorer::advance_tail(ManagedPtr<DisiWrapper> disi) {
  disi->doc = disi->iterator->advance(doc);
  if (disi->doc == doc) {
    add_lead(disi);
  } else {
    head.add(disi);
  }  // End if
}

void WANDScorer::advance_tail() {
  auto top = tail_priority_queue.pop_tail();
  advance_tail(top);
}

void WANDScorer::update_max_scores(int32_t target) {
  if (head.get_size() == 0) {
    upto = tail_priority_queue.tail[0]->scorer->advance_shallow(target);
  } else {
    int32_t next_upto = DocIdSetIterator::NO_MORE_DOCS;
    auto head_inner_array = head.get_internal_array();
    for (int32_t i = 0; i < head.get_size(); ++i) {
      auto head_disi = head_inner_array.vec[i];
      if (head_disi->doc <= next_upto) {
        next_upto = std::min(head_disi->scorer->advance_shallow(head_disi->doc), next_upto);
        head_disi->scaled_max_score = get_scale_max_score(head_disi->scorer->get_max_score(next_upto), scaling_factor);
      }
    }  // End for

    upto = next_upto;
  }  // End if

  tail_priority_queue.tail_max_score = 0L;

  for (int32_t i = 0; i < tail_priority_queue.tail_size; ++i) {
    auto tail_disi = tail_priority_queue.tail[i];
    tail_disi->scorer->advance_shallow(target);
    tail_disi->scaled_max_score = get_scale_max_score(tail_disi->scorer->get_max_score(upto), scaling_factor);
    tail_priority_queue.up_heap_max_score(i);
    tail_priority_queue.tail_max_score += tail_disi->scaled_max_score;
  }

  while (tail_priority_queue.tail_size > 0 && tail_priority_queue.tail_max_score >= min_competitive_score) {
    auto w = tail_priority_queue.pop_tail();
    w->doc = w->iterator->advance(target);
    head.add(w);
  }  // End while
}

void WANDScorer::update_max_scores_if_necessary(int32_t target) {
  assert(leads.empty());

  while (upto < DocIdSetIterator::NO_MORE_DOCS) {
    if (head.get_size() != 0) {
      if (head.top()->doc > upto) {
        assert(head.top()->doc >= target);
        update_max_scores(target);
      }

      break;
    }  // End if

    target = std::max(target, upto + 1);
    update_max_scores(target);
  }  // End while

  assert (head.get_size() == 0 && upto == DocIdSetIterator::NO_MORE_DOCS
              || (head.get_size() > 0 && head.top()->doc <= upto));
  assert(upto >= target);
}

void WANDScorer::move_to_next_candidate(int32_t target) {
  if (score_mode == ScoreMode::TOP_SCORES) {
    update_max_scores_if_necessary(target);

    assert(upto >= target);

    if (head.get_size() == 0) {
      assert(upto == DocIdSetIterator::NO_MORE_DOCS);
      doc = DocIdSetIterator::NO_MORE_DOCS;
      return;
    }
  }  // End if

  leads.clear();
  leads.emplace_back(head.pop());
  lead_max_score = leads.back()->scaled_max_score;
  doc = leads.back()->doc;

  while (head.get_size() > 0 && head.top()->doc == doc) {
    add_lead(head.top());
    head.pop();
  }
}

int32_t WANDScorer::do_next_competitive_candidate() {
  while (lead_max_score + tail_priority_queue.tail_max_score < min_competitive_score
      || leads.size() + tail_priority_queue.tail_size < min_should_match) {
    // No match on doc is possible, move to the next potential match
    push_back_leads(doc + 1);
    move_to_next_candidate(doc + 1);
    assert(ensure_consistent());
    if (doc == DocIdSetIterator::NO_MORE_DOCS) {
      break;
    }
  }  // End while

  return doc;
}

void WANDScorer::advance_all_tail() {
  for (int32_t i = tail_priority_queue.tail_size - 1; i >= 0; --i) {
    advance_tail(tail_priority_queue.tail[i]);
  }

  tail_priority_queue.tail_size = 0;
  tail_priority_queue.tail_max_score = 0L;
  assert(ensure_consistent());
}



//
// WANDScorerIterator
//

WANDScorerIterator::WANDScorerIterator(ManagedPtr<WANDScorer> _scorer)
    : DocIdSetIterator(),
      scorer(_scorer),
      doc(&(_scorer->doc)),
      cost(&(_scorer->cost)) {
}

int32_t WANDScorerIterator::advance(int32_t target) {
  assert(scorer->ensure_consistent());

  // Move 'lead' iterators back to the tail
  scorer->push_back_leads(target);

  // Advance 'head' as well
  scorer->advance_head(target);

  // Pop the new 'lead' from 'head'
  scorer->move_to_next_candidate(target);

  if (*doc != DocIdSetIterator::NO_MORE_DOCS) {
    assert(scorer->ensure_consistent());

    // Advance to the next possible match
    return scorer->do_next_competitive_candidate();
  }

  return DocIdSetIterator::NO_MORE_DOCS;
}



//
// WANDScorerTwoPhaseIterator
//

WANDScorerTwoPhaseIterator::WANDScorerTwoPhaseIterator(ManagedPtr<WANDScorerIterator> _approximation,
                                                       ManagedPtr<WANDScorer> _scorer)
    : TwoPhaseIterator(_approximation),
      scorer(_scorer),
      match_cost(_scorer->tail_priority_queue.tail.size()),
      lead_max_score(&(_scorer->lead_max_score)),
      tail_max_score(&(_scorer->tail_priority_queue.tail_max_score)),
      tail_size(&(_scorer->tail_priority_queue.tail_size)),
      min_competitive_score(&(_scorer->min_competitive_score)),
      min_should_match(&(_scorer->min_should_match)) {
}

bool WANDScorerTwoPhaseIterator::matches() {
  while (*lead_max_score < *min_competitive_score || scorer->leads.size() < *min_should_match) {
    if (*lead_max_score + *tail_max_score < *min_competitive_score
        || scorer->leads.size() + *tail_size < *min_should_match) {
      return false;
    } else {
      // A match on doc is still possible, try to
      // advance scorers from the tail
      scorer->advance_tail();
    }
  }

  return true;
}

//
// WANDScorerPriorityQueue
//

WANDScorerPriorityQueue::WANDScorerPriorityQueue(ManagedPtr<LinearAllocator> _allocator,
                                                 ManagedPtr<WANDScorer> _scorer)
    : tail(LinearAllocatorAdapterForSTL<ManagedPtr<DisiWrapper>>(_allocator)),
      tail_size(),
      min_should_match(&(_scorer->min_should_match)),
      tail_max_score(),
      min_competitive_score(&(_scorer->min_competitive_score)) {
  tail.resize(_scorer->all_scorers.size());
}

void WANDScorerPriorityQueue::add_tail(ManagedPtr<DisiWrapper> disi) {
  assert(tail_size < tail.size());
  tail[tail_size] = disi;
  up_heap_max_score(tail_size++);
}

ManagedPtr<DisiWrapper> WANDScorerPriorityQueue::pop_tail() {
  assert(tail_size > 0);
  auto result = tail[0];
  tail[0] = tail[--tail_size];
  down_heap_max_score();
  tail_max_score -= result->scaled_max_score;
  return result;
}

void WANDScorerPriorityQueue::up_heap_max_score(int32_t index) {
  auto node = tail[index];
  int32_t j = (index - 1) / 2;
  while (j >= 0 && greater_max_score(node, tail[j])) {
    tail[index] = tail[j];
    index = j;
    j = (j - 1) / 2;
  }

  tail[index] = node;
}

void WANDScorerPriorityQueue::down_heap_max_score() {
  int32_t i = 0;
  auto node = tail[0];
  int32_t j = i * 2 + 1;
  if (j < tail_size) {
    int32_t k = j * 2 + 2;
    if (k < tail_size && greater_max_score(tail[k], tail[j])) {
      j = k;
    }
    if (greater_max_score(tail[j], node)) {
      do {
        tail[i] = tail[j];
        i = j;
        j = i * 2 + 1;
        k = j * 2 + 2;
        if (k < tail_size && greater_max_score(tail[k], tail[j])) {
          j = k;
        }
      } while (j < tail_size && greater_max_score(tail[j], node));
      tail[i] = node;
    }
  }  // End if
}

bool WANDScorerPriorityQueue::greater_max_score(ManagedPtr<DisiWrapper> disi1,
                                                ManagedPtr<DisiWrapper> disi2) {
  if (disi1->scaled_max_score != disi2->scaled_max_score) {
    return disi1->scaled_max_score > disi2->scaled_max_score;
  }
  return disi1->cost < disi2->cost;
}

ManagedPtr<DisiWrapper> WANDScorerPriorityQueue::insert_tail_with_over_flow(ManagedPtr<DisiWrapper> s) {
  if (tail_max_score + s->scaled_max_score < *min_competitive_score || tail_size + 1 < *min_should_match) {
    // we have free room for this new entry
    add_tail(s);
    tail_max_score += s->scaled_max_score;
    return nullptr;
  } else if (tail_size == 0) {
    return s;
  } else {
    auto top = tail[0];
    if (!greater_max_score(top, s)) {
      return s;
    }

    // Swap top and s
    tail[0] = s;
    down_heap_max_score();
    tail_max_score += s->scaled_max_score - top->scaled_max_score;
    return top;
  }
}
