#include <ctime>
#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gundam/component/util.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "gundam/io/csvgraph.h"

inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }


template <class GraphType>
void ConstructGraph0(GraphType& g){
  // 1 -> 2
  // /\   |
  // |    |
  // 3 <---

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph1(GraphType& g){
  // 1 <- 2
  // |    /\
  // V    |
  // 3 ----

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 3, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 2, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph2(GraphType& g){
  // 1 <- 2
  // |    
  // V     
  // 3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 3, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph3(GraphType& g){
  // 1 -> 2 -> 4
  // |  / |  / |
  // V /  V /  V
  // 3 -> 5 -> 7
  // |  / |  / |
  // V /  V /  V
  // 6 -> 8 -> 9

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(6, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(7, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(8, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(9, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(1, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(2, 3, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 2, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(2, 4, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 5, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 5, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 6, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(4, 5, 42, 9);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 4, 42, 10);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 6, 42, 11);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 5, 42, 12);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(4, 7, 42, 13);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 7, 42, 14);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 8, 42, 15);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 8, 42, 16);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(7, 8, 42, 17);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(8, 7, 42, 18);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(7, 9, 42, 19);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(8, 9, 42, 20);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph4(GraphType& g){
  // 1 -> 2 -> 4
  // |    |    |
  // V    V    V
  // 3 -> 5 -> 7
  // |    |    |
  // V    V    V
  // 6 -> 8 -> 9

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(6, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(7, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(8, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(9, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(1, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(2, 4, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 5, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 5, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 6, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(4, 7, 42, 13);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 7, 42, 14);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 8, 42, 15);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 8, 42, 16);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(7, 9, 42, 19);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(8, 9, 42, 20);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template<typename GraphType>
bool TestGraphSame(const GraphType& g1,
                   const GraphType& g2){
  using VertexLabelType = typename GraphType
                                ::VertexType
                                 ::LabelType;
  using VertexIDType = typename GraphType
                             ::VertexType
                                 ::IDType;
  using EdgeLabelType = typename GraphType
                                ::EdgeType
                               ::LabelType;
  using EdgeIDType = typename GraphType
                             ::EdgeType
                               ::IDType;
  if (g1.CountVertex() != g2.CountVertex()){
    std::cout<<"in-equivalent vertex number not same"<<std::endl;
    return false;
  }
  // test vertex is same
  for (auto vertex_it = g1.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    auto vertex_ptr = g2.FindVertex(vertex_it->id());
    if (!vertex_ptr){
      std::cout<<"vertex: "<<vertex_ptr->id()
               <<" cannot be found in g2"<<std::endl;
      return false;
    }
    if (vertex_ptr->label() != vertex_it->label()) {
      // same id has different label
      return false;
    }
  }

  std::set<std::tuple<VertexIDType, //  src_id
                      VertexIDType, //  dst_id
                     EdgeLabelType, // edge_label
                        EdgeIDType>> g1_edge_set;

  // test edge is same 
  for (auto vertex_it = g1.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    for (auto edge_it = vertex_it->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      auto ret = g1_edge_set.emplace(edge_it->const_src_handle()->id(),
                                     edge_it->const_dst_handle()->id(),
                                     edge_it->label(),
                                     edge_it->id());
      // should have been added successfully
      // e.g. should not have duplicated edge
      assert(ret.second);
    }
  }

  for (auto vertex_it = g2.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    for (auto edge_it = vertex_it->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      auto temp_edge = std::make_tuple(
                            edge_it->const_src_handle()->id(),
                            edge_it->const_dst_handle()->id(),
                            edge_it->label(),
                            edge_it->id()); 
      auto it = g1_edge_set.find(temp_edge);
      if (it == g1_edge_set.cend()){
        // g1 does not have this edge
        std::cout<<"edge: "<<std::endl
                 <<"src_id: "<<edge_it->const_src_handle()->id()<<std::endl
                 <<"dst_id: "<<edge_it->const_dst_handle()->id()<<std::endl
                 <<"edge_label: "<<edge_it->label()<<std::endl
                 <<"edge_id: "<<edge_it->id()<<std::endl
                 <<" cannot be found in g1"<<std::endl;
        return false;
      }
      // g1 has this edge
      g1_edge_set.erase(it);
    }
  }
  if (!g1_edge_set.empty()){
    // g1 has edge that is not contained in g2
    for (const auto& remained_g1_edge : g1_edge_set){
      std::cout<<"edge: "<<std::endl
               <<"src_id: "<<std::get<0>(remained_g1_edge)<<std::endl
               <<"dst_id: "<<std::get<1>(remained_g1_edge)<<std::endl
               <<"edge_label: "<<std::get<2>(remained_g1_edge)<<std::endl
               <<"edge_id: "<<std::get<3>(remained_g1_edge)<<std::endl
               <<" cannot be found in g2"<<std::endl;
    }
    return false;
  }
  return true;
}

template<typename GraphType>
void TestCsvGraph(){
  std::string v_file = "test_v.csv",
              e_file = "test_e.csv";
  GraphType g0;
  ConstructGraph0(g0);
  GUNDAM::WriteCSVGraph(g0, v_file, e_file);
  GraphType g0_read;
  GUNDAM::ReadCSVGraph(g0_read, v_file, e_file);
  ASSERT_TRUE(TestGraphSame(g0, g0_read));

  GraphType g1;
  ConstructGraph1(g1);
  GUNDAM::WriteCSVGraph(g1, v_file, e_file);
  GraphType g1_read;
  GUNDAM::ReadCSVGraph(g1_read, v_file, e_file);
  ASSERT_TRUE(TestGraphSame(g1, g1_read));

  GraphType g2;
  ConstructGraph2(g2);
  GUNDAM::WriteCSVGraph(g2, v_file, e_file);
  GraphType g2_read;
  GUNDAM::ReadCSVGraph(g2_read, v_file, e_file);
  ASSERT_TRUE(TestGraphSame(g2, g2_read));

  GraphType g3;
  ConstructGraph3(g3);
  GUNDAM::WriteCSVGraph(g3, v_file, e_file);
  GraphType g3_read;
  GUNDAM::ReadCSVGraph(g3_read, v_file, e_file);
  ASSERT_TRUE(TestGraphSame(g3, g3_read));

  GraphType g4;
  ConstructGraph4(g4);
  GUNDAM::WriteCSVGraph(g4, v_file, e_file);
  GraphType g4_read;
  GUNDAM::ReadCSVGraph(g4_read, v_file, e_file);
  ASSERT_TRUE(TestGraphSame(g4, g4_read));
}

template <class GraphType, 
          class VertexFileList, 
          class   EdgeFileList>
void TestReadCSVGraph(const VertexFileList &v_list,
                      const   EdgeFileList &e_list) {
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
void TestCsvGraphSet(std::vector<GraphType> &graph_set, 
                     std::string &v_set_file,
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

  using G1 = LargeGraph<uint32_t, uint32_t, std::string, 
                        uint64_t, uint32_t, std::string>;

  using G2 = LargeGraph2<uint32_t, uint32_t, std::string, 
                         uint64_t, uint32_t, std::string>;  

  using G3 = SmallGraph<uint32_t, uint32_t, 
                        uint64_t, uint32_t>;

  using G4 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>, 
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>>;

  using G5 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>>;

  using G6 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

  using G7 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

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
  // std::vector<G2> graph_set;
  // graph_set.push_back(g1);
  // graph_set.push_back(g2);
  // std::string v_file = "/Users/apple/Desktop/test_v.csv";
  // std::string e_file = "/Users/apple/Desktop/test_e.csv";
  // TestCsvGraphSet(graph_set, v_file, e_file);

  TestCsvGraph<G1>();
  TestCsvGraph<G2>();
  TestCsvGraph<G3>();
  TestCsvGraph<G4>();
  TestCsvGraph<G5>();
  TestCsvGraph<G6>();
  TestCsvGraph<G7>();
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