#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/same_pattern.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template <typename GraphType>
void TestMove() {
  GraphType g0, g1;

  std::cout << "GraphType: " << typeid(g0).name() << std::endl;

  // complete the test here
  //ASSERT_TRUE(false);

  std::vector<typename GUNDAM::VertexHandle<GraphType>::type> vertex_handle_set;

  std::vector<typename GUNDAM::VertexHandle<const GraphType>::type> vertex_const_handle_set;

  ConstructGraph18(g0);

  const GraphType& g0_cref = g0;

  for (auto vertex_it = g0.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    vertex_handle_set.emplace_back(vertex_it);
  }

  for (auto vertex_it = g0_cref.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    vertex_const_handle_set.emplace_back(vertex_it);
  }

  g1 = std::move(g0);

  for (const auto& vertex_handle 
                 : vertex_handle_set) {
    ASSERT_NE(&g1, &g0);
    ASSERT_TRUE(g1.FindVertex(vertex_handle->id())
                           == vertex_handle);
  }

  for (const auto& vertex_const_handle 
                 : vertex_const_handle_set) {
    ASSERT_NE(&g1, &g0);
    ASSERT_TRUE(g1.FindVertex(vertex_const_handle->id())
                           == vertex_const_handle);
  }

  ConstructGraph18(g0);

  ASSERT_TRUE(GUNDAM::SamePattern(g0, g1));
}


TEST(TestGUNDAM, TestMove) {
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

  using G8 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<uint32_t>,
                   SetVertexAttributeKeyType<std::string>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<uint32_t>, 
                   SetEdgeAttributeKeyType<std::string>, 
                   SetVertexLabelContainerType<GUNDAM::ContainerType::Map>,
                   SetVertexIDContainerType<GUNDAM::ContainerType::Map>, 
                   SetVertexPtrContainerType<GUNDAM::ContainerType::Map>,
                   SetEdgeLabelContainerType<GUNDAM::ContainerType::Map>>;

  TestMove<G1>();
  TestMove<G2>();
  TestMove<G3>();
  TestMove<G4>();
  TestMove<G5>();
  TestMove<G6>();
  TestMove<G7>();
  TestMove<G8>();
}