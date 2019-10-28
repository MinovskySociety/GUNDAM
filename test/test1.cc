#include "gundam/graph.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

TEST(TestGUNDAM, GoogleTest) { ASSERT_TRUE(true); }

TEST(TestGUNDAM, GraphClass) {
  using namespace GUNDAM;

  Graph<> g;
  //Graph<SetAllowMultipleEdge<true>> g1;
  //Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<Label<std::string>>> g2;
  //
  //ASSERT_FALSE(g2.AddVertex(1, Label<std::string>("AAA")).IsNull());
  //ASSERT_FALSE(g2.AddVertex(2, Label<std::string>("AAA")).IsNull());
  //ASSERT_FALSE(g2.AddVertex(3, Label<std::string>("BBB")).IsNull());
  //ASSERT_FALSE(g2.AddVertex(4, Label<std::string>("BBB")).IsNull());
  //ASSERT_TRUE(g2.AddVertex(1, Label<std::string>("AAA")).IsNull());
  //ASSERT_TRUE(g2.AddVertex(3, Label<std::string>("AAA")).IsNull());

  ASSERT_TRUE(true);
}