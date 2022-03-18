#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/vertex_degree_filter.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template<class GraphType>
void TestVertexDegreeFilter() {
  GraphType g;

  using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;

  std::vector<VertexHandleType> qualified_vertex_set;

  ConstructGraph0(g);
  qualified_vertex_set = GUNDAM::VertexDegreeFilter<false>(g, 2);
  ASSERT_EQ(qualified_vertex_set.size(), 0);

  qualified_vertex_set.clear();
  qualified_vertex_set = GUNDAM::VertexDegreeFilter< true>(g, 2);
  ASSERT_EQ(qualified_vertex_set.size(), 3);

  ConstructGraph1(g);
  ConstructGraph2(g);
  ConstructGraph3(g);
  ConstructGraph4(g);
  ConstructGraph5(g);
  ConstructGraph6(g);

  return;
}

TEST(TestGUNDAM, TestVertexDegreeFilter) {
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

  TestVertexDegreeFilter<G1>();
  TestVertexDegreeFilter<G2>();
  TestVertexDegreeFilter<G3>();
  TestVertexDegreeFilter<G4>();
  TestVertexDegreeFilter<G5>();
  TestVertexDegreeFilter<G6>();
  TestVertexDegreeFilter<G7>();
}
