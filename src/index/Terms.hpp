#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_TERMS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_TERMS_HPP_

#include <string_view>
#include "../util/PointerTypes.hpp"
#include "TermsEnum.hpp"
#include "../search/SearchContext.hpp"
#include "../util/LinearAllocator.hpp"
#include "TermsStats.hpp"

namespace lucene::cyborg::index {

class Terms {
 public:
  virtual ~Terms() = default;

  virtual TermsStatsPtr get_stats(util::UseOnlyPtr<util::LinearAllocator> allocator) = 0;

  virtual util::ManagedPtr<index::TermsEnum> iterator(util::UseOnlyPtr<util::LinearAllocator> allocator) = 0;

  virtual int32_t size() = 0;

  virtual int64_t get_sum_total_term_freq() = 0;

  virtual int64_t get_sum_doc_freq() = 0;

  virtual int32_t get_doc_count() = 0;

  virtual bool has_freq() = 0;

  virtual bool has_offset() = 0;

  virtual bool has_positions() = 0;

  virtual bool has_payloads() = 0;

  virtual std::string_view get_min() = 0;

  virtual std::string_view get_max() = 0;
};  // Terms



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_TERMS_HPP_
