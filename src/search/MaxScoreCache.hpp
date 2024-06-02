#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_MAXSCORECACHE_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_MAXSCORECACHE_HPP_

#include "../util/PointerTypes.hpp"
#include "../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../index/ImpactsSource.hpp"
#include "Similarity.hpp"

namespace lucene::cyborg::search {

class MaxScoreCache {
  friend class ImpactsDISI;

  template<typename T>
  friend class ImpactDISIPco;

 public:
  MaxScoreCache(util::ManagedPtr<util::LinearAllocator> _allocator,
                util::ManagedPtr<index::ImpactsSource> impacts_source,
                util::ManagedPtr<SimScorer> scorer);

  int32_t advance_shallow(int32_t target);

  float get_max_score(int32_t upto);

  float get_max_score_for_level(int32_t level);

  int32_t get_skip_up_to(float min_score);

 private:
  struct ScoreAndUpto {
    float score;
    int32_t upto;
  };

  util::ManagedPtr<index::ImpactsSource> impacts_source;
  util::ManagedPtr<SimScorer> scorer;
  float global_max_score;
  util::LVector<ScoreAndUpto> max_score_cache;
};  // MaxScoreCache



}  // lucene::cyborg::search


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_MAXSCORECACHE_HPP_
