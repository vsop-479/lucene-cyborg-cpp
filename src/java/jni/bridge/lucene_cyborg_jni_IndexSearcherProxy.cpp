#include "lucene_cyborg_jni_IndexSearcherProxy.h"
#include "../collector/LuceneCyborgCollectorManagerBuilder.hpp"
#include "../query/LuceneCyborgQueryBuilder.hpp"
#include "../similarity/LuceneCyborgSimilarityBuilder.hpp"
#include "../search/IndexSearcherProxy.hpp"
#include "../../../search/IndexSearcher.hpp"
#include "DirectoryAndStaticIndexReader.hpp"

using lucene::cyborg::jni::DirectoryAndStaticIndexReader;
using lucene::cyborg::jni::IndexSearcherProxy;
using lucene::cyborg::jni::JNIResources;
using lucene::cyborg::jni::LuceneCyborgCollectorManagerBuilder;
using lucene::cyborg::jni::LuceneCyborgQueryBuilder;
using lucene::cyborg::jni::LuceneCyborgSimilarityBuilder;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::util::LinearAllocator;

void Java_lucene_cyborg_jni_IndexSearcherProxy_search(JNIEnv *env,
                                                      jobject index_searcher_proxy,
                                                      jlong index_reader_pointer,
                                                      jobject query_data,
                                                      jobject collector_manager_data) {
  // search context
  LinearAllocator allocator{};
  SearchContext srch_context{&allocator};

  // JNI resource cleaner
  JNIResources jni_resources{srch_context.linear_allocator};

  // similarity
  jobject similarity_data = IndexSearcherProxy::get_similarity_data(env, index_searcher_proxy);
  auto lucene_cyborg_similarity =
      LuceneCyborgSimilarityBuilder::build_similarity(env, similarity_data, &srch_context, jni_resources);

  // collector manager
  auto collector_manager_results_writer = LuceneCyborgCollectorManagerBuilder::build_collector_manager(
      env, collector_manager_data, &srch_context, jni_resources);

  // query
  auto query = LuceneCyborgQueryBuilder::build_query(env, query_data, &srch_context, jni_resources);

  // index searcher
  auto directory_and_static_index_reader = (DirectoryAndStaticIndexReader *) index_reader_pointer;
  IndexSearcher searcher{directory_and_static_index_reader->index_reader.get()};
  searcher.set_similarity(lucene_cyborg_similarity);

  // search
  searcher.search(&srch_context, query, collector_manager_results_writer->collector_manager);

  // results
  collector_manager_results_writer->write_results();
}
