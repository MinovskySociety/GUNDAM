#include <ctime>
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/graph.h"
#include "gundam/csvgraph.h"
#include "gundam/geneator.h"
#include "gundam/matchresult.h"
#include "gundam/util.h"
#include "gundam/vf2.h"

TEST(TestGUNDAM, WRITEMATCHRESULT) {
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
  int count =
      VF2<MatchSemantics::kIsomorphism>(query, target, -1, match_result);
  MatchResultToFile<VertexIDType, VertexIDType>(match_result, std::cout);
  std::ofstream file_out("match_result.csv");
  MatchResultToFile<VertexIDType, VertexIDType>(match_result, file_out);
}