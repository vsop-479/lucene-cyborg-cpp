#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SIMILARITY_BM25SIMILARITY_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SIMILARITY_BM25SIMILARITY_HPP_

#include "../Similarity.hpp"

namespace lucene::cyborg::search::similarities {

class BM25Scorer final : public SimScorer {
 public:
  BM25Scorer(float idf, util::ReadOnlyPtr<float> cache);

  float score(float freq, int64_t encoded_norm) final;

 private:
  util::ReadOnlyPtr<float> cache;
  float weight;
};  // BM25Scorer



class BM25Similarity final : public Similarity {
 public:
  explicit BM25Similarity(float k1 = 1.2, float b = 0.75, bool discount_overlaps = true);

  util::ManagedPtr<SimScorer> get_scorer(util::ManagedPtr<SearchContext> srch_context,
                                         float boost,
                                         util::ReadOnlyPtr<CollectionStatistics> collection_statistics,
                                         TermStatistics term_statistics[],
                                         int32_t term_statistics_size) final;

 private:
  float calculate_idf(int64_t doc_freq, int64_t doc_count);

  float avg_field_length(util::ReadOnlyPtr<CollectionStatistics> collectionStats);

  float k1;
  float b;
  bool discount_overlaps;
};  // BM25Similarity



}  // lucene::cyborg::search::similarities

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SIMILARITY_BM25SIMILARITY_HPP_
