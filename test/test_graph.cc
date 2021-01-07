#include <iostream>
#include <string>

#include "gtest/gtest.h"
#include "gundam/small_graph.h"
#include "gundam/large_graph.h"
#include "gundam/large_graph2.h"
#include "gundam/graph.h"

template<class GraphType>
void TestAddVertexAddEdge(GraphType& g){

  // AddVertex
  auto res1 = g.AddVertex(1, "AAA");
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(1, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g.AddVertex(2, std::string{"AAA"});
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(2, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g.AddVertex(3, std::string("BBB"));
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(3, res1.first->id());
  ASSERT_EQ("BBB", res1.first->label());

  res1 = g.AddVertex(4, "BBB");
  ASSERT_TRUE(res1.second);
  ASSERT_TRUE(res1.first);
  ASSERT_EQ(4, res1.first->id());
  ASSERT_EQ("BBB", res1.first->label());

  res1 = g.AddVertex(1, "AAA");
  ASSERT_FALSE(res1.second);
  ASSERT_EQ(1, res1.first->id());
  ASSERT_EQ("AAA", res1.first->label());

  res1 = g.AddVertex(3, "AAA");
  ASSERT_FALSE(res1.second);
  ASSERT_EQ(3, res1.first->id());
  ASSERT_TRUE(res1.first->label() == "BBB");

  // AddEdge
  auto res2 = g.AddEdge(1, 2, "aaa", 1);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 1);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g.AddEdge(2, 3, "aaa", 2);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 2);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g.AddEdge(3, 4, "aaa", 3);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 3);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g.AddEdge(4, 1, "aaa", 4);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 4);
  ASSERT_EQ(res2.first->label(), "aaa");

  res2 = g.AddEdge(2, 1, "bbb", 5);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 5);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g.AddEdge(3, 2, "bbb", 6);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 6);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g.AddEdge(4, 3, "bbb", 7);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 7);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g.AddEdge(1, 4, "bbb", 8);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 8);
  ASSERT_EQ(res2.first->label(), "bbb");

  res2 = g.AddEdge(1, 2, "ccc", 9);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 9);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g.AddEdge(3, 4, "ccc", 10);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 10);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g.AddEdge(3, 2, "ccc", 11);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 11);
  ASSERT_EQ(res2.first->label(), "ccc");

  res2 = g.AddEdge(4, 1, "ccc", 12);
  ASSERT_TRUE(res2.second);
  ASSERT_TRUE(res2.first);
  ASSERT_EQ(res2.first->id(), 12);
  ASSERT_EQ(res2.first->label(), "ccc");
}

