#include "gundam/vf2.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(TestGUNDAM, VF2) {
  using GraphType = GUNDAM::Graph<>;
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;
  bool VertexEqual(VertexConstPtr a, VertexConstPtr b) {
    return a->label() == b->label();
  }
  bool EdgeEqual(EdgeConstPtr edge_ptr1, EdgeConstPtr edge_ptr2) {
    return !(edge_ptr1->label() == edge_ptr2->label());
  }
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
  target.AddEdge(1, 2, EdgeLabelType(2), 1);
  target.AddEdge(3, 2, EdgeLabelType(2), 2);
  target.AddEdge(3, 1, EdgeLabelType(2), 3);
  std::vector<std::map<VertexConstPtr, VertexConstPtr>> match_result;
  GUNDAM::VF2<GUNDAM::MatchSemantics::kIsomorphism>(query, target, match_result,
                                                    VertexEqual, EdgeEqual);
  std::cout << match_result.size() << std::endl;
  if (!match_result.empty()) {
    for (auto &it : match_result) {
      std::cout << "query_id target_id:" << std::endl;
      for (auto it1 : it) {
        std::cout << it1.first->id() << " " << it1.second->id() << std::endl;
      }
    }
  }

  ASSERT_EQ(count, 2);
}