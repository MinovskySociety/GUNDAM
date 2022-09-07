#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/topological/dag/is_acyclic.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template <typename GraphType>
void TestIsAcyclic() {
  GraphType g;

  std::cout << "GraphType: " << typeid(g).name() << std::endl;

  ConstructGraph0(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph1(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph2(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph3(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph4(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph5(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph6(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph7(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph8(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph9(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph10(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph11(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph12(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph13(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph14(g);
  ASSERT_FALSE(GUNDAM::IsAcyclic(g));

  ConstructGraph15(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph16(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));

  ConstructGraph17(g);
  ASSERT_TRUE (GUNDAM::IsAcyclic(g));
}


TEST(TestGUNDAM, TestIsAcyclic) {
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

  TestIsAcyclic<G1>();
  TestIsAcyclic<G2>();
  TestIsAcyclic<G3>();
  TestIsAcyclic<G4>();
  TestIsAcyclic<G5>();
  TestIsAcyclic<G6>();
  TestIsAcyclic<G7>();
  TestIsAcyclic<G8>();
}