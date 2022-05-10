#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/algorithm/simulation/bisimulation.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template<class GraphType>
void TestBisimulation(){

  using EquivalentClassType = std::vector<
                              std::vector<typename GUNDAM::VertexHandle<GraphType>::type>>;

  EquivalentClassType equivalent_class_set;
  GraphType g;

  ConstructGraph18(g);
  GUNDAM::BisimulationGeneralCase(g, equivalent_class_set);

  EquivalentClassType equivalent_class_set_18 
    = { {g.FindVertex(0)},
        {g.FindVertex(1), g.FindVertex(3)},
        {g.FindVertex(2)},
        {g.FindVertex(4), g.FindVertex(5)},
        {g.FindVertex(6), g.FindVertex(7)} };

  for (auto& equivalent_class : equivalent_class_set_18) {
    std::sort(equivalent_class.begin(),
              equivalent_class.end());
  }
  std::sort(equivalent_class_set_18.begin(),
            equivalent_class_set_18.end());

  ASSERT_EQ(equivalent_class_set,
            equivalent_class_set_18);

  ConstructGraph19(g);
  GUNDAM::BisimulationGeneralCase(g, equivalent_class_set);

  EquivalentClassType equivalent_class_set_19
    = { {g.FindVertex(0)},
        {g.FindVertex(1), g.FindVertex(2)},
        {g.FindVertex(3), g.FindVertex(4),
         g.FindVertex(5), g.FindVertex(6)} };

  for (auto& equivalent_class : equivalent_class_set_19) {
    std::sort(equivalent_class.begin(),
              equivalent_class.end());
  }
  std::sort(equivalent_class_set_19.begin(),
            equivalent_class_set_19.end());

  ASSERT_EQ(equivalent_class_set,
            equivalent_class_set_19);

  ConstructGraph20(g);
  GUNDAM::BisimulationGeneralCase(g, equivalent_class_set);

  EquivalentClassType equivalent_class_set_20
    = { {g.FindVertex(0), g.FindVertex(1), g.FindVertex(2)} };

  for (auto& equivalent_class : equivalent_class_set_20) {
    std::sort(equivalent_class.begin(),
              equivalent_class.end());
  }
  std::sort(equivalent_class_set_20.begin(),
            equivalent_class_set_20.end());

  ASSERT_EQ(equivalent_class_set,
            equivalent_class_set_20);
  return;
}

TEST(TestGUNDAM, TestBisimulation) {
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

  TestBisimulation<G1>();
  TestBisimulation<G2>();
  TestBisimulation<G3>();
  TestBisimulation<G4>();
  TestBisimulation<G5>();
  TestBisimulation<G6>();
  TestBisimulation<G7>();
}
