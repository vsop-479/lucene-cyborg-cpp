#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDCONSUMER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDCONSUMER_HPP_

#include <cstdint>

namespace lucene::cyborg::search {

struct DocIdConsumer {
  virtual void collect_one_doc(int32_t doc_id) = 0;
};  // DocIdConsumer

}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDCONSUMER_HPP_
