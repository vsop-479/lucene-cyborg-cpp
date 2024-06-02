#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SIMPLETOPSCOREDOCCOLLECTORMANAGER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SIMPLETOPSCOREDOCCOLLECTORMANAGER_HPP_

#include "CollectorManager.hpp"
#include "SimpleTopScoreDocCollector.hpp"
#include "TopDocs.hpp"

namespace lucene::cyborg::search {

struct ScoreMergeSortQueue final : public util::PriorityQueue<util::ManagedPtr<HitQueue>, ScoreMergeSortQueue> {
  bool less_than(util::ManagedPtr<HitQueue> q0, util::ManagedPtr<HitQueue> q1) const noexcept {
    auto top0 = q0->top();
    auto top1 = q1->top();

    if (top0->score != top1->score) {
      return top0->score > top1->score;
    }

    // Tie breaker : shard index -> doc id
    if (top0->shard_index != top1->shard_index) {
      return top0->shard_index < top1->shard_index;
    }

    return top0->doc < top1->doc;
  }
};  // ScoreMergeSortQueue



class SimpleTopScoreDocCollectorManager final : public CollectorManager {
 public:
  SimpleTopScoreDocCollectorManager(util::UseOnlyPtr<SearchContext> srch_context,
                                    ScoreMode score_mode,
                                    int32_t num_hits,
                                    int32_t total_hits_threshold);

  util::ManagedPtr<SimpleTopScoreDocCollector> new_collector() final;

  void merge(util::ManagedPtr<Collector> collectors, bool is_last) final;

  TopDocs top_docs;

 private:
  util::UseOnlyPtr<SearchContext> srch_context;
  int32_t num_hits;
  int32_t hits_threshold;
  ScoreMergeSortQueue sort_queue;
  int32_t merge_count;
};  // TopScoreDocCollectorManager



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SIMPLETOPSCOREDOCCOLLECTORMANAGER_HPP_
