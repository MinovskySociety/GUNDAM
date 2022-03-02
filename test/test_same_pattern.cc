#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/same_pattern.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template <typename GraphType0,
          typename GraphType1>
void TestSamePattern() {

  GraphType0 g0;
  GraphType1 g1;

  ConstructGraph0(g0);
  ConstructGraph0(g1);
  ASSERT_TRUE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph0(g0);
  ConstructGraph1(g1);
  ASSERT_TRUE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph1(g0);
  ConstructGraph0(g1);
  ASSERT_TRUE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph1(g0);
  ConstructGraph1(g1);
  ASSERT_TRUE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph0(g0);
  ConstructGraph2(g1);
  ASSERT_FALSE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph2(g0);
  ConstructGraph0(g1);
  ASSERT_FALSE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph1(g0);
  ConstructGraph2(g1);
  ASSERT_FALSE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph2(g0);
  ConstructGraph1(g1);
  ASSERT_FALSE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph3(g0);
  ConstructGraph3(g1);
  ASSERT_TRUE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph3(g0);
  ConstructGraph4(g1);
  ASSERT_FALSE(GUNDAM::SamePattern(g0, g1));

  ConstructGraph4(g0);
  ConstructGraph3(g1);
  ASSERT_FALSE(GUNDAM::SamePattern(g0, g1));
}

TEST(TestGUNDAM, TestSamePattern) {
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

  TestSamePattern<G1, G1>();
  TestSamePattern<G1, G2>();
  TestSamePattern<G1, G3>();
  TestSamePattern<G1, G4>();
  TestSamePattern<G1, G5>();
  TestSamePattern<G1, G6>();
  TestSamePattern<G1, G7>();

  TestSamePattern<G2, G1>();
  TestSamePattern<G2, G2>();
  TestSamePattern<G2, G3>();
  TestSamePattern<G2, G4>();
  TestSamePattern<G2, G5>();
  TestSamePattern<G2, G6>();
  TestSamePattern<G2, G7>();

  TestSamePattern<G3, G1>();
  TestSamePattern<G3, G2>();
  TestSamePattern<G3, G3>();
  TestSamePattern<G3, G4>();
  TestSamePattern<G3, G5>();
  TestSamePattern<G3, G6>();
  TestSamePattern<G3, G7>();

  TestSamePattern<G4, G1>();
  TestSamePattern<G4, G2>();
  TestSamePattern<G4, G3>();
  TestSamePattern<G4, G4>();
  TestSamePattern<G4, G5>();
  TestSamePattern<G4, G6>();
  TestSamePattern<G4, G7>();

  TestSamePattern<G5, G1>();
  TestSamePattern<G5, G2>();
  TestSamePattern<G5, G3>();
  TestSamePattern<G5, G4>();
  TestSamePattern<G5, G5>();
  TestSamePattern<G5, G6>();
  TestSamePattern<G5, G7>();

  TestSamePattern<G6, G1>();
  TestSamePattern<G6, G2>();
  TestSamePattern<G6, G3>();
  TestSamePattern<G6, G4>();
  TestSamePattern<G6, G5>();
  TestSamePattern<G6, G6>();
  TestSamePattern<G6, G7>();

  TestSamePattern<G7, G1>();
  TestSamePattern<G7, G2>();
  // TestSamePattern<G7, G3>();
  TestSamePattern<G7, G4>();
  TestSamePattern<G7, G5>();
  TestSamePattern<G7, G6>();
  TestSamePattern<G7, G7>();
}