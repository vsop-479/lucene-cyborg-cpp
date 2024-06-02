#ifndef UNTITLED_INCLUDE_JAVA_JNI_COLLECTOR_COLLECTORMANAGERRESOURCE_HPP_
#define UNTITLED_INCLUDE_JAVA_JNI_COLLECTOR_COLLECTORMANAGERRESOURCE_HPP_

#include "../../../search/SearchContext.hpp"
#include "../../../search/SimpleTopScoreDocCollectorManager.hpp"
#include "../../../search/MatchCountCollectorManager.hpp"
#include "SimpleTopScoreDocsCollectorManagerData.hpp"
#include "TopDocs.hpp"
#include "ScoreDoc.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "CollectorManagerData.hpp"
#include "CollectorManagerTypeIds.hpp"
#include "../common/JNIResources.hpp"
#include "MatchCountCollectorManagerData.hpp"
#include <memory>

namespace lucene::cyborg::jni {

struct LuceneCyborgCollectorManagerResultsWriter {
  explicit LuceneCyborgCollectorManagerResultsWriter(search::ScoreMode _score_mode)
      : score_mode(_score_mode),
        collector_manager() {
  }

  virtual ~LuceneCyborgCollectorManagerResultsWriter() = default;

  virtual void write_results() {
    // No-op
  }

  search::ScoreMode score_mode;
  util::ManagedPtr<search::CollectorManager> collector_manager;
};  // LuceneCyborgCollectorManagerResultsWriter

using LuceneCyborgCollectorManagerResultsWriterPtr = util::LUniquePtr<LuceneCyborgCollectorManagerResultsWriter>;

struct LuceneCyborgCollectorManagerBuilder {
  static LuceneCyborgCollectorManagerResultsWriterPtr build_collector_manager(JNIEnv *env,
                                                                              jobject collector_manager_data,
                                                                              util::ManagedPtr<search::SearchContext> srch_context,
                                                                              JNIResources &jni_resources) {
    auto collector_manager_type_id = CollectorManagerData::get_type_id(env, collector_manager_data);
    auto score_mode_int_val = CollectorManagerData::get_score_mode(env, collector_manager_data);

    if (collector_manager_type_id == CollectorManagerTypeIds::SIMPLE_TOP_SCORE_DOCS_COLLECTOR_MANAGER) {
      return build_simple_top_score_docs_collector_manager(env,
                                                           collector_manager_data,
                                                           srch_context,
                                                           score_mode_int_val);
    } else if (collector_manager_type_id == CollectorManagerTypeIds::MATCH_COUNT_COLLECTOR_MANAGER) {
      return build_match_count_collector_manager(env,
                                                 collector_manager_data,
                                                 srch_context,
                                                 score_mode_int_val);
    }

    // TODO
    return nullptr;
  }

  static LuceneCyborgCollectorManagerResultsWriterPtr build_simple_top_score_docs_collector_manager(
      JNIEnv *env,
      jobject collector_manager_data,
      util::ManagedPtr<search::SearchContext> srch_context,
      jint score_mode_int_val);

  static LuceneCyborgCollectorManagerResultsWriterPtr build_match_count_collector_manager(
      JNIEnv *env,
      jobject collector_manager_data,
      util::ManagedPtr<search::SearchContext> srch_context,
      jint score_mode_int_val);

