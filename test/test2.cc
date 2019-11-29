#include "gundam/graph.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

TEST(TestGUNDAM, GoogleTest) { ASSERT_TRUE(true); }

TEST(TestGUNDAM, GraphClass) {
  using namespace GUNDAM;
  Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<Label<std::string>>,
        SetEdgeIDType<uint64_t>, SetEdgeLabelType<Label<int64_t>>,
        SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
        SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>
      g1; 
  // AddVertex
  auto res1 = g1.AddVertex(1, Label<std::string>("AAA"));
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(res1.first->id(), 1);
  ASSERT_EQ(res1.first->label(), Label<std::string>("AAA"));

  res1 = g1.AddVertex(2, Label<std::string>("AAA"));
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(res1.first->id(), 2);
  ASSERT_EQ(res1.first->label(), Label<std::string>("AAA"));
  // AddEdge
  using EdgeLabelType = decltype(g1)::EdgeType::LabelType;
  auto res2 = g1.AddEdge(2, 1, EdgeLabelType(1), 1);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 1);
  //ASSERT_EQ(res2.first->label(), Label<std::string>("ccc"));
  std::cout <<"Adge 1's label: "<< res2.first->label().to_string() << std::endl;

}