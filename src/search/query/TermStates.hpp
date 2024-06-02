#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_TERMSTATES_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_TERMSTATES_HPP_

#include "../../util/PointerTypes.hpp"
#include "../../index/LeafReaderContext.hpp"
#include "../../codecs/BlockTermState.hpp"

namespace lucene::cyborg::search {

struct TermStates {
  static util::ManagedPtr<TermStates> build(util::UseOnlyPtr<SearchContext> srch_context,
                                            util::TypedVector<index::LeafReaderContext> leaves,
                                            std::string_view field,
                                            std::string_view term,
                                            bool needs_stats);

  util::TypedVector<codecs::BlockTermState> states;
  util::TypedVector<util::ManagedPtr<index::TermsEnum>> terms_enums;
  int32_t doc_freq;
  int64_t total_term_freq;
};  // TermStates

}  // lucene::cyborg::search


#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_TERMSTATES_HPP_
