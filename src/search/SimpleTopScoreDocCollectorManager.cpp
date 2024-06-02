#include "SimpleTopScoreDocCollectorManager.hpp"

using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::SimpleTopScoreDocCollector;
using lucene::cyborg::search::SimpleTopScoreDocCollectorManager;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::PriorityQueue;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

//
// TopScoreDocCollectorManager
//

SimpleTopScoreDocCollectorManager::SimpleTopScoreDocCollectorManager(
    UseOnlyPtr<SearchContext> _srch_context,
    ScoreMode _score_mode,
    int32_t _num_hits,
    int32_t _total_hits_threshold)
    : CollectorManager(_score_mode),
      top_docs(),
      srch_context(_srch_context),
      num_hits(_num_hits),
      hits_threshold(std::max(_total_hits_threshold, _num_hits)),
      sort_queue(),
      merge_count() {
}

ManagedPtr<SimpleTopScoreDocCollector> SimpleTopScoreDocCollectorManager::new_collector() {
  return srch_context->linear_allocator->allocate_object<SimpleTopScoreDocCollector>(
      srch_context, num_hits, hits_threshold);
}

void SimpleTopScoreDocCollectorManager::merge(ManagedPtr<Collector> _collectors, bool is_last) {
  auto collector = (ManagedPtr<SimpleTopScoreDocCollector>) _collectors;

  if (merge_count == 0) {
    if (is_last) {
      // single thread case. directly make top docs from collector - inplace heap sort
      const int32_t top_doc_size = collector->top_docs_size();
      collector->hit_queue.inplace_sort();
      auto heap_arr = collector->hit_queue.get_heap_array();

      top_docs.total_hits.value = collector->total_hits;
      top_docs.total_hits.relation = collector->total_hits_relation;
      top_docs.score_docs.vec = &heap_arr.vec[1];
      top_docs.score_docs.len = top_doc_size;

      return;
    }
  }  // End if

  // TODO merge collectors : support multi thread
  assert(false);
}
