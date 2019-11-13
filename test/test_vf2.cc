#include "gundam/vf2.h"
#include "gundam/vf2_basic.h"

#include <iostream>

#include "gtest/gtest.h"

template <typename Ptr1, typename Ptr2>
bool LabelEqual(const Ptr1& a, const Ptr2& b) {
  return a->label() == b->label();
}

TEST(TestGUNDAM, VF2_1) {
  using namespace GUNDAM;
  using GraphType = GUNDAM::Graph<>;
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;  
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;

  GraphType query, target;
  VertexIDType query_id = 1, target_id = 1;

  // query
  query.AddVertex(1, VertexLabelType(0));
  query.AddVertex(2, VertexLabelType(1));
  query.AddVertex(3, VertexLabelType(0));
  query.AddEdge(1, 2, EdgeLabelType(1), 1);
  query.AddEdge(3, 2, EdgeLabelType(1), 2);
  // query.AddEdge(3,1,EdgeLabelType(1));

  // target
  target.AddVertex(1, VertexLabelType(0));
  target.AddVertex(2, VertexLabelType(1));
  target.AddVertex(3, VertexLabelType(0));
  target.AddEdge(1, 2, EdgeLabelType(1), 1);
  target.AddEdge(3, 2, EdgeLabelType(1), 2);
  target.AddEdge(3, 1, EdgeLabelType(1), 3);

  std::vector<std::map<VertexConstPtr, VertexConstPtr>> match_result;
  int count = VF2<MatchSemantics::kIsomorphism>(query, target, match_result);

  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
  ASSERT_EQ(count, 2);

  count = VF2<MatchSemantics::kIsomorphism>(
      query, target, match_result, 
	  LabelEqual<VertexConstPtr, VertexConstPtr>,
      LabelEqual<EdgeConstPtr, EdgeConstPtr>);

  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
  ASSERT_EQ(count, 2);
}

template <typename Ptr1, typename Ptr2>
bool LabelEqualCustom(const Ptr1& a, const Ptr2& b) {
  return *(uint32_t*)&a->label() ==
         static_cast<uint32_t>(stoi(b->label()));
}

TEST(TestGUNDAM, VF2_2) {
  using namespace GUNDAM;
  using QueryType =
      Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<Label<uint32_t>>,
            SetEdgeIDType<uint32_t>, SetEdgeLabelType<Label<uint32_t>>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  using TargetType =
      Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<std::string>,
            SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

  QueryType query;
  TargetType target;

  // query
  ASSERT_TRUE(query.AddVertex(1, 0).second);
  ASSERT_TRUE(query.AddVertex(2, 1).second);
  ASSERT_TRUE(query.AddVertex(3, 0).second);
  ASSERT_TRUE(query.AddEdge(1, 2, 1, 1).second);
  ASSERT_TRUE(query.AddEdge(3, 2, 1, 2).second);

  // target
  ASSERT_TRUE(target.AddVertex(1, "0").second);
  ASSERT_TRUE(target.AddVertex(2, "1").second);
  ASSERT_TRUE(target.AddVertex(3, "0").second);
  ASSERT_TRUE(target.AddEdge(1, 2, std::string("1"), 1).second);
  ASSERT_TRUE(target.AddEdge(3, 2, std::string("1"), 2).second);
  ASSERT_TRUE(target.AddEdge(3, 1, std::string("1"), 3).second);

  std::vector<std::map<QueryType::VertexConstPtr, TargetType::VertexConstPtr>>
      match_result;
  int count = VF2<MatchSemantics::kIsomorphism>(
      query, target, match_result,
      LabelEqualCustom<QueryType::VertexConstPtr, TargetType::VertexConstPtr>,
      LabelEqualCustom<QueryType::EdgeConstPtr, TargetType::EdgeConstPtr>);

  ASSERT_EQ(count, 2);
  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match: " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
}