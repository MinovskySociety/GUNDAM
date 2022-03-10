#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "gundam/tool/operator/merge_graph.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"
#include "gundam/tool/max_id.h"


template<class GraphType>
void TestMergeGraphs(){
  using VertexIDType = typename GraphType::VertexType::IDType;

  GraphType g0;
  // 1 -> 2 -> 3

  std::cout << "GraphType: " << typeid(g0).name() << std::endl;

  auto res1 = g0.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(2, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(3, 2);

  // AddEdge
  auto res2 = g0.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(2, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  GraphType g1;

  // 1 -> 5 -> 3


  res1 = g1.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(5, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(3, 2);

  // AddEdge
  res2 = g1.AddEdge(1, 5, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(5, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  std::vector<GraphType> path_vec;
  path_vec.push_back(g0);
  path_vec.push_back(g1);

  std::vector<VertexIDType> id_vec;

  GraphType mg0 = GUNDAM::MergeGraphSet(path_vec, id_vec);
  ASSERT_EQ(mg0.CountVertex(), 6);
  ASSERT_EQ(mg0.CountEdge(), 4);
  id_vec.push_back(1);

  GraphType mg1 = GUNDAM::MergeGraphSet(path_vec, id_vec);

  ASSERT_EQ(mg1.CountVertex(), 5);
  ASSERT_EQ(mg1.CountEdge(), 4);
  GraphType mg2 = GUNDAM::MergeGraphSet(path_vec, id_vec);

  std::vector<GraphType> star_vec;
  star_vec.push_back(mg1);
  star_vec.push_back(mg0);

  id_vec.clear();
  GraphType mg3 = GUNDAM::MergeGraphSet(star_vec, id_vec);

  ASSERT_EQ(mg3.CountVertex(), 11);
  ASSERT_EQ(mg3.CountEdge(), 8);

  return;
}

TEST(TestGUNDAM, TestMergeGraphs) {
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

  TestMergeGraphs<G1>();
  TestMergeGraphs<G2>();
  TestMergeGraphs<G3>();
  TestMergeGraphs<G4>();
  TestMergeGraphs<G5>();
  TestMergeGraphs<G6>();
  TestMergeGraphs<G7>();
}
