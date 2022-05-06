#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/algorithm/connected_component.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template<class GraphType>
void TestStrongConnectedComponent(){

  using StrongConnectedComponentIdType = std::vector<
                                         std::vector<typename GUNDAM::VertexID<GraphType>::type>>;

  StrongConnectedComponentIdType strong_connected_component_id;

  GraphType g;
  ConstructGraph12(g);
  StrongConnectedComponentIdType strong_connected_component_id_12 = {{0, 1, 2}, {3}, {4}};

  auto strong_connected_component_handle = GUNDAM::StrongConnectedComponent(g);

  strong_connected_component_id.clear();
  strong_connected_component_id.reserve(strong_connected_component_id_12.size());
  for (const auto& connected_component_handle 
          : strong_connected_component_handle) {
    std::vector<typename GUNDAM::VertexID<GraphType>::type> vertex_id_set;
    vertex_id_set.reserve(connected_component_handle.size());
    for (const auto& vertex_handle : connected_component_handle) {
      vertex_id_set.emplace_back(vertex_handle->id());
    }
    std::sort(vertex_id_set.begin(),
              vertex_id_set.end());
    strong_connected_component_id.emplace_back(std::move(vertex_id_set));
  }
  std::sort(strong_connected_component_id.begin(),
            strong_connected_component_id.end());
  ASSERT_EQ(strong_connected_component_id,
            strong_connected_component_id_12);

  ConstructGraph13(g);
  StrongConnectedComponentIdType strong_connected_component_id_13 = {{0, 1, 2, 3}, {4, 5, 6}, {7}};
  
  strong_connected_component_handle = GUNDAM::StrongConnectedComponent(g);
  strong_connected_component_id.clear();
  strong_connected_component_id.reserve(strong_connected_component_id_13.size());
  for (const auto& connected_component_handle 
          : strong_connected_component_handle) {
    std::vector<typename GUNDAM::VertexID<GraphType>::type> vertex_id_set;
    vertex_id_set.reserve(connected_component_handle.size());
    for (const auto& vertex_handle : connected_component_handle) {
      vertex_id_set.emplace_back(vertex_handle->id());
    }
    std::sort(vertex_id_set.begin(),
              vertex_id_set.end());
    strong_connected_component_id.emplace_back(std::move(vertex_id_set));
  }
  std::sort(strong_connected_component_id.begin(),
            strong_connected_component_id.end());
  ASSERT_EQ(strong_connected_component_id,
            strong_connected_component_id_13);

  ConstructGraph14(g);
  StrongConnectedComponentIdType strong_connected_component_id_14 = {{0, 1, 3}, {2, 4}, {5, 6, 7}};

  strong_connected_component_handle = GUNDAM::StrongConnectedComponent(g);
  strong_connected_component_id.clear();
  strong_connected_component_id.reserve(strong_connected_component_id_14.size());
  for (const auto& connected_component_handle 
          : strong_connected_component_handle) {
    std::vector<typename GUNDAM::VertexID<GraphType>::type> vertex_id_set;
    vertex_id_set.reserve(connected_component_handle.size());
    for (const auto& vertex_handle : connected_component_handle) {
      vertex_id_set.emplace_back(vertex_handle->id());
    }
    std::sort(vertex_id_set.begin(),
              vertex_id_set.end());
    strong_connected_component_id.emplace_back(std::move(vertex_id_set));
  }
  std::sort(strong_connected_component_id.begin(),
            strong_connected_component_id.end());
  ASSERT_EQ(strong_connected_component_id,
            strong_connected_component_id_14);
  return;
}

TEST(TestGUNDAM, TestStrongConnectedComponent) {
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

  TestStrongConnectedComponent<G1>();
  TestStrongConnectedComponent<G2>();
  TestStrongConnectedComponent<G3>();
  TestStrongConnectedComponent<G4>();
  TestStrongConnectedComponent<G5>();
  TestStrongConnectedComponent<G6>();
  TestStrongConnectedComponent<G7>();
}
