#include "BooleanScorer.hpp"
#include "../../pco/factory/PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer.hpp"

using lucene::cyborg::pco::PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer;
using lucene::cyborg::search::BulkScorer;
using lucene::cyborg::search::DocIdConsumer;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::query::BooleanScorer;
using lucene::cyborg::search::query::BooleanScorerOrCollector;
using lucene::cyborg::search::query::BooleanWeight;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MemoryUtils;
using lucene::cyborg::util::PriorityQueue;
using lucene::cyborg::util::PriorityQueueConstants;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

//
// BooleanScorer
//

BooleanScorer::BooleanScorer(ManagedPtr<LeafCollector> _leaf_collector,
                             ManagedPtr<BooleanWeight> _weight,
                             LVector<ManagedPtr<BulkScorer>> &&_scorers,
                             int32_t _min_should_match)
    : BulkScorer(),
      leaf_collector(_leaf_collector),
      cost(),
      leads(),
      head(),
      tail(),
      or_collector(_weight->srch_context,
                   _leaf_collector,
                   score_mode_need_scores(_weight->score_mode),
                   _min_should_match),
      min_should_match(_min_should_match) {
  if (_min_should_match < 1 || _min_should_match > _scorers.size()) [[unlikely]] {
    // TODO
    throw 13;
  }
  if (_scorers.size() <= 1) [[unlikely]] {
    // TODO
    throw 13;
  }

  auto allocator = _weight->srch_context->linear_allocator;

  // Init leads
  leads.len = _scorers.size();
  leads.vec = allocator->allocate_objects<ManagedPtr<BulkScorerAndDoc>>(leads.len);

  // Init heads, tails
  TypedVector<ManagedPtr<BulkScorerAndDoc>> heap_array;
  heap_array.len = _scorers.size() - _min_should_match + 1 + PriorityQueueConstants::START_INDEX;
  heap_array.vec = allocator->allocate_objects<ManagedPtr<BulkScorerAndDoc>>(heap_array.len);
  head.init(heap_array);

  if (_min_should_match > 1) {
    heap_array.len =
        // 1-based, make sure we're creating at least 2-sized array.
        std::max(_min_should_match - 1 + PriorityQueueConstants::START_INDEX, 2);
    heap_array.vec = allocator->allocate_objects<ManagedPtr<BulkScorerAndDoc>>(heap_array.len);
    tail.init(heap_array);
  } else {
    // Give a nullptr placeholder so that calling top() always gives nullptr.
    tail.get_heap_array().vec =
        // 1-based
        allocator->allocate_objects<ManagedPtr<BulkScorerAndDoc>>(1 + PriorityQueueConstants::START_INDEX);
  }  // End if

  // For searching
  for (auto scorer : _scorers) {
    scorer->reset_collector(&or_collector);
    auto evicted = tail.insert_with_overflow(allocator->allocate_object<BulkScorerAndDoc>(scorer));
    if (evicted) {
      head.add(*evicted);
    }
  }  // End for

  // Init cost
  cost = get_cost(allocator, _scorers, min_should_match);
}

int32_t BooleanScorer::score(int32_t min_doc_id, int32_t max_doc_id) {
  or_collector.score_and_doc.doc = -1;
  leaf_collector->set_scorer(&(or_collector.score_and_doc));

  auto top = advance_sub_bulk_scorers(min_doc_id);
  while (top->next < max_doc_id) {
    top = score_window(top, min_doc_id, max_doc_id);
  }

  return top->next;
}

