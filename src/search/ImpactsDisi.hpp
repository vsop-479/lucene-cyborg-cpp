#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_IMPACTSDISI_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_IMPACTSDISI_HPP_

#include "../util/PointerTypes.hpp"
#include "DocIdSetIterator.hpp"
#include "MaxScoreCache.hpp"

namespace lucene::cyborg::search {

class ImpactsDISI final : public DocIdSetIterator {
 public:
  ImpactsDISI(util::ManagedPtr<DocIdSetIterator> in, util::ManagedPtr<MaxScoreCache> max_score_cache);

  void set_min_competitive_score(float min_competitive_score);

  int32_t advance(int32_t target) final;

  int32_t next_doc() final;

  int32_t get_doc_id() final;

  int64_t get_cost() final;

 private:
  int advance_target(int32_t target);

  util::ManagedPtr<DocIdSetIterator> in;
  util::ManagedPtr<MaxScoreCache> max_score_cache;
  float min_competitive_score;
  int32_t upto;
  float max_score;
};  // ImpactsDISI



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_IMPACTSDISI_HPP_
