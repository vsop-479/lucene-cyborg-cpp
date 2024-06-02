#ifndef UNTITLED_INCLUDE_JNI_QUERY_QUERY_HPP_
#define UNTITLED_INCLUDE_JNI_QUERY_QUERY_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct QueryData {
  static jclass get_class(JNIEnv *env) {
    static jclass QUERY_DATA_CLASS = env->FindClass("lucene/cyborg/jni/query/QueryData");
    return QUERY_DATA_CLASS;
  }

  static jfieldID get_type_id_field_id(JNIEnv *env) {
    static jfieldID TYPE_ID_FIELD_ID = env->GetFieldID(get_class(env), "typeId", "I");
    return TYPE_ID_FIELD_ID;
  }

  static jint get_type_id(JNIEnv *env, jobject query_data) {
    return env->GetIntField(query_data, get_type_id_field_id(env));
  }
};  // QueryData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_QUERY_QUERY_HPP_
