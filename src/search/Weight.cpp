#include <limits>
#include "Weight.hpp"
#include "DefaultBulkScorer.hpp"

using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::search::BulkScorer;
using lucene::cyborg::search::ScorerSupplier;
using lucene::cyborg::search::Weight;
using lucene::cyborg::util::Bits;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::UseOnlyPtr;

//
// Weight
//

ManagedPtr<ScorerSupplier> Weight::get_scorer_supplier(
    UseOnlyPtr<LeafReaderContext> leaf_reader_context) {
  auto scorer = get_scorer(leaf_reader_context);
  return srch_context->linear_allocator->allocate_object<DefaultScorerSupplier>(scorer);
}

ManagedPtr<BulkScorer> Weight::get_bulk_scorer(
    UseOnlyPtr<LeafReaderContext> leaf_reader_context,
    UseOnlyPtr<LeafCollector> leaf_collector,
    UseOnlyPtr<Bits> live_docs) {
  auto scorer_supplier = get_scorer_supplier(leaf_reader_context);
  if (scorer_supplier) {
    scorer_supplier->set_top_level_scoring_clause();
    auto scorer = scorer_supplier->get(std::numeric_limits<int64_t>::max());
    return srch_context->linear_allocator->allocate_object<DefaultBulkScorer>(
        scorer, scorer->get_iterator(), scorer->two_phase_iterator(), leaf_collector, live_docs);
  } else {
    // No docs match
    return nullptr;
  }
}
