#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_DOCPOSPAYFILEPOINTERS_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_DOCPOSPAYFILEPOINTERS_HPP_

#include <cstdint>

namespace lucene::cyborg::codecs::v90 {

struct DocPosPayFilePointers {
  int64_t doc_start_fp;
  int64_t pos_start_fp;
  int64_t pay_start_fp;
};  // DocPosPayFilePointers



}  // lucene::cyborg::codecs::v90

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_LUCENE90_DOCPOSPAYFILEPOINTERS_HPP_
