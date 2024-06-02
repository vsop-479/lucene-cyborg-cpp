#include <limits>
#include "MaxScoreCache.hpp"
#include "../util/MemoryUtils.hpp"

using lucene::cyborg::index::Impacts;
using lucene::cyborg::index::ImpactsSource;
using lucene::cyborg::search::MaxScoreCache;
using lucene::cyborg::search::SimScorer;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MemoryUtils;
using lucene::cyborg::util::UseOnlyPtr;

//
// MaxScoreCache
//

MaxScoreCache::MaxScoreCache(ManagedPtr<LinearAllocator> _allocator,
                             ManagedPtr<ImpactsSource> _impacts_source,
                             ManagedPtr<SimScorer> _scorer)
    : impacts_source(_impacts_source),
      scorer(_scorer),
      global_max_score(_scorer->score(std::numeric_limits<float>::max(), 1)),
      max_score_cache(LinearAllocatorAdapterForSTL<ScoreAndUpto>{_allocator}) {
}

int32_t MaxScoreCache::advance_shallow(int32_t target) {
  impacts_source->advance_shallow(target);
  return impacts_source->get_impacts()->get_doc_id_up_to(0);
}

float MaxScoreCache::get_max_score_for_level(int32_t level) {
  assert(level >= 0);
  auto impacts = impacts_source->get_impacts();
  const int32_t level_up_to = impacts->get_doc_id_up_to(level);
  const int32_t cache_min_size = level + 1;
  if (max_score_cache.size() < cache_min_size) {
    const auto org_size = max_score_cache.size();
    max_score_cache.resize(MemoryUtils::oversize(cache_min_size));
    for (int32_t i = org_size; i < max_score_cache.size(); ++i) {
      max_score_cache[i].upto = -1;
    }
  }

  if (max_score_cache[level].upto >= level_up_to) {
    return max_score_cache[level].score;
  } else {
    max_score_cache[level].upto = level_up_to;
    auto impacts_at_level = impacts->get_impacts(level);
    float max_score = 0;
    for (int32_t i = 0; i < impacts_at_level.len; ++i) {
      const auto &impact = impacts_at_level.vec[i];
      if (auto score = scorer->score(impact.freq, impact.norm); score > max_score) {
        max_score = score;
      }
    }
    return max_score_cache[level].score = max_score;
  }  // End if
}

int32_t MaxScoreCache::get_skip_up_to(float min_score) {
  auto impacts = impacts_source->get_impacts();
  const int32_t num_levels = impacts->get_num_levels();
  int32_t level;
  for (int32_t l = 0; l < num_levels; ++l) {
    if (get_max_score_for_level(l) >= min_score) {
      level = l - 1;
      goto RET;
    }
  }

  level = num_levels - 1;
  RET:
  return level != -1 ? impacts->get_doc_id_up_to(level) : -1;
}

float MaxScoreCache::get_max_score(int32_t upto) {
  auto impacts = impacts_source->get_impacts();
  int32_t level = -1;
  for (int32_t l = 0, num_levels = impacts->get_num_levels(); l < num_levels; ++l) {
    const int32_t impacts_up_to = impacts->get_doc_id_up_to(l);
    if (upto <= impacts_up_to) {
      level = l;
      break;
    }
  }  // End for

  return level != -1 ? get_max_score_for_level(level) : global_max_score;
}
