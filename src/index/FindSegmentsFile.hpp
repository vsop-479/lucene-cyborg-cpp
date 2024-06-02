#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_FINDSEGMENTSFILE_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_FINDSEGMENTSFILE_HPP_

#include <cstdint>
#include <algorithm>
#include <optional>
#include "../store/MMapDirectory.hpp"
#include "IndexFileNames.hpp"
#include "SegmentInfos.hpp"

namespace lucene::cyborg::index {

template<typename T>
class FindSegmentsFile {
 public:
  explicit FindSegmentsFile(store::MMapDirectoryPtr _directory)
      : directory(std::move(_directory)) {
  }

  virtual ~FindSegmentsFile() = default;

  T run() {
    int64_t last_gen = -1;
    int64_t gen = -1;
    std::optional<std::exception> exception;

    // Loop until we succeed fst_in calling doBody() without
    // hitting an Exception. An Exception most likely
    // means an IW deleted our commit while opening
    // the time it took us to load the now-old infos files
    // (and segments files). It's also possible it's a
    // true error (corrupt index).  To distinguish these,
    // on each retry we must see "forward progress" on
    // which generation we are trying to load.  If we
    // don't, then the original error is real and we throw
    // it.

    while (true) {
      last_gen = gen;
      std::vector<std::string> files = directory->list_all();
      std::vector<std::string> files2 = directory->list_all();
      std::sort(files.begin(), files.end());
      std::sort(files2.begin(), files2.end());

      if (files != files2) {
        // list_all() is weakly consistent, this means we hit "concurrent modification exception"
        continue;
      }
      gen = SegmentInfos::get_last_commit_generation(files);

      // TODO
      /*
      if (infoStream != null) {
        message("directory listing gen=" + gen);
      }
      */

      if (gen == -1) {
        // TODO
        // throw new IndexNotFoundException(
        //     "no segments* file found fst_in " + directory + ": files: " + Arrays.toString(files));
        throw 13;
      } else if (gen > last_gen) {
        std::string segment_file_name =
            IndexFileNames::file_name_from_generation(IndexFileNames::SEGMENTS, "", gen);

        try {
          T to_return = doBody(segment_file_name);
          // TODO
          // if (infoStream != null) {
          //   message("success on " + segmentFileName);
          // }
          return to_return;
        } catch (const std::exception &err) {
          if (!exception) {
            exception = err;
          }

          // TODO
          // if (infoStream != null) {
          //   message(
          //       "primary Exception on '"
          //           + segmentFileName
          //           + "': "
          //           + err
          //           + "'; will retry: gen = "
          //           + gen);
          // }
        }
      } else {
        if (exception) {
          throw std::exception{std::move(*exception)};
        } else {
          // TODO unexpected error
        }
      }
    }  // End while
  }

  virtual T doBody(const std::string &segment_file_name) = 0;

  store::MMapDirectoryPtr directory;
};  // FindSegmentsFile



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_FINDSEGMENTSFILE_HPP_
