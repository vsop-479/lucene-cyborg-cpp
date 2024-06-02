#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREMODE_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREMODE_HPP_

namespace lucene::cyborg::search {

enum ScoreMode {
  COMPLETE = 0b11, COMPETE_NO_SCORES = 0b10, TOP_SCORES = 0b01
};  // ScoreMode

inline bool score_mode_need_scores(const ScoreMode score_mode) noexcept {
  return score_mode & 0b01;
}

inline bool score_mode_is_exhaustive(const ScoreMode score_mode) noexcept {
  return score_mode & 0b10;
}

}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_SCOREMODE_HPP_
