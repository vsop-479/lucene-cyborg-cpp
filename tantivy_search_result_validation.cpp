#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <filesystem>

#include "./src/store/MMapDirectory.hpp"
#include "./src/index/MMapDirectoryReader.hpp"
#include "./src/search/IndexSearcher.hpp"
#include "./src/search/MatchCountCollectorManager.hpp"
#include "./src/search/SimpleTopScoreDocCollectorManager.hpp"
#include "./src/search/query/TermQuery.hpp"
#include "./src/search/query/BooleanQuery.hpp"
#include "src/util/LinearAllocatorAdaptorForSTL.hpp"

using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::index::MMapDirectoryReader;
using lucene::cyborg::index::MMapDirectoryReaderPtr;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::index::Terms;
using lucene::cyborg::search::Collector;
using lucene::cyborg::search::MatchCountCollectorManager;
using lucene::cyborg::search::CollectorManager;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::ScoreDoc;
using lucene::cyborg::search::ScoreMode;
using lucene::cyborg::search::SearchContext;
using lucene::cyborg::search::SimpleTopScoreDocCollectorManager;
using lucene::cyborg::search::query::BooleanClause;
using lucene::cyborg::search::query::BooleanQuery;
using lucene::cyborg::search::query::BooleanQueryBuilder;
using lucene::cyborg::search::query::TermQuery;
using lucene::cyborg::store::MMapDirectory;
using lucene::cyborg::store::MMapDirectoryPtr;
using lucene::cyborg::util::LVector;
using lucene::cyborg::util::LinearAllocator;
using lucene::cyborg::util::LinearAllocatorAdapterForSTL;
using lucene::cyborg::util::ManagedPtr;
using lucene::cyborg::util::TypedVector;
using lucene::cyborg::util::UseOnlyPtr;

using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
using std::chrono::nanoseconds;
using std::chrono::microseconds;

