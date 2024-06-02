#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_COLLECTORMANAGERTYPEIDS_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_COLLECTORMANAGERTYPEIDS_HPP_

#include <cstdint>

namespace lucene::cyborg::jni {

struct CollectorManagerTypeIds {
  static constexpr int32_t SIMPLE_TOP_SCORE_DOCS_COLLECTOR_MANAGER = 0;
  static constexpr int32_t MATCH_COUNT_COLLECTOR_MANAGER = 1;
};  // CollectorManagerTypeIds



}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_COLLECTORMANAGERTYPEIDS_HPP_
