#include "Similarity.hpp"

using lucene::cyborg::search::CollectionStatistics;

//
// CollectionStatistics
//

void CollectionStatistics::validation_check() {
  if (field.empty()) [[unlikely]] {
    // TODO
    throw 13;
  }

  if (max_doc <= 0) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException("maxDoc must be positive, maxDoc: " + maxDoc);
  }

  if (doc_count <= 0) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException("docCount must be positive, docCount: " + docCount);
  }

  if (doc_count > max_doc) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException(
    //     "docCount must not exceed maxDoc, docCount: " + docCount + ", maxDoc: " + maxDoc);
  }

  if (sum_doc_freq <= 0) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException("sumDocFreq must be positive, sumDocFreq: " + sumDocFreq);
  }

  if (sum_doc_freq < doc_count) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException(
    //     "sumDocFreq must be at least docCount, sumDocFreq: "
    //         + sumDocFreq
    //         + ", docCount: "
    //         + docCount);
  }

  if (sum_total_term_freq <= 0) [[unlikely]] {
    // TODO
    throw 13;
    // throw new IllegalArgumentException(
    //     "sumTotalTermFreq must be positive, sumTotalTermFreq: " + sumTotalTermFreq);
  }

  if (sum_total_term_freq < sum_doc_freq) [[unlikely]] {
    // TODO
    throw 13;
//    throw new IllegalArgumentException(
//        "sumTotalTermFreq must be at least sumDocFreq, sumTotalTermFreq: "
//            + sumTotalTermFreq
//            + ", sumDocFreq: "
//            + sumDocFreq);
//
  }
}