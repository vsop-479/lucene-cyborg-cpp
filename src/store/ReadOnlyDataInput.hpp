#ifndef LUCENE_CYBORG_CPP_SRC_STORE_READONLYDATAINPUT_HPP_
#define LUCENE_CYBORG_CPP_SRC_STORE_READONLYDATAINPUT_HPP_

#include <cstdint>
#include "../util/PointerTypes.hpp"

namespace lucene::cyborg::store {

struct ReadOnlyDataInput {
  util::ReadOnlyPtr<uint8_t> org_data;
  util::ReadOnlyPtr<uint8_t> data;
  int64_t offset;
  int64_t length;
};  // ReadOnlyDataInput



}  // lucene::cyborg::store

#endif //LUCENE_CYBORG_CPP_SRC_STORE_READONLYDATAINPUT_HPP_
