#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_TERMSSTATS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_TERMSSTATS_HPP_

#include <string_view>
#include <memory>

namespace lucene::cyborg::index {

struct TermsStats {
  virtual ~TermsStats() = default;

  virtual std::string to_string() = 0;
};  // TermsStats

using TermsStatsPtr = std::unique_ptr<TermsStats>;

}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_TERMSSTATS_HPP_
