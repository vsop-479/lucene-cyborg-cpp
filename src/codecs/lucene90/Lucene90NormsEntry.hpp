#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSENTRY_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSENTRY_HPP_

#include <cstdint>

namespace lucene::cyborg::codecs::v90 {

struct Lucene90NormsEntry {
  int64_t docs_with_field_offset;
  int64_t docs_with_field_length;
  int64_t norms_offset;
  int32_t num_docs_with_field;
  int16_t jump_table_entry_count;
  uint8_t dense_rank_power;
  int8_t bytes_per_norm;
};  // Lucene90NormsEntry



}  // namespace lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90NORMSENTRY_HPP_
