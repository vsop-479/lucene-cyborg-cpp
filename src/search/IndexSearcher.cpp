#include "IndexSearcher.hpp"
#include "./similarity/Bm25Similarity.hpp"

using lucene::cyborg::index::MMapDirectoryReader;
using lucene::cyborg::search::CollectionStatistics;
using lucene::cyborg::search::CollectorManager;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::Query;
using lucene::cyborg::search::Similarity;
using lucene::cyborg::search::TermStatistics;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::similarities::BM25Similarity;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// IndexSearcher
//

static BM25Similarity DEFAULT_SIMILARITY{};

IndexSearcher::IndexSearcher(ManagedPtr<MMapDirectoryReader> _index_reader)
    : index_reader(_index_reader),
      similarity(&DEFAULT_SIMILARITY) {
}

void IndexSearcher::set_similarity(ManagedPtr<Similarity> _similarity) {
  similarity = _similarity;
}

ManagedPtr<Similarity> IndexSearcher::get_similarity() noexcept {
  return similarity;
}

void IndexSearcher::search(UseOnlyPtr<SearchContext> srch_context,
                           UseOnlyPtr<Query> query,
                           UseOnlyPtr<CollectorManager> collector_manager) {
  query = rewrite(srch_context, query, true);
  auto weight = create_weight(srch_context, query, collector_manager->get_score_mode(), 1);
  auto first_collector = collector_manager->new_collector();
  do_search(srch_context, weight, collector_manager, first_collector);
}

ManagedPtr<Weight> IndexSearcher::create_weight(UseOnlyPtr<SearchContext> srch_context,
                                                UseOnlyPtr<Query> query,
                                                ScoreMode score_mode,
                                                float boost) {
  auto weight = query->create_weight(srch_context, this, score_mode, boost);
  // TODO
  // 1. query cache
  return weight;
}

ManagedPtr<Query> IndexSearcher::rewrite(UseOnlyPtr<SearchContext> srch_context,
                                         UseOnlyPtr<Query> original_query,
                                         bool needs_score) {
  auto query = original_query;
  for (auto rewritten_query = query->rewrite(srch_context, this, needs_score);
       rewritten_query != query;
       rewritten_query = query->rewrite(srch_context, this, needs_score)) {
    query = rewritten_query;
  }
  return query;
}

ManagedPtr<CollectionStatistics> IndexSearcher::get_collection_statistics(UseOnlyPtr<SearchContext> srch_context,
                                                                          const std::string_view &field) {
  assert(!field.empty());
  int64_t doc_count = 0;
  int64_t sum_total_term_freq = 0;
  int64_t sum_doc_freq = 0;

  for (int32_t i = 0, until = index_reader->leaves.size(); i < until; ++i) {
    auto ctx = &index_reader->leaves[i];
    auto terms = ctx->segment_reader->segment_core_readers->fields->terms(field, srch_context);
    doc_count += terms->get_doc_count();
    sum_total_term_freq += terms->get_sum_total_term_freq();
    sum_doc_freq += terms->get_sum_doc_freq();
  }

  if (doc_count > 0) {
    auto collection_statistics = srch_context->linear_allocator->allocate_object<CollectionStatistics>(
        field, index_reader->max_doc, doc_count, sum_total_term_freq, sum_doc_freq);
    collection_statistics->validation_check();
    return collection_statistics;
  }

  return nullptr;
}

TermStatistics IndexSearcher::get_term_statistics(const std::string_view &term,
                                                  int32_t doc_freq,
                                                  int64_t total_term_freq) {
  // TODO : validate values
  return TermStatistics{term, doc_freq, total_term_freq};
}

void IndexSearcher::do_search(UseOnlyPtr<SearchContext> srch_context,
                              UseOnlyPtr<Weight> weight,
                              UseOnlyPtr<CollectorManager> collector_manager,
                              UseOnlyPtr<Collector> collector) {
  auto gc_mark = srch_context->linear_allocator->mark();
  do_search_single_thread(srch_context, weight, collector);
  collector_manager->merge(collector, 1);
  srch_context->linear_allocator->gc(gc_mark);
}

void IndexSearcher::do_search_single_thread(UseOnlyPtr<SearchContext> srch_context,
                                            UseOnlyPtr<Weight> weight,
                                            UseOnlyPtr<Collector> collector) {
  collector->set_weight(weight);
  const auto num_segments = index_reader->leaves.size();
  for (int32_t i = 0; i < num_segments; ++i) {
    auto gc_mark = srch_context->linear_allocator->mark();
    auto ctx = &index_reader->leaves[i];
    auto leaf_collector = collector->get_leaf_collector(ctx);
    if (auto scorer = weight->get_bulk_scorer(ctx, leaf_collector, ctx->segment_reader->get_live_docs())) {
      scorer->score();
    }
    leaf_collector->finish();
    srch_context->linear_allocator->gc(gc_mark);
  }  // End for
}
