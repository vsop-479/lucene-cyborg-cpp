#ifndef LUCENE_CYBORG_CPP_SRC_INDEX_SLOWIMPACTSENUM_HPP_
#define LUCENE_CYBORG_CPP_SRC_INDEX_SLOWIMPACTSENUM_HPP_

#include "ImpactsEnum.hpp"
#include "../util/PointerTypes.hpp"

namespace lucene::cyborg::index {

struct SlowImpactsDummyImpacts final : public Impacts {
  SlowImpactsDummyImpacts();

  int32_t get_doc_id_up_to(int32_t level) final;

  util::TypedVector<Impact> get_impacts(int32_t level) final;
};  // SlowImpactsDummyImpacts



class SlowImpactsEnum final : public ImpactsEnum {
 public:
  explicit SlowImpactsEnum(util::ManagedPtr<PostingsEnum> delegate);

  void advance_shallow(int32_t target) final {
    // No-op
  }

  int32_t get_doc_id() final;

  int get_freq() final;

  int32_t next_doc() final;

  int64_t get_cost() final;

  int32_t advance(int32_t target) final;

  util::ManagedPtr<SlowImpactsDummyImpacts> get_impacts() override;

 private:
  util::ManagedPtr<PostingsEnum> delegate;
};  // SlowImpactsEnum



}  // lucene::cyborg::index

#endif //LUCENE_CYBORG_CPP_SRC_INDEX_SLOWIMPACTSENUM_HPP_
