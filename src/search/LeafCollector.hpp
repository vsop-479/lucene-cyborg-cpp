#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_LEAFCOLLECTOR_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_LEAFCOLLECTOR_HPP_

#include "Scorable.hpp"
#include "DocIdStream.hpp"
#include "../util/PointerTypes.hpp"
#include "DocIdSetIterator.hpp"

namespace lucene::cyborg::search {

class LeafCollector : public DocIdConsumer {
 public:
  virtual void collect(util::UseOnlyPtr<DocIdStream> doc_id_stream) {
    doc_id_stream->for_each(this);
  }

  virtual void finish() {
    // no-op
  }

  virtual void set_scorer(util::ManagedPtr<Scorable> _scorer) {
    scorer = _scorer;
  }

 protected:
  util::ManagedPtr<Scorable> scorer = nullptr;
};  // LeafCollector



}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_LEAFCOLLECTOR_HPP_
