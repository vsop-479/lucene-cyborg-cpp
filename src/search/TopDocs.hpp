#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_TOPDOCS_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_TOPDOCS_HPP_

#include "TotalHits.hpp"
#include "../util/PointerTypes.hpp"
#include "ScoreDoc.hpp"

namespace lucene::cyborg::search {

struct TopDocs {
  TotalHits total_hits;
  util::TypedVector<util::ManagedPtr<ScoreDoc>> score_docs;
};  // TopDocs



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_TOPDOCS_HPP_
