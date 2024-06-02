#include <cmath>
#include "Bm25Similarity.hpp"

using lucene::cyborg::search::CollectionStatistics;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::SimScorer;
using lucene::cyborg::search::TermStatistics;
using lucene::cyborg::search::similarities::BM25Scorer;
using lucene::cyborg::search::similarities::BM25Similarity;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::ReadOnlyPtr;

constexpr int32_t CACHE_SIZE = 256;

static float LENGTH_TABLE[CACHE_SIZE] = {
    0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0, 11.0, 12.0, 13.0, 14.0, 15.0, 16.0, 17.0, 18.0, 19.0, 20.0,
    21.0, 22.0, 23.0, 24.0, 25.0, 26.0, 27.0, 28.0, 29.0, 30.0, 31.0, 32.0, 33.0, 34.0, 35.0, 36.0, 37.0, 38.0, 39.0,
    40.0, 42.0, 44.0, 46.0, 48.0, 50.0, 52.0, 54.0, 56.0, 60.0, 64.0, 68.0, 72.0, 76.0, 80.0, 84.0, 88.0, 96.0, 104.0,
    112.0, 120.0, 128.0, 136.0, 144.0, 152.0, 168.0, 184.0, 200.0, 216.0, 232.0, 248.0, 264.0, 280.0, 312.0, 344.0,
    376.0, 408.0, 440.0, 472.0, 504.0, 536.0, 600.0, 664.0, 728.0, 792.0, 856.0, 920.0, 984.0, 1048.0, 1176.0, 1304.0,
    1432.0, 1560.0, 1688.0, 1816.0, 1944.0, 2072.0, 2328.0, 2584.0, 2840.0, 3096.0, 3352.0, 3608.0, 3864.0, 4120.0,
    4632.0, 5144.0, 5656.0, 6168.0, 6680.0, 7192.0, 7704.0, 8216.0, 9240.0, 10264.0, 11288.0, 12312.0, 13336.0, 14360.0,
    15384.0, 16408.0, 18456.0, 20504.0, 22552.0, 24600.0, 26648.0, 28696.0, 30744.0, 32792.0, 36888.0, 40984.0, 45080.0,
    49176.0, 53272.0, 57368.0, 61464.0, 65560.0, 73752.0, 81944.0, 90136.0, 98328.0, 106520.0, 114712.0, 122904.0,
    131096.0, 147480.0, 163864.0, 180248.0, 196632.0, 213016.0, 229400.0, 245784.0, 262168.0, 294936.0, 327704.0,
    360472.0, 393240.0, 426008.0, 458776.0, 491544.0, 524312.0, 589848.0, 655384.0, 720920.0, 786456.0, 851992.0,
    917528.0, 983064.0, 1048600.0, 1179672.0, 1310744.0, 1441816.0, 1572888.0, 1703960.0, 1835032.0, 1966104.0,
    2097176.0, 2359320.0, 2621464.0, 2883608.0, 3145752.0, 3407896.0, 3670040.0, 3932184.0, 4194328.0, 4718616.0,
    5242904.0, 5767192.0, 6291480.0, 6815768.0, 7340056.0, 7864344.0, 8388632.0, 9437208.0, 1.0485784E7, 1.153436E7,
    1.2582936E7, 1.3631512E7, 1.4680088E7, 1.5728664E7, 1.677724E7, 1.8874392E7, 2.0971544E7, 2.3068696E7, 2.5165848E7,
    2.7263E7, 2.9360152E7, 3.1457304E7, 3.3554456E7, 3.774876E7, 4.1943064E7, 4.613737E7, 5.033167E7, 5.4525976E7,
    5.872028E7, 6.2914584E7, 6.710889E7, 7.5497496E7, 8.3886104E7, 9.227471E7, 1.0066332E8, 1.0905193E8, 1.17440536E8,
    1.25829144E8, 1.3421776E8, 1.5099498E8, 1.677722E8, 1.845494E8, 2.0132662E8, 2.1810384E8, 2.3488106E8, 2.5165827E8,
    2.684355E8, 3.0198992E8, 3.3554435E8, 3.6909878E8, 4.0265322E8, 4.3620765E8, 4.6976208E8, 5.033165E8, 5.368709E8,
    6.039798E8, 6.7108864E8, 7.381975E8, 8.053064E8, 8.724152E8, 9.395241E8, 1.00663296E9, 1.0737418E9, 1.2079596E9,
    1.3421773E9, 1.476395E9, 1.6106127E9, 1.7448305E9, 1.8790482E9, 2.0132659E9};

//
// BM25Similarity
//

BM25Similarity::BM25Similarity(float _k1, float _b, bool _discount_overlaps)
    : Similarity(),
      k1(_k1),
      b(_b),
      discount_overlaps(_discount_overlaps) {
  if (k1 < 0 || !std::isfinite(k1)) [[unlikely]] {
    // TODO
    throw 13;
  }
  if (b < 0 || b > 1 || std::isnan(b)) [[unlikely]] {
    // TODO
    throw 13;
  }
}

ManagedPtr<SimScorer> BM25Similarity::get_scorer(
    ManagedPtr<SearchContext> srch_context,
    float boost,
    ReadOnlyPtr<CollectionStatistics> collection_statistics,
    TermStatistics term_statistics[],
    const int32_t term_statistics_size) {
  double idf = 0;
  for (int32_t i = 0; i < term_statistics_size; ++i) {
    idf += calculate_idf(term_statistics[i].doc_freq, collection_statistics->doc_count);
  }
  float avgdl = avg_field_length(collection_statistics);

  auto cache = srch_context->linear_allocator->allocate_objects<float>(CACHE_SIZE);
  for (int32_t i = 0; i < CACHE_SIZE; ++i) {
    cache[i] = 1.0f / (k1 * ((1 - b) + (b * LENGTH_TABLE[i]) / avgdl));
  }
  return srch_context->linear_allocator->allocate_object<BM25Scorer>(idf, cache);
}

float BM25Similarity::calculate_idf(int64_t doc_freq, int64_t doc_count) {
  return std::log((double) (1 + (doc_count - doc_freq + 0.5) / (doc_freq + 0.5)));
}

float BM25Similarity::avg_field_length(ReadOnlyPtr<CollectionStatistics> collection_stats) {
  return collection_stats->sum_total_term_freq / ((double) collection_stats->doc_count);
}

//
// BM25Scorer
//

float BM25Scorer::score(float freq, int64_t encoded_norm) {
  // In order to guarantee monotonicity with both freq and norm without
  // promoting to doubles, we rewrite freq / (freq + norm) to
  // 1 - 1 / (1 + freq * 1/norm).
  // freq * 1/norm is guaranteed to be monotonic for both freq and norm due
  // to the fact that multiplication and division round to the nearest
  // float. And then monotonicity is preserved through composition via
  // x -> 1 + x and x -> 1 - 1/x.
  // Finally, we expand weight * (1 - 1 / (1 + freq * 1/norm)) to
  // weight - weight / (1 + freq * 1/norm), which runs slightly faster.
  const float norm_inverse = cache[encoded_norm & 0xFFULL];
  return weight - weight / (1.0f + freq * norm_inverse);
}

BM25Scorer::BM25Scorer(float _idf,
                       ReadOnlyPtr<float> _cache)
    : SimScorer(),
      cache(_cache),
      weight(_idf) {
}
