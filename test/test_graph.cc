#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gundam/small_graph.h"
#include "gundam/large_graph.h"
#include "gundam/large_graph2.h"
#include "gundam/graph.h"

//TEST(TestGUNDAM, TestGraph) {
//  using namespace GUNDAM;
//
//  using G0 = Graph<>;
//
//  using G1 = Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<std::string>,
//                   SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
//                   SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
//                   SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;
//
//  using G1A = Graph<SetVertexLabelType<std::string>, SetVertexIDType<uint32_t>,
//                    SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
//                    SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
//                    SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;
//
//  // using G2 = Graph<SetVertexIDType<uint64_t>,
//  // SetVertexLabelType<std::string>,
//  //                 SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
//  //                 SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
//  //                 SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;
//
//  std::cout << std::is_same<G1, G1A>::value << std::endl;
//  std::cout << std::is_constructible<G1, G1A>::value << std::endl;
//
//  G0 g0;
//
//  G1 g1;
//
//  G1 g1a{g1};
//
//  ASSERT_TRUE(true);
//}

template <class GraphType>
void TestGraphVertexEdge() {
  using namespace GUNDAM;                 

  GraphType g1;

  // AddVertex
  auto res1 = g1.AddVertex(1, "AAA");
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(1, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g1.AddVertex(2, std::string{"AAA"});
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(2, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g1.AddVertex(3, std::string("BBB"));
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(3, res1.first->id());
  ASSERT_EQ("BBB", res1.first->label());

  res1 = g1.AddVertex(4, "BBB");
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
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
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 1);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(2, 3, "aaa", 2);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 2);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(3, 4, "aaa", 3);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 3);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(4, 1, "aaa", 4);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 4);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g1.AddEdge(2, 1, "bbb", 5);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 5);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(3, 2, "bbb", 6);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 6);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(4, 3, "bbb", 7);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 7);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(1, 4, "bbb", 8);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 8);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g1.AddEdge(1, 2, "ccc", 9);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 9);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g1.AddEdge(3, 4, "ccc", 10);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 10);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g1.AddEdge(3, 2, "ccc", 11);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 11);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g1.AddEdge(4, 1, "ccc", 12);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 12);
  ASSERT_EQ(res2.first->label(), "ccc");

  auto v = g1.FindVertex(0);
  ASSERT_FALSE(v);

  auto v1 = g1.FindVertex(1);
  ASSERT_TRUE(v1);
  ASSERT_EQ(v1->id(), 1);

  auto v2 = g1.FindVertex(2);
  ASSERT_TRUE(v2);
  ASSERT_EQ(v2->id(), 2);
  
  //auto e1 = v1->FindOutEdge("aaa", v2, 1);
  //ASSERT_TRUE(e1);
  //ASSERT_EQ(e1->id(), 1);
  //ASSERT_EQ(e1->label(), "aaa");

  //auto e1b = v2->FindInEdge(1);
  //ASSERT_TRUE(e1b);
  //ASSERT_EQ(e1b->id(), 1);
  //ASSERT_EQ(e1b->label(), "aaa");

  //ASSERT_EQ(e1, e1b);

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

TEST(TestGUNDAM, TestGraphVertexEdge) {
  using namespace GUNDAM;

  using G0 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<std::string>, 
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<std::string>>;

  using G1 = LargeGraph<uint32_t, std::string, std::string, 
                        uint64_t, std::string, std::string>;

  using G2 = LargeGraph2<uint32_t, std::string, std::string, 
                         uint64_t, std::string, std::string>;  

  using G3 = SmallGraph<uint32_t, std::string, 
                        uint64_t, std::string>;

  TestGraphVertexEdge<G0>();
  TestGraphVertexEdge<G1>();
  TestGraphVertexEdge<G2>();
  TestGraphVertexEdge<G3>();

}