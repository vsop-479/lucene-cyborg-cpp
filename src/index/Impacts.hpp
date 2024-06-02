#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTS_HPP_

#include <cstdint>
#include "../util/PointerTypes.hpp"

namespace lucene::cyborg::index {

struct Impact {
  int32_t freq;
  int64_t norm;
};  // Impact


class Impacts {
 public:
  explicit Impacts(util::ReadOnlyPtr<int32_t> _num_levels)
      : num_levels(_num_levels) {
  }

  [[nodiscard]] int32_t get_num_levels() const noexcept {
    return *num_levels;
  }

  virtual int32_t get_doc_id_up_to(int32_t level) = 0;

  virtual util::TypedVector<Impact> get_impacts(int32_t level) = 0;

 protected:
  util::ReadOnlyPtr<int32_t> num_levels;
};  // Impacts



}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_IMPACTS_HPP_
