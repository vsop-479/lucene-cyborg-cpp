#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SIMILARITY_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SIMILARITY_HPP_

#include "../util/PointerTypes.hpp"
#include "SearchContext.hpp"

namespace lucene::cyborg::search {

struct CollectionStatistics {
  CollectionStatistics(
      std::string_view _field,
      int64_t _max_doc,
      int64_t _doc_count,
      int64_t _sum_total_term_freq,
      int64_t _sum_doc_freq)
      : field(_field),
        max_doc(_max_doc),
        doc_count(_doc_count),
        sum_total_term_freq(_sum_total_term_freq),
        sum_doc_freq(_sum_doc_freq) {
  }

  void validation_check();

  std::string_view field;
  int64_t max_doc;
  int64_t doc_count;
  int64_t sum_total_term_freq;
  int64_t sum_doc_freq;
};  // CollectionStatistics



struct TermStatistics {
  std::string_view term;
  int64_t doc_freq;
  int64_t total_term_freq;
};  // TermStatistics



struct SimScorer {
  virtual float score(float freq, int64_t norm) = 0;
};  // SimScorer


struct Similarity {
  virtual util::ManagedPtr<SimScorer> get_scorer(util::ManagedPtr<SearchContext> srch_context,
                                                 float boost,
                                                 util::ReadOnlyPtr<CollectionStatistics> collection_statistics,
                                                 TermStatistics term_statistics[],
                                                 int32_t term_statistics_size) = 0;
};  // Similarity



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SIMILARITY_HPP_
