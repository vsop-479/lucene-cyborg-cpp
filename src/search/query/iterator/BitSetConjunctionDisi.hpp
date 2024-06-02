#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_BITSETCONJUNCTIONDISI_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_BITSETCONJUNCTIONDISI_HPP_

#include "BitSetIterator.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../../util/TypeInfo.hpp"
#include "../../../util/BitSet.hpp"

namespace lucene::cyborg::search::query {

class BitSetConjunctionDISI final : public DocIdSetIterator {
 public:
  static const util::TypeInfoPtr TYPE_INFO;

  BitSetConjunctionDISI(util::ManagedPtr<DocIdSetIterator> lead,
                        util::LVector<util::ManagedPtr<BitSetIterator>> &&bit_set_iterators);

  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final;

  int32_t next_doc() final;

  util::ManagedPtr<DocIdSetIterator> lead;
  util::LVector<util::ManagedPtr<BitSetIterator>> bit_set_iterators;
  util::LVector<util::ManagedPtr<util::BitSet>> bit_sets;
  int32_t min_length;

 private:
  int32_t do_next(int32_t doc);

#ifndef NDEBUG
  bool assert_iterators_on_same_doc();
#endif
};  // BitSetConjunctionDISI

}  // lucene::cyborg::search::query


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_BITSETCONJUNCTIONDISI_HPP_
