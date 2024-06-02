#ifndef UNTITLED_INCLUDE_JNI_COLLECTOR_COLLECTORMANAGERDATA_HPP_
#define UNTITLED_INCLUDE_JNI_COLLECTOR_COLLECTORMANAGERDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct CollectorManagerData {
  static jclass get_class(JNIEnv *env) {
    static jclass COLLECTOR_MANAGER_DATA_CLASS = env->FindClass("lucene/cyborg/jni/collector/CollectorManagerData");
    return COLLECTOR_MANAGER_DATA_CLASS;
  }

  static jfieldID get_type_id_field_id(JNIEnv *env) {
    static jfieldID TYPE_ID_FIELD_ID = env->GetFieldID(get_class(env), "typeId", "I");
    return TYPE_ID_FIELD_ID;
  }

  static jint get_type_id(JNIEnv *env, jobject collector_manager_data) {
    return env->GetIntField(collector_manager_data, get_type_id_field_id(env));
  }

  static jfieldID get_score_mode_field_id(JNIEnv *env) {
    static jfieldID SCORE_MODE_FIELD_ID = env->GetFieldID(get_class(env), "scoreMode", "I");
    return SCORE_MODE_FIELD_ID;
  }

  static jint get_score_mode(JNIEnv *env, jobject collector_manager_data) {
    return env->GetIntField(collector_manager_data, get_score_mode_field_id(env));
  }
};  // CollectorManagerData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_COLLECTOR_COLLECTORMANAGERDATA_HPP_
