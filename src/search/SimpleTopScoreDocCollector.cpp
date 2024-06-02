#include <limits>
#include <cmath>
#include "../index/LeafReaderContext.hpp"
#include "SimpleTopScoreDocCollector.hpp"

using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::search::LeafCollector;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::SimpleTopScoreDocCollector;
using lucene::cyborg::search::SimpleTopScoreDocLeafCollector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

//
// SimpleTopScoreDocCollector
//

SimpleTopScoreDocCollector::SimpleTopScoreDocCollector(ManagedPtr<SearchContext> _srch_context,
                                                       int32_t _num_hits,
                                                       int32_t _hits_threshold)
    : Collector(_srch_context),
      doc_base(),
      total_hits(),
      hit_count(),
      hit_queue(),
      score_docs(),
      min_competitive_score(),
      hits_threshold(_hits_threshold),
      total_hits_relation(TotalHitsRelation::EQUAL_TO) {
  score_docs = _srch_context->linear_allocator->allocate_objects<ScoreDoc>(_num_hits);

  // build a heap array having only -Inf
  TypedVector<ManagedPtr<ScoreDoc>> heap_array;
  heap_array.len = _num_hits + 1;  // heap is 1-based.
  heap_array.vec = _srch_context->linear_allocator->allocate_objects<ManagedPtr<ScoreDoc>>(_num_hits + 1);
  for (int32_t i = 0; i < _num_hits; ++i) {
    score_docs[i].score = -INFINITY;
    score_docs[i].doc = std::numeric_limits<int32_t>::max();
    heap_array.vec[i + 1] = &(score_docs[i]);
  }
  hit_queue.init_with_pre_built_heap(heap_array);
}

ManagedPtr<LeafCollector> SimpleTopScoreDocCollector::get_leaf_collector(ManagedPtr<LeafReaderContext> leaf_reader_context) {
  min_competitive_score = 0;
  doc_base = leaf_reader_context->doc_base;
  return srch_context->linear_allocator->allocate_object<SimpleTopScoreDocLeafCollector>(this);
}

//
// SimpleTopScoreDocLeafCollector
//

SimpleTopScoreDocLeafCollector::SimpleTopScoreDocLeafCollector(
    ManagedPtr<SimpleTopScoreDocCollector> _simple_top_score_doc_collector)
    : LeafCollector(),
      total_hits(&(_simple_top_score_doc_collector->total_hits)),
      hit_count(&(_simple_top_score_doc_collector->hit_count)),
      hits_threshold(&(_simple_top_score_doc_collector->hits_threshold)),
      hit_queue(&(_simple_top_score_doc_collector->hit_queue)),
      hit_queue_top(_simple_top_score_doc_collector->hit_queue.top()),
      total_hits_relation(&(_simple_top_score_doc_collector->total_hits_relation)),
      doc_base(&(_simple_top_score_doc_collector->doc_base)),
      min_competitive_score(&(_simple_top_score_doc_collector->min_competitive_score)) {
}

void SimpleTopScoreDocLeafCollector::collect_one_doc(int32_t doc_id) {
  float score = scorer->get_score();

  // This collector relies on the fact that scorers produce positive values:
  assert(score >= 0); // NOTE: false for NaN

  (*total_hits)++;
  (*hit_count)++;

  if (score <= hit_queue_top->score) {
    if (*total_hits_relation == TotalHitsRelation::EQUAL_TO) {
      // We just reached total_hits_threshold, we can start setting the min
      // competitive score now
      update_min_competitive_score(scorer);
    }
    // Since docs are returned in-order (i.e., increasing doc ID), a document
    // with equal score to hit_queue_top->score cannot compete since HitQueue favors
    // documents with lower doc Ids. Therefore, reject those docs too.
    return;
  }

  hit_queue_top->doc = doc_id + *doc_base;
  hit_queue_top->score = score;
  hit_queue->update_top();
  hit_queue_top = hit_queue->top();
  update_min_competitive_score(scorer);
}

void SimpleTopScoreDocLeafCollector::set_scorer(ManagedPtr<Scorable> _scorer) {
  LeafCollector::set_scorer(_scorer);
  update_min_competitive_score(_scorer);
}

void SimpleTopScoreDocLeafCollector::update_min_competitive_score(UseOnlyPtr<Scorable> scorer) {
  if ((*hit_count) > (*hits_threshold)
      && hit_queue_top
      && hit_queue_top->score != -INFINITY) { // -Infinity is the score of sentinels
    // Since we tie-break on doc id and collect in doc id order, we can require the next float
    const float local_min_score = std::nextafter(hit_queue_top->score, INFINITY);

    if (local_min_score > (*min_competitive_score)) {
      scorer->set_min_competitive_score(local_min_score);
      *total_hits_relation = TotalHitsRelation::GREATER_THAN_OR_EQUAL_TO;
      *min_competitive_score = local_min_score;
    }
  }  // End if
}
