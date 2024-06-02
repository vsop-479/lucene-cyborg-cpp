#include "DocValues.hpp"

using lucene::cyborg::index::DocValues;
using lucene::cyborg::index::EmptyNumericDocValues;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;

//
// EmptyNumericDocValues
//
bool EmptyNumericDocValues::advance_exact(int32_t target) {
  doc = target;
  return false;
}

int64_t EmptyNumericDocValues::get_long_value() {
  assert(false);
  return 0;
}

int64_t EmptyNumericDocValues::get_cost() {
  return 0;
}

int32_t EmptyNumericDocValues::advance(int32_t target) {
  return doc = NO_MORE_DOCS;
}

int32_t EmptyNumericDocValues::get_doc_id() {
  return doc;
}

int32_t EmptyNumericDocValues::next_doc() {
  return doc = NO_MORE_DOCS;
}



//
// DocValues
//

ManagedPtr<EmptyNumericDocValues> DocValues::empty_numeric(
    ManagedPtr<LinearAllocator> allocator) {
  return allocator->allocate_object<EmptyNumericDocValues>();
}
