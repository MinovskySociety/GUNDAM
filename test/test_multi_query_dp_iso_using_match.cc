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

#include "gundam/algorithm/multi_query_dp_iso.h"

inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }

template <class  QueryGraph, 
          class TargetGraph>
void TestMultiQueryDpiso() {
  using namespace GUNDAM;

  using VertexLabelType = typename  QueryGraph::VertexType::LabelType;
  using   EdgeLabelType = typename TargetGraph::  EdgeType::LabelType;

   QueryGraph  query;
  TargetGraph target;

  using  QueryVertexHandle = typename GUNDAM::VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename GUNDAM::VertexHandle<TargetGraph>::type;

  using MatchMap = std::map<QueryVertexHandle,
                           TargetVertexHandle>;

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

  std::vector<QueryGraph> query_graph_list;

  query_graph_list.emplace_back(query);

  auto prune_callback = [](int pattern_idx,
                           const MatchMap& match){
    // does not prune
    return false;
  };

  int single_match_counter = 0;
  bool pattern_idx_is_not_zero = false;

  auto match_callback_single = [&single_match_counter,
                                &pattern_idx_is_not_zero](int pattern_idx,
                                const MatchMap& match) -> bool {
    if (pattern_idx != 0){
      pattern_idx_is_not_zero = true;
    }
    // continue matching
    single_match_counter++;
    return true;
  };

  GUNDAM::MultiQueryDpiso(query_graph_list,
                          target,
                          prune_callback,
                          match_callback_single);
  ASSERT_EQ(single_match_counter, 2);
  ASSERT_FALSE(pattern_idx_is_not_zero);
  return;
}

TEST(TestGUNDAM, MultiQueryDpiso_1) {
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

  using LG  = LargeGraph<uint32_t, uint32_t, std::string, 
                         uint32_t, uint32_t, std::string>;
  using LG2 = LargeGraph2<uint32_t, uint32_t, std::string, 
                          uint32_t, uint32_t, std::string>;
  using SG  =       SmallGraph<uint32_t, uint32_t, uint32_t, uint32_t>;
  using SSG = SimpleSmallGraph<uint32_t, uint32_t, uint32_t, uint32_t>;

  TestMultiQueryDpiso<G1, G1>();
  TestMultiQueryDpiso<G1, G2>();
  TestMultiQueryDpiso<G1, G3>();
  TestMultiQueryDpiso<G2, G1>();
  TestMultiQueryDpiso<G2, G2>();
  TestMultiQueryDpiso<G2, G3>();
  TestMultiQueryDpiso<G3, G1>();
  TestMultiQueryDpiso<G3, G2>();
  TestMultiQueryDpiso<G3, G3>();
  TestMultiQueryDpiso<LG, G1>();
  TestMultiQueryDpiso<LG, G2>();
  TestMultiQueryDpiso<LG, G3>();
  TestMultiQueryDpiso<LG, LG>();
  TestMultiQueryDpiso<LG, LG2>();
  TestMultiQueryDpiso<LG2, G1>();
  TestMultiQueryDpiso<LG2, G2>();
  TestMultiQueryDpiso<LG2, G3>();
  TestMultiQueryDpiso<LG2, LG>();
  TestMultiQueryDpiso<LG2, LG2>();
  // TestMultiQueryDpiso<SG, G1>();
  // TestMultiQueryDpiso<SG, G2>();
  // TestMultiQueryDpiso<SG, G3>();
  // TestMultiQueryDpiso<SG, LG>();
  // TestMultiQueryDpiso<SSG, G1>();
  // TestMultiQueryDpiso<SSG, G2>();
  // TestMultiQueryDpiso<SSG, G3>();
  // TestMultiQueryDpiso<SSG, LG>();
}