int64_t BooleanScorer::get_cost(ManagedPtr<LinearAllocator> allocator,
                                LVector<ManagedPtr<BulkScorer>> &scorers,
                                int32_t min_should_match) {
  struct BulkScorerPriorityQueue final : public PriorityQueue<ManagedPtr<BulkScorer>, BulkScorerPriorityQueue> {
    bool less_than(ManagedPtr<BulkScorer> bs0, ManagedPtr<BulkScorer> bs1) {
      return bs0->get_cost() > bs1->get_cost();
    }
  };  // HeadPriorityQueue

  TypedVector<ManagedPtr<BulkScorer>> heap_array;
  heap_array.len = scorers.size() - min_should_match + 1 + 1;
  heap_array.vec = allocator->allocate_objects<ManagedPtr<BulkScorer>>(heap_array.len);
  BulkScorerPriorityQueue pq{};
  pq.init(heap_array);

  for (auto scorer : scorers) {
    pq.insert_with_overflow(scorer);
  }

  int64_t cost = 0;
  for (int32_t i = PriorityQueueConstants::START_INDEX; i < heap_array.len; ++i) {
    cost += heap_array.vec[i]->get_cost();
  }

  return cost;
}

ManagedPtr<BooleanScorer::BulkScorerAndDoc> BooleanScorer::score_window(ManagedPtr<BulkScorerAndDoc> top,
                                                                        int32_t min_doc_id,
                                                                        int32_t max_doc_id) {
  int32_t window_base = top->next & ~BS_MASK;
  int32_t window_min = std::max(min_doc_id, window_base);
  int32_t window_max = std::min(max_doc_id, window_base + BS_BUCKET_LIST_SIZE);

  // Fill 'leads' with all scorers from 'head' that are in the right window
  leads.vec[0] = head.top();
  head.pop();
  int32_t max_freq = 1;
  while (head.get_size() > 0 && head.top()->next < window_max) {
    leads.vec[max_freq++] = head.top();
    head.pop();
  }

  if (min_should_match != 1 || max_freq != 1) {
    // general case, collect through a bit set first and then replay
    score_window_multiple_scorers(window_min, window_max, max_freq);
    return head.top();
  } else {
    // special case: only one get_scorer can match in the current window,
    // we can collect directly
    auto bulk_scorer = leads.vec[0];
    score_window_single_scorer(bulk_scorer, window_min, window_max, max_doc_id);
    return head.add(bulk_scorer);
  }  // End if
}

ManagedPtr<BooleanScorer::BulkScorerAndDoc> BooleanScorer::advance_sub_bulk_scorers(int32_t min_doc_id) {
  assert(tail.get_size() == min_should_match - 1);
  auto head_top = head.top();
  auto tail_top = tail.top();

  while (head_top->next < min_doc_id) {
    if (tail_top == nullptr || head_top->cost <= tail_top->cost) {
      head_top->advance(min_doc_id);
      head.update_top();
      head_top = head.top();
    } else {
      // swap the top of head and tail
      auto previous_head_top = head_top;
      tail_top->advance(min_doc_id);
      head.update_top(tail_top);
      head_top = head.top();
      tail.update_top(previous_head_top);
      tail_top = tail.top();
    }
  }  // End while

  return head_top;
}

void BooleanScorer::score_window_multiple_scorers(int32_t window_min,
                                                  int32_t window_max,
                                                  int32_t max_freq) {
  while (max_freq < min_should_match && max_freq + tail.get_size() >= min_should_match) {
    // a match is still possible
    auto candidate = tail.top();
    tail.pop();
    candidate->advance(window_min);
    if (candidate->next < window_max) {
      leads.vec[max_freq++] = candidate;
    } else {
      head.add(candidate);
    }
  }  // End while

  if (max_freq >= min_should_match) {
    // There might be matches in other scorers from the tail too
    for (int32_t i = PriorityQueueConstants::START_INDEX; i < tail.get_size(); ++i) {
      leads.vec[max_freq++] = tail.get_heap_array().vec[i];
    }
    tail.clear();

    score_window_into_bit_set_and_replay(window_min, window_max, max_freq);
  }  // End if

  // Push back scorers into head and tail
  for (int32_t i = 0; i < max_freq; ++i) {
    auto evicted = head.insert_with_overflow(leads.vec[i]);
    if (evicted) {
      tail.add(*evicted);
    }
  }  // End for
}

