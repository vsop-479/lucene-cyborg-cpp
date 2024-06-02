#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SIMPLETOPSCOREDOCCOLLECTOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SIMPLETOPSCOREDOCCOLLECTOR_HPP_

#include "Collector.hpp"
#include "ScoreDoc.hpp"
#include "../util/PriorityQueue.hpp"
#include "../util/LinearAllocator.hpp"
#include "HitQueue.hpp"
#include "TotalHits.hpp"

namespace lucene::cyborg::search {

class SimpleTopScoreDocCollector;

class SimpleTopScoreDocLeafCollector final : public LeafCollector {
 public:
  explicit SimpleTopScoreDocLeafCollector(util::ManagedPtr<SimpleTopScoreDocCollector> simple_top_score_doc_collector);

  void set_scorer(util::ManagedPtr<Scorable> scorer) final;

  void collect_one_doc(int32_t doc_id) final;

 private:
  void update_min_competitive_score(util::UseOnlyPtr<Scorable> scorer);

  util::ManagedPtr<int32_t> total_hits;
  util::ManagedPtr<int32_t> hit_count;
  util::ManagedPtr<int32_t> hits_threshold;
  util::ManagedPtr<HitQueue> hit_queue;
  util::ManagedPtr<ScoreDoc> hit_queue_top;
  util::ManagedPtr<TotalHitsRelation> total_hits_relation;
  util::ManagedPtr<int32_t> doc_base;
  util::ManagedPtr<float> min_competitive_score;
};  // SimpleTopScoreDocLeafCollector



class SimpleTopScoreDocCollector final : public Collector {
  friend class SimpleTopScoreDocLeafCollector;
  friend class SimpleTopScoreDocCollectorManager;

 public:
  SimpleTopScoreDocCollector(util::ManagedPtr<search::SearchContext> srch_context,
                             int32_t num_hits,
                             int32_t hits_threshold);

  util::ManagedPtr<LeafCollector> get_leaf_collector(util::ManagedPtr<index::LeafReaderContext> leaf_reader_context) final;

 private:
  int32_t top_docs_size() noexcept {
    return total_hits < hit_queue.get_size() ? total_hits : hit_queue.get_size();
  }

  int32_t doc_base;
  int32_t total_hits;
  int32_t hit_count;
  HitQueue hit_queue;
  util::ManagedPtr<ScoreDoc> score_docs;
  float min_competitive_score;
  int32_t hits_threshold;
  TotalHitsRelation total_hits_relation;
};  // SimpleTopScoreDocCollector



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SIMPLETOPSCOREDOCCOLLECTOR_HPP_
