#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_WANDSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_WANDSCORER_HPP_

#include "Scorer.hpp"
#include "ScoreMode.hpp"
#include "../util/LinearAllocatorAdaptorForSTL.hpp"
#include "DisiWrapper.hpp"
#include "DisiPriorityQueue.hpp"

namespace lucene::cyborg::search {

class WANDScorer;

struct WANDScorerPriorityQueue {
  WANDScorerPriorityQueue(util::ManagedPtr<util::LinearAllocator> allocator, util::ManagedPtr<WANDScorer> scorer);

  void add_tail(util::ManagedPtr<DisiWrapper> disi);

  util::ManagedPtr<DisiWrapper> pop_tail();

  void up_heap_max_score(int32_t index);

  void down_heap_max_score();

  bool greater_max_score(util::ManagedPtr<DisiWrapper> disi1, util::ManagedPtr<DisiWrapper> disi2);

  util::ManagedPtr<DisiWrapper> insert_tail_with_over_flow(util::ManagedPtr<DisiWrapper> s);

  util::LVector<util::ManagedPtr<DisiWrapper>> tail;
  int32_t tail_size;
  util::ReadOnlyPtr<int32_t> min_should_match;
  int64_t tail_max_score;
  util::ReadOnlyPtr<int64_t> min_competitive_score;
};  // WANDScorerPriorityQueue



struct WANDScorerIterator final : public DocIdSetIterator {
  explicit WANDScorerIterator(util::ManagedPtr<WANDScorer> scorer);

  int64_t get_cost() final {
    return *cost;
  }

  int32_t get_doc_id() final {
    return *doc;
  }

  int32_t next_doc() final {
    return advance(*doc + 1);
  }

  int32_t advance(int32_t target) final;

  util::ManagedPtr<WANDScorer> scorer;
  util::ReadOnlyPtr<int32_t> doc;
  util::ReadOnlyPtr<int64_t> cost;
};  // WANDScorerIterator



struct WANDScorerTwoPhaseIterator final : public TwoPhaseIterator {
  WANDScorerTwoPhaseIterator(util::ManagedPtr<WANDScorerIterator> approximation, util::ManagedPtr<WANDScorer> scorer);

  bool matches() final;

  float get_match_cost() final {
    return match_cost;
  }

  util::ManagedPtr<WANDScorer> scorer;
  float match_cost;
  util::ReadOnlyPtr<int64_t> lead_max_score;
  util::ReadOnlyPtr<int64_t> tail_max_score;
  util::ReadOnlyPtr<int32_t> tail_size;
  util::ReadOnlyPtr<int64_t> min_competitive_score;
  util::ReadOnlyPtr<int32_t> min_should_match;
};  // WANDScorerTwoPhaseIterator



class WANDScorer final : public Scorer {
  friend class WANDScorerPriorityQueue;
  friend class WANDScorerIterator;
  friend class WANDScorerTwoPhaseIterator;

 public:
  WANDScorer(util::ManagedPtr<Weight> weight,
             util::LVector<util::ManagedPtr<Scorer>> &&scorers,
             int32_t min_should_match, ScoreMode score_mode);
 public:
  int32_t get_doc_id() final;

  float get_max_score(int32_t upto) final;

  float get_score() final;

  int32_t advance_shallow(int32_t target) final;

  void set_min_competitive_score(float min_score) final;

  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator() final;

 private:
  bool ensure_consistent();

  void add_lead(util::ManagedPtr<DisiWrapper> lead);

  void push_back_leads(int32_t target);

  void advance_head(int32_t target);

  void advance_tail(util::ManagedPtr<DisiWrapper> disi);

  void advance_tail();

  void update_max_scores(int32_t target);

  void update_max_scores_if_necessary(int32_t target);

  void move_to_next_candidate(int32_t target);

  int32_t do_next_competitive_candidate();

  void advance_all_tail();

  int32_t doc;
  int32_t upto;
  int32_t scaling_factor;
  int64_t lead_max_score;
  int64_t cost;
  int64_t min_competitive_score;
  DisiPriorityQueue head;
  int32_t min_should_match;
  ScoreMode score_mode;
  util::LVector<util::ManagedPtr<Scorer>> all_scorers;
  WANDScorerPriorityQueue tail_priority_queue;
  util::LVector<util::ManagedPtr<DisiWrapper>> leads;
};  // WANDScorer



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_WANDSCORER_HPP_
