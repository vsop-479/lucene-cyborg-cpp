#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_FIELDS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_FIELDS_HPP_

#include <memory>
#include <vector>
#include <string>
#include <cstdint>
#include "Terms.hpp"
#include "../util/PointerTypes.hpp"
#include "../search/SearchContext.hpp"

namespace lucene::cyborg::index {

class Fields {
 public:
  virtual ~Fields() = default;

  [[nodiscard]] const std::vector<std::string> &get_field_names() const noexcept {
    return field_names;
  }

  virtual int32_t size() = 0;

  virtual util::ManagedPtr<index::Terms> terms(
      std::string_view field_name,
      util::ManagedPtr<lucene::cyborg::search::SearchContext> srch_context) = 0;

 protected:
  std::vector<std::string> field_names;
};  // Fields

using FieldsPtr = std::shared_ptr<Fields>;

}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_FIELDS_HPP_
