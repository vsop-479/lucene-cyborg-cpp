#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANCLAUSE_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANCLAUSE_HPP_

#include "../../util/PointerTypes.hpp"
#include "Query.hpp"

namespace lucene::cyborg::search::query {

struct BooleanClause {
  static constexpr int32_t NUM_OCCURS = 4;
  enum Occur {
    MUST, FILTER, SHOULD, MUST_NOT
  };

  [[nodiscard]] bool is_prohibited() const noexcept {
    return occur == Occur::MUST_NOT;
  }

  [[nodiscard]] bool is_required() const noexcept {
    return occur == Occur::MUST || occur == Occur::FILTER;
  }

  [[nodiscard]] bool is_scoring() const noexcept {
    return occur == Occur::MUST || occur == Occur::SHOULD;
  }

  util::ManagedPtr<Query> query;
  Occur occur;
}; // BooleanClause



template<typename T>
struct OccurEnumMap {
  template<typename... J>
  OccurEnumMap(J &&... init_val)
      : map() {
    for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
      map[i] = T{std::forward<J>(init_val)...};
    }
  }

  OccurEnumMap() = default;

  OccurEnumMap(const OccurEnumMap &other) {
    operator=(other);
  }

  OccurEnumMap(OccurEnumMap &&other) noexcept {
    operator=(std::move(other));
  }

  OccurEnumMap &operator=(const OccurEnumMap &other) {
    if (this != &other) {
      for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
        map[i] = other.map[i];
      }
    }

    return *this;
  }

  OccurEnumMap &operator=(OccurEnumMap &&other) noexcept {
    if (this != &other) {
      for (int32_t i = 0; i < BooleanClause::NUM_OCCURS; ++i) {
        map[i] = std::move(other.map[i]);
      }
    }

    return *this;
  }

  T &operator[](int32_t idx) noexcept {
    return map[idx];
  }

  const T &operator[](int32_t idx) const noexcept {
    return map[idx];
  }

 private:
  T map[BooleanClause::NUM_OCCURS];
};  // OccurEnumMap



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANCLAUSE_HPP_
