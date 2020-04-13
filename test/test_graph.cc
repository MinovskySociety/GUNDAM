#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gundam/graph.h"

TEST(TestGUNDAM, TestGraph) {
  using namespace GUNDAM;

  using G0 = Graph<>;

  using G1 = Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<std::string>,
                   SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
                   SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
                   SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  using G1A = Graph<SetVertexLabelType<std::string>, SetVertexIDType<uint32_t>,
                    SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
                    SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
                    SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;
  
  //using G2 = Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<std::string>,
  //                 SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
  //                 SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //                 SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  std::cout << std::is_same<G1, G1A>::value << std::endl;
  std::cout << std::is_constructible<G1, G1A>::value << std::endl;

  G0 g0;

  G1 g1;

  G1 g1a{g1};

  ASSERT_TRUE(true);
}

TEST(TestGUNDAM, TestGraphVertexEdge) {
  using namespace GUNDAM;

  using G1 = Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<std::string>,
                   SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
                   SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
                   SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;
  
  G1 g1;

  // AddVertex
  auto res1 = g1.AddVertex(1, "AAA");
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(1, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g1.AddVertex(2, std::string{"AAA"});
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(2, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g1.AddVertex(3, std::string("BBB"));
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(3, res1.first->id());
  ASSERT_EQ("BBB", res1.first->label());

  res1 = g1.AddVertex(4, "BBB");
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(4, res1.first->id());
  ASSERT_EQ("BBB", res1.first->label());

  res1 = g1.AddVertex(1, "AAA");
  ASSERT_FALSE(res1.second);
  ASSERT_EQ(1, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g1.AddVertex(3, "AAA");
  ASSERT_FALSE(res1.second);
  ASSERT_EQ(3, res1.first->id());
  ASSERT_TRUE(res1.first->label() == "BBB");

  // AddEdge
  auto res2 = g1.AddEdge(1, 2, "aaa", 1);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 1);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(2, 3, "aaa", 2);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 2);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(3, 4, "aaa", 3);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 3);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(4, 1, "aaa", 4);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 4);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(2, 1, "bbb", 5);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 5);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(3, 2, "bbb", 6);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 6);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(4, 3, "bbb", 7);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 7);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(1, 4, "bbb", 8);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 8);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(1, 2, "ccc", 9);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 9);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g1.AddEdge(3, 4, "ccc", 10);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 10);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g1.AddEdge(3, 2, "ccc", 11);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 11);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g1.AddEdge(4, 1, "ccc", 12);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 12);
  ASSERT_EQ(res2.first->label(), "ccc");

  auto v = g1.FindVertex(0);
  ASSERT_TRUE(v.IsNull());

  auto v1 = g1.FindVertex(1);
  ASSERT_FALSE(v1.IsNull());
  ASSERT_EQ(v1->id(), 1);

  auto v2 = g1.FindVertex(2);
  ASSERT_FALSE(v2.IsNull());
  ASSERT_EQ(v2->id(), 2);

  auto e1 = v1->FindOutEdge("aaa", v2, 1);
  ASSERT_FALSE(e1.IsNull());
  ASSERT_EQ(e1->id(), 1);
  ASSERT_EQ(e1->label(), "aaa");

  auto e1b = v2->FindInEdge(1);
  ASSERT_FALSE(e1b.IsNull());
  ASSERT_EQ(e1b->id(), 1);
  ASSERT_EQ(e1b->label(), "aaa");

  ASSERT_EQ(e1, e1b);

  int count = 0;
  for (auto v_it = g1.VertexCBegin(); !v_it.IsDone(); ++v_it) {
    ++count;
  }
  ASSERT_EQ(count, 4);

  count = 0;
  for (auto e_out_it = v1->OutEdgeCBegin(); !e_out_it.IsDone(); ++e_out_it) {
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_in_it = v1->InEdgeCBegin(); !e_in_it.IsDone(); ++e_in_it) {
    ++count;
  }
  ASSERT_EQ(count, 3);
}

