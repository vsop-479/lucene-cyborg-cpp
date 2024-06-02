#ifndef LUCENE_CYBORG_CPP_SRC_UTIL_IOUTILS_HPP_
#define LUCENE_CYBORG_CPP_SRC_UTIL_IOUTILS_HPP_

namespace lucene::cyborg::util {

class IOUtils {
 public:
  template<typename ResourcesVec>
  static void close_while_handling_exception(ResourcesVec &resources_vec) {
    // TODO
  }
};

}  // lucene::cyborg::util

#endif //LUCENE_CYBORG_CPP_SRC_UTIL_IOUTILS_HPP_
