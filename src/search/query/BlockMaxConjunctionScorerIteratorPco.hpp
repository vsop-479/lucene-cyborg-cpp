#ifndef LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BLOCKMAXCONJUNCTIONSCORERITERATORPCO_HPP_
#define LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BLOCKMAXCONJUNCTIONSCORERITERATORPCO_HPP_

#include "../Scorer.hpp"
#include "../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "BlockMaxConjunctionScorer.hpp"

namespace lucene::cyborg::search {

template <typename LeadDocIdSetIterator>
class BlockMaxConjunctionScorerIteratorPco final : public DocIdSetIterator {
 public:
  static uint64_t PCO_INDEX;

  BlockMaxConjunctionScorerIteratorPco(util::ManagedPtr<LeadDocIdSetIterator> _lead,
                                       util::ManagedPtr<BlockMaxConjunctionScorer> _scorer)
      : DocIdSetIterator(),
        max_score(),
        up_to(-1),
        lead(_lead),
        scorer(_scorer) {
    pco_type_index = PCO_INDEX;
  }

  int64_t get_cost() final {
    return lead->get_cost();
  }

  int32_t advance(int32_t target) final {
    return do_next(lead->advance(advance_target(target)));
  }

  int32_t get_doc_id() final {
    return lead->get_doc_id();
  }

  int32_t next_doc() final {
    return advance(get_doc_id() + 1);
  }

 private:
  void move_to_next_block(int32_t target) {
    up_to = scorer->advance_shallow(target);
    max_score = scorer->get_max_score(up_to);
  }

  int32_t advance_target(int32_t target) {
    if (target > up_to) {
      move_to_next_block(target);
    }

    const auto min_score = scorer->min_score;

    while (true) {
      assert(up_to >= target);

      if (max_score >= min_score) {
        return target;
      }

      if (up_to == NO_MORE_DOCS) {
        return NO_MORE_DOCS;
      }

      target = up_to + 1;

      move_to_next_block(target);
    }  // End while
  }

  int32_t do_next(int32_t target) {
    while (true) {
      LOOP_START:
      assert(target == lead->get_doc_id());

      if (target == NO_MORE_DOCS) {
        return NO_MORE_DOCS;
      }

      if (target > up_to) {
        // This check is useful when scorers return information about blocks
        // that do not actually have any matches. Otherwise `target` will always
        // be in the current block already since it is always the result of
        // lead.advance(advance_target(some_doc_id))
        const int32_t next_target = advance_target(target);
        if (next_target != target) {
          target = lead->advance(next_target);
          continue;
        }
      }  // End if

      assert(target <= up_to);

      // then find agreement with other iterators
      for (int32_t i = 1; i < scorer->approximations.size(); ++i) {
        auto other = scorer->approximations[i];
        // other.doc may already be equal to doc if we "continued advanceHead"
        // on the previous iteration and the advance on the lead scorer exactly matched.
        if (other->get_doc_id() < target) {
          const int32_t next = other->advance(target);

          if (next > target) {
            // iterator beyond the current doc - advance lead and continue to the new highest doc.
            target = lead->advance(advance_target(next));
            goto LOOP_START;
          }
        }

        assert(other->get_doc_id() == target);
      }  // End for

      // success - all iterators are on the same doc and the score is competitive
      return target;
    }  // End while
  }

  float max_score;
  int32_t up_to = -1;
  util::ManagedPtr<LeadDocIdSetIterator> lead;
  util::ManagedPtr<BlockMaxConjunctionScorer> scorer;
};  // BlockMaxConjunctionScorerIterator



}  // namespace lucene::cyborg::search

#endif //LUCENE_CYBORG_CPP_SRC_SEARCH_QUERY_BLOCKMAXCONJUNCTIONSCORERITERATORPCO_HPP_
