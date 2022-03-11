#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "gundam/tool/operator/merge_graph.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"
#include "gundam/tool/max_id.h"


template<class GraphType>
void TestMergeGraphs(){
  using VertexIDType = typename GraphType::VertexType::IDType;
  using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;
  using   EdgeHandleType = typename GUNDAM::  EdgeHandle<GraphType>::type;

  GraphType g0;
  // 1 -> 2 -> 3

  std::cout << "GraphType: " << typeid(g0).name() << std::endl;

  auto res1 = g0.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(2, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(3, 2);

  // AddEdge
  auto res2 = g0.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(2, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  GraphType g1;

  // 1 -> 5 -> 3


  res1 = g1.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(5, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(3, 2);

  // AddEdge
  res2 = g1.AddEdge(1, 5, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(5, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  GraphType g2;

  // 1 <-> 5 -> 3


  res1 = g2.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(5, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(3, 2);

  // AddEdge
  res2 = g2.AddEdge(1, 5, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(5, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(5, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  std::vector<GraphType> path_vec;
  path_vec.push_back(g0);
  path_vec.push_back(g1);

  std::vector<VertexIDType> id_vec;

  GraphType mg0 = GUNDAM::MergeGraphSet(path_vec, id_vec);
  ASSERT_EQ(mg0.CountVertex(), 6);
  ASSERT_EQ(mg0.CountEdge(), 4);
  id_vec.push_back(1);


/*  for (auto vertex_it = mg0.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    std::cout << "vertex " << vertex_it->id() << " label " << vertex_it->label() << std::endl;
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
              !out_edge_it.IsDone();
              out_edge_it++) {
      std::cout << "to " << (out_edge_it->dst_handle())->id() << " label " << (out_edge_it->dst_handle())->label()
                << " with edge label " << out_edge_it->label() << std::endl;
    }
  }*/

  GraphType mg1 = GUNDAM::MergeGraphSet(path_vec, id_vec);

  ASSERT_EQ(mg1.CountVertex(), 5);
  ASSERT_EQ(mg1.CountEdge(), 4);

/*  for (auto vertex_it = mg1.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    std::cout << "vertex " << vertex_it->id() << " label " << vertex_it->label() << std::endl;
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
              !out_edge_it.IsDone();
              out_edge_it++) {
      std::cout << "to " << (out_edge_it->dst_handle())->id() << " label " << (out_edge_it->dst_handle())->label()
                << " with edge label " << out_edge_it->label() << std::endl;
    }
  }*/


  GraphType mg2 = GUNDAM::MergeGraphSet(path_vec, id_vec);

  std::vector<GraphType> star_vec;
  star_vec.push_back(mg1);
  star_vec.push_back(mg0);

  id_vec.clear();
  GraphType mg3 = GUNDAM::MergeGraphSet(star_vec, id_vec);

/*  for (auto vertex_it = mg3.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    std::cout << "vertex " << vertex_it->id() << " label " << vertex_it->label() << std::endl;
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
              !out_edge_it.IsDone();
              out_edge_it++) {
      std::cout << "to " << (out_edge_it->dst_handle())->id() << " label " << (out_edge_it->dst_handle())->label()
                << " with edge label " << out_edge_it->label() << std::endl;
    }
  }*/


  ASSERT_EQ(mg3.CountVertex(), 11);
  ASSERT_EQ(mg3.CountEdge(), 8);

  path_vec.clear();
  id_vec.clear();
  path_vec.push_back(g2);
  path_vec.push_back(g2);
  path_vec.push_back(g1);
  path_vec.push_back(g0);

  GraphType mg4 = GUNDAM::MergeGraphSet(path_vec, id_vec);
  ASSERT_EQ(mg4.CountVertex(), 12);
  ASSERT_EQ(mg4.CountEdge(), 10);

  id_vec.push_back(1);

  GraphType mg5 = GUNDAM::MergeGraphSet(path_vec, id_vec);
  ASSERT_EQ(mg5.CountVertex(), 9);
  ASSERT_EQ(mg5.CountEdge(), 10);

  id_vec.push_back(5);

  path_vec.pop_back();

  GraphType mg6 = GUNDAM::MergeGraphSet(path_vec, id_vec);
  ASSERT_EQ(mg6.CountVertex(), 5);
  ASSERT_EQ(mg6.CountEdge(), 8);

  path_vec.push_back(g0);
  id_vec.pop_back();
  id_vec.push_back(3);


  GraphType mg7 = GUNDAM::MergeGraphSet(path_vec, id_vec);
  ASSERT_EQ(mg7.CountVertex(), 6);
  ASSERT_EQ(mg7.CountEdge(), 10);

  return;
}

TEST(TestGUNDAM, TestMergeGraphs) {
  using namespace GUNDAM;

  using G1 = LargeGraph<uint32_t, uint32_t, std::string, 
                        uint64_t, uint32_t, std::string>;

  using G2 = LargeGraph2<uint32_t, uint32_t, std::string, 
                         uint64_t, uint32_t, std::string>;  

  using G3 = SmallGraph<uint32_t, uint32_t, 
                        uint64_t, uint32_t>;

  using G4 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>, 
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>>;

  using G5 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>>;

  using G6 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

  using G7 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

  TestMergeGraphs<G1>();
  TestMergeGraphs<G2>();
  TestMergeGraphs<G3>();
  TestMergeGraphs<G4>();
  TestMergeGraphs<G5>();
  TestMergeGraphs<G6>();
  TestMergeGraphs<G7>();
}
