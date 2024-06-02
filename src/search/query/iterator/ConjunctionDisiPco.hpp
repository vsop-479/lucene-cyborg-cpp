#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONDISIPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONDISIPCO_HPP_

#include "../../DocIdSetIterator.hpp"
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "../../../util/PointerTypes.hpp"
#include "../../../util/TypeInfo.hpp"

namespace lucene::cyborg::search::query {

struct ConjunctionDISIPcoBase : public DocIdSetIterator {
  static const util::TypeInfoPtr TYPE_INFO;

  ConjunctionDISIPcoBase()
      : DocIdSetIterator(TYPE_INFO) {
  }

  static util::ManagedPtr<DocIdSetIterator> create(util::UseOnlyPtr<util::LinearAllocator> allocator,
                                                   util::LVector<util::ManagedPtr<DocIdSetIterator>> &&_iterators);
}; // ConjunctionDISIPcoBase



template<typename Lead1DISI, typename Lead2DISI>
class ConjunctionDisiPco final : public ConjunctionDISIPcoBase {
 public:
  explicit ConjunctionDisiPco(util::ManagedPtr<Lead1DISI> _lead1,
                              util::ManagedPtr<Lead2DISI> _lead2,
                              util::ManagedPtr<util::LVector<util::ManagedPtr<DocIdSetIterator>>> _iterators)
      : ConjunctionDISIPcoBase(),
        lead1(_lead1),
        lead2(_lead2),
        others(std::move(*_iterators)) {
  }

  int64_t get_cost() final {
    return lead1->get_cost();
  }

  int32_t advance(int32_t target) final {
    assert(assert_iters_on_same_doc());
    return do_next(lead1->advance(target));
  }

  int32_t get_doc_id() final {
    return lead1->get_doc_id();
  }

  int32_t next_doc() final {
    assert(assert_iters_on_same_doc());
    return do_next(lead1->next_doc());
  }

  util::ManagedPtr<Lead1DISI> lead1;
  util::ManagedPtr<Lead2DISI> lead2;
  util::LVector<util::ManagedPtr<DocIdSetIterator>> others;

 private:
  int32_t do_next(int32_t doc) {
    while (true) {
      ADVANCE_HEAD:
      assert(doc == lead1->get_doc_id());

      const int32_t next2 = lead2->advance(doc);
      if (next2 != doc) {
        doc = lead1->advance(next2);
        if (next2 != doc) {
          goto ADVANCE_HEAD;
        }
      }

      for (int32_t i = 2; i < others.size(); ++i) {
        auto &other = others[i];
        if (other->get_doc_id() < doc) {
          const int32_t next = other->advance(doc);

          if (next > doc) {
            doc = lead1->advance(next);
            goto ADVANCE_HEAD;
          }
        }
      }

      return doc;
    }  // End while
  }

#ifndef NDEBUG
  bool assert_iters_on_same_doc() {
    auto cur_doc = lead1->get_doc_id();
    bool iterators_on_the_same_doc = (lead2->get_doc_id() == cur_doc);
    for (int32_t i = 0; (i < others.size() && iterators_on_the_same_doc); ++i) {
      iterators_on_the_same_doc = iterators_on_the_same_doc && (others[i]->get_doc_id() == cur_doc);
    }
    return iterators_on_the_same_doc;
  }
#endif
};  // ConjunctionDisiPco



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_ITERATOR_CONJUNCTIONDISIPCO_HPP_
