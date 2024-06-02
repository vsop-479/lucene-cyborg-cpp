#include <bit>
#include "MaxScoreBulkScorer.hpp"
#include "../../util/MathUtil.hpp"

using lucene::cyborg::search::LeafCollector;
using lucene::cyborg::search::Scorer;
using lucene::cyborg::search::query::MaxScoreBulkScorer;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MathUtil;

//
// MaxScoreBulkScorer
//
MaxScoreBulkScorer::MaxScoreBulkScorer(
    ManagedPtr<LinearAllocator> _allocator,
    int32_t _max_doc,
    LVector<ManagedPtr<Scorer>> &&_scorers,
    ManagedPtr<LeafCollector> _leaf_collector,
    ManagedPtr<Bits> _accept_docs)
    : BulkScorer(),
      leaf_collector(_leaf_collector),
      accept_docs(_accept_docs),
      max_doc(_max_doc),
      all_scorers(LinearAllocatorAdapterForSTL<ManagedPtr<DisiWrapper>>(_allocator)),
      scratch(LinearAllocatorAdapterForSTL<ManagedPtr<DisiWrapper>>(_allocator)),
      essential_queue(),
      first_essential_scorer(),
      cost(),
      min_competitive_score(),
      min_competitive_score_updated(),
      scorable(this),
      max_score_sums(LinearAllocatorAdapterForSTL<double>(_allocator)),
      window_matches(),
      window_scores() {
  const int32_t num_scorers = _scorers.size();
  all_scorers.resize(num_scorers);
  scratch.resize(num_scorers);
  int64_t cost = 0;
  for (int32_t i = 0; i < num_scorers; ++i) {
    auto disi_wrapper = _allocator->allocate_object<DisiWrapper>(_scorers[i]);
    cost += disi_wrapper->cost;
    all_scorers[i] = disi_wrapper;
  }
  this->cost = cost;
  essential_queue = _allocator->allocate_object<DisiPriorityQueue>(num_scorers, _allocator);
  max_score_sums.resize(num_scorers);
}

int32_t MaxScoreBulkScorer::score(int32_t min_doc_id, int32_t max_doc_id) {
  leaf_collector->set_scorer(&scorable);

  // This scorer computes outer windows based on impacts that are stored in the index. These outer
  // windows should be small enough to provide good upper bounds of scores, and big enough to make
  // sure we spend more time collecting docs than recomputing windows.
  // Then within these outer windows, it creates inner windows of size WINDOW_SIZE that help
  // collect matches into a bitset and save the overhead of rebalancing the priority queue on
  // every match.

  int32_t outer_window_min = min_doc_id;
  while (outer_window_min < max_doc_id) {
    int32_t outer_window_max = update_max_window_scores(outer_window_min);
    outer_window_max = std::min(outer_window_max, max_doc_id);
    if (!partition_scorers()) {
      // No matches in this window
      outer_window_min = outer_window_max;
      continue;
    }

    auto top = essential_queue->top();
    while (top->doc < outer_window_min) {
      top->doc = top->iterator->advance(outer_window_min);
      top = essential_queue->update_top();
    }

    while (top->doc < outer_window_max) {
      score_inner_window(outer_window_max);
      top = essential_queue->top();

      if (min_competitive_score_updated) {
        min_competitive_score_updated = false;
        if (!partition_scorers()) {
          outer_window_min = outer_window_max;
          goto OUTER_CONTINUE;
        } else {
          // Partitioning may have swapped essential and non-essential scorers, and some of the
          // non-essential scorers may be behind the last scored doc. So let's advance to the next
          // candidate match.
          const int32_t next_candidate_match = top->doc;
          top = essential_queue->top();
          while (top->doc < next_candidate_match) {
            top->doc = top->iterator->advance(next_candidate_match);
            top = essential_queue->update_top();
          }
        }
      }
    }
    outer_window_min = outer_window_max;
    OUTER_CONTINUE:;
  }  // End while

  return next_candidate(max_doc_id);
}

void MaxScoreBulkScorer::score_inner_window(int32_t max) {
  auto top = essential_queue->top();
  auto top2 = essential_queue->top2();
  if (top2 == nullptr) {
    score_inner_window_single_essential_clause(max);
  } else if (top2->doc - (INNER_WINDOW_SIZE / 2) >= top->doc) {
    // The first half of the window would match a single clause. Let's collect this single clause
    // until the next doc ID of the next clause.
    score_inner_window_single_essential_clause(std::min(max, top2->doc));
  } else {
    score_inner_window_multiple_essential_clauses(max);
  }  // End if
}

