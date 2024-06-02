#ifndef UNTITLED_INCLUDE_JAVA_JNI_COLLECTOR_MATCHCOUNTCOLLECTORMANAGERDATA_HPP_
#define UNTITLED_INCLUDE_JAVA_JNI_COLLECTOR_MATCHCOUNTCOLLECTORMANAGERDATA_HPP_

#include <jni.h>
#include <cstdint>

namespace lucene::cyborg::jni {

struct MatchCountCollectorManagerData {
  static jclass get_class(JNIEnv *env) {
    static jclass MATCH_COUNT_COLLECTOR_MANAGER_CLASS =
        env->FindClass("lucene/cyborg/jni/collector/MatchCountCollectorManagerData");
    return MATCH_COUNT_COLLECTOR_MANAGER_CLASS;
  }

  static jfieldID get_count_field_id(JNIEnv *env) {
    static jfieldID COUNT_FIELD_ID = env->GetFieldID(get_class(env), "count", "J");
    return COUNT_FIELD_ID;
  }

  static void set_results(JNIEnv *env,
                          jobject match_count_collector_manager,
                          int64_t count) {
    env->SetIntField(match_count_collector_manager, get_count_field_id(env), (jlong) count);
  }
};  // MatchCountCollectorManagerData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JAVA_JNI_COLLECTOR_MATCHCOUNTCOLLECTORMANAGERDATA_HPP_
