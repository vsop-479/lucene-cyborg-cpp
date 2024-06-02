#ifndef UNTITLED_INCLUDE_JNI_COLLECTOR_SIMPLETOPSCOREDOCSCOLLECTORMANAGERDATA_HPP_
#define UNTITLED_INCLUDE_JNI_COLLECTOR_SIMPLETOPSCOREDOCSCOLLECTORMANAGERDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct SimpleTopScoreDocsCollectorManagerData {
  static jclass get_class(JNIEnv *env) {
    static jclass SIMPLE_TOP_SCORE_DOCS_COLLECTOR_MANAGER_DATA_CLASS =
        env->FindClass("lucene/cyborg/jni/collector/SimpleTopScoreDocsCollectorManagerData");
    return SIMPLE_TOP_SCORE_DOCS_COLLECTOR_MANAGER_DATA_CLASS;
  }

  static jfieldID get_num_hits_field_id(JNIEnv *env) {
    static jfieldID NUM_HITS_FIELD_ID = env->GetFieldID(get_class(env), "numHits", "I");
    return NUM_HITS_FIELD_ID;
  }

  static jint get_num_hits(JNIEnv *env, jobject collector_manager_data) {
    return env->GetIntField(collector_manager_data, get_num_hits_field_id(env));
  }

  static jfieldID get_total_hits_threshold_field_id(JNIEnv *env) {
    static jfieldID TOTAL_HITS_THRESHOLD_FIELD_ID = env->GetFieldID(get_class(env), "totalHitsThreshold", "I");
    return TOTAL_HITS_THRESHOLD_FIELD_ID;
  }

  static jint get_total_hits_threshold(JNIEnv *env, jobject collector_manager_data) {
    return env->GetIntField(collector_manager_data, get_total_hits_threshold_field_id(env));
  }

  static jfieldID get_top_docs_field_id(JNIEnv *env) {
    static jfieldID
        TOP_DOCS_FIELD_ID = env->GetFieldID(get_class(env), "topDocs", "Llucene/cyborg/jni/collector/TopDocs;");
    return TOP_DOCS_FIELD_ID;
  }

  static jobject get_top_docs(JNIEnv *env, jobject collector_manager_data) {
    return env->GetObjectField(collector_manager_data, get_top_docs_field_id(env));
  }
};  // SimpleTopScoreDocsCollectorManagerData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_COLLECTOR_SIMPLETOPSCOREDOCSCOLLECTORMANAGERDATA_HPP_
