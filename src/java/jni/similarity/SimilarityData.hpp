#ifndef UNTITLED_INCLUDE_JNI_SIMILARITY_SIMILARITYDATA_HPP_
#define UNTITLED_INCLUDE_JNI_SIMILARITY_SIMILARITYDATA_HPP_

#include <jni.h>

namespace lucene::cyborg::jni {

struct SimilarityData {
  static jclass get_class(JNIEnv *env) {
    static jclass SIMILARITY_DATA_CLASS = env->FindClass("lucene/cyborg/jni/similarity/SimilarityData");
    return SIMILARITY_DATA_CLASS;
  }

  static jfieldID get_type_id_field_id(JNIEnv *env) {
    static jfieldID TYPE_ID_FIELD_ID = env->GetFieldID(get_class(env), "typeId", "I");
    return TYPE_ID_FIELD_ID;
  }

  static jint get_type_id(JNIEnv *env, jobject similarity_data) {
    return env->GetIntField(similarity_data, get_type_id_field_id(env));
  }
};  // SimilarityData



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JNI_SIMILARITY_SIMILARITYDATA_HPP_