bool bool_query_search_validation(const std::filesystem::path &validation_base_dir,
                                  ManagedPtr<MMapDirectoryReader> index_reader) {
  std::filesystem::path srch_validation_txt = validation_base_dir / "tantivy_srch_benchmark.bool_search_results";
  std::string field = "text";

  std::ifstream file_in{srch_validation_txt};
  std::string line;

  struct BoolSearchNightlyBenchResult {
    std::string query_type;
    std::vector<std::string> terms;
    int64_t total_hits;
    int32_t total_relation_ord;
    std::vector<ScoreDoc> score_docs;
  };

  std::vector<BoolSearchNightlyBenchResult> bool_search_results;

  while (std::getline(file_in, line)) {
    auto &result = bool_search_results.emplace_back();

    // 1. query type
    result.query_type = line;

    // 2. terms
    std::getline(file_in, line);
    const int32_t num_bool_terms = std::stoi(line);
    result.terms.reserve(num_bool_terms);
    for (int32_t i = 0; i < num_bool_terms; ++i) {
      std::getline(file_in, line);
      result.terms.emplace_back(line);
    }

    // 3. total hits
    std::getline(file_in, line);
    result.total_hits = std::stoll(line);

    // 4. relation ord
    std::getline(file_in, line);
    result.total_relation_ord = std::stoi(line);

    // 5. List[DocId + Score]
    std::getline(file_in, line);
    const int32_t num_results = std::stoi(line);

    for (int32_t i = 0; i < num_results; ++i) {
      std::getline(file_in, line);
      auto &score_doc = result.score_docs.emplace_back();
      for (int32_t j = 0, k = 0; j <= line.size(); ++j) {
        if (j == line.size() || line[j] == '|') {
          if (k == 0) {
            score_doc.doc = std::stoi(line.substr(k, j));
          } else {
            score_doc.score = std::stod(line.substr(k, j));
          }
          k = j + 1;
        }
      }
    }
  }  // End while

  //std::cout << "Num answers: " << bool_search_results.size() << std::endl;

  for (const auto &result : bool_search_results) {
    //std::cout << "-------------------------------------" << std::endl;
    //std::cout << "Query type=[" << result.query_type << ']' << std::endl;
    //std::cout << "Testing terms=[";
    for (const auto &term : result.terms) {
      //std::cout << term << ", ";
    }
    //std::cout << ']' << std::endl;

    auto t1 = high_resolution_clock::now();
    LinearAllocator allocator{};
    BooleanQueryBuilder builder{&allocator};
    if (result.query_type.starts_with("And")) {
      for (const auto &term : result.terms) {
        builder.add_clause(BooleanClause{
            allocator.allocate_object<TermQuery>(field, term), BooleanClause::Occur::MUST});
      }
    } else {
      for (const auto &term : result.terms) {
        builder.add_clause(BooleanClause{
            allocator.allocate_object<TermQuery>(field, term), BooleanClause::Occur::SHOULD});
      }
    }

    auto bool_q = builder.build();
    IndexSearcher searcher{index_reader};
    SearchContext srch_context{&allocator};
    SimpleTopScoreDocCollectorManager simple_top_score_doc_collector_manager{
        &srch_context, ScoreMode::TOP_SCORES, 100, 100};
    searcher.search(&srch_context, bool_q, &simple_top_score_doc_collector_manager);
    auto t2 = high_resolution_clock::now();
    //std::cout << "Took: " << duration_cast<nanoseconds>(t2 - t1).count() << "ns" << std::endl;

    // total hit
    if (simple_top_score_doc_collector_manager.top_docs.total_hits.value != result.total_hits) {
      std::cout << "Failed. Expected hit count=[" << result.total_hits << "]"
                << ", acquired [" << simple_top_score_doc_collector_manager.top_docs.total_hits.value << "]"
                << std::endl;
      return false;
    }

    // total ord
    if (((int32_t) simple_top_score_doc_collector_manager.top_docs.total_hits.relation) != result.total_relation_ord) {
      std::cout << "Failed. Expected hit relation ord=[" << result.total_relation_ord << "]"
                << ", acquired [" << ((int32_t) simple_top_score_doc_collector_manager.top_docs.total_hits.relation)
                << "]"
                << std::endl;
      return false;
    }

    auto &score_docs = simple_top_score_doc_collector_manager.top_docs.score_docs;

    if (score_docs.len != result.score_docs.size()) {
      std::cout << "Failed. Expected #results=[" << result.score_docs.size() << "]"
                << ", acquired [" << score_docs.len << "]" << std::endl;
    }

    for (int32_t i = 0; i < result.score_docs.size(); ++i) {
      const auto &expected = result.score_docs[i];
      const auto acquired = score_docs.vec[i];

      if (acquired->doc != expected.doc) {
        std::cout << "Failed. Expected [" << i << "] th doc as ["
                  << expected.doc << "], acquired [" << acquired->doc << "]"
                  << std::endl;

        goto FAILED_MATCH;
      }

      if (std::abs(acquired->score - expected.score) > 1e-6) {
        std::cout << "Failed. Expected [" << i << "] th score as ["
                  << expected.score << "], acquired [" << acquired->score << "]"
                  << std::endl;

        goto FAILED_MATCH;
      }

      continue;

      FAILED_MATCH:
      //std::cout << "###########################" << std::endl;

      for (int32_t k = 0; k < result.score_docs.size(); ++k) {
        auto &expected = result.score_docs[k];
        auto &acquired = score_docs.vec[k];
        const bool wrong = expected.doc != acquired->doc || (std::abs(expected.score - acquired->score) > 1e-6);

        std::cout << (wrong ? "    XXXXXX " : "") << "doc=[ans:" << expected.doc << " vs "
                  << acquired->doc << "], score=[ans:" << expected.score << " vs "
                  << acquired->score << "]" << std::endl;
      }

      return false;
    }

    // //std::cout << "Total hits: "
    //           << simple_top_score_doc_collector_manager.top_docs.total_hits.value
    //           << ", relation: " << (int) simple_top_score_doc_collector_manager.top_docs.total_hits.relation
    //           << std::endl;
    // for (int32_t j = 0; j < score_docs.len; ++j) {
    //   //std::cout << "doc: " << score_docs.vec[j]->doc << ", score: " << score_docs.vec[j]->score << std::endl;
    // }
  }  // End for

  return true;
}

