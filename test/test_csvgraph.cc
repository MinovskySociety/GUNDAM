#include <ctime>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gundam/component/util.h"
#include "gundam/graph_type/graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/io/csvgraph.h"

inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }

template <class GraphType, class VertexFileList, class EdgeFileList>
void TestReadCSVGraph(const VertexFileList &v_list,
                      const EdgeFileList &e_list) {
  using namespace GUNDAM;

  auto begin_time = GetTime();

  GraphType g1;

  ReadCSVGraph(g1, v_list, e_list);

  auto end_time = GetTime();

  std::cout << "  Time: " << end_time - begin_time << "ms" << std::endl;

  // PrintGraph(g1);
  std::cout << "Vertex: " << g1.CountVertex() << std::endl;
  std::cout << "  Edge: " << g1.CountEdge() << std::endl;

  ASSERT_TRUE(true);
}
template <class GraphType>
void TestCsvGraphSet(std::vector<GraphType> &graph_set, std::string &v_set_file,
                     std::string &e_set_file) {
  GUNDAM::ReadCSVGraphSet(graph_set, v_set_file, e_set_file);
  for (auto &graph : graph_set) {
    ASSERT_TRUE(graph.CountVertex() == 2);
    ASSERT_TRUE(graph.CountEdge() == 1);
  }
  std::string res_v_file = "/Users/apple/Desktop/res_v.csv";
  std::string res_e_file = "/Users/apple/Desktop/res_e.csv";
  GUNDAM::WriteCSVGraphSet(graph_set, res_v_file, res_e_file);
}
TEST(TestGUNDAM, ReadCSVGraph) {
  using namespace GUNDAM;

  using G1 = Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint32_t>,
                   SetEdgeIDType<uint32_t>, SetEdgeLabelType<uint32_t>,
                   SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
                   SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeAttributeKeyType<std::string>>;

  using G2 = LargeGraph<uint64_t, uint32_t, std::string, uint64_t, uint32_t,
                        std::string>;

  std::vector<std::string> v_list{"/mnt/d/Work/cu4999/liantong_v_user.csv",
                                  "/mnt/d/Work/cu4999/liantong_v_phone.csv",
                                  "/mnt/d/Work/cu4999/liantong_v_web.csv"};

  std::vector<std::string> e_list{
      "/mnt/d/Work/cu4999/liantong_e_call.csv",
      "/mnt/d/Work/cu4999/liantong_e_send_message.csv",
      "/mnt/d/Work/cu4999/liantong_e_visit.csv"};

  // const char *v_list = "D://Work/cu5000/liantong_n.csv";
  // const char *e_list = "D://Work/cu5000/liantong_e.csv";

  // TestReadCSVGraph<G1>(v_list, e_list);
  // TestReadCSVGraph<G2>(v_list, e_list);

  G2 g1, g2;
  std::vector<G2> graph_set;
  graph_set.push_back(g1);
  graph_set.push_back(g2);
  std::string v_file = "/Users/apple/Desktop/test_v.csv";
  std::string e_file = "/Users/apple/Desktop/test_e.csv";
  TestCsvGraphSet(graph_set, v_file, e_file);
}

// template <class GraphType>
// void TestWriteReadCSVGraph() {
//  GraphType g1, g1a;
//
//  // g1
//  ASSERT_TRUE(g1.AddVertex(1, 0).second);
//  ASSERT_TRUE(g1.AddVertex(2, 1).second);
//  ASSERT_TRUE(g1.AddVertex(3, 1).second);
//  ASSERT_TRUE(g1.AddEdge(1, 2, "10", 1).second);
//  ASSERT_TRUE(g1.AddEdge(1, 3, "10", 2).second);
//
//  auto v1 = g1.FindVertex(1);
//  ASSERT_TRUE(v1);
//  ASSERT_TRUE(v1->AddAttribute<std::string>("name", "v1").second);
//  ASSERT_TRUE(v1->AddAttribute<int>("height", 100).second);
//  ASSERT_TRUE(v1->AddAttribute<double>("weight", 9.99).second);
//  ASSERT_TRUE(
//      v1->AddAttribute<GUNDAM::DateTime>("time", "2000-1-1 10:10:10").second);
//
//  auto e1 = g1.FindEdge(1);
//  ASSERT_TRUE(e1);
//  ASSERT_TRUE(e1->AddAttribute<std::string>(100, "e1").second);
//  ASSERT_TRUE(e1->AddAttribute<int>(101, 100).second);
//  ASSERT_TRUE(e1->AddAttribute<double>(102, 9.99).second);
//  ASSERT_TRUE(
//      e1->AddAttribute<GUNDAM::DateTime>(103, "2000-1-1 10:10:10").second);
//
//  PrintGraph(g1);
//
//  ASSERT_TRUE(WriteCSVGraph(g1, "g1_v.csv", "g1_e.csv") >= 0);
//
//  ASSERT_TRUE(ReadCSVGraph(g1a, std::vector<const char *>{"g1_v.csv"},
//                           std::vector<const char *>{"g1_e.csv"}) >= 0);
//
//  PrintGraph(g1a);
//}
//
// TEST(TestGUNDAM, WriteReadCSVGraph) {
//  using namespace GUNDAM;
//
//  //using GraphType =
//  //    Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint16_t>,
//  //          SetEdgeIDType<uint32_t>, SetEdgeLabelType<std::string>,
//  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
//  //          SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>,
//  //          SetVertexAttributeKeyType<std::string>,
//  //          SetEdgeAttributeKeyType<uint32_t>>;
//
//  using GraphType = LargeGraph<uint32_t, uint16_t, std::string, uint32_t,
//                               std::string, std::uint32_t>;
//
//  TestWriteReadCSVGraph<GraphType>();
//}