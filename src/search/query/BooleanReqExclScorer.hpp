#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANREQEXCLSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANREQEXCLSCORER_HPP_

#include "../Scorer.hpp"

namespace lucene::cyborg::search::query {

class BooleanReqExclScorer final : public Scorer {
 public:
  BooleanReqExclScorer(util::ManagedPtr<Scorer> req_scorer, util::ManagedPtr<Scorer> excl_scorer);

  float get_score() final {
    return req_scorer->get_score();
  }

  int32_t get_doc_id() final {
    return req_approximation->get_doc_id();
  }

  void set_min_competitive_score(float min_score) final {
    // The score of this get_scorer is the same as the score of 'req_scorer'.
    req_scorer->set_min_competitive_score(min_score);
  }

  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator() final;

  int32_t advance_shallow(int32_t target) final {
    return req_scorer->advance_shallow(target);
  }

  float get_max_score(int32_t upto) final {
    return req_scorer->get_max_score(upto);
  }

 private:
  util::ManagedPtr<Scorer> req_scorer;
  // two-phase views of the scorers, or null if they do not support approximations
  util::ManagedPtr<TwoPhaseIterator> req_two_phase_iterator;
  util::ManagedPtr<TwoPhaseIterator> excl_two_phase_iterator;
  // approximations of the scorers, or the scorers themselves if they don't support approximations
  util::ManagedPtr<DocIdSetIterator> req_approximation;
  util::ManagedPtr<DocIdSetIterator> excl_approximation;
};  // BooleanReqExclScorer



template<bool IsExclTwoPhaseExpensiveThanReqTwoPhase>
class BooleanReqExclScorerTwoPhaseIterator final : public TwoPhaseIterator {
 public:
  BooleanReqExclScorerTwoPhaseIterator(util::ManagedPtr<DocIdSetIterator> _req_approximation,
                                       util::ManagedPtr<DocIdSetIterator> _excl_approximation,
                                       util::ManagedPtr<TwoPhaseIterator> _req_two_phase_iterator,
                                       util::ManagedPtr<TwoPhaseIterator> _excl_two_phase_iterator,
                                       float _match_cost)
      : TwoPhaseIterator(_req_approximation),
        match_cost_val(_match_cost),
        req_approximation(_req_approximation),
        excl_approximation(_excl_approximation),
        req_two_phase_iterator(_req_two_phase_iterator),
        excl_two_phase_iterator(_excl_two_phase_iterator) {
  }

  bool matches() final {
    int32_t doc = req_approximation->get_doc_id();
    // check if the doc is not excluded
    int32_t excl_doc = excl_approximation->get_doc_id();
    if (excl_doc < doc) {
      excl_doc = excl_approximation->advance(doc);
    }
    if (excl_doc != doc) {
      return (!req_two_phase_iterator || req_two_phase_iterator->matches());
    }

    if constexpr (IsExclTwoPhaseExpensiveThanReqTwoPhase) {
      return (!req_two_phase_iterator || req_two_phase_iterator->matches())
          && (excl_two_phase_iterator && !excl_two_phase_iterator->matches());
    } else {
      return (excl_two_phase_iterator && !excl_two_phase_iterator->matches())
          && (!req_two_phase_iterator || req_two_phase_iterator->matches());
    }
  }

  float get_match_cost() final {
    return match_cost_val;
  }

  util::ManagedPtr<DocIdSetIterator> req_approximation;
  util::ManagedPtr<DocIdSetIterator> excl_approximation;
  util::ManagedPtr<TwoPhaseIterator> req_two_phase_iterator;
  util::ManagedPtr<TwoPhaseIterator> excl_two_phase_iterator;
  float match_cost_val;
};  // BooleanReqExclScorerTwoPhaseIterator



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANREQEXCLSCORER_HPP_
