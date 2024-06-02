#include "BooleanQuery.hpp"
#include "BooleanWeight.hpp"

using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::Query;
using lucene::cyborg::search::QueryIdentifier;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::Weight;
using lucene::cyborg::search::query::BooleanQuery;
using lucene::cyborg::search::query::BooleanQueryBuilder;
using lucene::cyborg::search::query::BooleanWeight;
using lucene::cyborg::util::LUnorderedMap;
using lucene::cyborg::util::LUnorderedSet;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::MemoryUtils;
using lucene::cyborg::util::ReadOnlyPtr;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

//
// BooleanQueryBuilder
//

BooleanQueryBuilder::BooleanQueryBuilder(ManagedPtr<LinearAllocator> _allocator)
    : clauses(LinearAllocatorAdapterForSTL<BooleanClause>{_allocator}),
      allocator(_allocator),
      minimum_number_should_match() {
}

void BooleanQueryBuilder::set_minimum_number_should_match(int32_t _minimum_number_should_match) noexcept {
  minimum_number_should_match = _minimum_number_should_match;
}

void BooleanQueryBuilder::add_clause(const BooleanClause &clause) {
  clauses.emplace_back(clause);
}

ManagedPtr<BooleanQuery> BooleanQueryBuilder::build() {
  return allocator->allocate_object<BooleanQuery>(allocator,
                                                  minimum_number_should_match,
                                                  TypedVector<BooleanClause>{clauses.data(), (int32_t) clauses.size()});
}

//
// BooleanQuery
//

const QueryIdentifier BooleanQuery::QUERY_IDENTIFIER{"BooleanQuery"};

BooleanQuery::BooleanQuery(ManagedPtr<LinearAllocator> _allocator,
                           int32_t _minimum_number_should_match,
                           const TypedVector<BooleanClause> &_clauses)
    : Query(&QUERY_IDENTIFIER),
      hash_cache(),
      minimum_number_should_match(_minimum_number_should_match),
      clauses(_clauses),
      clause_sets(LinearAllocatorAdapterForSTL<ManagedPtr<Query>>(_allocator)) {
  for (int32_t i = 0; i < _clauses.len; ++i) {
    clause_sets[_clauses.vec[i].occur].emplace_back(_clauses.vec[i].query);
  }
  auto gc_mark = _allocator->mark();
  for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
    if (auto &queries = clause_sets[i]; queries.size()) {
      LUnorderedSet<ReadOnlyPtr<Query>, QueryHash, QueryEqual> dedup_query{
          LinearAllocatorAdapterForSTL<ReadOnlyPtr<Query>>(_allocator)};
      int32_t dedup_idx = 0;
      for (int32_t j = 0; j < queries.size(); ++j) {
        if (dedup_query.emplace(queries[j]).second) {
          queries[dedup_idx++] = queries[j];
        }
      }
      queries.resize(dedup_idx);
    }
  }  // End for
  _allocator->gc(gc_mark);
}

bool BooleanQuery::is_pure_disjunction() const noexcept {
  return minimum_number_should_match <= 1
      && (clauses.len == clause_sets[BooleanClause::Occur::SHOULD].size());
}

ManagedPtr<Weight> BooleanQuery::create_weight(UseOnlyPtr<SearchContext> srch_context,
                                               ManagedPtr<IndexSearcher> index_searcher,
                                               ScoreMode score_mode,
                                               float boost) {
  return srch_context->linear_allocator->allocate_object<BooleanWeight>(
      srch_context, this, index_searcher, score_mode, boost);
}

