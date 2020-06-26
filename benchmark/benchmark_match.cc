#include <ctime>
#include <map>
#include <string>
#include <vector>

#include "gundam/csvgraph.h"
#include "gundam/graph.h"
#include "gundam/large_graph.h"
#include "gundam/simple_small_graph.h"
#include "gundam/small_graph.h"
#include "gundam/vf2.h"

inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }

template <class QueryGraph, class TargetGraph>
int VF2_Run(const QueryGraph &query_graph, const TargetGraph &target_graph) {
  using namespace GUNDAM;
  using MatchMap = std::map<typename QueryGraph::VertexConstPtr,
                            typename TargetGraph::VertexConstPtr>;
  using MatchResult = std::vector<MatchMap>;

  MatchResult match_result;
  int result = VF2(query_graph, target_graph, -1, match_result);

  return result;
}

struct CSVGraphInfo {
  std::vector<std::string> vertex_files;
  std::vector<std::string> edge_files;
};

struct MatchBenchmarkConfigure1 {
  CSVGraphInfo target_graph_info;
  std::vector<CSVGraphInfo> query_graph_info_list;
  int match_times;
};

template <class QueryGraph, class TargetGraph>
int MatchBenchmark1() {
  int result;
  MatchBenchmarkConfigure1 config;
  std::vector<QueryGraph> query_graph_list;
  TargetGraph target_graph;

  std::cout << "Match Benchmark1 (VF2)" << std::endl;

  // Init config;
  config.match_times = 1;

  config.query_graph_info_list.emplace_back();
  config.query_graph_info_list.back().vertex_files.emplace_back(
      "/share/work/match_benchmark/q1_v.csv");
  config.query_graph_info_list.back().edge_files.emplace_back(
      "/share/work/match_benchmark/q1_e.csv");
  
  // config.query_graph_info_list.emplace_back();
  // config.query_graph_info_list.back().vertex_files.emplace_back(
  //     "/share/work/match_benchmark/q2_v.csv");
  // config.query_graph_info_list.back().edge_files.emplace_back(
  //     "/share/work/match_benchmark/q2_e.csv");
  
  // config.query_graph_info_list.emplace_back();
  // config.query_graph_info_list.back().vertex_files.emplace_back(
  //     "/share/work/match_benchmark/q3_v.csv");
  // config.query_graph_info_list.back().edge_files.emplace_back(
  //     "/share/work/match_benchmark/q3_e.csv");

  config.target_graph_info.vertex_files.emplace_back(
      "/share/work/cu4999_1/liantong_v.csv");
  config.target_graph_info.edge_files.emplace_back(
      "/share/work/cu4999_1/liantong_e.csv");

  // Loading graphs
  auto begin_time = GetTime();
  std::cout  << std::endl << "Loading query graph..." << std::endl;
  for (size_t i = 0; i < config.query_graph_info_list.size(); ++i) {
    const auto &info = config.query_graph_info_list[i];
    query_graph_list.emplace_back();
    std::cout << "Query #" << (i + 1) << std::endl;
    result = GUNDAM::ReadCSVGraph(query_graph_list.back(), info.vertex_files,
                                  info.edge_files);
    if (result < 0) {
      return result;
    }
  }

  std::cout  << std::endl  << "Loading target graph..." << std::endl;
  result =
      GUNDAM::ReadCSVGraph(target_graph, config.target_graph_info.vertex_files,
                           config.target_graph_info.edge_files);
  if (result < 0) {
    return result;
  }

  std::cout << "Load time: " << GetTime() - begin_time << " ms" << std::endl;

  // Match
  begin_time = GetTime();
  std::cout  << std::endl  << "Matcing..." << std::endl;
  for (int i = 0; i < config.match_times; ++i) {
    for (int j = 0; j < query_graph_list.size(); ++j) {
      const auto &query_graph = query_graph_list[j];
      result = VF2_Run(query_graph, target_graph);
      if (i == 0) {
        std::cout << "Query #" << (j + 1) << ": " << result << std::endl;
      }
      if (result < 0) {
        return result;
      }
    }
  }
  std::cout << "Match time: " << GetTime() - begin_time << " ms" << std::endl;

  // Release graphs
  std::cout  << std::endl  << "Releasing graphs..." << std::endl;
  begin_time = GetTime();

  target_graph.Clear();
  query_graph_list.clear();

  std::cout << "Release time: " << GetTime() - begin_time << " ms" << std::endl;
  return 0;
}

int main() {
  using namespace GUNDAM;

  using GQ = Graph<SetVertexIDType<uint32_t>,
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetVertexIDContainerType<ContainerType::Map>,
                   SetEdgeIDType<uint32_t>,
                   SetEdgeLabelType<uint32_t>,
                   SetEdgeAttributeKeyType<std::string>>;

  using GT = Graph<SetVertexIDType<uint64_t>,
                   SetVertexLabelType<uint32_t>,                   
                   SetVertexAttributeKeyType<std::string>,
                   SetVertexIDContainerType<ContainerType::Map>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>,
                   SetEdgeAttributeKeyType<std::string>>;

  using LGQ = LargeGraph<uint32_t, uint32_t, std::string, 
                         uint32_t, uint32_t, std::string>;

  using LGT = LargeGraph<uint64_t, uint32_t, std::string, 
                         uint64_t, uint32_t, std::string>;

  std::cout << "case 3" << std::endl;
  MatchBenchmark1<LGQ, GT>();

  std::cout << "case 1" << std::endl;
  MatchBenchmark1<GQ, GT>();                       
  //std::cout << "case 2" << std::endl;
  //MatchBenchmark1<LGQ, LGT>();
  
  //std::cout << "case 4" << std::endl;
  //MatchBenchmark1<GQ, LGT>();

  return 0;
}