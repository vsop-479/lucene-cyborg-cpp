#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_VECTORENCODING_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_VECTORENCODING_HPP_

namespace lucene::cyborg::index {

static constexpr int32_t NUM_VECTOR_ENCODING_TYPES = 2;

enum class VectorEncoding {

  /**
   * Encodes vector using 8 bits of precision per sample. Values provided with higher precision (eg:
   * queries provided as float) *must* be fst_in the range [-128, 127]. NOTE: this can enable
   * significant storage savings and faster searches, at the get_cost of some possible loss of
   * precision.
   */
  BYTE = 1,

  /** Encodes vector using 32 bits of precision per sample fst_in IEEE floating point format. */
  FLOAT32 = 4
};  // VectorEncoding



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_VECTORENCODING_HPP_