ManagedPtr<Query> BooleanQuery::rewrite(UseOnlyPtr<SearchContext> srch_context,
                                        UseOnlyPtr<IndexSearcher> index_searcher,
                                        bool needs_score) {
  if (clauses.len <= 0) [[unlikely]] {
    // TODO
    throw 13;
  }

  // Optimize 1-clause queries
  if (clauses.len == 1) {
    const auto &clause = clauses.vec[0];
    if (minimum_number_should_match == 1 && clause.occur == BooleanClause::Occur::SHOULD) {
      return clause.query;
    } else if (minimum_number_should_match == 0) {
      switch (clause.occur) {
        case BooleanClause::Occur::MUST: {
          return clause.query;
        }
        case BooleanClause::Occur::FILTER: {
          if (needs_score) {
            // TODO : BoostQuery
            assert(false);
          } else {
            return clause.query;
          }
        }
        case BooleanClause::Occur::SHOULD: {
          return clause.query;
        }
        case BooleanClause::Occur::MUST_NOT: {
          // No positive clauses
          // TODO : MatchNoDocsQuery
          assert(false);
        }
      }  // End switch
    } // End if
  }  // End if

  // recursively rewrite
  {
    BooleanQueryBuilder builder{srch_context->linear_allocator};
    builder.set_minimum_number_should_match(minimum_number_should_match);
    bool actually_rewritten = false;
    for (int32_t i = 0; i < clauses.len; ++i) {
      auto &clause = clauses.vec[i];
      auto query = clause.query;
      BooleanClause::Occur occur = clause.occur;
      ManagedPtr<Query> rewritten = query->rewrite(
          srch_context, index_searcher,
          occur != BooleanClause::Occur::FILTER && occur != BooleanClause::Occur::MUST_NOT);

      if ((rewritten && rewritten != query) || false /* TODO query.getClass() == MatchNoDocsQuery.class*/) {
        // rewrite clause
        actually_rewritten = true;
        if (false/*TODO : rewritten.getClass() == MatchNoDocsQuery.class*/) {
          switch (occur) {
            case BooleanClause::SHOULD:
            case BooleanClause::MUST_NOT:
              // the clause can be safely ignored
              break;
            case BooleanClause::MUST:
            case BooleanClause::FILTER:return rewritten;
          }
        } else {
          builder.add_clause(BooleanClause{rewritten, occur});
        }
      } else {
        // leave as-is
        builder.add_clause(clause);
      }
    }  // End for

    if (actually_rewritten) {
      return builder.build();
    }
  }

  // remove duplicate FILTER and MUST_NOT clauses
  {
    int32_t clause_count = 0;
    for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
      clause_count += clause_sets[i].size();
    }

    if (clause_count != clauses.len) {
      // since clause_sets implicitly de-duplicates FILTER and MUST_NOT
      // clauses, this means there were duplicates
      BooleanQueryBuilder rewritten{srch_context->linear_allocator};
      rewritten.set_minimum_number_should_match(minimum_number_should_match);
      for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
        for (auto query : clause_sets[i]) {
          rewritten.add_clause(BooleanClause{query, (BooleanClause::Occur) i});
        }
      }
      return rewritten.build();
    }  // End if
  }


  // Check whether some clauses are both required and excluded
  auto &must_not_clauses = clause_sets[BooleanClause::Occur::MUST_NOT];
  if (!must_not_clauses.empty()) {
    LUnorderedSet<ReadOnlyPtr<Query>, QueryHash, QueryEqual> dedup_query{
        LinearAllocatorAdapterForSTL<ReadOnlyPtr<Query>>(srch_context->linear_allocator)};
    dedup_query.reserve(clause_sets[BooleanClause::Occur::MUST_NOT].size());

    for (auto query : clause_sets[BooleanClause::Occur::MUST_NOT]) {
      dedup_query.emplace(query);
    }

    // TODO : fail fast
    // if (mustNotClauses.contains(new MatchAllDocsQuery())) {
    //   return new MatchNoDocsQuery("MUST_NOT clause is MatchAllDocsQuery");
    // }

    for (auto query : clause_sets[BooleanClause::Occur::MUST]) {
      if (dedup_query.contains(query)) {
        // TODO
        throw 13;
      }
    }

    for (auto query : clause_sets[BooleanClause::Occur::FILTER]) {
      if (dedup_query.contains(query)) {
        // TODO
        throw 13;
      }
    }
  }

  // remove FILTER clauses that are also MUST clauses or that match all documents
  if (!clause_sets[BooleanClause::Occur::FILTER].empty()) {
    LUnorderedSet<ReadOnlyPtr<Query>, QueryHash, QueryEqual> filters{
        LinearAllocatorAdapterForSTL<ReadOnlyPtr<Query>>(srch_context->linear_allocator)};
    filters.reserve(clause_sets[BooleanClause::Occur::FILTER].size());
    for (auto query : clause_sets[BooleanClause::Occur::FILTER]) {
      filters.emplace(query);
    }

    bool modified = false;
    if (filters.size() > 1 || !clause_sets[BooleanClause::Occur::MUST].empty()) {
      // TODO
      // modified = filters.remove(new MatchAllDocsQuery());
    }

    for (auto query : clause_sets[BooleanClause::Occur::MUST]) {
      modified |= filters.erase(query) > 0;
    }
    if (modified) {
      BooleanQueryBuilder builder{srch_context->linear_allocator};
      builder.set_minimum_number_should_match(minimum_number_should_match);
      for (int32_t i = 0; i < clauses.len; ++i) {
        if (clauses.vec[i].occur != BooleanClause::Occur::FILTER) {
          builder.add_clause(clauses.vec[i]);
        }
      }
      for (auto filter : filters) {
        builder.add_clause(BooleanClause{const_cast<Query *>(filter), BooleanClause::Occur::FILTER});
      }
      return builder.build();
    }
  }

  // convert FILTER clauses that are also SHOULD clauses to MUST clauses
  if (!clause_sets[BooleanClause::Occur::SHOULD].empty() && !clause_sets[BooleanClause::Occur::FILTER].empty()) {
    auto &filters = clause_sets[BooleanClause::Occur::FILTER];
    auto &shoulds = clause_sets[BooleanClause::Occur::SHOULD];

    LUnorderedSet<ReadOnlyPtr<Query>, QueryHash, QueryEqual> intersection{
        LinearAllocatorAdapterForSTL<ReadOnlyPtr<Query>>(srch_context->linear_allocator)};
    intersection.reserve(filters.size());
    intersection.insert(filters.begin(), filters.end());

    LUnorderedSet<ReadOnlyPtr<Query>, QueryHash, QueryEqual> shoulds_set{
        LinearAllocatorAdapterForSTL<ReadOnlyPtr<Query>>(srch_context->linear_allocator)};
    shoulds_set.reserve(shoulds.size());
    shoulds_set.insert(shoulds.begin(), shoulds.end());
    for (auto it = intersection.begin(); it != intersection.end();) {
      if (shoulds_set.contains(*it)) {
        ++it;
      } else {
        it = intersection.erase(it);
      }
    }

    if (!intersection.empty()) {
      BooleanQueryBuilder builder{srch_context->linear_allocator};
      int32_t min_should_match = this->minimum_number_should_match;

      for (int32_t i = 0; i < clauses.len; ++i) {
        auto &clause = clauses.vec[i];
        if (intersection.contains(clause.query)) {
          if (clause.occur == BooleanClause::Occur::SHOULD) {
            builder.add_clause(BooleanClause{clause.query, BooleanClause::Occur::MUST});
            --min_should_match;
          }
        } else {
          builder.add_clause(clause);
        }
      }  // End for

      builder.set_minimum_number_should_match(std::max(0, min_should_match));
      return builder.build();
    }
  }  // End if

  // Deduplicate SHOULD clauses by summing up their boosts
  if (!clause_sets[BooleanClause::Occur::SHOULD].empty() && minimum_number_should_match <= 1) {
    LUnorderedMap<ManagedPtr<Query>, double, QueryHash, QueryEqual> should_clauses(
        clause_sets[BooleanClause::Occur::SHOULD].size(),
        LinearAllocatorAdapterForSTL<std::pair<ManagedPtr<Query> const, double>>{srch_context->linear_allocator});

    for (auto query : clause_sets[BooleanClause::Occur::SHOULD]) {
      double boost = 1;
      // TODO
      // while (query instanceof BoostQuery) {
      //   BoostQuery bq = (BoostQuery) query;
      //   boost *= bq.getBoost();
      //   query = bq.getQuery();
      // }

      auto it = should_clauses.find(query);
      should_clauses.emplace(query, (it != should_clauses.end() ? it->second : 0) + boost);
    }  // End for

    if (should_clauses.size() != clause_sets[BooleanClause::Occur::SHOULD].size()) {
      BooleanQueryBuilder builder{srch_context->linear_allocator};
      builder.set_minimum_number_should_match(minimum_number_should_match);

      for (auto &entry : should_clauses) {
        auto query = entry.first;
        float boost = entry.second;
        if (boost != 1) {
          // TODO
          // Tquery = new BoostQuery(query, boost);
        }
        builder.add_clause(BooleanClause{query, BooleanClause::Occur::SHOULD});
      }  // End for

      for (int32_t i = 0; i < clauses.len; ++i) {
        if (clauses.vec[i].occur != BooleanClause::Occur::SHOULD) {
          builder.add_clause(clauses.vec[i]);
        }
      }  // End for

      return builder.build();
    }
  }  // End if

  // Deduplicate MUST clauses by summing up their boosts
  if (!clause_sets[BooleanClause::Occur::MUST].empty()) {
    LUnorderedMap<ManagedPtr<Query>, double, QueryHash, QueryEqual> must_clauses(
        clause_sets[BooleanClause::Occur::MUST].size(),
        LinearAllocatorAdapterForSTL<std::pair<ManagedPtr<Query> const, double>>{srch_context->linear_allocator});

    for (auto query : clause_sets[BooleanClause::Occur::MUST]) {
      double boost = 1;
      // TODO
      // while (query instanceof BoostQuery) {
      //   BoostQuery bq = (BoostQuery) query;
      //   boost *= bq.getBoost();
      //   query = bq.getQuery();
      // }

      auto it = must_clauses.find(query);
      must_clauses.emplace(query, (it != must_clauses.end() ? it->second : 0) + boost);
    }

    if (must_clauses.size() != clause_sets[BooleanClause::Occur::MUST].size()) {
      BooleanQueryBuilder builder{srch_context->linear_allocator};
      builder.set_minimum_number_should_match(minimum_number_should_match);
      for (auto &entry : must_clauses) {
        auto query = entry.first;
        float boost = entry.second;
        if (boost != 1) {
          // TODO
          // query = new BoostQuery(query, boost);
        }
        builder.add_clause(BooleanClause{query, BooleanClause::Occur::MUST});
      }

      for (int32_t i = 0; i < clauses.len; ++i) {
        if (clauses.vec[i].occur != BooleanClause::Occur::MUST) {
          builder.add_clause(clauses.vec[i]);
        }
      }

      return builder.build();
    }  // End if
  }

  // Rewrite queries whose single scoring clause is a MUST clause on a
  // MatchAllDocsQuery to a ConstantScoreQuery
  {
    auto &musts = clause_sets[BooleanClause::Occur::MUST];
    auto &filters = clause_sets[BooleanClause::Occur::FILTER];
    if (musts.size() == 1 && !filters.empty()) {
      auto must = musts[0];
      float boost = 1;
      // TODO
      // if (must instanceof BoostQuery) {
      //   BoostQuery boostQuery = (BoostQuery) must;
      //   must = boostQuery.getQuery();
      //   boost = boostQuery.getBoost();
      // }
      // TODO
      // if (must.getClass() == MatchAllDocsQuery.class) {
      //   // our single scoring clause matches everything: rewrite to a CSQ on the filter
      //   // ignore SHOULD clause for now
      //   BooleanQuery.Builder
      //   builder = new BooleanQuery.Builder();
      //   for (BooleanClause clause : clauses) {
      //     switch (clause.getOccur()) {
      //       case FILTER:
      //       case MUST_NOT:builder.add(clause);
      //         break;
      //       case MUST:
      //       case SHOULD:
      //       default:
      //         // ignore
      //         break;
      //     }
      //   }
      //   Query rewritten = builder.build();
      //   rewritten = new ConstantScoreQuery(rewritten);
      //   if (boost != 1f) {
      //     rewritten = new BoostQuery(rewritten, boost);
      //   }

      //   // now add back the SHOULD clauses
      //   builder =
      //       new BooleanQuery.Builder()
      //       .setMinimumNumberShouldMatch(getMinimumNumberShouldMatch())
      //       .add(rewritten, Occur.MUST);
      //   for (Query query : clauseSets.get(Occur.SHOULD)) {
      //     builder.add(query, Occur.SHOULD);
      //   }
      //   rewritten = builder.build();
      //   return rewritten;
      // }
    }  // End if
  }

  // Flatten nested disjunctions, this is important for block-max WAND to perform well
  if (minimum_number_should_match <= 1) {
    BooleanQueryBuilder builder{srch_context->linear_allocator};
    builder.set_minimum_number_should_match(minimum_number_should_match);
    bool actually_rewritten = false;
    for (int32_t i = 0; i < clauses.len; ++i) {
      auto &clause = clauses.vec[i];
      if (clause.occur == BooleanClause::Occur::SHOULD
          && clause.query->get_query_identifier() == &BooleanQuery::QUERY_IDENTIFIER) {
        auto inner_query = (BooleanQuery *) clause.query;
        if (inner_query->is_pure_disjunction()) {
          actually_rewritten = true;
          for (int32_t j = 0; j < inner_query->clauses.len; ++j) {
            builder.add_clause(inner_query->clauses.vec[j]);
          }
        } else {
          builder.add_clause(clause);
        }
      } else {
        builder.add_clause(clause);
      }
    }  // End for
    if (actually_rewritten) {
      return builder.build();
    }
  }

  // SHOULD clause count less than or equal to minimumNumberShouldMatch
  // Important(this can only be processed after nested clauses have been flattened)
  {
    auto &shoulds = clause_sets[BooleanClause::Occur::SHOULD];
    if (!shoulds.empty()) {
      if (shoulds.size() < minimum_number_should_match) {
        // TODO
        throw 13;
        // return new MatchNoDocsQuery("SHOULD clause count less than minimumNumberShouldMatch");
      }

      if (shoulds.size() == minimum_number_should_match) {
        BooleanQueryBuilder builder{srch_context->linear_allocator};
        for (int32_t i = 0; i < clauses.len; ++i) {
          auto &clause = clauses.vec[i];
          if (clause.occur == BooleanClause::Occur::SHOULD) {
            builder.add_clause(BooleanClause{clause.query, BooleanClause::Occur::MUST});
          } else {
            builder.add_clause(clause);
          }
        }  // End for

        return builder.build();
      }  // End if
    }  // End if
  }

  return Query::rewrite(srch_context, index_searcher, needs_score);
}

uint64_t BooleanQuery::hash() const noexcept {
  if (hash_cache) {
    return *hash_cache;
  } else {
    uint64_t hash_value = 31ULL + minimum_number_should_match;
    QueryHash query_hash{};
    for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
      for (auto query : clause_sets[i]) {
        hash_value = 31ULL * hash_value + query_hash(query);
      }
    }
    const_cast<BooleanQuery *>(this)->hash_cache = hash_value;
    return hash_value;
  }  // End if
}

bool BooleanQuery::equals(ReadOnlyPtr<Query> other) const noexcept {
  if (Query::equals(other)) {
    auto other_boolean_query = (ReadOnlyPtr<BooleanQuery>) other;
    if (minimum_number_should_match == other_boolean_query->minimum_number_should_match) {
      for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
        if (clause_sets[i].size() != other_boolean_query->clause_sets[i].size()) {
          return false;
        }
        for (int32_t j = 0; j < clause_sets[i].size(); ++j) {
          if (!clause_sets[i][j]->equals(other_boolean_query->clause_sets[i][j])) {
            return false;
          }
        }
      }  // End for

      return true;
    }  // End if
  }  // End if

  return false;
}
