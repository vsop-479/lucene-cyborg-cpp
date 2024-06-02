#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_FIELDINFO_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_FIELDINFO_HPP_

#include <memory>
#include <string>
#include <unordered_map>
#include "IndexOptions.hpp"
#include "IndexFileNames.hpp"
#include "DocValuesType.hpp"
#include "VectorEncoding.hpp"
#include "VectorSimilarityFunction.hpp"

namespace lucene::cyborg::index {

struct FieldInfo {
  FieldInfo(
      std::string _name,
      int32_t _number,
      bool _store_term_vector,
      bool _omit_norms,
      bool _store_payloads,
      IndexOptions _index_options,
      DocValuesType _doc_values,
      int64_t _dv_gen,
      std::unordered_map<std::string, std::string> _attributes,
      int32_t _point_dimension_count,
      int32_t _point_index_dimension_count,
      int32_t _point_num_bytes,
      int32_t _vector_dimension,
      VectorEncoding _vector_encoding,
      VectorSimilarityFunction _vector_similarity_function,
      bool _soft_deletes_field);

  void check_consistency() const;

  bool has_vectors() const noexcept {
    return store_term_vector;
  }

  bool has_norms() const noexcept {
    return index_options != IndexOptions::NONE && !omit_norms;
  }

  /** Returns true if any payloads exist for this field. */
  bool has_payloads() const noexcept {
    return store_payloads;
  }

  /** Returns whether any (numeric) vector values exist for this field */
  bool has_vector_values() const noexcept {
    return vector_dimension > 0;
  }

  bool is_soft_deletes_field() const noexcept {
    return soft_deletes_field;
  }

  /** Field's name */
  std::string name;
  /** Internal field number */
  int32_t number;
  DocValuesType doc_values_type = DocValuesType::NONE;
  // True if any document indexed term vectors
  bool store_term_vector;
  bool omit_norms; // omit norms associated with indexed fields
  IndexOptions index_options;
  bool store_payloads; // whether this field stores payloads together with term positions
  std::unordered_map<std::string, std::string> attributes;
  int64_t dv_gen;

  /**
   * If both of these are positive it means this field indexed points (see {@link
   * org.apache.lucene.codecs.PointsFormat}).
   */
  int32_t point_dimension_count;
  int32_t point_index_dimension_count;
  int32_t point_num_bytes;
  // if it is a positive value, it means this field indexes vectors
  int32_t vector_dimension;
  VectorEncoding vector_encoding;
  VectorSimilarityFunction vector_similarity_function;
  // whether this field is used as the soft-deletes field
  bool soft_deletes_field;
};  // FieldInfo

using FieldInfoPtr = std::shared_ptr<FieldInfo>;

}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_FIELDINFO_HPP_
