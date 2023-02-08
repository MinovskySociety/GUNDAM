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

  ConstructGraph0(g);
  source_set = {g.VertexBegin()};
  auto neighbor_0 = GUNDAM::GetNeighbors<GraphType>(source_set);
  auto neighbor_1 = GUNDAM::GetNeighbors<GraphType, true>(source_set);

  ASSERT_LE(neighbor_0.size(), neighbor_1.size());

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