#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDSTREAM_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDSTREAM_HPP_

#include <cstdint>
#include "../util/PointerTypes.hpp"
#include "../util/Macros.hpp"
#include "DocIdConsumer.hpp"

namespace lucene::cyborg::search {

struct DefaultCountDocIdStream final : public DocIdConsumer {
  int32_t count = 0;

  LC_ALWAYSINLINE void collect_one_doc(int32_t doc_id) final {
    ++count;
  }
};  // DefaultCountDocIdStream

struct DocIdStream {
  virtual void for_each(util::UseOnlyPtr<DocIdConsumer> doc_id_consumer) = 0;

  virtual int32_t count() {
    DefaultCountDocIdStream count_doc_id_stream{};

    for_each(&count_doc_id_stream);
    return count_doc_id_stream.count;
  }
};  // DocIdStream



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DOCIDSTREAM_HPP_
