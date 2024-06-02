#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_CONJUNCTIONSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_CONJUNCTIONSCORER_HPP_

#include "../Scorer.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"

namespace lucene::cyborg::search::query {

class ConjunctionScorer final : public Scorer {
 public:
  ConjunctionScorer(util::ManagedPtr<Weight> weight,
                    util::LVector<util::ManagedPtr<Scorer>> &&required_scorers,
                    util::LVector<util::ManagedPtr<Scorer>> &&scoring_scorers);

  float get_score() final;

  int32_t get_doc_id() final;

  util::ManagedPtr<DocIdSetIterator> get_iterator() final;

  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator() override;

  float get_max_score(int32_t upto) final;

  util::ManagedPtr<DocIdSetIterator> conjunction_iterator;
  util::LVector<util::ManagedPtr<Scorer>> required_scorers;
  util::LVector<util::ManagedPtr<Scorer>> scoring_scorers;
};  // ConjunctionScorer



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_CONJUNCTIONSCORER_HPP_