bool bool_query_agg_validation(const std::filesystem::path &validation_base_dir,
                               ManagedPtr<MMapDirectoryReader> index_reader) {
  std::filesystem::path srch_validation_txt = validation_base_dir / "tantivy_srch_benchmark.bool_agg_results";
  std::string field = "text";

  std::ifstream file_in{srch_validation_txt};
  std::string line;

  struct BoolAggResult {
    std::string query_type;
    std::vector<std::string> terms;
    int64_t total_hits;
  };  // BoolAggResult

  std::vector<BoolAggResult> bool_search_results;

  while (std::getline(file_in, line)) {
    auto &result = bool_search_results.emplace_back();

    // 1. query type
    result.query_type = line;

    // 2. terms
    std::getline(file_in, line);
    const int32_t num_bool_terms = std::stoi(line);
    result.terms.reserve(num_bool_terms);
    for (int32_t i = 0; i < num_bool_terms; ++i) {
      std::getline(file_in, line);
      result.terms.emplace_back(line);
    }

    // 3. total hits
    std::getline(file_in, line);
    result.total_hits = std::stoll(line);
  }  // End while

  //std::cout << "Num answers: " << bool_search_results.size() << std::endl;

  auto end = high_resolution_clock::now();

  for (const auto &result : bool_search_results) {
    auto start = high_resolution_clock::now();
    //std::cout << "Destructor took: " << duration_cast<microseconds>(start - end).count() << "ns" << std::endl;

    //std::cout << "-------------------------------------" << std::endl;
    //std::cout << "Query type=[" << result.query_type << ']' << std::endl;
    //std::cout << "Testing terms=[";
    //for (const auto &term : result.terms) {
    //std::cout << term << ", ";
    //}
    //std::cout << ']' << std::endl;

    auto t1 = high_resolution_clock::now();
    LinearAllocator allocator{};
    BooleanQueryBuilder builder{&allocator};
    if (result.query_type.starts_with("And")) {
      for (const auto &term : result.terms) {
        builder.add_clause(BooleanClause{
            allocator.allocate_object<TermQuery>("text", term), BooleanClause::Occur::FILTER});
      }
    } else {
      for (const auto &term : result.terms) {
        builder.add_clause(BooleanClause{
            allocator.allocate_object<TermQuery>("text", term), BooleanClause::Occur::SHOULD});
      }
    }

    auto bool_q = builder.build();
    IndexSearcher searcher{index_reader};
    SearchContext srch_context{&allocator};
    MatchCountCollectorManager match_count_collector_manager{&srch_context};
    searcher.search(&srch_context, bool_q, &match_count_collector_manager);
    end = high_resolution_clock::now();
    //std::cout << "Took: " << duration_cast<microseconds>(end - t1).count() << "micro" << std::endl;

    // total hit
    if (match_count_collector_manager.count != result.total_hits) {
      std::cout << "Failed. Expected hit count=[" << result.total_hits << "]"
                << ", acquired [" << match_count_collector_manager.count << "]"
                << std::endl;
      return false;
    }

    // //std::cout << "Total hits: "
    //           << simple_top_score_doc_collector_manager.top_docs.total_hits.value
    //           << ", relation: " << (int) simple_top_score_doc_collector_manager.top_docs.total_hits.relation
    //           << std::endl;
    // for (int32_t j = 0; j < score_docs.len; ++j) {
    //   //std::cout << "doc: " << score_docs.vec[j]->doc << ", score: " << score_docs.vec[j]->score << std::endl;
    // }
  }  // End for

  return true;
}

int main(int argc, char *argv[]) {
  if (argc != 3) {
    std::cerr << "Missing parameters" << std::endl;
    std::cerr << argv[0] << " <resources base directory path> <lucene index path>" << std::endl;
    return 1;
  }

  std::filesystem::path validation_base_dir = argv[1];
  std::filesystem::path index_path = argv[2];

  auto t1 = high_resolution_clock::now();
  MMapDirectoryPtr dir = std::make_shared<MMapDirectory>(index_path.string());
  auto index_reader = MMapDirectoryReader::open(dir);
  auto t2 = high_resolution_clock::now();

  duration<double, std::milli> ms_double = t2 - t1;
  std::cout << "MMap directory creation took " << ms_double.count() << "ms\n";

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

  if (!bool_query_agg_validation(validation_base_dir, index_reader.get())) {
    std::cout << "bool_query_agg_validation failed" << std::endl;
    return 1;
  }
  if (!bool_query_search_validation(validation_base_dir, index_reader.get())) {
    std::cout << "bool_query_search_validation failed" << std::endl;
    return 1;
  }
}
