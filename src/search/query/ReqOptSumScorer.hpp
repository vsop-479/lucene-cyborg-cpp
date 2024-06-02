#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_REQOPTSUMSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_REQOPTSUMSCORER_HPP_

#include "../Scorer.hpp"
#include "../ScoreMode.hpp"

namespace lucene::cyborg::search::query {

class ReqOptSumScorer;

class ReqOptSumScorerIterator final : public DocIdSetIterator {
 public:
  explicit ReqOptSumScorerIterator(util::ManagedPtr<ReqOptSumScorer> req_opt_sum_scorer);

  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final;

  int32_t next_doc() final;

 private:
  void move_to_next_block(int32_t target);

  int32_t advance_internal(int32_t target);

  int32_t advance_impacts(int32_t target);

  util::ManagedPtr<ReqOptSumScorer> req_opt_sum_scorer;
  int32_t up_to;
  float max_score;
  util::ManagedPtr<Scorer> req_scorer;
  util::ManagedPtr<bool> opt_is_required;
  util::ReadOnlyPtr<float> min_score;
  util::ReadOnlyPtr<float> req_max_score;
  util::ManagedPtr<DocIdSetIterator> req_approximation;
  util::ManagedPtr<DocIdSetIterator> opt_approximation;
};  // ReqOptSumScorerIterator



class ReqOptSumScorerTwoPhaseIterator final : public TwoPhaseIterator {
 public:
  ReqOptSumScorerTwoPhaseIterator(util::ManagedPtr<ReqOptSumScorer> req_opt_sum_scorer,
                                  util::ManagedPtr<TwoPhaseIterator> req_two_phase);

  bool matches() final;

  float get_match_cost() final;

  util::ManagedPtr<TwoPhaseIterator> req_two_phase;
  util::ManagedPtr<TwoPhaseIterator> opt_two_phase;
  util::ReadOnlyPtr<bool> opt_is_required;
  util::ManagedPtr<DocIdSetIterator> opt_approximation;
  util::ManagedPtr<Scorer> req_scorer;
};  // ReqOptSumScorerTwoPhaseIterator



class ReqOptSumScorer final : public Scorer {
  friend class ReqOptSumScorerTwoPhaseIterator;
  friend class ReqOptSumScorerIterator;

 public:
  ReqOptSumScorer(util::ManagedPtr<Scorer> req_scorer, util::ManagedPtr<Scorer> opt_scorer, ScoreMode score_mode);

  float get_score() final;

  int32_t get_doc_id() final {
    return req_scorer->get_doc_id();
  }

  void set_min_competitive_score(float min_score) final {
    this->min_score = min_score;
    // Potentially move to a conjunction
    if (req_max_score < min_score) {
      opt_is_required = true;
    }
  }

  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator() final {
    return two_phase;
  }

  int32_t advance_shallow(int32_t target) final;

  float get_max_score(int32_t upto) final;

 private:
  util::ManagedPtr<Scorer> req_scorer;
  util::ManagedPtr<Scorer> opt_scorer;
  util::ManagedPtr<DocIdSetIterator> req_approximation;
  util::ManagedPtr<DocIdSetIterator> opt_approximation;
  util::ManagedPtr<TwoPhaseIterator> opt_two_phase;
  util::ManagedPtr<DocIdSetIterator> approximation;
  util::ManagedPtr<TwoPhaseIterator> two_phase;
  float min_score;
  float req_max_score;
  bool opt_is_required;
};  // ReqOptSumScorer



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_REQOPTSUMSCORER_HPP_
