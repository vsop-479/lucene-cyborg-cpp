#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_TERMSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_TERMSENUM_HPP_

#include <cstdint>
#include "../util/PointerTypes.hpp"
#include "PostingsEnum.hpp"
#include "ImpactsEnum.hpp"

namespace lucene::cyborg::index {

class TermsEnum {
 public:
  virtual ~TermsEnum() = default;

  enum SeekStatus {
    /** The term was not found, and the end of iteration was hit. */
    END,
    /** The precise term was found. */
    FOUND,
    /** A different term was found after the requested term */
    NOT_FOUND
  };  // SeekStatus

  virtual std::string_view term() = 0;

  virtual bool seek_exact(std::string_view target) = 0;

  virtual int32_t doc_freq() = 0;

  virtual int64_t total_term_freq() = 0;

  virtual util::ManagedPtr<PostingsEnum> postings(PostingsEnum::Flags flags) = 0;

  virtual util::ManagedPtr<ImpactsEnum> impacts(PostingsEnum::Flags flags) = 0;
};  // TermsEnum



//class EmptyTermsEnum final : public TermsEnum {
// public:
//  std::string_view term() final {
//
//  }
//
//  bool seek_exact(std::string_view target) final {
//    // no-op
//  }
//
//  int32_t doc_freq() final {
//
//  }
//
//  int64_t total_term_freq() final {
//
//  }
//
//  util::ManagedPtr<PostingsEnum> postings(PostingsEnum::Flags flags) final {
//
//  }
//
//  util::ManagedPtr<ImpactsEnum> impacts(PostingsEnum::Flags flags) final {
//
//  }
//};  // EmptyTermsEnum



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_TERMSENUM_HPP_
