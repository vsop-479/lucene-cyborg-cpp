#ifndef UNTITLED_INCLUDE_JNI_SEARCH_INDEXSEARCHERDATA_HPP_
#define UNTITLED_INCLUDE_JNI_SEARCH_INDEXSEARCHERDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct IndexSearcherProxy {
  static jclass get_class(JNIEnv *env) {
    static jclass INDEX_SEARCHER_DATA_CLASS = env->FindClass("lucene/cyborg/jni/IndexSearcherProxy");
    return INDEX_SEARCHER_DATA_CLASS;
  }

  static jfieldID get_similarity_data_field_id(JNIEnv *env) {
    static jfieldID
        SIMILARITY_DATA_FIELD_ID =
        env->GetFieldID(get_class(env), "similarityData", "Llucene/cyborg/jni/similarity/SimilarityData;");
    return SIMILARITY_DATA_FIELD_ID;
  }

  static jobject get_similarity_data(JNIEnv *env, jobject index_searcher) {
    return env->GetObjectField(index_searcher, get_similarity_data_field_id(env));
  }
};  // IndexSearcherProxy



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_SEARCH_INDEXSEARCHERDATA_HPP_
