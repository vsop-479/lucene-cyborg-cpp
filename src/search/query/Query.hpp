#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_QUERY_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_QUERY_HPP_

#include <string>
#include "../Weight.hpp"
#include "../../util/PointerTypes.hpp"
#include "../ScoreMode.hpp"

namespace lucene::cyborg::search {

class IndexSearcher;

struct QueryIdentifier {
  std::string class_name;
};  // QueryIdentifier

class Query {
 public:
  explicit Query(util::ReadOnlyPtr<QueryIdentifier> query_identifier);

  virtual util::ManagedPtr<Weight> create_weight(util::UseOnlyPtr<SearchContext> srch_context,
                                                 util::ManagedPtr<IndexSearcher> index_searcher,
                                                 ScoreMode score_mode,
                                                 float boost) = 0;

  virtual util::ManagedPtr<Query> rewrite(util::UseOnlyPtr<SearchContext> srch_context,
                                          util::UseOnlyPtr<IndexSearcher> index_searcher,
                                          bool needs_score) {
    return this;
  }

  virtual uint64_t hash() const noexcept = 0;

  virtual bool equals(util::ReadOnlyPtr<Query> other) const noexcept;

  [[nodiscard]] auto get_query_identifier() const noexcept {
    return query_identifier;
  }

 protected:
  util::ReadOnlyPtr<QueryIdentifier> query_identifier;
  uint64_t class_name_hash;
};  // Query


struct QueryHash {
  uint64_t operator()(util::ReadOnlyPtr<Query> q) const noexcept {
    return q->hash();
  }
};  // QueryHash

struct QueryEqual {
  bool operator()(util::ReadOnlyPtr<Query> q1, util::ReadOnlyPtr<Query> q2) const noexcept {
    return q1->equals(q2);
  }
};  // QueryEqual


}  // lucene::cyborg::codecs::search


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_QUERY_HPP_