void MaxScoreBulkScorer::score_inner_window_single_essential_clause(int32_t up_to) {
  auto top = essential_queue->top();

  // single essential clause in this window, we can iterate it directly and skip the bitset.
  // this is a common case for 2-clauses queries
  if (!accept_docs) {
    for (int32_t doc = top->doc; doc < up_to; doc = top->iterator->next_doc()) {
      score_non_essential_clauses(doc, top->scorer->get_score());
      if (min_competitive_score_updated) {
        // force scorers to be partitioned again before collecting more hits
        top->iterator->next_doc();
        break;
      }
    }  // End for
  } else {
    for (int32_t doc = top->doc; doc < up_to; doc = top->iterator->next_doc()) {
      if (!accept_docs->get(doc)) {
        continue;
      }
      score_non_essential_clauses(doc, top->scorer->get_score());
      if (min_competitive_score_updated) {
        // force scorers to be partitioned again before collecting more hits
        top->iterator->next_doc();
        break;
      }
    }  // End for
  }  // End if
  top->doc = top->iterator->get_doc_id();
  essential_queue->update_top();
}

void MaxScoreBulkScorer::score_inner_window_multiple_essential_clauses(int32_t max) {
  auto top = essential_queue->top();

  int32_t inner_window_min = top->doc;
  int32_t inner_window_max = (int32_t) std::min((int64_t) max, (int64_t) inner_window_min + INNER_WINDOW_SIZE);

  // Collect matches of essential clauses into a bitset
  do {
    if (!accept_docs) {
      for (int32_t doc = top->doc; doc < inner_window_max; doc = top->iterator->next_doc()) {
        const uint32_t i = doc - inner_window_min;
        window_matches[i >> 6U] |= uint64_t(1) << (i & 63U);
        window_scores[i] += top->scorer->get_score();
      }  // End for
    } else {
      for (int32_t doc = top->doc; doc < inner_window_max && accept_docs->get(doc); doc = top->iterator->next_doc()) {
        const uint32_t i = doc - inner_window_min;
        window_matches[i >> 6U] |= uint64_t(1) << (i & 63U);
        window_scores[i] += top->scorer->get_score();
      }  // End for
    }  // End if
    top->doc = top->iterator->get_doc_id();
    top = essential_queue->update_top();
  } while (top->doc < inner_window_max);

  for (uint32_t word_index = 0; word_index < REQUIRED_BITS_FOR_INNER_WINDOW_SIZE; ++word_index) {
    uint64_t bits = window_matches[word_index];
    window_matches[word_index] = 0;
    while (bits) {
      uint32_t ntz = std::countr_zero(bits);
      bits ^= 1ULL << ntz;
      const uint32_t index = (word_index << 6U) | ntz;
      const double score = window_scores[index];
      window_scores[index] = 0;
      const uint32_t doc = inner_window_min + index;
      score_non_essential_clauses(doc, score);
    }  // End while
  }  // End for
}

int32_t MaxScoreBulkScorer::update_max_window_scores(int32_t window_min) {
  // Only use essential scorers to compute the window's max doc ID, in order to avoid constantly
  // recomputing max scores over small windows
  int32_t first_window_lead = std::min(first_essential_scorer, (int32_t) all_scorers.size() - 1);
  for (int32_t i = 0; i < first_window_lead; ++i) {
    auto scorer = all_scorers[i];
    if (scorer->doc < window_min) {
      scorer->scorer->advance_shallow(window_min);
    }
  }
  int32_t window_max = DocIdSetIterator::NO_MORE_DOCS;
  for (int32_t i = first_window_lead; i < all_scorers.size(); ++i) {
    auto scorer = all_scorers[i];
    const int32_t up_to = scorer->scorer->advance_shallow(std::max(scorer->doc, window_min));
    window_max = std::min((int64_t) window_max, (int64_t) up_to + 1); // up_to is inclusive
  }

  // Score at least an entire inner window of docs
  window_max = std::max(
      window_max, (int32_t) std::min(
          (int64_t) std::numeric_limits<int32_t>::max(), (int64_t) window_min + INNER_WINDOW_SIZE));

  for (auto scorer : all_scorers) {
    if (scorer->doc < window_max) {
      scorer->max_window_score = scorer->scorer->get_max_score(window_max - 1);
    } else {
      scorer->max_window_score = 0;
    }
  }  // End for

  return window_max;
}

