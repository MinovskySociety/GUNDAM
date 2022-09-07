#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "gundam/tool/same_pattern.h"
#include "gundam/tool/is_match_to.h"
#include "gundam/tool/operator/merge_vertex.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template<class GraphType>
void TestMergeVertex() {
  GraphType graph_15;
  ConstructGraph15(graph_15);
  std::vector<typename GUNDAM::VertexHandle<GraphType>::type> 
    vertex_handle_to_merge_15 = {graph_15.FindVertex(1),
                                 graph_15.FindVertex(7)};

  GraphType merged_graph_15;
  ConstructGraph16(merged_graph_15);

  auto temp_merged_graph_15 = GUNDAM::MergeVertex(graph_15, vertex_handle_to_merge_15);

  ASSERT_TRUE((GUNDAM::SamePattern(temp_merged_graph_15, 
                                        merged_graph_15)));
  GUNDAM::Match<GraphType,
                GraphType> match_15;
  for (auto vertex_it = temp_merged_graph_15.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    match_15.AddMap(vertex_it, merged_graph_15.FindVertex(vertex_it->id()));
  }
  ASSERT_TRUE(GUNDAM::IsMatchTo(temp_merged_graph_15,
                                     merged_graph_15, 
                                            match_15));
  ASSERT_TRUE(GUNDAM::IsMatchTo(merged_graph_15, 
                           temp_merged_graph_15,
                                       match_15.Reverse()));

             
  std::vector<typename GUNDAM::VertexHandle<GraphType>::type> 
    vertex_handle_to_merge_15_2 = {graph_15.FindVertex(4),
                                   graph_15.FindVertex(5)};

  GraphType merged_graph_15_2;
  ConstructGraph17(merged_graph_15_2);

  auto temp_merged_graph_15_2 = GUNDAM::MergeVertex(graph_15, vertex_handle_to_merge_15_2);

  ASSERT_TRUE((GUNDAM::SamePattern(temp_merged_graph_15_2, 
                                        merged_graph_15_2)));
  GUNDAM::Match<GraphType,
                GraphType> match_15_2;
  for (auto vertex_it = temp_merged_graph_15_2.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    match_15_2.AddMap(vertex_it, merged_graph_15_2.FindVertex(vertex_it->id()));
  }
  ASSERT_TRUE(GUNDAM::IsMatchTo(temp_merged_graph_15_2,
                                     merged_graph_15_2, 
                                            match_15_2));
  ASSERT_TRUE(GUNDAM::IsMatchTo(merged_graph_15_2, 
                           temp_merged_graph_15_2,
                                       match_15_2.Reverse()));
  return;
}

TEST(TestGUNDAM, TestMergeVertex) {
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

  TestMergeVertex<G1>();
  TestMergeVertex<G2>();
  TestMergeVertex<G3>();
  TestMergeVertex<G4>();
  TestMergeVertex<G5>();
  TestMergeVertex<G6>();
  TestMergeVertex<G7>();
}
