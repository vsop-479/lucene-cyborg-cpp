# lucene-cyborg-cpp [alpha version]

This repository ported Lucene 9.8.0 search functionality in C++.
At the moment, not all the features are available in this repository.

Example:
```
std::filesystem::path index_path = ...

MMapDirectoryPtr dir = std::make_shared<MMapDirectory>(index_path.string());
auto index_reader = MMapDirectoryReader::open(dir);
std::cout << "MMap directory creation." << std::endl;

const auto sub_readers_size = index_reader->sub_readers.size();
std::cout << "sub_readers_size(#segments): " << sub_readers_size << std::endl;

auto &reader = index_reader->sub_readers[0];
SearchContext search_ctx{};
ManagedPtr<Terms> terms = reader->segment_core_readers->fields->terms("text", &search_ctx);
const int64_t num_terms = terms->size();
std::cout << "num_terms: " << num_terms << std::endl;

const int64_t doc_count_body = terms->get_doc_count();
std::cout << "doc_count_body: " << doc_count_body << std::endl;

auto max_terms_in_body = terms->get_max();
std::cout << "max_terms_in_body: " << max_terms_in_body << std::endl;
std::cout << "the size of max_terms_in_body: " << max_terms_in_body.size() << std::endl;

auto min_terms_in_body = terms->get_min();
std::cout << "min_terms_in_body: " << min_terms_in_body << std::endl;
std::cout << "the size of min_terms_in_body: " << min_terms_in_body.size() << std::endl;

int64_t sum_total_term_freq = terms->get_sum_total_term_freq();
std::cout << "sum_total_term_freq: " << sum_total_term_freq << std::endl;

// Create linear allocator.
LinearAllocator allocator{};

// Create boolean query builder.
BooleanQueryBuilder builder{&allocator};
builder.add_clause(BooleanClause{
allocator.allocate_object<TermQuery>("text", "griffith"), BooleanClause::Occur::FILTER});
allocator.allocate_object<TermQuery>("text", "observatory"), BooleanClause::Occur::FILTER});


IndexSearcher searcher{index_reader};
SearchContext srch_context{&allocator};
SimpleTopScoreDocCollectorManager simple_top_score_doc_collector_manager{
        &srch_context, ScoreMode::TOP_SCORES, 100, 100};
searcher.search(&srch_context, bool_q, &simple_top_score_doc_collector_manager);

for (int32_t i = 0; i < result.score_docs.size(); ++i) {
  auto &score_doc = result.score_docs[i];
  std::cout << "doc: " << score_doc.doc << ", score=" << score_doc.score << std::endl;
}
   
```
