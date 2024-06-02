#ifndef UNTITLED_INCLUDE_JAVA_JNI_QUERY_QUERYRESOURCE_HPP_
#define UNTITLED_INCLUDE_JAVA_JNI_QUERY_QUERYRESOURCE_HPP_

#include "../common/JString.hpp"
#include "../../../search/SearchContext.hpp"
#include "../../../search/query/TermQuery.hpp"
#include "../../../search/query/BooleanQuery.hpp"
#include "../../../search/query/BooleanClause.hpp"
#include "TermQueryData.hpp"
#include "BooleanQueryData.hpp"
#include "../common/JNIResources.hpp"
#include "QueryData.hpp"
#include "QueryTypeIds.hpp"
#include <memory>

namespace lucene::cyborg::jni {

struct LuceneCyborgQueryBuilder {
  static util::ManagedPtr<search::Query> build_query(JNIEnv *env,
                                                     jobject query_data,
                                                     util::ManagedPtr<search::SearchContext> srch_context,
                                                     JNIResources &jni_resources) {
    const auto query_type_id = QueryData::get_type_id(env, query_data);

    if (query_type_id == QueryTypeIds::TERM_QUERY) {
      return build_term_query(env, query_data, srch_context, jni_resources);
    } else if (query_type_id == QueryTypeIds::BOOLEAN_QUERY) {
      return build_boolean_query(env, query_data, srch_context, jni_resources);
    }

    // TODO : 13
    throw 13;
  }

  static util::ManagedPtr<search::query::TermQuery> build_term_query(JNIEnv *env,
                                                                     jobject term_query_data,
                                                                     util::ManagedPtr<search::SearchContext> srch_context,
                                                                     JNIResources &jni_resources);

  static util::ManagedPtr<search::query::BooleanQuery> build_boolean_query(JNIEnv *env,
                                                                           jobject boolean_query_data,
                                                                           util::ManagedPtr<search::SearchContext> srch_context,
                                                                           JNIResources &jni_resources);
};  // LuceneCyborgQueryBuilder



util::ManagedPtr<search::query::TermQuery> LuceneCyborgQueryBuilder::build_term_query(JNIEnv *env,
                                                                                      jobject term_query_data,
                                                                                      util::ManagedPtr<search::SearchContext> srch_context,
                                                                                      JNIResources &jni_resources) {
  auto field_jstr = (jstring) TermQueryData::get_field(env, term_query_data);
  JString field{env, field_jstr};
  std::string_view field_str_view = field.get_string();
  auto term_jstr = (jstring) TermQueryData::get_term(env, term_query_data);
  JString term{env, term_jstr};
  std::string_view term_str_view = term.get_string();
  jni_resources.add_jstring(std::move(field));
  jni_resources.add_jstring(std::move(term));
  return srch_context->linear_allocator->allocate_object<search::query::TermQuery>(field_str_view, term_str_view);
}

search::query::BooleanClause::Occur get_boolean_clause_occur(const jint clause) {
  static constexpr int32_t BOOLEAN_CLAUSE_MUST = 0;
  static constexpr int32_t BOOLEAN_CLAUSE_FILTER = 1;
  static constexpr int32_t BOOLEAN_CLAUSE_SHOULD = 2;
  static constexpr int32_t BOOLEAN_CLAUSE_MUST_NOT = 3;

  if (clause == BOOLEAN_CLAUSE_MUST) {
    return search::query::BooleanClause::Occur::MUST;
  } else if (clause == BOOLEAN_CLAUSE_FILTER) {
    return search::query::BooleanClause::Occur::FILTER;
  } else if (clause == BOOLEAN_CLAUSE_SHOULD) {
    return search::query::BooleanClause::Occur::SHOULD;
  } else if (clause == BOOLEAN_CLAUSE_MUST_NOT) {
    return search::query::BooleanClause::Occur::MUST_NOT;
  } else {
    // TODO
    throw 13;
  }  // End if
}

util::ManagedPtr<search::query::BooleanQuery> LuceneCyborgQueryBuilder::build_boolean_query(JNIEnv *env,
                                                                                            jobject boolean_query_data,
                                                                                            util::ManagedPtr<search::SearchContext> srch_context,
                                                                                            JNIResources &jni_resources) {
  search::query::BooleanQueryBuilder builder{srch_context->linear_allocator};
  const auto min_should_match = BooleanQueryData::get_minimum_number_should_match(env, boolean_query_data);
  builder.set_minimum_number_should_match(min_should_match);

  const auto num_clauses = BooleanQueryData::get_next(env, boolean_query_data);
  if (num_clauses > 0) [[likely]] {
    auto clauses = BooleanQueryData::get_clauses(env, boolean_query_data);
    auto clauses_primitive = env->GetIntArrayElements(clauses, nullptr);

    auto queries = BooleanQueryData::get_queries(env, boolean_query_data);
    for (int32_t i = 0; i < num_clauses; ++i) {
      jobject query_data = env->GetObjectArrayElement(queries, i);
      auto lucene_cyborg_query = LuceneCyborgQueryBuilder::build_query(env, query_data, srch_context, jni_resources);
      builder.add_clause(search::query::BooleanClause{
          lucene_cyborg_query, get_boolean_clause_occur(clauses_primitive[i])});
    }
  }  // End if

  return builder.build();
}

}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JAVA_JNI_QUERY_QUERYRESOURCE_HPP_
