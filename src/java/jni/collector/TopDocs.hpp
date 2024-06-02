#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_TOPDOCS_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_TOPDOCS_HPP_

#include <jni.h>
#include <cstdint>

namespace lucene::cyborg::jni {

struct TopDocs {
  static constexpr int32_t TOTAL_HITS_RELATION_EQUAL_TO = 0;
  static constexpr int32_t TOTAL_HITS_RELATION_GREATER_THAN_OR_EQUAL_TO = 1;

  static jclass get_class(JNIEnv *env) {
    static jclass TOP_DOCS_CLASS = env->FindClass("lucene/cyborg/jni/collector/TopDocs");
    return TOP_DOCS_CLASS;
  }

  static jfieldID get_total_hits_field_id(JNIEnv *env) {
    static jfieldID TOTAL_HITS_FIELD_ID = env->GetFieldID(get_class(env), "totalHits", "J");
    return TOTAL_HITS_FIELD_ID;
  }

  static jfieldID get_total_hits_relation_field_id(JNIEnv *env) {
    static jfieldID TOTAL_HITS_RELATION_FIELD_ID = env->GetFieldID(get_class(env), "totalHitsRelation", "I");
    return TOTAL_HITS_RELATION_FIELD_ID;
  }

  static jfieldID get_score_docs_field_id(JNIEnv *env) {
    static jfieldID
        SCORE_DOCS_FIELD_ID = env->GetFieldID(get_class(env), "scoreDocs", "[Llucene/cyborg/jni/collector/ScoreDoc;");
    return SCORE_DOCS_FIELD_ID;
  }

  static void set_total_hits(JNIEnv *env, jobject top_docs, jlong total_hits, jint relation) {
    env->SetLongField(top_docs, get_total_hits_field_id(env), total_hits);
    env->SetIntField(top_docs, get_total_hits_relation_field_id(env), relation);
  }

  static jobjectArray get_score_docs(JNIEnv *env, jobject top_docs) {
    return (jobjectArray) env->GetObjectField(top_docs, get_score_docs_field_id(env));
  }
};  // TopDocs



}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COLLECTOR_TOPDOCS_HPP_
