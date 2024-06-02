#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDSETITERATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDSETITERATOR_HPP_

#include <limits>
#include <cstdint>
#include "../util/TypeInfo.hpp"
#include "../pco/typeindex/PcoTypeIndexSupport.hpp"

namespace lucene::cyborg::search {

class DocIdSetIterator : public pco::PcoTypeIndexSupport {
 public:
  static const util::TypeInfoPtr TYPE_INFO;

  static constexpr int32_t NO_MORE_DOCS = std::numeric_limits<int32_t>::max();
  static constexpr int32_t ADVANCE_NOT_YET_CALLED = -1;

  explicit DocIdSetIterator(util::TypeInfoPtr _iterator_type_info = TYPE_INFO)
      : pco::PcoTypeIndexSupport(pco::PcoObjectTypeIndex::LUCENE_CYBORG_SEARCH_DOC_ID_SET_ITERATOR),
        iterator_type_info(_iterator_type_info) {
  }

  virtual int64_t get_cost() = 0;

  virtual int32_t advance(int32_t target) = 0;

  virtual int32_t get_doc_id() = 0;

  virtual int32_t next_doc() = 0;

  [[nodiscard]] util::TypeInfoPtr get_type_info() const noexcept {
    return iterator_type_info;
  }

 protected:
  util::TypeInfoPtr iterator_type_info;
};  // DocIdSetIterator



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDSETITERATOR_HPP_
