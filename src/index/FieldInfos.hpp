#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_FIELDINFOS_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_FIELDINFOS_HPP_

#include <memory>
#include <vector>
#include <unordered_map>
#include "FieldInfo.hpp"

namespace lucene::cyborg::index {

class FieldInfos {
 public:
  explicit FieldInfos(std::vector<FieldInfoPtr> _field_infos);

  FieldInfoPtr &field_info(const int32_t field_no);

  FieldInfoPtr &field_info(std::string_view field_name);

  bool has_freq;
  bool has_postings;
  bool has_prox;
  bool has_payloads;
  bool has_offsets;
  bool has_vectors;
  bool has_norms;
  bool has_doc_values;
  bool has_point_values;
  bool has_vector_values;
  std::string soft_deletes_field;

  // Used only by field_info(int)
  std::vector<FieldInfoPtr> by_number;

  std::unordered_map<std::string_view, FieldInfoPtr> by_name;
  std::vector<FieldInfoPtr> values;

 private:
  std::vector<std::unique_ptr<char[]>> names;
};  // FieldInfos

using FieldInfosPtr = std::shared_ptr<FieldInfos>;

}  // lucene::cyborg::index


#endif //LUCENE_CYBORG_CPP_SRC_INDEX_FIELDINFOS_HPP_
