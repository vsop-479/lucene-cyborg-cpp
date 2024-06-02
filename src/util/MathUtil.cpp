#include <cmath>
#include "MathUtil.hpp"

using lucene::cyborg::util::MathUtil;

//
// MathUtil
//

double MathUtil::sum_upper_bound(double sum, int32_t num_values) {
  if (num_values <= 2) {
    // When there are only two clauses, the sum is always the same regardless
    // of the order.
    return sum;
  }

  // The error of sums depends on the order in which values are summed up. In
  // order to avoid this issue, we compute an upper bound of the value that
  // the sum may take. If the max relative error is b, then it means that two
  // sums are always within 2*b of each other.
  // For conjunctions, we could skip this error factor since the order in which
  // scores are summed up is predictable, but in practice, this wouldn't help
  // much since the delta that is introduced by this error factor is usually
  // cancelled by the float cast.
  const double b = sum_relative_error_bound(num_values);
  return 2 * b * sum + sum;
}

double MathUtil::sum_relative_error_bound(int32_t num_values) {
  const static double U = std::scalbn(1.0, -52);

  if (num_values <= 1) {
    return 0;
  }

  // u = unit roundoff in the paper, also called machine precision or machine epsilon
  return (num_values - 1) * U;
}
