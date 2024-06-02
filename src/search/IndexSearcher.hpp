#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_INDEXSEARCHER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_INDEXSEARCHER_HPP_

#include "../index/MMapDirectoryReader.hpp"
#include "Similarity.hpp"
#include "CollectorManager.hpp"
#include "./query/Query.hpp"
#include "../index/LeafReaderContext.hpp"

namespace lucene::cyborg::search {

class IndexSearcher {
 public:
  explicit IndexSearcher(util::UseOnlyPtr<index::MMapDirectoryReader> index_reader);

  void set_similarity(util::ManagedPtr<Similarity> similarity);

  util::ManagedPtr<Similarity> get_similarity() noexcept;

  void search(util::UseOnlyPtr<SearchContext> srch_context,
              util::UseOnlyPtr<Query> query,
              util::UseOnlyPtr<CollectorManager> collector_manager);

  util::ManagedPtr<Weight> create_weight(util::UseOnlyPtr<SearchContext> srch_context,
                                         util::UseOnlyPtr<Query> query,
                                         ScoreMode score_mode,
                                         float boost);

  util::TypedVector<index::LeafReaderContext> get_leaf_reader_contexts() noexcept {
    return util::TypedVector<index::LeafReaderContext>{
        index_reader->leaves.data(),
        (int32_t) index_reader->leaves.size()
    };
  }

  util::ManagedPtr<CollectionStatistics> get_collection_statistics(util::UseOnlyPtr<SearchContext> srch_context,
                                                                   const std::string_view &field);

  virtual TermStatistics get_term_statistics(
      const std::string_view &term, int32_t doc_freq, int64_t total_term_freq);

 private:
  void do_search(util::UseOnlyPtr<SearchContext> srch_context,
                 util::UseOnlyPtr<Weight> weight,
                 util::UseOnlyPtr<CollectorManager> collector_manager,
                 util::UseOnlyPtr<Collector> aggregator);

  void do_search_single_thread(util::UseOnlyPtr<SearchContext> srch_context,
                               util::UseOnlyPtr<Weight> weight,
                               util::UseOnlyPtr<Collector> collector);

  util::ManagedPtr<Query> rewrite(util::UseOnlyPtr<SearchContext> srch_context,
                                  util::UseOnlyPtr<Query> query,
                                  bool needs_score);

  util::ManagedPtr<index::MMapDirectoryReader> index_reader;
  util::ManagedPtr<Similarity> similarity;
};  // IndexSearcher



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_INDEXSEARCHER_HPP_
