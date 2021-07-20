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

#include "gundam/algorithm/match_semantics.h"

#include "gundam/graph_type/graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/simple_small_graph.h"
#include "gundam/graph_type/small_graph.h"

#include "gundam/algorithm/multi_query_match.h"

inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }

template <class  QueryGraph, 
          class TargetGraph,
          enum GUNDAM::MatchAlgorithm match_algorithm>
void TestMultiQuery() {
  using namespace GUNDAM;

  using VertexLabelType = typename  QueryGraph::VertexType::LabelType;
  using   EdgeLabelType = typename TargetGraph::  EdgeType::LabelType;

   QueryGraph  query;
  TargetGraph target;

  std::cout << " QueryGraphType: " << typeid( query).name() << std::endl
            << "TargetGraphType: " << typeid(target).name() << std::endl;

  using  QueryVertexHandle = typename GUNDAM::VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename GUNDAM::VertexHandle<TargetGraph>::type;

  using MatchType = Match<QueryGraph, TargetGraph>;

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
  target.AddVertex(4, VertexLabelType(2));
  target.AddVertex(5, VertexLabelType(3));
  target.AddEdge(1, 2, EdgeLabelType(1), 1);
  target.AddEdge(3, 2, EdgeLabelType(1), 2);
  target.AddEdge(3, 1, EdgeLabelType(1), 3);
  target.AddEdge(4, 3, EdgeLabelType(1), 4);
  target.AddEdge(5, 1, EdgeLabelType(1), 5);

  std::vector<QueryGraph> query_graph_list;

  query_graph_list.emplace_back(query);

  std::function<bool(int, const GUNDAM::Match<QueryGraph, TargetGraph>&)>
    prune_callback = [](int pattern_idx,
                        const MatchType& match) -> bool {
    std::cout << "***********" << std::endl
              << "pattern_idx: "
              <<  pattern_idx
              << std::endl;
    // does not prune
    return false;
  };

  int single_match_counter = 0;
  bool pattern_idx_is_not_zero = false;

  std::function<bool(int, const GUNDAM::Match<QueryGraph, TargetGraph>&)>
      match_callback_single = [&single_match_counter,
                               &pattern_idx_is_not_zero](
                           int pattern_idx,
                           const MatchType& match) -> bool {
    if (pattern_idx != 0){
      pattern_idx_is_not_zero = true;
    }
    single_match_counter++;
    // continue matching
    return true;
  };

  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_single);
  ASSERT_EQ(single_match_counter, 2);
  ASSERT_FALSE(pattern_idx_is_not_zero);

  const int match_limit = 1;
  std::function<bool(int, const GUNDAM::Match<QueryGraph, TargetGraph>&)>
     match_callback_single_with_match_limit 
                = [&single_match_counter,
                    &match_limit,
                    &pattern_idx_is_not_zero](int pattern_idx,
                              const MatchType& match) -> bool {
    if (pattern_idx != 0){
      pattern_idx_is_not_zero = true;
    }
    single_match_counter++;
    if (single_match_counter == match_limit){
      // no longer need to matching
      return false;
    }
    // continue matching
    return true;
  };
  single_match_counter = 0;
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_single_with_match_limit);
  ASSERT_EQ(single_match_counter, match_limit);
  ASSERT_FALSE(pattern_idx_is_not_zero);

  QueryGraph query2(query);
  query2.AddVertex(4, VertexLabelType(2));
  query2.AddEdge(4, 3, EdgeLabelType(1), 4);
  query_graph_list.emplace_back(query2);

  std::vector<int> multi_match_counter(2, 0);

  bool pattern_idx_exceed_limit = false;
  std::function<bool(int, const GUNDAM::Match<QueryGraph, TargetGraph>&)>
       match_callback_multi
                  = [&multi_match_counter,
                     &pattern_idx_exceed_limit](int pattern_idx,
                                const MatchType& match) -> bool {
    std::cout << "###########" << std::endl
              << "pattern_idx: "
              <<  pattern_idx
              << std::endl;
    if (pattern_idx < 0
     || pattern_idx >= multi_match_counter.size()){
      pattern_idx_exceed_limit = true;
      return false;
    }
    multi_match_counter[pattern_idx]++;
    // continue matching
    return true;
  };
  // two query pattern, one is contained in the other
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_multi);
  ASSERT_EQ(multi_match_counter[0], 2);
  ASSERT_EQ(multi_match_counter[1], 1);
  ASSERT_FALSE(pattern_idx_exceed_limit);

  return;

  multi_match_counter.clear();
  multi_match_counter.resize(2, 0);
  std::function<bool(int, const GUNDAM::Match<QueryGraph, TargetGraph>&)>
   match_callback_multi_with_limit
                  = [&multi_match_counter,
                     &match_limit,
                     &pattern_idx_exceed_limit](int pattern_idx,
                                const MatchType& match) -> bool {
    if (pattern_idx < 0
     || pattern_idx >= multi_match_counter.size()){
      pattern_idx_exceed_limit = true;
      return false;
    }
    assert(multi_match_counter[pattern_idx] >= 0);
    assert(multi_match_counter[pattern_idx] < match_limit);
    multi_match_counter[pattern_idx]++;
    if (multi_match_counter[pattern_idx] == match_limit){
      return false;
    }
    // continue matching
    return true;
  };
  // two query pattern, one is contained in the other
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_multi_with_limit);
  ASSERT_EQ(multi_match_counter[0], match_limit);
  ASSERT_EQ(multi_match_counter[1], match_limit);
  ASSERT_FALSE(pattern_idx_exceed_limit);

  QueryGraph query3(query);
  query3.AddVertex(5, VertexLabelType(3));
  query3.AddEdge(5, 1, EdgeLabelType(1), 5);
  query_graph_list.emplace_back(query3);
  assert(query_graph_list.size() == 3);
  multi_match_counter.clear();
  multi_match_counter.resize(3, 0);
  // three query pattern, one is contained in the other two patterns
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_multi);
  ASSERT_EQ(multi_match_counter[0], 2);
  ASSERT_EQ(multi_match_counter[1], 1);
  ASSERT_EQ(multi_match_counter[2], 1);
  ASSERT_FALSE(pattern_idx_exceed_limit);

  query_graph_list.clear();
  query_graph_list.emplace_back(query2);
  query_graph_list.emplace_back(query3);
  assert(query_graph_list.size() == 2);
  multi_match_counter.clear();
  multi_match_counter.resize(2, 0);
  // two query pattern, the mcs of them does not contained in these two patterns
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_multi);
  ASSERT_EQ(multi_match_counter[0], 1);
  ASSERT_EQ(multi_match_counter[1], 1);
  ASSERT_FALSE(pattern_idx_exceed_limit);

  QueryGraph query4(query2);
  query4.AddVertex(5, VertexLabelType(3));
  query4.AddEdge(5, 1, EdgeLabelType(1), 5);
  query_graph_list.emplace_back(query4);
  assert(query_graph_list.size() == 3);
  multi_match_counter.clear();
  multi_match_counter.resize(3, 0);
  // three query pattern, both query_graph_list[0] and query_graph_list[1]
  // are contained in query_graph_list[2], the mcs of query_graph_list[0]
  // and query_graph_list[1] is not contained in query_graph_list
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_callback,
                                           match_callback_multi);
  ASSERT_EQ(multi_match_counter[0], 1);
  ASSERT_EQ(multi_match_counter[1], 1);
  ASSERT_EQ(multi_match_counter[2], 1);
  ASSERT_FALSE(pattern_idx_exceed_limit);

  std::function<bool(int, const GUNDAM::Match<QueryGraph, TargetGraph>&)>
      prune_3_callback = [](int pattern_idx,
                            const MatchType& match){
    // prune pattern with idx 0
    if (pattern_idx == 3){
      return true;
    }
    return false;
  };

  query_graph_list.emplace_back(query);
  multi_match_counter.clear();
  multi_match_counter.resize(4, 0);
  // three query pattern, both query_graph_list[0] and query_graph_list[1]
  // are contained in query_graph_list[2], the mcs of query_graph_list[0]
  // and query_graph_list[1] is not contained in query_graph_list
  GUNDAM::MultiQueryMatch<MatchSemantics::kIsomorphism,
                          match_algorithm>(query_graph_list,
                                           target,
                                           prune_3_callback,
                                           match_callback_multi);
  ASSERT_EQ(multi_match_counter[0], 1);
  ASSERT_EQ(multi_match_counter[1], 1);
  ASSERT_EQ(multi_match_counter[2], 1);
  ASSERT_EQ(multi_match_counter[3], 0);
  ASSERT_FALSE(pattern_idx_exceed_limit);
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

  TestMultiQuery<G1, G1, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G1, G2, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G1, G3, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G1, SG, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G1, LG, GUNDAM::MatchAlgorithm::kDagDp>();

  TestMultiQuery<G2, G1, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G2, G2, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G2, G3, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G2, SG, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G2, LG, GUNDAM::MatchAlgorithm::kDagDp>();

  TestMultiQuery<G3, G1, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G3, G2, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G3, G3, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G3, SG, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<G3, LG, GUNDAM::MatchAlgorithm::kDagDp>();
  
  TestMultiQuery<LG, G1, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<LG, G2, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<LG, G3, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<LG, SG, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<LG, LG, GUNDAM::MatchAlgorithm::kDagDp>();

  TestMultiQuery<SG, G1, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<SG, G2, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<SG, G3, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<SG, LG, GUNDAM::MatchAlgorithm::kDagDp>();
  TestMultiQuery<SG, SG, GUNDAM::MatchAlgorithm::kDagDp>();

  TestMultiQuery<G1, G1, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G1, G2, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G1, G3, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G1, SG, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G1, LG, GUNDAM::MatchAlgorithm::kVf2>();

  TestMultiQuery<G2, G1, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G2, G2, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G2, G3, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G2, SG, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G2, LG, GUNDAM::MatchAlgorithm::kVf2>();

  TestMultiQuery<G3, G1, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G3, G2, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G3, G3, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G3, SG, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<G3, LG, GUNDAM::MatchAlgorithm::kVf2>();
  
  TestMultiQuery<LG, G1, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<LG, G2, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<LG, G3, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<LG, SG, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<LG, LG, GUNDAM::MatchAlgorithm::kVf2>();

  TestMultiQuery<SG, G1, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<SG, G2, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<SG, G3, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<SG, LG, GUNDAM::MatchAlgorithm::kVf2>();
  TestMultiQuery<SG, SG, GUNDAM::MatchAlgorithm::kVf2>();

  // TestMultiQuery<SSG, G1>();
  // TestMultiQuery<SSG, G2>();
  // TestMultiQuery<SSG, G3>();
  // TestMultiQuery<SSG, LG>();
  // TestMultiQuery<SSG, SG>();
}
