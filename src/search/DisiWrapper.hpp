#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_DISIWRAPPER_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_DISIWRAPPER_HPP_

#include "Scorer.hpp"

namespace lucene::cyborg::search {

struct DisiWrapper {
  explicit DisiWrapper(util::UseOnlyPtr<Scorer> _scorer)
      : doc(-1),
        match_cost(),
        cost(),
        iterator(_scorer->get_iterator()),
        scorer(_scorer),
        next(),
        approximation(iterator),
        two_phase_iterator(_scorer->two_phase_iterator()),
        scaled_max_score(),
        max_window_score() {
    cost = iterator->get_cost();
    if (two_phase_iterator) {
      approximation = two_phase_iterator->approximation;
      match_cost = two_phase_iterator->get_match_cost();
    }
  }

  // the current doc, used for comparison
  int32_t doc;
  // the match get_cost for two-phase iterators, 0 otherwise
  float match_cost;
  int64_t cost;
  util::ManagedPtr<DocIdSetIterator> iterator;
  util::ManagedPtr<Scorer> scorer;
  // reference to a next element, see #top_list
  util::ManagedPtr<DisiWrapper> next;
  // An approximation of the iterator, or the iterator itself if it does not
  // support two-phase iteration
  util::ManagedPtr<DocIdSetIterator> approximation;
  // A two-phase view of the iterator, or null if the iterator does not support
  // two-phase iteration
  util::ManagedPtr<TwoPhaseIterator> two_phase_iterator;
  // For WANDScorer
  int64_t scaled_max_score;
  // For MaxScoreBulkScorer
  float max_window_score;
};  // DisiWrapper


}  // lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_DISIWRAPPER_HPP_
