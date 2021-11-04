#include <cstdint>
#include <cstring>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "gtest/gtest.h"

#include "gundam/data_type/label.h"

#include "gundam/graph_type/graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/simple_small_graph.h"
#include "gundam/graph_type/small_graph.h"

#include "gundam/algorithm/simulation.h"

inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }

template <class  QueryGraph, 
          class TargetGraph>
void TestStrongSimulation_1() {
  using namespace GUNDAM;

  using VertexLabelType = typename  QueryGraph::VertexType::LabelType;
  using   EdgeLabelType = typename TargetGraph::  EdgeType::LabelType;

   QueryGraph  query;
  TargetGraph target;

  std::cout << "TestStrongSimulation_1:  QueryGraphType: " << typeid( query).name() << std::endl
            << "TestStrongSimulation_1: TargetGraphType: " << typeid(target).name() << std::endl;

  // query
  query.AddVertex(1, VertexLabelType(0));
  query.AddVertex(2, VertexLabelType(1));
  query.AddVertex(3, VertexLabelType(0));
  query.AddEdge(1, 2, EdgeLabelType(1), 1);
  query.AddEdge(3, 2, EdgeLabelType(1), 2);
  // query.AddEdge(3, 1, EdgeLabelType(1), 3);

  // target
  target.AddVertex(1, VertexLabelType(0));
  target.AddVertex(2, VertexLabelType(1));
  target.AddVertex(3, VertexLabelType(0));
  target.AddEdge(1, 2, EdgeLabelType(1), 1);
  target.AddEdge(3, 2, EdgeLabelType(1), 2);
  target.AddEdge(3, 1, EdgeLabelType(1), 3);

  GUNDAM::StrongSimulation<GUNDAM::MatchSemantics
                                 ::kStrongSimulation>(query, target);                
  // ASSERT_EQ(count, 2);
  // ASSERT_EQ(count, match_result1.size());
  // std::cout << "count: " << match_result1.size() << std::endl;

  // const auto&  query2 = query;
  // const auto& target2 = target;
  // GUNDAM::StrongSimulation<GUNDAM::MatchSemantics
  //                                ::kStrongSimulation>(query2, target2);
  // // ASSERT_EQ(count, 2);
  // // ASSERT_EQ(count, match_result2.size());
  // // std::cout << "count: " << match_result2.size() << std::endl;

  // const auto& query3 = query;
  // auto& target3 = target;
  // GUNDAM::StrongSimulation<GUNDAM::MatchSemantics
  //                          ::kStrongSimulation>(
  //              query3, target3);
  // // ASSERT_EQ(count, 2);
  // // ASSERT_EQ(count, match_result3.size());

  // auto& query4 = query;
  // const auto& target4 = target;
  // GUNDAM::StrongSimulation<GUNDAM::MatchSemantics
  //                          ::kStrongSimulation>(
  //              query4, target4);

  // std::cout << "count: " << match_result4.size() << std::endl;
}

TEST(TestGUNDAM, StrongSimulation_1) {
  using namespace GUNDAM;

  using G1
   = Graph<SetVertexIDType<uint32_t>, 
           SetVertexLabelType<uint32_t>,
           SetEdgeIDType<uint32_t>, 
           SetEdgeLabelType<uint32_t>>;

  using G2
   = Graph<SetVertexIDType<uint32_t>,
           SetVertexAttributeStoreType<AttributeType::kGrouped>,
           SetVertexLabelType<uint32_t>,
           SetVertexLabelContainerType<ContainerType::Vector>,
           SetVertexIDContainerType<ContainerType::Map>,
           SetVertexPtrContainerType<ContainerType::Vector>,
           SetEdgeLabelContainerType<ContainerType::Vector>,
           SetVertexAttributeKeyType<std::string>,
           SetEdgeIDType<uint32_t>,
           SetEdgeAttributeStoreType<AttributeType::kGrouped>,
           SetEdgeLabelType<uint32_t>,
           SetEdgeAttributeKeyType<std::string>>;

  using G3
   = Graph<SetVertexIDType<uint32_t>,
           SetVertexAttributeStoreType<AttributeType::kGrouped>,
           SetVertexLabelType<uint32_t>,
           SetVertexLabelContainerType<ContainerType::Vector>,
           SetVertexIDContainerType<ContainerType::Vector>,
           SetVertexPtrContainerType<ContainerType::Vector>,
           SetEdgeLabelContainerType<ContainerType::Vector>,
           SetVertexAttributeKeyType<std::string>,
           SetEdgeIDType<uint32_t>,
           SetEdgeAttributeStoreType<AttributeType::kGrouped>,
           SetEdgeLabelType<uint32_t>,
           SetEdgeAttributeKeyType<std::string>>;

  using LG = LargeGraph<uint32_t, uint32_t, std::string, 
                        uint32_t, uint32_t, std::string>;
  using SG  =       SmallGraph<uint32_t, uint32_t, uint32_t, uint32_t>;
  using SSG = SimpleSmallGraph<uint32_t, uint32_t, uint32_t, uint32_t>;

  TestStrongSimulation_1<G1, G1>();
  TestStrongSimulation_1<G1, G2>();
  TestStrongSimulation_1<G1, G3>();
  TestStrongSimulation_1<G1, SG>();
  TestStrongSimulation_1<G1, LG>();

  TestStrongSimulation_1<G2, G1>();
  TestStrongSimulation_1<G2, G2>();
  TestStrongSimulation_1<G2, G3>();
  TestStrongSimulation_1<G2, SG>();
  TestStrongSimulation_1<G2, LG>();

  TestStrongSimulation_1<G3, G1>();
  TestStrongSimulation_1<G3, G2>();
  TestStrongSimulation_1<G3, G3>();
  TestStrongSimulation_1<G3, SG>();
  TestStrongSimulation_1<G3, LG>();
  
  TestStrongSimulation_1<LG, G1>();
  TestStrongSimulation_1<LG, G2>();
  TestStrongSimulation_1<LG, G3>();
  TestStrongSimulation_1<LG, SG>();
  TestStrongSimulation_1<LG, LG>();

  TestStrongSimulation_1<SG, G1>();
  TestStrongSimulation_1<SG, G2>();
  TestStrongSimulation_1<SG, G3>();
  TestStrongSimulation_1<SG, LG>();
  TestStrongSimulation_1<SG, SG>();

  // TestStrongSimulation_1<SSG, G1>();
  // TestStrongSimulation_1<SSG, G2>();
  // TestStrongSimulation_1<SSG, G3>();
  // TestStrongSimulation_1<SSG, LG>();
  // TestStrongSimulation_1<SSG, SG>();
}