void BooleanScorer::score_window_single_scorer(ManagedPtr<BulkScorerAndDoc> bulk_scorer,
                                               int32_t window_min,
                                               int32_t window_max,
                                               int32_t max_doc_id) {
  assert(tail.get_size() == 0);
  const int32_t next_window_base = head.top()->next & ~BS_MASK;
  const int32_t end_doc_id = std::max(window_max, std::min(max_doc_id, next_window_base));

  bulk_scorer->scorer->reset_collector(leaf_collector);
  bulk_scorer->score(window_min, end_doc_id);
  bulk_scorer->scorer->reset_collector(&or_collector);
  leaf_collector->set_scorer(&(or_collector.score_and_doc));
}

void BooleanScorer::score_window_into_bit_set_and_replay(int32_t min_doc_id,
                                                         int32_t max_doc_id,
                                                         int32_t num_scorers) {

  or_collector.prepare_collecting_multiple_sources(min_doc_id);

  for (int32_t i = 0; i < num_scorers; ++i) {
    auto scorer = leads.vec[i];
    assert(scorer->next < max_doc_id);
    scorer->score(min_doc_id, max_doc_id);
  }  // End for

  or_collector.parent_collector->collect(&or_collector);
}



//
//  BooleanScorerOrCollector
//

BooleanScorerOrCollector::BooleanScorerOrCollector(ManagedPtr<SearchContext> _srch_context,
                                                   ManagedPtr<LeafCollector> _parent_collector,
                                                   bool _needs_score,
                                                   int32_t _min_should_match)
    : LeafCollector(),
      DocIdStream(),
      parent_collector(_parent_collector),
      srch_context(_srch_context),
      buckets(),
      score_and_doc(),
      needs_score(_needs_score),
      min_should_match(_min_should_match),
      base_doc_id() {
  if (_needs_score || _min_should_match > 1) {
    buckets.vec = _srch_context->linear_allocator->allocate_objects<Bucket>(BS_BUCKET_LIST_SIZE);
    buckets.len = BS_BUCKET_LIST_SIZE;
  }
  valid_indices.vec = _srch_context->linear_allocator->allocate_objects<uint64_t>(BS_INDICES_SET_SIZE);
  valid_indices.len = BS_INDICES_SET_SIZE;
}

void BooleanScorerOrCollector::collect_one_doc(int32_t doc_id) {
  const uint32_t bucket_index = ((uint32_t) doc_id) & BS_MASK;
  const int32_t bitset_index = bucket_index >> 6U;
  valid_indices.vec[bitset_index] |= (uint64_t(1) << (bucket_index & 63U));

  if (buckets.vec) {
    auto &bucket = buckets.vec[bucket_index];
    bucket.freq += 1;
    if (needs_score) {
      bucket.score += scorer->get_score();
    }
  }  // End if
}

void BooleanScorerOrCollector::prepare_collecting_multiple_sources(int32_t _base_doc_id) {
  base_doc_id = _base_doc_id;

  MemoryUtils::memset(valid_indices.vec, 0, sizeof(uint64_t) * BS_INDICES_SET_SIZE);
}

void BooleanScorerOrCollector::for_each(UseOnlyPtr<DocIdConsumer> doc_id_consumer) {
  auto replayer = PcoTargetClassFactoryOfLuceneCyborgSearchQueryBooleanScorerOrCollectorReplayer::create(
      doc_id_consumer,
      this,
      srch_context->linear_allocator);
  replayer->for_each();
}

int32_t BooleanScorerOrCollector::count() {
  if (min_should_match > 1) {
    return DocIdStream::count();
  } else {
    int32_t count = 0;

    for (int32_t i = 0; i < BS_INDICES_SET_SIZE; ++i) {
      LC_PREFETCH(&valid_indices.vec[i], 0, 0);
      count += std::popcount(valid_indices.vec[i]);
    }

    return count;
  }  // End if
}
