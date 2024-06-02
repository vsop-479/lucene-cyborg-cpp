#include "FieldInfos.hpp"
#include <cstring>

using lucene::cyborg::index::FieldInfos;
using lucene::cyborg::index::FieldInfoPtr;

//
// FieldInfos
//

FieldInfos::FieldInfos(std::vector<FieldInfoPtr> _field_infos)
    : has_freq(),
      has_postings(),
      has_prox(),
      has_payloads(),
      has_offsets(),
      has_vectors(),
      has_norms(),
      has_doc_values(),
      has_point_values(),
      has_vector_values(),
      soft_deletes_field(),
      by_number(),
      by_name(),
      values(),
      names() {
  bool has_vectors = false;
  bool has_postings = false;
  bool has_prox = false;
  bool has_payloads = false;
  bool has_offsets = false;
  bool has_freq = false;
  bool has_norms = false;
  bool has_doc_values = false;
  bool has_point_values = false;
  bool has_vector_values = false;
  std::string soft_deletes_field;

  int32_t size = 0; // number of elements fst_in byNumberTemp, number of used array slots
  std::vector<FieldInfoPtr> by_number_temp(10); // initial array capacity of 10
  for (auto &info : _field_infos) {
    if (info->number < 0) {
      // throw new IllegalArgumentException(
      //    "illegal field number: " + info.number + " for field " + info.name);
      // TODO
      throw 13;
    }
    size = info->number >= size ? info->number + 1 : size;
    if (info->number >= by_number_temp.size()) { // grow array
      by_number_temp.resize(info->number + 1);
    }
    auto previous = by_number_temp[info->number];
    if (previous) {
      // throw new IllegalArgumentException(
      //     "duplicate field numbers: "
      //         + previous.name
      //         + " and "
      //         + info.name
      //         + " have: "
      //         + info.number);
      // TODO
      throw 13;
    }
    by_number_temp[info->number] = info;

    if (by_name.contains(info->name)) {
      // throw new IllegalArgumentException(
      //     "duplicate field names: "
      //         + previous.number
      //         + " and "
      //         + info.number
      //         + " have: "
      //         + info.name);
      // TODO
      throw 13;
    }

    names.emplace_back(std::make_unique<char[]>(info->name.size()));
    std::memcpy(names.back().get(), info->name.data(), info->name.size());
    by_name.emplace(std::string_view(names.back().get(), info->name.size()), info);

    has_vectors |= info->has_vectors();
    has_postings |= info->index_options != IndexOptions::NONE;
    has_prox |= info->index_options >= IndexOptions::DOCS_AND_FREQS_AND_POSITIONS;
    has_freq |= info->index_options != IndexOptions::DOCS;
    has_offsets |=
        info->index_options >= IndexOptions::DOCS_AND_FREQS_AND_POSITIONS_AND_OFFSETS;
    has_norms |= info->has_norms();
    has_doc_values |= info->doc_values_type != DocValuesType::NONE;
    has_payloads |= info->has_payloads();
    has_point_values |= (info->point_dimension_count != 0);
    has_vector_values |= (info->vector_dimension != 0);
    if (info->is_soft_deletes_field()) {
      if (!soft_deletes_field.empty() && soft_deletes_field != info->name) {
        // throw new IllegalArgumentException(
        //     "multiple soft-deletes fields [" + info.name + ", " + softDeletesField + "]");
        // TODO
        throw 13;
      }
      soft_deletes_field = info->name;
    }
  }  // End for

  this->has_vectors = has_vectors;
  this->has_postings = has_postings;
  this->has_prox = has_prox;
  this->has_payloads = has_payloads;
  this->has_offsets = has_offsets;
  this->has_freq = has_freq;
  this->has_norms = has_norms;
  this->has_doc_values = has_doc_values;
  this->has_point_values = has_point_values;
  this->has_vector_values = has_vector_values;
  this->soft_deletes_field = soft_deletes_field;

  std::vector<FieldInfoPtr> values_temp;
  by_number.resize(size);
  for (int32_t i = 0; i < size; i++) {
    by_number[i] = by_number_temp[i];
    if (by_number_temp[i]) {
      values_temp.emplace_back(by_number_temp[i]);
    }
  }
  values = std::move(values_temp);
}

FieldInfoPtr EMPTY_FIELD_INFO{};

FieldInfoPtr &FieldInfos::field_info(const int32_t field_no) {
  if (field_no >= 0) [[likely]] {
    if (field_no >= by_number.size()) {
      return EMPTY_FIELD_INFO;
    }

    return by_number[field_no];
  }

  // TODO
  throw 13;
}

FieldInfoPtr &FieldInfos::field_info(std::string_view field_name) {
  auto it = by_name.find(field_name);
  if (it != by_name.end()) {
    return it->second;
  }

  return EMPTY_FIELD_INFO;
}
