#include <numeric>
#include <algorithm>
#include "ScorerUtil.hpp"

using lucene::cyborg::search::ScorerUtil;

//
// ScorerUtil
//

int64_t ScorerUtil::cost_with_min_should_match(int64_t *costs,
                                               int32_t num_scorers,
                                               int32_t min_should_match) noexcept {
  if (min_should_match > 1) {
    const int32_t num_least_iterators = num_scorers - min_should_match + 1;
    std::nth_element(costs, costs + num_least_iterators, costs + num_scorers);
    return std::reduce(costs, costs + num_least_iterators);
  }
  return std::reduce(costs, costs + num_scorers);
}
