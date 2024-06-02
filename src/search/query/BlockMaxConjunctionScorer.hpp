#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BLOCKMAXCONJUNCTIONSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BLOCKMAXCONJUNCTIONSCORER_HPP_

#include "../Scorer.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"

namespace lucene::cyborg::search {

class BlockMaxConjunctionScorer;

class BlockMaxConjunctionScorerTwoPhaseIterator final : public TwoPhaseIterator {
 public:
  BlockMaxConjunctionScorerTwoPhaseIterator(util::ManagedPtr<DocIdSetIterator> approximation,
                                            float match_cost,
                                            util::ManagedPtr<BlockMaxConjunctionScorer> scorer);

  bool matches() final;

  float get_match_cost() final;

 private:
  float match_cost;
  util::ManagedPtr<BlockMaxConjunctionScorer> scorer;
};  // BlockMaxConjunctionScorerTwoPhaseIterator



class BlockMaxConjunctionScorerIterator final : public DocIdSetIterator {
 public:
  BlockMaxConjunctionScorerIterator(util::ManagedPtr<DocIdSetIterator> lead,
                                    util::ManagedPtr<BlockMaxConjunctionScorer> scorer);
  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final;

  int32_t next_doc() final;

 private:
  void move_to_next_block(int32_t target);

  int32_t advance_target(int32_t target);

  int32_t do_next(int32_t target);

  float max_score;
  int32_t up_to = -1;
  util::ManagedPtr<DocIdSetIterator> lead;
  util::ManagedPtr<BlockMaxConjunctionScorer> scorer;
};  // BlockMaxConjunctionScorerIterator



class BlockMaxConjunctionScorer final : public Scorer {
 public:
  BlockMaxConjunctionScorer(util::ManagedPtr<Weight> weight, util::LVector<util::ManagedPtr<Scorer>> &&scorers_list);

  util::ManagedPtr<BlockMaxConjunctionScorerTwoPhaseIterator> two_phase_iterator() final;

  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

  int32_t get_doc_id() final;

  float get_score() final;

  int32_t advance_shallow(int32_t target) final;

  float get_max_score(int32_t up_to) final;

  void set_min_competitive_score(float score) final;

 private:
  util::ManagedPtr<DocIdSetIterator> approximation();

  util::LVector<util::ManagedPtr<Scorer>> scorers;

 public:
  util::LVector<util::ManagedPtr<TwoPhaseIterator>> two_phases;
  util::LVector<util::ManagedPtr<DocIdSetIterator>> approximations;
  float min_score;
};  // BlockMaxConjunctionScorer



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BLOCKMAXCONJUNCTIONSCORER_HPP_
