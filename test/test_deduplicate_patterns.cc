#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "gundam/tool/deduplicate_patterns/deduplicate_patterns.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

template<class GraphType>
void TestDeduplicatePatterns(){
 

  GraphType g0;

  // 1 -> 2 -> 3
  // /\        |
  // |         V
  // 8         4
  // /\        |
  // |         V
  // 7 <- 6 <- 5


  auto res1 = g0.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(5, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(6, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(7, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(8, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g0.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(2, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(3, 4, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(4, 5, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(5, 6, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(6, 7, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(7, 8, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(8, 1, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);


  GraphType g2;


  // 1 -> 2    5 -> 6
  // /\   |    /\   |
  // |    V    |    V
  // 4 <- 3    8 <- 7

  // AddVertex
  res1 = g2.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(5, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(6, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(7, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(8, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  res2 = g2.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(2, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(3, 4, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(4, 1, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g2.AddEdge(5, 6, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(6, 7, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(7, 8, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(8, 5, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  GraphType g3;


  // 2 -> 4    7 -> 6
  // /\   |    /\   |
  // |    V    |    V
  // 1 <- 3    8 <- 5

  // AddVertex
  res1 = g3.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(5, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(6, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(7, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(8, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  res2 = g3.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(4, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(3, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(2, 4, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g3.AddEdge(7, 6, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(8, 7, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(5, 8, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(6, 5, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);




  std::vector<GraphType> patterns;
  patterns.push_back(g0);
  patterns.push_back(g0);
  patterns.push_back(g2);
  patterns.push_back(g2);
  patterns.push_back(g3);
  patterns.push_back(g3);

  GUNDAM::DeduplicatePatternsWithDfsCode(patterns);

  

  ASSERT_EQ(patterns.size(), 2);

  return;
}

TEST(TestGUNDAM, TestDeduplicatePatterns) {
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

  TestDeduplicatePatterns<G1>();
  TestDeduplicatePatterns<G2>();
  TestDeduplicatePatterns<G3>();
  TestDeduplicatePatterns<G4>();
  TestDeduplicatePatterns<G5>();
  TestDeduplicatePatterns<G6>();
  TestDeduplicatePatterns<G7>();
}
