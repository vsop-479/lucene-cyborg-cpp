#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COMMON_JNIRESOURCES_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COMMON_JNIRESOURCES_HPP_

#include <vector>
#include "../../../util/LinearAllocatorAdaptorForSTL.hpp"
#include "JString.hpp"
#include "JPrimitiveArray.hpp"

namespace lucene::cyborg::jni {

class JNIResources {
 public:
  explicit JNIResources(util::ManagedPtr<util::LinearAllocator> _allocator)
      : jstrings(util::LinearAllocatorAdapterForSTL<JString>(_allocator)),
        jarrays(util::LinearAllocatorAdapterForSTL<JPrimitiveArrayVariant>(_allocator)) {
  }

  void add_jstring(JString &&jstr) {
    jstrings.emplace_back(std::move(jstr));
  }

  void add_jarray(JPrimitiveArrayVariant &&jarray) {
    jarrays.emplace_back(std::move(jarray));
  }

 private:
  util::LVector<JString> jstrings;
  util::LVector<JPrimitiveArrayVariant> jarrays;
};  // JNIResources



}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COMMON_JNIRESOURCES_HPP_
