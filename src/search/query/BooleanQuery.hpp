#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANQUERY_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANQUERY_HPP_

#include <optional>
#include "BooleanClause.hpp"
#include "../../util/LinearAllocator.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"

namespace lucene::cyborg::search::query {

class BooleanQuery;

class BooleanQueryBuilder {
 public:
  explicit BooleanQueryBuilder(util::ManagedPtr<util::LinearAllocator> allocator);

  void set_minimum_number_should_match(int32_t _minimum_number_should_match) noexcept;

  void add_clause(const BooleanClause &clause);

  util::ManagedPtr<BooleanQuery> build();

 private:
  util::LVector<BooleanClause> clauses;
  util::ManagedPtr<util::LinearAllocator> allocator;
  int32_t minimum_number_should_match;
};  // BooleanQueryBuilder



class BooleanQuery final : public Query {
  friend class BooleanWeight;

 public:
  static const QueryIdentifier QUERY_IDENTIFIER;

  BooleanQuery(util::ManagedPtr<util::LinearAllocator> allocator,
               int32_t minimum_number_should_match,
               const util::TypedVector<BooleanClause> &clauses);

  util::ManagedPtr<Weight> create_weight(util::UseOnlyPtr<SearchContext> srch_context,
                                         util::ManagedPtr<IndexSearcher> index_searcher,
                                         ScoreMode score_mode,
                                         float boost) final;

  util::ManagedPtr<Query> rewrite(util::UseOnlyPtr<SearchContext> srch_context,
                                  util::UseOnlyPtr<IndexSearcher> index_searcher,
                                  bool needs_score) final;

  uint64_t hash() const noexcept final;

  bool equals(util::ReadOnlyPtr<Query> other) const noexcept final;

  bool is_pure_disjunction() const noexcept;

 private:
  std::optional<uint64_t> hash_cache;
  int32_t minimum_number_should_match;
  util::TypedVector<BooleanClause> clauses;
  OccurEnumMap<util::LVector<util::ManagedPtr<Query>>> clause_sets;
};  // BooleanQuery



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANQUERY_HPP_
