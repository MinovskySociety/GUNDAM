#include "gundam/graph.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

TEST(TestGUNDAM, GoogleTest) { ASSERT_TRUE(true); }

TEST(TestGUNDAM, GraphClass) {
  using namespace GUNDAM;

  Graph<> g;
  Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<Label<std::string>>,
        SetEdgeIDType<uint64_t>, SetEdgeLabelType<Label<std::string>>,
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

  res1 = g1.AddVertex(3, Label<std::string>("BBB"));
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(res1.first->id(), 3);
  ASSERT_EQ(res1.first->label(), Label<std::string>("BBB"));

  res1 = g1.AddVertex(4, Label<std::string>("BBB"));
  ASSERT_TRUE(res1.second);
  ASSERT_FALSE(res1.first.IsNull());
  ASSERT_EQ(res1.first->id(), 4);
  ASSERT_EQ(res1.first->label(), Label<std::string>("BBB"));

  res1 = g1.AddVertex(1, Label<std::string>("AAA"));
  ASSERT_FALSE(res1.second);
  ASSERT_EQ(res1.first->id(), 1);
  ASSERT_TRUE(res1.first->label().CanMatchTo(Label<std::string>("AAA")));

  res1 = g1.AddVertex(3, Label<std::string>("AAA"));
  ASSERT_FALSE(res1.second);
  ASSERT_EQ(res1.first->id(), 3);
  ASSERT_TRUE(res1.first->label() == Label<std::string>("BBB"));

  // AddEdge
  auto res2 = g1.AddEdge(1, 2, Label<std::string>("aaa"), 1);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 1);
  ASSERT_EQ(res2.first->label(), Label<std::string>("aaa"));

  res2 = g1.AddEdge(2, 3, Label<std::string>("aaa"), 2);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 2);
  ASSERT_EQ(res2.first->label(), Label<std::string>("aaa"));

  res2 = g1.AddEdge(3, 4, Label<std::string>("aaa"), 3);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 3);
  ASSERT_EQ(res2.first->label(), Label<std::string>("aaa"));

  res2 = g1.AddEdge(4, 1, Label<std::string>("aaa"), 4);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 4);
  ASSERT_EQ(res2.first->label(), Label<std::string>("aaa"));

  res2 = g1.AddEdge(2, 1, Label<std::string>("bbb"), 5);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 5);
  ASSERT_EQ(res2.first->label(), Label<std::string>("bbb"));

  res2 = g1.AddEdge(3, 2, Label<std::string>("bbb"), 6);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 6);
  ASSERT_EQ(res2.first->label(), Label<std::string>("bbb"));

  res2 = g1.AddEdge(4, 3, Label<std::string>("bbb"), 7);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 7);
  ASSERT_EQ(res2.first->label(), Label<std::string>("bbb"));

  res2 = g1.AddEdge(1, 4, Label<std::string>("bbb"), 8);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 8);
  ASSERT_EQ(res2.first->label(), Label<std::string>("bbb"));

  res2 = g1.AddEdge(1, 2, Label<std::string>("ccc"), 9);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 9);
  ASSERT_EQ(res2.first->label(), Label<std::string>("ccc"));

  res2 = g1.AddEdge(3, 4, Label<std::string>("ccc"), 10);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 10);
  ASSERT_EQ(res2.first->label(), Label<std::string>("ccc"));

  res2 = g1.AddEdge(3, 2, Label<std::string>("ccc"), 11);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 11);
  ASSERT_EQ(res2.first->label(), Label<std::string>("ccc"));

  res2 = g1.AddEdge(4, 1, Label<std::string>("ccc"), 12);
  ASSERT_TRUE(res2.second);
  ASSERT_FALSE(res2.first.IsNull());
  ASSERT_EQ(res2.first->id(), 12);
  ASSERT_EQ(res2.first->label(), Label<std::string>("ccc"));

  auto v = g1.FindVertex(0);
  ASSERT_TRUE(v.IsNull());

  auto v1 = g1.FindVertex(1);
  ASSERT_FALSE(v1.IsNull());
  ASSERT_EQ(v1->id(), 1);

  auto v2 = g1.FindVertex(2);
  ASSERT_FALSE(v2.IsNull());
  ASSERT_EQ(v2->id(), 2);

  auto e1 = v1->FindOutEdge(Label<std::string>("aaa"), v2, 1);
  ASSERT_FALSE(e1.IsNull());
  ASSERT_EQ(e1->id(), 1);
  ASSERT_EQ(e1->label(), Label<std::string>("aaa"));

  auto e1b = v2->FindInEdge(1);
  ASSERT_FALSE(e1b.IsNull());
  ASSERT_EQ(e1b->id(), 1);
  ASSERT_EQ(e1b->label(), Label<std::string>("aaa"));

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

  auto g2{g1};

  ASSERT_TRUE(true);
}