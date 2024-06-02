#ifndef UNTITLED_INCLUDE_JAVA_JNI_SIMILARITY_SIMILARITYRESOURCE_HPP_
#define UNTITLED_INCLUDE_JAVA_JNI_SIMILARITY_SIMILARITYRESOURCE_HPP_

#include "../../../search/SearchContext.hpp"
#include "BM25SimilarityData.hpp"
#include "../common/JNIResources.hpp"
#include "../../../search/similarity/Bm25Similarity.hpp"
#include "SimilarityTypeIds.hpp"
#include "SimilarityData.hpp"
#include <memory>

namespace lucene::cyborg::jni {

struct LuceneCyborgSimilarityBuilder {
  static util::ManagedPtr<search::Similarity> build_similarity(JNIEnv *env,
                                                               jobject similarity_data,
                                                               util::ManagedPtr<search::SearchContext> srch_context,
                                                               JNIResources &jni_resources) {
    auto similarity_type_id = SimilarityData::get_type_id(env, similarity_data);

    if (similarity_type_id == SimilarityTypeIds::BM25_SIMILARITY) {
      return build_bm25_similarity(env, similarity_data, srch_context, jni_resources);
    }

    // TODO
    return nullptr;
  }

  static util::ManagedPtr<search::Similarity> build_bm25_similarity(JNIEnv *env,
                                                                    jobject bm25_similarity_data,
                                                                    util::ManagedPtr<search::SearchContext> srch_context,
                                                                    JNIResources &jni_resources);
};  // LuceneCyborgSimilarityBuilder



util::ManagedPtr<search::Similarity> LuceneCyborgSimilarityBuilder::build_bm25_similarity(JNIEnv *env,
                                                                                          jobject bm25_similarity_data,
                                                                                          util::ManagedPtr<search::SearchContext> srch_context,
                                                                                          JNIResources &jni_resources) {
  const float k1 = BM25SimilarityData::get_k1(env, bm25_similarity_data);
  const float b = BM25SimilarityData::get_b(env, bm25_similarity_data);
  const bool discount_overlaps = BM25SimilarityData::get_discount_overlaps(env, bm25_similarity_data);

  return srch_context->linear_allocator
      ->allocate_object<search::similarities::BM25Similarity>(k1, b, discount_overlaps);
}

}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JAVA_JNI_SIMILARITY_SIMILARITYRESOURCE_HPP_
