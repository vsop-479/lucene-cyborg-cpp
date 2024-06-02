#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_TYPEINFO_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_TYPEINFO_HPP_

namespace lucene::cyborg::util {

struct TypeInfo {
  template<typename T>
  static const TypeInfo *get_type_info(const char *type_desc) noexcept {
    static TypeInfo TYPE_INFO{type_desc};
    return &TYPE_INFO;
  }

  const char *type_desc;
};  // TypeInfo

using TypeInfoPtr = const TypeInfo *;

}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_TYPEINFO_HPP_
