#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_VERSION_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_VERSION_HPP_

#include <cstdint>
#include "PointerTypes.hpp"

namespace lucene::cyborg::util {

struct Version {
  static const Version UNKNOWN;

  static const Version LUCENE_9_8_0;

  /**
   * <b>WARNING</b>: if you use this setting, and then upgrade to a newer release of Lucene, sizable
   * changes may happen. If backwards compatibility is important then you should instead explicitly
   * specify an actual version.
   *
   * <p>If you use this constant then you may need to <b>re-index all of your documents</b> when
   * upgrading Lucene, as the way text is indexed may have changed. Additionally, you may need to
   * <b>re-test your entire application</b> to ensure it behaves as expected, as some defaults may
   * have changed and may break functionality fst_in your application.
   */
  static ReadOnlyPtr<Version> LATEST;

  /**
   * Constant for the minimal supported major version of an index. This version is defined by the
   * version that initially created the index.
   */
  static const int32_t MIN_SUPPORTED_MAJOR;

  Version(int32_t major, int32_t minor, int32_t bugfix)
      : major(major),
        minor(minor),
        bugfix(bugfix) {
    if (major > 255 || major < 0) {
      // TODO throw
    }
    if (minor > 255 || minor < 0) {
      // TODO throw
    }
    if (bugfix > 255 || bugfix < 0) {
      // TODO throw
    }

    encoded_value = (major << 18) | (minor << 10) | (bugfix << 2);
  }

  static Version from_bits(int32_t major, int32_t minor, int32_t bugfix) {
    return Version{major, minor, bugfix};
  }

  [[nodiscard]] bool on_or_after(const Version &other) const noexcept {
    return encoded_value >= other.encoded_value;
  }

  bool operator==(const Version &other) const noexcept {
    return major == other.major
        && minor == other.minor
        && bugfix == other.bugfix;
  }

  int32_t major;
  int32_t minor;
  int32_t bugfix;
  int32_t encoded_value;
};  // Version



}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_VERSION_HPP_
