#include "FieldInfo.hpp"

using lucene::cyborg::index::FieldInfo;
using lucene::cyborg::index::IndexOptions;
using lucene::cyborg::index::DocValuesType;
using lucene::cyborg::index::VectorEncoding;
using lucene::cyborg::index::VectorSimilarityFunction;

//
// FieldInfo
//

FieldInfo::FieldInfo(std::string _name,
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
                     bool _soft_deletes_field)
    : name(std::move(_name)),
      number(_number),
      doc_values_type(_doc_values),
      store_term_vector(_index_options != IndexOptions::NONE && _store_term_vector),
      omit_norms(_index_options != IndexOptions::NONE && _omit_norms),
      index_options(_index_options),
      store_payloads(_index_options != IndexOptions::NONE && _store_payloads),
      attributes(std::move(_attributes)),
      dv_gen(_dv_gen),
      point_dimension_count(_point_dimension_count),
      point_index_dimension_count(_point_index_dimension_count),
      point_num_bytes(_point_num_bytes),
      vector_dimension(_vector_dimension),
      vector_encoding(_vector_encoding),
      vector_similarity_function(_vector_similarity_function),
      soft_deletes_field(_soft_deletes_field) {
  check_consistency();
}

void FieldInfo::check_consistency() const {
  if (index_options != IndexOptions::NONE) {
    if (index_options < IndexOptions::DOCS_AND_FREQS_AND_POSITIONS && store_payloads) [[unlikely]] {
      // TODO
      throw 13;
    }
  } else {
    if (store_term_vector) [[unlikely]] {
      // TODO
      // throw new IllegalArgumentException(
      //     "non-indexed field '" + name + "' cannot store term vectors");
      throw 13;
    }
    if (store_payloads) [[unlikely]] {
      // throw new IllegalArgumentException(
      //     "non-indexed field '" + name + "' cannot store payloads");
      throw 13;
    }
    if (omit_norms) [[unlikely]] {
      // throw new IllegalArgumentException("non-indexed field '" + name + "' cannot omit norms");
      throw 13;
    }
  }  // End if

  if (dv_gen != -1 && doc_values_type == DocValuesType::NONE) [[unlikely]] {
    // throw new IllegalArgumentException("non-indexed field '" + name + "' cannot omit norms");
    // throw new IllegalArgumentException(
    //     "field '"
    //         + name
    //         + "' cannot have a docvalues update generation without having docvalues");
    // TODO
    throw 13;
  }

  if (point_dimension_count < 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "pointDimensionCount must be >= 0; got "
    //         + pointDimensionCount
    //         + " (field: '"
    //         + name
    //         + "')");
    // TODO
    throw 13;
  }

  if (point_index_dimension_count < 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "pointIndexDimensionCount must be >= 0; got "
    //         + pointIndexDimensionCount
    //         + " (field: '"
    //         + name
    //         + "')");
    // TODO
    throw 13;
  }

  if (point_num_bytes < 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "pointNumBytes must be >= 0; got " + pointNumBytes + " (field: '" + name + "')");
    // TODO
    throw 13;
  }

  if (point_dimension_count != 0 && point_num_bytes == 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "pointNumBytes must be > 0 when pointDimensionCount="
    //         + pointDimensionCount
    //         + " (field: '"
    //         + name
    //         + "')");
    throw 13;
  }

  if (point_index_dimension_count != 0 && point_dimension_count == 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "pointIndexDimensionCount must be 0 when pointDimensionCount=0"
    //         + " (field: '"
    //         + name
    //         + "')");
    throw 13;
  }

  if (point_num_bytes != 0 && point_dimension_count == 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "pointDimensionCount must be > 0 when pointNumBytes="
    //         + pointNumBytes
    //         + " (field: '"
    //         + name
    //         + "')");
    throw 13;
  }

  if (vector_dimension < 0) [[unlikely]] {
    // throw new IllegalArgumentException(
    //     "vectorDimension must be >=0; got " + vectorDimension + " (field: '" + name + "')");
    throw 13;
  }
}
