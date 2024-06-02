#include <chrono>
#include <string>
#include <iostream>
#include <fstream>

#include "./src/store/MMapDirectory.hpp"
#include "./src/index/MMapDirectoryReader.hpp"
#include "./src/search/IndexSearcher.hpp"
#include "./src/search/SimpleTopScoreDocCollectorManager.hpp"
#include "./src/search/MatchCountCollectorManager.hpp"
#include "./src/search/query/TermQuery.hpp"
#include "./src/search/query/Query.hpp"
#include "./src/search/query/BooleanQuery.hpp"
#include "src/util/LinearAllocatorAdaptorForSTL.hpp"

using lucene::cyborg::index::LeafReaderContext;
using lucene::cyborg::index::MMapDirectoryReader;
using lucene::cyborg::index::MMapDirectoryReaderPtr;
using lucene::cyborg::index::PostingsEnum;
using lucene::cyborg::index::Terms;
using lucene::cyborg::search::Collector;
using lucene::cyborg::search::CollectorManager;
using lucene::cyborg::search::DocIdSetIterator;
using lucene::cyborg::search::IndexSearcher;
using lucene::cyborg::search::MatchCountCollectorManager;
using lucene::cyborg::search::Query;
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

ManagedPtr<Query> parse_query_str(LinearAllocator &allocator,
                                  const std::string_view &query_str) {
  BooleanQueryBuilder builder{&allocator};

  int j = 0;
  for (int i = 0; i < query_str.size(); ++i) {
    if (query_str[i] == ' ') {
      if (query_str[j] == '+') {
        builder.add_clause(BooleanClause{
            allocator.allocate_object<TermQuery>(
                "text", query_str.substr(j + 1, i - j - 1)), BooleanClause::Occur::MUST});
      } else {
        builder.add_clause(BooleanClause{
            allocator.allocate_object<TermQuery>(
                "text", query_str.substr(j, i - j)), BooleanClause::Occur::SHOULD});
      }

      j = i + 1;
    }
  }

  if (query_str[j] == '+') {
    builder.add_clause(BooleanClause{
        allocator.allocate_object<TermQuery>(
            "text", query_str.substr(j + 1, query_str.size() - j - 1)), BooleanClause::Occur::MUST});
  } else {
    builder.add_clause(BooleanClause{
        allocator.allocate_object<TermQuery>(
            "text", query_str.substr(j, query_str.size() - j)), BooleanClause::Occur::SHOULD});
  }

  return builder.build();
}

template<int N>
void top_search(IndexSearcher &searcher, const std::string_view &query_str) {
  LinearAllocator allocator{};
  SearchContext srch_context{&allocator};
  SimpleTopScoreDocCollectorManager simple_top_score_doc_collector_manager{
      &srch_context, ScoreMode::TOP_SCORES, N, N};
  auto bool_q = parse_query_str(allocator, query_str);
  searcher.search(&srch_context, bool_q, &simple_top_score_doc_collector_manager);
  std::cout << 1 << '\n';
}

int count(IndexSearcher &searcher, const std::string_view &query_str) {
  LinearAllocator allocator{};
  SearchContext srch_context{&allocator};
  MatchCountCollectorManager match_count_collector_manager{&srch_context};
  auto bool_q = parse_query_str(allocator, query_str);
  searcher.search(&srch_context, bool_q, &match_count_collector_manager);
  return match_count_collector_manager.count;
}

int main(int argc, const char *argv[]) {
  constexpr char DELIMITER = '\t';

  std::string path = "./idx";
  MMapDirectoryPtr dir = std::make_shared<MMapDirectory>(path);
  auto index_reader = MMapDirectoryReader::open(dir);
  IndexSearcher searcher{index_reader.get()};

  std::string line;
  while (std::getline(std::cin, line)) {
    std::string_view read_line = line;
    int i = 0;
    for (int until = line.size(); i < until; ++i) {
      if (line[i] == DELIMITER) {
        break;
      }
    }

    std::string_view command = read_line.substr(0, i);
    std::string_view query_str = read_line.substr(i + 1, read_line.size() - i - 1);

    if (command == "TOP_10") {
      top_search<10>(searcher, query_str);
    } else if (command == "TOP_100") {
      top_search<100>(searcher, query_str);
    } else if (command == "TOP_1000") {
      top_search<1000>(searcher, query_str);
    } else if (command == "COUNT") {
      int cnt = count(searcher, query_str);
      std::cout << cnt << '\n';
    } else {
      std::cout << "UNSUPPORTED\n";
    }
  }  // End while:w

}
