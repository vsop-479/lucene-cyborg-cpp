#ifndef UNTITLED_INCLUDE_JAVA_JNI_COMMON_JSTRING_HPP_
#define UNTITLED_INCLUDE_JAVA_JNI_COMMON_JSTRING_HPP_

#include <jni.h>
#include <string_view>
#include <cassert>

namespace lucene::cyborg::jni {

class JString {
 public:
  explicit JString(JNIEnv *_env, jstring _jstr)
      : env(_env),
        jstr(_jstr),
        jstr_view() {
    assert(_env);
  }

  JString(const JString &) = delete;

  JString(JString &&other) noexcept
      : env(other.env),
        jstr(other.jstr),
        jstr_view(other.jstr_view) {
    other.jstr_view = {};
  }

  JString &operator=(const JString &) = delete;

  JString &operator=(JString &&other) noexcept {
    if (this != &other) [[likely]] {
      env = other.env;
      jstr = other.jstr;
      jstr_view = other.jstr_view;
      other.jstr_view = {};
    }

    return *this;
  }

  const std::string_view &get_string() {
    if (jstr_view.data()) [[likely]] {
      return jstr_view;
    }

    // The given Java string instance can be null
    if (jstr) {
      auto native_str = env->GetStringUTFChars(jstr, nullptr);
      jstr_view = std::string_view(native_str);
    }

    return jstr_view;
  }

  ~JString() {
    if (jstr_view.data()) {
      env->ReleaseStringUTFChars(jstr, jstr_view.data());
    }
  }

 private:
  JNIEnv *env;
  jstring jstr;
  std::string_view jstr_view;
};  // JString



}  // namespace lucene::cyborg::jni

#endif //UNTITLED_INCLUDE_JAVA_JNI_COMMON_JSTRING_HPP_
