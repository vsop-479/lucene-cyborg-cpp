#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_DISJUNCTIONSUMSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_DISJUNCTIONSUMSCORER_HPP_

#include <limits>
#include "DisjunctionScorer.hpp"

namespace lucene::cyborg::search::query {

class DisjunctionSumScorer final : public DisjunctionScorer {
 public:
  DisjunctionSumScorer(util::ManagedPtr<Weight> weight,
                       util::LVector<util::ManagedPtr<Scorer>> &&sub_scorers,
                       ScoreMode score_mode);

 protected:
  float do_score(const util::TypedVector<util::ManagedPtr<DisiWrapper>> &sub_matches) override;

 public:
  float get_max_score(int32_t upto) final {
    return std::numeric_limits<float>::max();
  }
};  // DisjunctionSumScorer



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_DISJUNCTIONSUMSCORER_HPP_