void MaxScoreBulkScorer::score_non_essential_clauses(int32_t doc, double essential_score) {
  double score = essential_score;
  for (int32_t i = first_essential_scorer - 1; i >= 0; --i) {
    float max_possible_score =
        (float) MathUtil::sum_upper_bound(score + max_score_sums[i], all_scorers.size());
    if (max_possible_score < min_competitive_score) {
      // Hit is not competitive.
      return;
    }

    auto scorer = all_scorers[i];
    if (scorer->doc < doc) {
      scorer->doc = scorer->iterator->advance(doc);
    }
    if (scorer->doc == doc) {
      score += scorer->scorer->get_score();
    }
  }  // End for

  scorable.doc = doc;
  scorable.score = (float) score;
  leaf_collector->collect_one_doc(doc);
}

bool MaxScoreBulkScorer::partition_scorers() {
// Partitioning scorers is an optimization problem: the optimal set of non-essential scorers is
  // the subset of scorers whose sum of max window scores is less than the minimum competitive
  // score that maximizes the sum of costs.
  // Computing the optimal solution to this problem would take O(2^num_clauses). As a first
  // approximation, we take the first scorers sorted by max_window_score / cost whose sum of max
  // scores is less than the minimum competitive scores. In the common case, maximum scores are
  // inversely correlated with document frequency so this is the same as only sorting by maximum
  // score, as described in the MAXSCORE paper and gives the optimal solution. However, this can
  // make a difference when using custom scores (like FuzzyQuery), high query-time boosts, or
  // scoring based on wacky weights.
  scratch = all_scorers;
  std::sort(scratch.begin(),
            scratch.end(),
            [](ManagedPtr<DisiWrapper> disi_wrapper1, ManagedPtr<DisiWrapper> disi_wrapper2) {
              const double cost1 = disi_wrapper1->max_window_score / std::max((int64_t) 1, disi_wrapper1->cost);
              const double cost2 = disi_wrapper2->max_window_score / std::max((int64_t) 1, disi_wrapper2->cost);
              return cost1 < cost2;
            });

  double max_score_sum = 0;
  first_essential_scorer = 0;
  for (int32_t i = 0; i < all_scorers.size(); ++i) {
    auto w = scratch[i];
    double new_max_score_sum = max_score_sum + w->max_window_score;
    auto max_score_sum_float = (float) MathUtil::sum_upper_bound(new_max_score_sum, first_essential_scorer + 1);
    if (max_score_sum_float < min_competitive_score) {
      max_score_sum = new_max_score_sum;
      all_scorers[first_essential_scorer] = w;
      max_score_sums[first_essential_scorer] = max_score_sum;
      ++first_essential_scorer;
    } else {
      all_scorers[all_scorers.size() - 1 - (i - first_essential_scorer)] = w;
    }
  }  // End for

  if (first_essential_scorer == all_scorers.size()) {
    return false;
  }

  essential_queue->clear();
  for (int32_t i = first_essential_scorer; i < all_scorers.size(); ++i) {
    essential_queue->add(all_scorers[i]);
  }
  return true;
}

int32_t MaxScoreBulkScorer::next_candidate(int32_t range_end) {
  if (range_end >= max_doc) {
    return DocIdSetIterator::NO_MORE_DOCS;
  }

  int32_t next = DocIdSetIterator::NO_MORE_DOCS;
  for (auto scorer : all_scorers) {
    if (scorer->doc < range_end) {
      return range_end;
    } else {
      next = std::min(next, scorer->doc);
    }
  }  // End for

  return next;
}



//
// MaxScoreBulkScorer::ScoreAndDoc
//

MaxScoreBulkScorer::ScoreAndDoc::ScoreAndDoc(ManagedPtr<MaxScoreBulkScorer> _max_score_bulk_scorer)
    : Scorable(),
      score(),
      doc(-1),
      max_score_bulk_scorer(_max_score_bulk_scorer) {
}

void MaxScoreBulkScorer::ScoreAndDoc::set_min_competitive_score(float min_score) {
  max_score_bulk_scorer->min_competitive_score = min_score;
  max_score_bulk_scorer->min_competitive_score_updated = true;
}
