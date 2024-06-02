#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_DISJUNCTIONSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_DISJUNCTIONSCORER_HPP_

#include "../Scorer.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../DisiPriorityQueue.hpp"
#include "../ScoreMode.hpp"
#include "../../util/PriorityQueue.hpp"
#include "./iterator/DisjunctionDisiApproximation.hpp"

namespace lucene::cyborg::search::query {

class DisjunctionScorerTwoPhaseIterator;

class DisjunctionScorer : public Scorer {
 public:
  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator() final;

  float get_score() final;

  int32_t get_doc_id() final;

 protected:
  DisjunctionScorer(util::ManagedPtr<Weight> weight,
                    util::LVector<util::ManagedPtr<Scorer>> &&sub_scorers,
                    ScoreMode score_mode);

  virtual float do_score(const util::TypedVector<util::ManagedPtr<DisiWrapper>> &sub_matches) = 0;

  util::ManagedPtr<DisjunctionDisiApproximation> approximation;
  util::ManagedPtr<DisjunctionScorerTwoPhaseIterator> two_phase;
  DisiPriorityQueue sub_scorers;
  bool needs_scores;
};  // DisjunctionScorer



class DisjunctionScorerTwoPhaseIterator final : public TwoPhaseIterator {
 public:
  struct UnverifiedPriorityQueue final : public util::PriorityQueue<util::ManagedPtr<DisiWrapper>,
                                                                    UnverifiedPriorityQueue> {
    bool less_than(util::ManagedPtr<DisiWrapper> a, util::ManagedPtr<DisiWrapper> b) noexcept {
      return a->match_cost < b->match_cost;
    }
  };  // HeadPriorityQueue

  DisjunctionScorerTwoPhaseIterator(util::ManagedPtr<DisjunctionDisiApproximation> approximation,
                                    util::ManagedPtr<DisiPriorityQueue> sub_scorers,
                                    util::ManagedPtr<util::LinearAllocator> allocator,
                                    float match_cost,
                                    bool needs_scores);

  bool matches() final;

  float get_match_cost() final {
    return match_cost;
  }

  util::TypedVector<util::ManagedPtr<DisiWrapper>> get_sub_matches();

  util::ManagedPtr<DisiPriorityQueue> sub_scorers;
  UnverifiedPriorityQueue unverified_matches;
  util::ManagedPtr<DisiWrapper> verified_matches;
  util::LVector<util::ManagedPtr<DisiWrapper>> sub_matches;
  float match_cost;
  bool needs_scores;
};  // DisjunctionScorerTwoPhaseIterator



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_DISJUNCTIONSCORER_HPP_
