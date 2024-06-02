#include "TermStates.hpp"

using lucene::cyborg::codecs::BlockTermState;
using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::index::TermsEnum;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::TermStates;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

//
// TermStates
//

ManagedPtr<TermStates> TermStates::build(UseOnlyPtr<SearchContext> srch_context,
                                         TypedVector<LeafReaderContext> leaves,
                                         std::string_view field,
                                         std::string_view term,
                                         bool needs_stats) {
  auto allocator = srch_context->linear_allocator;
  auto term_states = srch_context->linear_allocator->allocate_object<TermStates>();
  term_states->terms_enums.vec = allocator->allocate_objects<ManagedPtr<TermsEnum>>(leaves.len);
  term_states->terms_enums.len = leaves.len;

  if (needs_stats) {
    term_states->states.vec = allocator->allocate_objects<BlockTermState>(leaves.len);
    term_states->states.len = leaves.len;
  }

  for (int32_t i = 0; i < leaves.len; ++i) {
    auto terms = leaves.vec[i].segment_reader->segment_core_readers->fields->terms(field, srch_context);
    if (terms) {
      auto terms_enum = terms->iterator(srch_context->linear_allocator);
      if (terms_enum->seek_exact(term)) {
        term_states->terms_enums.vec[i] = terms_enum;
        if (needs_stats) {
          // TODO : Term state
          term_states->doc_freq += terms_enum->doc_freq();
          term_states->total_term_freq += terms_enum->total_term_freq();
        }
      }
    } else {
      term_states->terms_enums.vec[i] = nullptr;
    }
  }  // End for

  return term_states;
}
