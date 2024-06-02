#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_INDEXWRITERCONSTANTS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_INDEXWRITERCONSTANTS_HPP_

#include <cstdint>
namespace lucene::cyborg::index {

struct IndexWriterConstants {
  // Hard limit on maximum number of documents that may be added to the index. If you try to add
  // more than this you'll hit {@code IllegalArgumentException}.
  // We defensively subtract 128 to be well below the lowest
  // ArrayUtil.MAX_ARRAY_LENGTH on "typical" JVMs.  We don't just use
  // ArrayUtil.MAX_ARRAY_LENGTH here because this can vary across JVMs:
  static constexpr int32_t MAX_DOCS = INT32_MAX - 128;

  // Maximum value of the token position fst_in an indexed field.
  static constexpr int32_t MAX_POSITION = INT32_MAX - 128;

  // Use package-private instance var to enforce the limit so testing
  // can use less electricity:
  static constexpr int32_t ACTUAL_MAX_DOCS = MAX_DOCS;
};  // IndexWriterConstants



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_INDEXWRITERCONSTANTS_HPP_
