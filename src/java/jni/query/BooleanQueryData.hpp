#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_QUERY_BOOLEANQUERYDATA_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_QUERY_BOOLEANQUERYDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct BooleanQueryData {
  static jclass get_class(JNIEnv *env) {
    static jclass BOOLEAN_QUERY_DATA_CLASS = env->FindClass("lucene/cyborg/jni/query/BooleanQueryData");
    return BOOLEAN_QUERY_DATA_CLASS;
  }

  static jfieldID get_minimum_number_should_match_field_id(JNIEnv *env) {
    static jfieldID MINIMUM_NUMBER_SHOULD_MATCH_FIELD_ID =
        env->GetFieldID(get_class(env), "minimumNumberShouldMatch", "I");
    return MINIMUM_NUMBER_SHOULD_MATCH_FIELD_ID;
  }

  static jint get_minimum_number_should_match(JNIEnv *env, jobject boolean_query_data) {
    return env->GetIntField(boolean_query_data, get_minimum_number_should_match_field_id(env));
  }

  static jfieldID get_next_field_id(JNIEnv *env) {
    static jfieldID NEXT_FIELD_ID = env->GetFieldID(get_class(env), "next", "I");
    return NEXT_FIELD_ID;
  }

  static jint get_next(JNIEnv *env, jobject boolean_query_data) {
    return env->GetIntField(boolean_query_data, get_next_field_id(env));
  }

  static jfieldID get_queries_field_id(JNIEnv *env) {
    static jfieldID
        QUERIES_FIELD_ID = env->GetFieldID(get_class(env), "queries", "[Llucene/cyborg/jni/query/QueryData;");
    return QUERIES_FIELD_ID;
  }

  static jobjectArray get_queries(JNIEnv *env, jobject boolean_query_data) {
    return (jobjectArray) env->GetObjectField(boolean_query_data, get_queries_field_id(env));
  }

  static jfieldID get_clauses_field_id(JNIEnv *env) {
    static jfieldID CLAUSES_FIELD_ID = env->GetFieldID(get_class(env), "clauses", "[I");
    return CLAUSES_FIELD_ID;
  }

  static jintArray get_clauses(JNIEnv *env, jobject boolean_query_data) {
    return (jintArray) env->GetObjectField(boolean_query_data, get_clauses_field_id(env));
  }
};  // BooleanQueryData



}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_QUERY_BOOLEANQUERYDATA_HPP_