TEST(TestGUNDAM, TestGraphAttribute) {
  using namespace GUNDAM;

  using G1 = Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<std::string>,
                   SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
                   SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
                   SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  G1 g;
  bool res;

  G1::VertexPtr v1, v2;
  std::tie(v1, res) = g.AddVertex(1, "1");
  ASSERT_TRUE(res);
  ASSERT_FALSE(v1.IsNull());

  std::tie(v2, res) = g.AddVertex(2, "1");
  ASSERT_TRUE(res);
  ASSERT_FALSE(v2.IsNull());

  G1::EdgePtr e1, e2;
  std::tie(e1, res) = g.AddEdge(1, 2, "a", 1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(e1.IsNull());

  std::tie(e2, res) = g.AddEdge(2, 1, "b", 2);
  ASSERT_TRUE(res);
  ASSERT_FALSE(e1.IsNull());

  G1::VertexAttributePtr va1;
  std::tie(va1, res) = v1->AddAttribute(1, 1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va1.IsNull());

  G1::VertexAttributePtr va2;
  std::tie(va2, res) = g.FindVertex(1)->AddAttribute(2, 2.1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va2.IsNull());

  G1::VertexAttributePtr va3;
  const char *str = "33";
  std::tie(va3, res) = v1->AddAttribute(3, str);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va3.IsNull());
  int count = 0;

  auto res_pair = v1->AddAttribute(4, std::string{"44"});
  ASSERT_TRUE(res_pair.second);
  ASSERT_FALSE(res_pair.first.IsNull());

  res_pair = v1->AddAttribute(1, 5);
  ASSERT_FALSE(res_pair.second);
  ASSERT_FALSE(res_pair.first.IsNull());
  ASSERT_EQ(1, res_pair.first->const_value<int>());

  for (G1::EdgeAttributeIterator it = v1->AttributeBegin(); !it.IsDone(); it++) {
    std::cout << "key = " << it->key() << std::endl
              << "value type = " << EnumToString(it->value_type_id())
              << std::endl;
    ++count;
  }
  ASSERT_EQ(4, count);

  for (const auto &key : {1, 2, 3}) {
    G1::VertexAttributePtr va = v1->FindAttributePtr(key);

    std::cout << "key = " << va->key() << std::endl
              << "value type = " << EnumToString(va->value_type_id())
              << std::endl;
  }

  // Attributes
  ASSERT_TRUE(v1->AddAttribute(5, std::string("abc")).second);

  auto v_attr = v1->FindAttributePtr(5);
  ASSERT_FALSE(v_attr.IsNull());
  ASSERT_EQ(v_attr->template value<std::string>(), "abc");

  ASSERT_FALSE(v1->AddAttribute<std::string>(5, "abcd").second);

  ASSERT_TRUE(v1->SetAttribute<std::string>(5, "abcd").second);

  auto v_cattr = v1->FindConstAttributePtr(5);
  ASSERT_FALSE(v_cattr.IsNull());
  ASSERT_EQ(v_cattr->template const_value<std::string>(), "abcd");

  ASSERT_FALSE(v1->SetAttribute<std::string>(6, "abcde").second);

  ASSERT_TRUE(e1->AddAttribute<std::string>(5, "abc").second);

  auto e_attr = e1->FindAttributePtr(5);
  ASSERT_FALSE(e_attr.IsNull());
  ASSERT_EQ(e_attr->template value<std::string>(), "abc");

  ASSERT_FALSE(e1->template AddAttribute<std::string>(5, "abcd").second);

  ASSERT_TRUE(e1->SetAttribute<std::string>(5, "abcd").second);

  auto e_cattr = e1->FindConstAttributePtr(5);
  ASSERT_FALSE(e_cattr.IsNull());
  ASSERT_EQ(e_cattr->template const_value<std::string>(), "abcd");

  ASSERT_FALSE(e1->SetAttribute<std::string>(6, "abcde").second);
}