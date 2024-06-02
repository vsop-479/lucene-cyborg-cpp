#ifndef LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COMMON_JPRIMITIVEARRAY_HPP_
#define LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COMMON_JPRIMITIVEARRAY_HPP_

#include <jni.h>
#include <type_traits>
#include <variant>

namespace lucene::cyborg::jni {

template<typename T>
concept JPrimitiveType = std::is_same_v<T, jboolean>
    || std::is_same_v<T, jbyte>
    || std::is_same_v<T, jchar>
    || std::is_same_v<T, jshort>
    || std::is_same_v<T, jint>
    || std::is_same_v<T, jlong>
    || std::is_same_v<T, jfloat>
    || std::is_same_v<T, jdouble>;

template<JPrimitiveType JPrimitive, typename JArray>
class JPrimitiveArray {
 public:
  JPrimitiveArray(JNIEnv *_env, JArray _jarray, JPrimitive *_primitive_jarray, jint _mode)
      : env(_env),
        jarray(_jarray),
        primitive_jarray(_primitive_jarray),
        mode(_mode) {
  }

  JPrimitiveArray(const JPrimitiveArray &) = delete;

  JPrimitiveArray(JPrimitiveArray &&other) noexcept
      : env(other.env),
        jarray(other.jarray),
        primitive_jarray(other.primitive_jarray),
        mode(other.mode) {
    other.jarray = nullptr;
  }

  JPrimitiveArray &operator=(const JPrimitiveArray &) = delete;

  JPrimitiveArray &operator=(JPrimitiveArray &&other) noexcept {
    if (this != &other) [[likely]] {
      env = other.env;
      jarray = other.jarray;
      primitive_jarray = other.primitive_jarray;
      mode = other.mode;
      other.jarray = nullptr;
    }

    return *this;
  }

  ~JPrimitiveArray() {
    // Given Java array can be null
    if (jarray) {
      if constexpr (std::is_same_v<JPrimitive, jboolean>) {
        env->ReleaseBooleanArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jbyte>) {
        env->ReleaseByteArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jchar>) {
        env->ReleaseCharArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jshort>) {
        env->ReleaseShortArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jint>) {
        env->ReleaseIntArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jlong>) {
        env->ReleaseLongArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jfloat>) {
        env->ReleaseFloatArrayElements(jarray, primitive_jarray, mode);
      } else if constexpr (std::is_same_v<JPrimitive, jdouble>) {
        env->ReleaseDoubleArrayElements(jarray, primitive_jarray, mode);
      }
    }  // End if
  }

 private:
  JNIEnv *env;
  JArray jarray;
  JPrimitive *primitive_jarray;
  jint mode;
};  // JPrimitiveArray



using JPrimitiveArrayVariant = std::variant<
    JPrimitiveArray<jboolean, jbooleanArray>,
    JPrimitiveArray<jbyte, jbyteArray>,
    JPrimitiveArray<jchar, jcharArray>,
    JPrimitiveArray<jshort, jshortArray>,
    JPrimitiveArray<jint, jintArray>,
    JPrimitiveArray<jlong, jlongArray>,
    JPrimitiveArray<jfloat, jfloatArray>,
    JPrimitiveArray<jdouble, jdoubleArray>>;

}  // namespace lucene::cyborg::jni

#endif //LUCENE_CYBORG_CPP_SRC_JAVA_JNI_COMMON_JPRIMITIVEARRAY_HPP_
