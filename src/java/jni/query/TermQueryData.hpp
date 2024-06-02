#ifndef UNTITLED_INCLUDE_JNI_QUERY_TERMQUERYDATA_HPP_
#define UNTITLED_INCLUDE_JNI_QUERY_TERMQUERYDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct TermQueryData {
  static jclass get_class(JNIEnv *env) {
    static jclass TERM_QUERY_DATA_CLASS = env->FindClass("lucene/cyborg/jni/query/TermQueryData");
    return TERM_QUERY_DATA_CLASS;
  }

  static jfieldID get_field_field_id(JNIEnv *env) {
    static jfieldID FIELD_FIELD_ID = env->GetFieldID(get_class(env), "field", "Ljava/lang/String;");
    return FIELD_FIELD_ID;
  }

  static jobject get_field(JNIEnv *env, jobject term_query_data) {
    return env->GetObjectField(term_query_data, get_field_field_id(env));
  }

  static jfieldID get_term_field_id(JNIEnv *env) {
    static jfieldID TERM_FIELD_ID = env->GetFieldID(get_class(env), "term", "Ljava/lang/String;");
    return TERM_FIELD_ID;
  }

  static jobject get_term(JNIEnv *env, jobject term_query_data) {
    return env->GetObjectField(term_query_data, get_term_field_id(env));
  }
};  // TermQueryData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_QUERY_TERMQUERYDATA_HPP_
