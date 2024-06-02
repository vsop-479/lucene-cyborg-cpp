#ifndef UNTITLED_INCLUDE_JNI_SIMILARITY_BM25SIMILARITYDATA_HPP_
#define UNTITLED_INCLUDE_JNI_SIMILARITY_BM25SIMILARITYDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct BM25SimilarityData {
  static jclass get_class(JNIEnv *env) {
    static jclass BM25_SIMILARITY_DATA_CLASS = env->FindClass("lucene/cyborg/jni/similarity/BM25SimilarityData");
    return BM25_SIMILARITY_DATA_CLASS;
  }

  static jfieldID get_k1_field_id(JNIEnv *env) {
    static jfieldID K1_FIELD_ID = env->GetFieldID(get_class(env), "k1", "F");
    return K1_FIELD_ID;
  }

  static jfloat get_k1(JNIEnv *env, jobject bm25_similarity_data) {
    return env->GetFloatField(bm25_similarity_data, get_k1_field_id(env));
  }

  static jfieldID get_b_field_id(JNIEnv *env) {
    static jfieldID B_FIELD_ID = env->GetFieldID(get_class(env), "b", "F");
    return B_FIELD_ID;
  }

  static jfloat get_b(JNIEnv *env, jobject bm25_similarity_data) {
    return env->GetFloatField(bm25_similarity_data, get_b_field_id(env));
  }

  static jfieldID get_discount_overlaps_field_id(JNIEnv *env) {
    static jfieldID DISCOUNT_OVERLAPS_FIELD_ID = env->GetFieldID(get_class(env), "discountOverlaps", "Z");
    return DISCOUNT_OVERLAPS_FIELD_ID;
  }

  static jboolean get_discount_overlaps(JNIEnv *env, jobject bm25_similarity_data) {
    return env->GetBooleanField(bm25_similarity_data, get_discount_overlaps_field_id(env));
  }
};  // BM25SimilarityData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_SIMILARITY_BM25SIMILARITYDATA_HPP_