  search::ScoreMode score_mode;
  util::ManagedPtr<search::CollectorManager> collector_manager;
};  // LuceneCyborgCollectorManagerBuilder

search::ScoreMode get_score_mode(int32_t score_mode_int_val) {
  static constexpr int32_t COMPLETE = 0;
  static constexpr int32_t TOP_SCORES = 1;

  if (score_mode_int_val == COMPLETE) {
    return search::ScoreMode::COMPLETE;
  } else if (score_mode_int_val == TOP_SCORES) {
    return search::ScoreMode::TOP_SCORES;
  } else {
    // TODO
    throw 13;
  }  // End if
}

LuceneCyborgCollectorManagerResultsWriterPtr LuceneCyborgCollectorManagerBuilder::build_match_count_collector_manager(
    JNIEnv *env,
    jobject collector_manager_data,
    util::ManagedPtr<search::SearchContext> srch_context,
    jint score_mode_int_val) {
  struct MatchCountCollectorManagerResultsWriter final : public LuceneCyborgCollectorManagerResultsWriter {
    explicit MatchCountCollectorManagerResultsWriter(
        util::ManagedPtr<search::SearchContext> _srch_context,
        search::ScoreMode _score_mode,
        JNIEnv *_env,
        jobject _collector_manager_data)
        : LuceneCyborgCollectorManagerResultsWriter(_score_mode),
          env(_env),
          collector_manager_data(_collector_manager_data),
          match_count_collector_manager(_srch_context) {
      collector_manager = &match_count_collector_manager;
    }

    void write_results() final {
      MatchCountCollectorManagerData::set_results(
          env,
          collector_manager_data,
          match_count_collector_manager.count);
    }

    JNIEnv *env;
    jobject collector_manager_data;
    search::MatchCountCollectorManager match_count_collector_manager;
  };  // MatchCountCollectorManagerResultsWriter

  return LuceneCyborgCollectorManagerResultsWriterPtr(
      srch_context->linear_allocator->allocate_object<MatchCountCollectorManagerResultsWriter>(
          srch_context,
          get_score_mode(score_mode_int_val),
          env,
          collector_manager_data));
}

LuceneCyborgCollectorManagerResultsWriterPtr LuceneCyborgCollectorManagerBuilder::build_simple_top_score_docs_collector_manager(
    JNIEnv *env,
    jobject collector_manager_data,
    util::ManagedPtr<search::SearchContext> srch_context,
    jint score_mode_int_val) {
  struct SimpleTopScoreDocsCollectorResultsWriter final : public LuceneCyborgCollectorManagerResultsWriter {
    SimpleTopScoreDocsCollectorResultsWriter(
        util::ManagedPtr<search::SearchContext> _srch_context,
        search::ScoreMode _score_mode,
        jint _num_hits,
        jint _total_hits_threshold,
        JNIEnv *_env,
        jobject _collector_manager_data)
        : LuceneCyborgCollectorManagerResultsWriter(_score_mode),
          simple_top_score_doc_collector_manager(_srch_context, _score_mode, _num_hits, _total_hits_threshold),
          env(_env),
          collector_manager_data(_collector_manager_data) {
      collector_manager = &simple_top_score_doc_collector_manager;
    }

    void write_results() final {
      jobject top_docs = SimpleTopScoreDocsCollectorManagerData::get_top_docs(env, collector_manager_data);

      // set total hits
      const auto &total_hits = simple_top_score_doc_collector_manager.top_docs.total_hits;
      TopDocs::set_total_hits(env, top_docs, total_hits.value,
                              total_hits.relation == search::TotalHitsRelation::EQUAL_TO
                              ? TopDocs::TOTAL_HITS_RELATION_EQUAL_TO
                              : TopDocs::TOTAL_HITS_RELATION_GREATER_THAN_OR_EQUAL_TO);

      // set score docs
      jobjectArray score_docs = TopDocs::get_score_docs(env, top_docs);
      const auto &acquired_score_docs = simple_top_score_doc_collector_manager.top_docs.score_docs;
      for (int32_t i = 0; i < acquired_score_docs.len; ++i) {
        ScoreDoc::set_doc_score(env, score_docs, i, acquired_score_docs.vec[i]->doc, acquired_score_docs.vec[i]->score);
      }
    }

    search::SimpleTopScoreDocCollectorManager simple_top_score_doc_collector_manager;
    JNIEnv *env;
    jobject collector_manager_data;
  };  // SimpleTopScoreDocsCollectorResultsWriter

  jint num_hits = SimpleTopScoreDocsCollectorManagerData::get_num_hits(env, collector_manager_data);
  jint total_hits_threshold =
      SimpleTopScoreDocsCollectorManagerData::get_total_hits_threshold(env, collector_manager_data);

  return LuceneCyborgCollectorManagerResultsWriterPtr(
      srch_context->linear_allocator->allocate_object<SimpleTopScoreDocsCollectorResultsWriter>(
          srch_context,
          get_score_mode(score_mode_int_val),
          num_hits,
          total_hits_threshold,
          env,
          collector_manager_data));
}

}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JAVA_JNI_COLLECTOR_COLLECTORMANAGERRESOURCE_HPP_
