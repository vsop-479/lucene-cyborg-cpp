#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90TERMSTATE_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90TERMSTATE_HPP_

#include <cstdint>

namespace lucene::cyborg::codecs::v90 {

struct Lucene90TermState {
  uint8_t index_options;
  int32_t doc_freq;
  int32_t singleton_doc_id;
  int64_t total_term_freq;
  int64_t doc_start_fp;
  int64_t pos_start_fp;
  int64_t pay_start_fp;
  int64_t last_pos_block_offset;
  int64_t skip_offset;
};  // Lucene90TermState



}  // namespace lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_LUCENE90TERMSTATE_HPP_
