#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_POINTERTYPES_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_POINTERTYPES_HPP_

#include <cstdint>
#include <string_view>

namespace lucene::cyborg::util {

template<typename T>
using ReadOnlyPtr = const T *;

template<typename T>
using UseOnlyPtr = T *;

template<typename T>
using ManagedPtr = T *;

template<typename T>
struct TypedVector {
  ManagedPtr<T> vec;
  int32_t len;
};  // TypedVector

template<typename T>
struct TypedReadOnlyVector {
  ReadOnlyPtr<T> vec;
  int32_t len;
};  // TypedReadOnlyVector

using ManagedString = TypedVector<const char>;

using OverwriteBytes = char *;

}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_POINTERTYPES_HPP_
