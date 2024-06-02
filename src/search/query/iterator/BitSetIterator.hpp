#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_BITSETITERATOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_BITSETITERATOR_HPP_

#include "../../../util/TypeInfo.hpp"
#include "../../DocIdSetIterator.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../../util/BitSet.hpp"

namespace lucene::cyborg::search::query {

class BitSetIterator final : public DocIdSetIterator {
 public:
  static const util::TypeInfoPtr TYPE_INFO;

  BitSetIterator(util::ManagedPtr<util::BitSet> bits, int64_t cost);

  int64_t get_cost() final {
    return cost;
  }

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final {
    return doc;
  }

  int32_t next_doc() final {
    return advance(doc + 1);
  }

  util::ManagedPtr<util::BitSet> get_bit_set() noexcept {
    return bits;
  }

  void set_doc_id(int32_t doc_id) noexcept {
    doc = doc_id;
  }

 private:
  util::ManagedPtr<util::BitSet> bits;
  int32_t length;
  int64_t cost;
  int32_t doc;
};  // BitSetIterator



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_BITSETITERATOR_HPP_
