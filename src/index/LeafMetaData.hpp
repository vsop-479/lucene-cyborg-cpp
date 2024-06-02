#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_LEAFMETADATA_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_LEAFMETADATA_HPP_

#include <cstdint>
#include "../util/Version.hpp"

namespace lucene::cyborg::index {

struct LeafMetaData {
  /** Expert: Sole constructor. Public for use by custom {@link LeafReader} impls. */
  LeafMetaData(int32_t _created_version_major, util::Version _min_version)
      : created_version_major(_created_version_major),
        min_version(_min_version) {
    if (_created_version_major > util::Version::LATEST->major) [[unlikely]] {
      // TODO
      throw 13;
    }

    if (_created_version_major < 6) [[unlikely]] {
      // TODO
      throw 13;
    }

    if (_created_version_major >= 7 && _min_version == util::Version::UNKNOWN) [[unlikely]] {
      // TODO
      throw 13;
    }
  }

  /**
   * Get the Lucene version that created this index. This can be used to implement backward
   * compatibility on top of the codec API. A return value of {@code 6} indicates that the created
   * version is unknown.
   */
  int32_t created_version_major;

  /**
   * Return the minimum Lucene version that contributed documents to this index, or {@code null} if
   * this information is not available.
   */
  util::Version min_version;
};  // LeafMetaData



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_LEAFMETADATA_HPP_
