#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_SCOREDOC_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_SCOREDOC_HPP_

#include <jni.h>
#include <cstdint>

namespace lucene::cyborg::jni {

struct ScoreDoc {
  static jclass get_class(JNIEnv *env) {
    static jclass SCORE_DOC_CLASS = env->FindClass("lucene/cyborg/jni/collector/ScoreDoc");
    return SCORE_DOC_CLASS;
  }

  static jfieldID get_doc_field_id(JNIEnv *env) {
    static jfieldID DOC_FIELD_ID = env->GetFieldID(get_class(env), "doc", "I");
    return DOC_FIELD_ID;
  }

  static void set_doc(JNIEnv *env, jobject score_doc, jint doc) {
    env->SetIntField(score_doc, get_doc_field_id(env), doc);
  }

  static jfieldID get_score_field_id(JNIEnv *env) {
    static jfieldID SCORE_FIELD_ID = env->GetFieldID(get_class(env), "score", "F");
    return SCORE_FIELD_ID;
  }

  static void set_score(JNIEnv *env, jobject score_doc, jfloat score) {
    env->SetFloatField(score_doc, get_score_field_id(env), score);
  }

  static void set_doc_score(JNIEnv *env, jobjectArray score_doc_arr, jsize index, jint doc, jfloat score) {
    jobject score_doc = env->GetObjectArrayElement(score_doc_arr, index);
    set_doc(env, score_doc, doc);
    set_score(env, score_doc, score);
  }
};  // ScoreDoc



}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_SCOREDOC_HPP_
