#ifndef LUCENE_CYBORG_CPP_SRC_CODECS_NORMSPRODUCER_HPP_
#define LUCENE_CYBORG_CPP_SRC_CODECS_NORMSPRODUCER_HPP_

#include <memory>
#include "../index/NumericDocValues.hpp"

namespace lucene::cyborg::codecs {

struct NormsProducer {
  virtual ~NormsProducer() = default;

  virtual util::ManagedPtr<index::NumericDocValues> get_norms(util::ManagedPtr<util::LinearAllocator> allocator,
                                                              util::UseOnlyPtr<index::FieldInfo> field) = 0;

  virtual void close() = 0;
};  // NormsProducer

using NormsProducerPtr = std::shared_ptr<NormsProducer>;

}  // namespace lucene::cyborg::codecs

#endif //LUCENE_CYBORG_CPP_SRC_CODECS_NORMSPRODUCER_HPP_
