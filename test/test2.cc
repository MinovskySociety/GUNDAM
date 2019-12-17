#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gundam/graph.h"

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
  // ASSERT_EQ(res2.first->label(), Label<std::string>("ccc"));
  std::cout << "Adge 1's label: " << res2.first->label().to_string()
            << std::endl;
}

TEST(TestGUNDAM, Graph_Edge) {
  using namespace GUNDAM;

  using GraphType1 =
      Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<std::string>,
            SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  GraphType1 g1;

  GraphType1::VertexPtr v1, v3;

  bool res_bool;
  std::tie(v1, res_bool) = g1.AddVertex(1, "AAA");
  ASSERT_TRUE(res_bool);

  ASSERT_TRUE(g1.AddVertex(2, std::string("AAA")).second);

  auto res1 = g1.AddVertex(3, "BBB");
  ASSERT_TRUE(res1.second);
  v3 = res1.first;
  ASSERT_FALSE(v3.IsNull());

  GraphType1::EdgePtr e1, e2;

  auto res2 = g1.AddEdge(1, 2, "a", 1);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(1, res2.first->id());
  ASSERT_EQ(1, res2.first->const_src_ptr()->id());
  ASSERT_EQ(2, res2.first->const_dst_ptr()->id());
  e1 = res2.first;
  
  std::tie(e2, res_bool) = g1.AddEdge(2, 3, "b", 2);
  ASSERT_TRUE(res_bool);
  ASSERT_FALSE(e2.IsNull());
  ASSERT_EQ(2, e2->id());
  ASSERT_EQ(2, e2->const_src_ptr()->id());
  ASSERT_EQ(3, e2->const_dst_ptr()->id());

  ASSERT_TRUE(g1.AddEdge(1, 3, "b", 3).second);

  auto e_it1 = v1->OutEdgeCBegin();
  GraphType1::EdgeConstPtr e1_const1 = e_it1;  
  ASSERT_EQ(e1->id(), e1_const1->id());
  ASSERT_EQ(e1->const_src_ptr(), e1_const1->const_src_ptr());
  ASSERT_EQ(e1->const_dst_ptr(), e1_const1->const_dst_ptr());  

  //e1->label();   
  //e1 == e1;
  //e1 == e1_const;

  auto e_it2 = v1->OutEdgeCBegin("a");
  GraphType1::EdgeConstPtr e1_const2{e_it2};
  ASSERT_EQ(e1->id(), e1_const2->id());
  ASSERT_EQ(e1->const_src_ptr(), e1_const2->const_src_ptr());
  ASSERT_EQ(e1->const_dst_ptr(), e1_const2->const_dst_ptr());  

  res2 = g1.AddEdge(1, 1, "sss", 4);
  ASSERT_TRUE(res2.second);
  ASSERT_EQ(res2.first->const_src_ptr(), res2.first->const_dst_ptr());
}