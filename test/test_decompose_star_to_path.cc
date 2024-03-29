#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "test_data/test_pattern_set.h"

#include "gundam/tool/topological/star/decompose_star_to_path.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

template<class GraphType>
void TestDecomposeStarToPath(){

  GraphType g0;

  // 6(label 7) -> 7(label 8)
  // /\
  // |
  // 1(label 2) -> 2(label 4) -> 3 (label 5)
  // |
  // \/
  // 4(label 4)-> 5(label 5)

  auto res1 = g0.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(4, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(5, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(6, 7);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(7, 8);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  auto res2 = g0.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(1, 4, 4, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(4, 5, 4, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(1, 6, 4, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(6, 7, 4, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  std::vector<GraphType> path_vec;
  path_vec = GUNDAM::DecomposeStarToPath<true>(g0);
  ASSERT_EQ(path_vec.size(), 3);

  path_vec = GUNDAM::DecomposeStarToPath<false>(g0);
  ASSERT_EQ(path_vec.size(), 3);
 
  GraphType g1;

  // 1(label 2) -> 2(label 4) -> 3 (label 5)
  // |
  // \/
  // 4(label 4)-> 5(label 5)

  res1 = g1.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(4, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(5, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g1.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(1, 4, 4, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(4, 5, 4, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec = GUNDAM::DecomposeStarToPath<true>(g1);
  ASSERT_EQ(path_vec.size(), 1);
  ASSERT_EQ(path_vec[0].CountVertex(), 5);
  ASSERT_EQ(path_vec[0].CountEdge(), 4);

  path_vec = GUNDAM::DecomposeStarToPath<false>(g1);
  ASSERT_EQ(path_vec.size(), 2);
  ASSERT_EQ(path_vec[0].CountVertex(), 3);
  ASSERT_EQ(path_vec[1].CountVertex(), 3);
  ASSERT_EQ(path_vec[0].CountEdge(), 2);
  ASSERT_EQ(path_vec[1].CountEdge(), 2);
 

  GraphType g2;

  // 6(label 7) <- 7(label 8)
  // |
  // \/
  // 1(label 2) -> 2(label 4) -> 3 (label 5)
  // |
  // \/
  // 4(label 4)-> 5(label 5)

  res1 = g2.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(4, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(5, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(6, 7);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(7, 8);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g2.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(1, 4, 4, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(4, 5, 4, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(6, 1, 4, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(7, 6, 4, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec = GUNDAM::DecomposeStarToPath<true>(g2);
  ASSERT_EQ(path_vec.size(), 3);
  ASSERT_EQ(path_vec[0].CountVertex(), 3);
  ASSERT_EQ(path_vec[1].CountVertex(), 3);
  ASSERT_EQ(path_vec[2].CountVertex(), 3);
  ASSERT_EQ(path_vec[0].CountEdge(), 2);
  ASSERT_EQ(path_vec[1].CountEdge(), 2);
  ASSERT_EQ(path_vec[2].CountEdge(), 2);

  path_vec = GUNDAM::DecomposeStarToPath<false>(g2);
  ASSERT_EQ(path_vec.size(), 0);


  GraphType g5;
  ConstructGraph5(g5);

  path_vec = GUNDAM::DecomposeStarToPath<true>(g5);
  ASSERT_EQ(path_vec.size(), 1);
  for (const auto& path : path_vec) {
    std::string path_str;
    path_str << path;
    std::cout << "path_str: " << path_str << std::endl;
  }
  std::cout << std::endl;

  path_vec.clear();

  path_vec = GUNDAM::DecomposeStarToPath<false>(g5);
  ASSERT_EQ(path_vec.size(), 2);
  for (const auto& path : path_vec) {
    std::string path_str;
    path_str << path;
    std::cout << "path_str: " << path_str << std::endl;
  }
  std::cout << std::endl;
  return;
}


TEST(TestGUNDAM, TestDecomposeStarToPath) {
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

  TestDecomposeStarToPath<G1>();
  TestDecomposeStarToPath<G2>();
  TestDecomposeStarToPath<G3>();
  TestDecomposeStarToPath<G4>();
  TestDecomposeStarToPath<G5>();
  TestDecomposeStarToPath<G6>();
  TestDecomposeStarToPath<G7>();
}
