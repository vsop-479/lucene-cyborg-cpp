#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_QUERY_QUERYTYPEIDS_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_QUERY_QUERYTYPEIDS_HPP_

#include <cstdint>

namespace lucene::cyborg::jni {

struct QueryTypeIds {
  static constexpr int32_t TERM_QUERY = 0;
  static constexpr int32_t BOOLEAN_QUERY = 1;
};  // QueryTypeIds



}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_QUERY_QUERYTYPEIDS_HPP_
