#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONDISI_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONDISI_HPP_

#include "../../DocIdSetIterator.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../../util/TypeInfo.hpp"

namespace lucene::cyborg::search::query {

class ConjunctionDISI final : public DocIdSetIterator {
 public:
  static const util::TypeInfoPtr TYPE_INFO;

  explicit ConjunctionDISI(util::LVector<util::ManagedPtr<DocIdSetIterator>> &&iterators);

  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final;

  int32_t next_doc() final;

  util::ManagedPtr<DocIdSetIterator> lead1;
  util::ManagedPtr<DocIdSetIterator> lead2;
  util::LVector<util::ManagedPtr<DocIdSetIterator>> others;

 private:
  int32_t do_next(int32_t doc);

#ifndef NDEBUG
  bool assert_iters_on_same_doc();
#endif
};  // ConjunctionDISI



}  // lucene::cyborg::search::query


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONDISI_HPP_
