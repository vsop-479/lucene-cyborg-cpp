#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_MATCHCOUNTCOLLECTORMANAGER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_MATCHCOUNTCOLLECTORMANAGER_HPP_

#include "CollectorManager.hpp"
#include "SearchContext.hpp"
#include "../util/PointerTypes.hpp"
#include "../util/Macros.hpp"

namespace lucene::cyborg::search {

//
// MatchCountLeafCollector
//

struct MatchCountLeafCollector final : public LeafCollector {
  explicit MatchCountLeafCollector(util::ManagedPtr<int32_t> _count)
      : LeafCollector(),
        count(_count) {
  }

  LC_ALWAYSINLINE void collect_one_doc(int32_t doc_id) final {
    *count += 1;
  }

  LC_ALWAYSINLINE void collect(util::UseOnlyPtr<DocIdStream> doc_id_stream) final {
    *count += doc_id_stream->count();
  }

  util::ManagedPtr<int32_t> count;
};  // MatchCountLeafCollector



//
// MatchCountAggregator
//

struct MatchCountCollector final : public search::Collector {
  explicit MatchCountCollector(util::UseOnlyPtr<SearchContext> _srch_context,
                               util::ManagedPtr<int32_t> _count)
      : Collector(_srch_context),
        count(_count) {
  }

  util::ManagedPtr<LeafCollector> get_leaf_collector(util::ManagedPtr<index::LeafReaderContext> leaf_reader_context) final {
    return srch_context->linear_allocator->allocate_object<MatchCountLeafCollector>(count);
  }

  util::ManagedPtr<int32_t> count;
};  // MatchCountCollector



//
// MatchCountCollectorManager
//

struct MatchCountCollectorManager final : public search::CollectorManager {
  explicit MatchCountCollectorManager(util::UseOnlyPtr<SearchContext> _srch_context)
      : CollectorManager(ScoreMode::COMPETE_NO_SCORES),
        srch_context(_srch_context),
        count() {
  }

  util::ManagedPtr<Collector> new_collector() final {
    return srch_context->linear_allocator->allocate_object<MatchCountCollector>(
        srch_context,
        &count);
  }

  util::UseOnlyPtr<SearchContext> srch_context;
  int32_t count;
};  // MatchCountCollectorManager



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_MATCHCOUNTCOLLECTORMANAGER_HPP_
