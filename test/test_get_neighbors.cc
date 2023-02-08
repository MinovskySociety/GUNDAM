#include "gtest/gtest.h"

#include "gundam/tool/get_neighbors.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template<class GraphType>
void TestGetNeighbors() {

  GraphType g;

  std::cout << "GraphType: " << typeid(g).name() << std::endl;

  std::vector<typename GUNDAM::VertexHandle<GraphType>::type> source_set;

  auto vertex_callback = [](const typename GUNDAM::VertexHandle<GraphType>::type&) -> bool {
    return true;
  };

  ConstructGraph0(g);
  source_set = {g.VertexBegin()};
  auto neighbor_vertex_0 = GUNDAM::GetNeighborVertexSet<GraphType      >(source_set);
  auto neighbor_vertex_1 = GUNDAM::GetNeighborVertexSet<GraphType, true>(source_set);
  auto neighbor_vertex_2 = GUNDAM::GetNeighborVertexSet<GraphType      >(source_set, vertex_callback);
  auto neighbor_vertex_3 = GUNDAM::GetNeighborVertexSet<GraphType, true>(source_set, vertex_callback);

  ASSERT_LE(neighbor_vertex_0.size(), neighbor_vertex_1.size());
  ASSERT_EQ(neighbor_vertex_2.size(), 0);
  ASSERT_EQ(neighbor_vertex_3.size(), 0);

  auto edge_callback = [](const typename GUNDAM::EdgeHandle<GraphType>::type&) -> bool {
    return true;
  };

  ConstructGraph0(g);
  source_set = {g.VertexBegin()};
  auto neighbor_edge_0 = GUNDAM::GetNeighborEdgeSet<GraphType      >(source_set);
  auto neighbor_edge_1 = GUNDAM::GetNeighborEdgeSet<GraphType, true>(source_set);
  auto neighbor_edge_2 = GUNDAM::GetNeighborEdgeSet<GraphType      >(source_set, edge_callback);
  auto neighbor_edge_3 = GUNDAM::GetNeighborEdgeSet<GraphType, true>(source_set, edge_callback);

  ASSERT_LE(neighbor_edge_0.size(), neighbor_edge_1.size());
  ASSERT_EQ(neighbor_edge_2.size(), 0);
  ASSERT_EQ(neighbor_edge_3.size(), 0);

  // wenzhi: todo
  // complete the test here
  return;
}


TEST(TestGUNDAM, TestGetNeighbors) {
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

  TestGetNeighbors<G1>();
  TestGetNeighbors<G2>();
  TestGetNeighbors<G3>();
  TestGetNeighbors<G4>();
  TestGetNeighbors<G5>();
  TestGetNeighbors<G6>();
  TestGetNeighbors<G7>();

  TestGetNeighbors<GraphBase<G1>>();
  TestGetNeighbors<GraphBase<G2>>();
  TestGetNeighbors<GraphBase<G3>>();
  TestGetNeighbors<GraphBase<G4>>();
  TestGetNeighbors<GraphBase<G5>>();
  TestGetNeighbors<GraphBase<G6>>();
  TestGetNeighbors<GraphBase<G7>>();
}