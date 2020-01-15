#include "gundam/vf2.h"

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "gtest/gtest.h"
TEST(TestGUNDAM, MULTI_VF2) {
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
  std::cout << "flag=" << query.AddEdge(1, 2, EdgeLabelType(1), 1).second
            << std::endl;
  std::cout << "flag=" << query.AddEdge(1, 2, EdgeLabelType(1), 2).second
            << std::endl;
  // query.AddEdge(3,1,EdgeLabelType(1));

  // target
  target.AddVertex(1, VertexLabelType(0));
  target.AddVertex(2, VertexLabelType(1));
  target.AddEdge(1, 2, EdgeLabelType(1), 1);

  std::vector<std::map<VertexConstPtr, VertexConstPtr>> match_result;
  int count =
      VF2<MatchSemantics::kIsomorphism>(query, target, -1, match_result);

  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
  ASSERT_EQ(count, 0);
}