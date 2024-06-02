#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANREQEXCLBULKSCORER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANREQEXCLBULKSCORER_HPP_

#include "../BulkScorer.hpp"
#include "../../util/Bits.hpp"

namespace lucene::cyborg::search::query {

class BooleanReqExclBulkScorer final : public BulkScorer {
 public:
  BooleanReqExclBulkScorer(
      util::ManagedPtr<BulkScorer> _req,
      util::ManagedPtr<DocIdSetIterator> _excl)
      : BulkScorer(),
        req(_req),
        excl(_excl) {
  }

  int32_t score(int32_t min_doc_id, int32_t max_doc_id) final {
    return do_processing(min_doc_id, max_doc_id);
  }

  int64_t get_cost() final {
    return req->get_cost();
  }

  util::ManagedPtr<LeafCollector> get_collector() final {
    return req->get_collector();
  }

  util::ManagedPtr<LeafCollector> reset_collector(util::ManagedPtr<LeafCollector> new_collector) final {
    return req->reset_collector(new_collector);
  }

  util::ManagedPtr<BulkScorer> req;
  util::ManagedPtr<DocIdSetIterator> excl;

 private:
  int32_t do_processing(int32_t min_doc_id, int32_t max_doc_id) {
    int32_t up_to = min_doc_id;
    int32_t excl_doc = excl->get_doc_id();

    while (up_to < max_doc_id) {
      if (excl_doc < up_to) {
        excl_doc = excl->advance(up_to);
      }
      if (excl_doc == up_to) {
        // up_to is excluded, so we can consider that we scored up to up_to + 1
        ++up_to;
        excl_doc = excl->next_doc();
      } else {
        up_to = req->score(up_to, std::min(excl_doc, max_doc_id));
      }
    }  // End while

    if (up_to == max_doc_id) {
      up_to = req->score(up_to, up_to);
    }  // End if

    return up_to;
  }
};  // BooleanReqExclBulkScorer



}  // lucene::cyborg::search::query

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BOOLEANREQEXCLBULKSCORER_HPP_