template<class GraphType>
void TestConstGraph(const GraphType& g){

  using VertexPtr      = typename GraphType::VertexPtr;
  using VertexConstPtr = typename GraphType::VertexConstPtr;

  using EdgePtr      = typename GraphType::EdgePtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;

  auto v = g.FindVertex(0);
  ASSERT_FALSE(v);

  auto v1 = g.FindVertex(1);
  ASSERT_TRUE(v1);
  ASSERT_EQ(v1->id(), 1);

  auto v2 = g.FindVertex(2);
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
  for (auto v_it = g.VertexCBegin(); 
           !v_it.IsDone(); 
          ++v_it) {
    VertexConstPtr vertex_const_ptr = v_it;
    ASSERT_EQ(vertex_const_ptr->id(), v_it->id());
    ++count;
  }
  ASSERT_EQ(count, 4);
  
  count = 0;
  for (auto v_it = g.VertexBegin(); 
           !v_it.IsDone(); 
          ++v_it) {
    VertexConstPtr vertex_const_ptr = v_it;
    ASSERT_EQ(vertex_const_ptr->id(), v_it->id());
    ++count;
  }
  ASSERT_EQ(count, 4);

  count = 0;
  for (auto e_out_it = v1->OutEdgeCBegin(); 
           !e_out_it.IsDone(); 
          ++e_out_it) {
    // EdgeConstPtr edge_const_ptr = e_out_it;
    // ASSERT_EQ(edge_const_ptr->const_src_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_out_it = v1->OutEdgeBegin(); 
           !e_out_it.IsDone(); 
          ++e_out_it) {
    // EdgeConstPtr edge_const_ptr = e_out_it;
    // ASSERT_EQ(edge_const_ptr->const_src_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_in_it = v1->InEdgeCBegin(); 
           !e_in_it.IsDone(); 
          ++e_in_it) {
    // EdgeConstPtr edge_const_ptr = e_in_it;
    // ASSERT_EQ(edge_const_ptr->const_dst_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_in_it = v1->InEdgeBegin(); 
           !e_in_it.IsDone(); 
          ++e_in_it) {
    // EdgeConstPtr edge_const_ptr = e_in_it;
    // ASSERT_EQ(edge_const_ptr->const_dst_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);
}

template <class GraphType>
void TestGraphVertexEdge() {
  using namespace GUNDAM;                 

  using VertexPtr      = typename GraphType::VertexPtr;
  using VertexConstPtr = typename GraphType::VertexConstPtr;

  using EdgePtr      = typename GraphType::EdgePtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;

  GraphType g1;

  TestAddVertexAddEdge(g1);

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
  for (auto v_it = g1.VertexCBegin(); 
           !v_it.IsDone(); 
          ++v_it) {
    VertexConstPtr vertex_const_ptr = v_it;
    ASSERT_EQ(vertex_const_ptr->id(), v_it->id());
    ++count;
  }
  ASSERT_EQ(count, 4);
  
  count = 0;
  for (auto v_it = g1.VertexBegin(); 
           !v_it.IsDone(); 
          ++v_it) {
    VertexPtr vertex_ptr = v_it;
    ASSERT_EQ(vertex_ptr->id(), v_it->id());
    // VertexConstPtr vertex_const_ptr = v_it;
    // ASSERT_EQ(vertex_const_ptr->id(), v_it->id());
    ++count;
  }
  ASSERT_EQ(count, 4);

  count = 0;
  for (auto e_out_it = v1->OutEdgeCBegin(); 
           !e_out_it.IsDone(); 
          ++e_out_it) {
    // EdgeConstPtr edge_const_ptr = e_out_it;
    // ASSERT_EQ(edge_const_ptr->const_src_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_out_it = v1->OutEdgeBegin(); 
           !e_out_it.IsDone(); 
          ++e_out_it) {
    // EdgePtr edge_ptr = e_out_it;
    // ASSERT_EQ(edge_ptr->src_ptr(), v1);
    // EdgeConstPtr edge_const_ptr = e_out_it;
    // ASSERT_EQ(edge_const_ptr->const_src_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_in_it = v1->InEdgeCBegin(); 
           !e_in_it.IsDone(); 
          ++e_in_it) {
    // EdgeConstPtr edge_const_ptr = e_in_it;
    // ASSERT_EQ(edge_const_ptr->const_dst_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  count = 0;
  for (auto e_in_it = v1->InEdgeBegin(); 
           !e_in_it.IsDone(); 
          ++e_in_it) {
    // EdgePtr edge_ptr = e_in_it;
    // ASSERT_EQ(edge_ptr->dst_ptr(), v1);
    // EdgeConstPtr edge_const_ptr = e_in_it;
    // ASSERT_EQ(edge_const_ptr->const_dst_ptr(), v1);
    ++count;
  }
  ASSERT_EQ(count, 3);

  TestConstGraph(g1);
}

TEST(TestGUNDAM, TestGraphVertexEdge) {
  using namespace GUNDAM;

  using G1 = LargeGraph<uint32_t, std::string, std::string, 
                        uint64_t, std::string, std::string>;

  using G2 = LargeGraph2<uint32_t, std::string, std::string, 
                         uint64_t, std::string, std::string>;  

  using G3 = SmallGraph<uint32_t, std::string, 
                        uint64_t, std::string>;

  using G4 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<std::string>, 
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<std::string>>;

  using G5 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>>;

  using G6 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

  using G7 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

  TestGraphVertexEdge<G1>();
  TestGraphVertexEdge<G2>();
  TestGraphVertexEdge<G3>();
  TestGraphVertexEdge<G4>();
  TestGraphVertexEdge<G5>();
  TestGraphVertexEdge<G6>();
  TestGraphVertexEdge<G7>();
}