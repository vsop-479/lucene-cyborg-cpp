#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_DOCVALUES_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_DOCVALUES_HPP_

#include "NumericDocValues.hpp"
#include "../util/PointerTypes.hpp"
#include "../util/LinearAllocator.hpp"

namespace lucene::cyborg::index {

struct EmptyNumericDocValues final : public NumericDocValues {
  bool advance_exact(int32_t target) final;

  int64_t get_long_value() final;

  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  int32_t get_doc_id() final;

  int32_t next_doc() final;

 private:
  int32_t doc = -1;
};  // EmptyNumericDocValues



class DocValues {
 public:
  static util::ManagedPtr<EmptyNumericDocValues> empty_numeric(util::ManagedPtr<util::LinearAllocator> allocator);
};  // DocValues



}  // namespace lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_DOCVALUES_HPP_
