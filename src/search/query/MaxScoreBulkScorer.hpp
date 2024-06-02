#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_MAXSCOREBULKSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_MAXSCOREBULKSCORER_HPP_

#include "../BulkScorer.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../Scorer.hpp"
#include "../../util/Bits.hpp"
#include "../DisiPriorityQueue.hpp"
#include "../../util/FixedBitSet.hpp"

namespace lucene::cyborg::search::query {

class MaxScoreBulkScorer final : public BulkScorer {
 public:
  MaxScoreBulkScorer(util::ManagedPtr<util::LinearAllocator> allocator,
                     int32_t max_doc,
                     util::LVector<util::ManagedPtr<Scorer>> &&scorers,
                     util::ManagedPtr<LeafCollector> leaf_collector,
                     util::ManagedPtr<util::Bits> accept_docs);

  int32_t score(int32_t min_doc_id, int32_t max_doc_id) final;

  int64_t get_cost() final {
    return cost;
  }

  util::ManagedPtr<LeafCollector> get_collector() final {
    return leaf_collector;
  }

  util::ManagedPtr<LeafCollector> reset_collector(util::ManagedPtr<LeafCollector> new_collector) final {
    std::swap(leaf_collector, new_collector);
    return new_collector;
  }

 private:
  static constexpr int32_t INNER_WINDOW_SIZE = 1U << 11U;
  static constexpr int32_t REQUIRED_BITS_FOR_INNER_WINDOW_SIZE = util::FixedBitSet::bits_to_words(INNER_WINDOW_SIZE);

  struct ScoreAndDoc final : public Scorable {
    explicit ScoreAndDoc(util::ManagedPtr<MaxScoreBulkScorer> max_score_bulk_scorer);

    float get_score() final {
      return score;
    }

    int32_t get_doc_id() final {
      return doc;
    }

    void set_min_competitive_score(float min_score) final;

    util::ManagedPtr<MaxScoreBulkScorer> max_score_bulk_scorer;
    float score;
    int32_t doc;
  };  // ScoreAndDoc

  void score_inner_window(int32_t max);

  void score_inner_window_single_essential_clause(int32_t max);

  void score_inner_window_multiple_essential_clauses(int32_t up_to);

  int32_t update_max_window_scores(int32_t window_min);

  void score_non_essential_clauses(int32_t doc, double essential_score);

  bool partition_scorers();

  int32_t next_candidate(int32_t range_end);

  util::ManagedPtr<LeafCollector> leaf_collector;
  util::ManagedPtr<util::Bits> accept_docs;
  int32_t max_doc;
  // All scorers, sorted by increasing max score.
  util::LVector<util::ManagedPtr<DisiWrapper>> all_scorers;
  util::LVector<util::ManagedPtr<DisiWrapper>> scratch;
  // These are the last scorers from `all_scorers` that are "essential", ie. required for a match to
  // have a competitive score.
  util::ManagedPtr<DisiPriorityQueue> essential_queue;
  // Index of the first essential scorer, ie. essential_queue contains all scorers from
  // all_scorers[first_essential_scorer:]. All scorers below this index are non-essential.
  int32_t first_essential_scorer;
  int64_t cost;
  float min_competitive_score;
  bool min_competitive_score_updated;
  ScoreAndDoc scorable;
  util::LVector<double> max_score_sums;
  uint64_t window_matches[REQUIRED_BITS_FOR_INNER_WINDOW_SIZE];
  double window_scores[INNER_WINDOW_SIZE];
};  // MaxScoreBulkScorer



}  // namespace lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_MAXSCOREBULKSCORER_HPP_
