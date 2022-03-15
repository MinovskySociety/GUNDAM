#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/topological/star/star_cover_by_path.h"

#include "gundam/graph_type/graph_base.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

template<class GraphType>
void TestCoverByPath(){

  std::vector<GraphType> path_vec;
  GraphType p0;

  // 1(label 2) -> 2(label 4) -> 3 (label 5)

  auto res1 = p0.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p0.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p0.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  auto res2 = p0.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = p0.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec.emplace_back(p0);

  GraphType p1;

  // 1(label 3) -> 2(label 4) -> 3 (label 5)

  res1 = p1.AddVertex(1, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p1.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p1.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = p1.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = p1.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec.emplace_back(p1);

  GraphType p2;

  // 1(label 2) -> 2(label 7) -> 3 (label 7)

  res1 = p2.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p2.AddVertex(2, 7);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p2.AddVertex(3, 7);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = p2.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = p2.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec.emplace_back(p2);

  GraphType p3;

  // 1(label 1) -> 2(label 4) <- 3 (label 5)

  res1 = p3.AddVertex(1, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p3.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p3.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = p3.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = p3.AddEdge(3, 2, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec.emplace_back(p3);

  GraphType p4;

  // 1(label 2) -> 2(label 7) -> 3 (label 8)

  res1 = p4.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p4.AddVertex(2, 7);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p4.AddVertex(3, 8);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = p4.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = p4.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec.emplace_back(p4);

  GraphType p5;

  // 1(label 2) -> 2(label 9)

  res1 = p5.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = p5.AddVertex(2, 9);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = p5.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  path_vec.emplace_back(p5);

  GraphType g0;

  // 1(label 2) -> 2(label 4) -> 3 (label 5)

  res1 = g0.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g0.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);


  GraphType g1;

  // 1(label 9) -> 2(label 4) -> 3 (label 5)

  res1 = g1.AddVertex(1, 9);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g1.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);


  GraphType g2;
  // 1(label 5)

  res1 = g2.AddVertex(1, 5);

  GraphType g3;

  // 1(label 2) -> 2(label 4) <- 3 (label 5)

  res1 = g3.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g3.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g3.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g3.AddEdge(3, 2, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

// FixBug: dont take the label of edge into account!!
/*  GraphType g4;

  // 1(label 2) -> 2(label 4) -> 3 (label 5)

  res1 = g4.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g4.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g4.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g4.AddEdge(1, 2, 9, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g4.AddEdge(2, 3, 9, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);*/

  GraphType g5;

  // 6(label 7) -> 7(label 8)
  // /\
  // |
  // 1(label 2) -> 2(label 4) -> 3 (label 5)
  // |
  // \/
  // 4(label 4)-> 5(label 5)

  res1 = g5.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g5.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g5.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g5.AddVertex(4, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g5.AddVertex(5, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g5.AddVertex(6, 7);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g5.AddVertex(7, 8);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g5.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g5.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g5.AddEdge(1, 4, 4, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g5.AddEdge(4, 5, 4, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g5.AddEdge(1, 6, 4, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g5.AddEdge(6, 7, 4, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  GraphType g6;

  // 6(label 10) -> 7(label 8)
  // /\
  // |
  // 1(label 2) -> 2(label 4) -> 3 (label 5)
  // |
  // \/
  // 4(label 4)-> 5(label 5)

  res1 = g6.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g6.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g6.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g6.AddVertex(4, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g6.AddVertex(5, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g6.AddVertex(6, 10);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g6.AddVertex(7, 8);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g6.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g6.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g6.AddEdge(1, 4, 4, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g6.AddEdge(4, 5, 4, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g6.AddEdge(1, 6, 4, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g6.AddEdge(6, 7, 4, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);





  bool cover = false;

  cover = GUNDAM::StarCoverByPath(g0, path_vec);
  ASSERT_TRUE(cover);

  cover = GUNDAM::StarCoverByPath(g1, path_vec);
  ASSERT_FALSE(cover);

  cover = GUNDAM::StarCoverByPath(g2, path_vec);
  ASSERT_TRUE(cover);

  cover = GUNDAM::StarCoverByPath(g3, path_vec);
  ASSERT_FALSE(cover);

//  cover = GUNDAM::StarCoverByPath(g4, path_vec);
//  ASSERT_TRUE(cover);

  cover = GUNDAM::StarCoverByPath(g5, path_vec);
  ASSERT_TRUE(cover);

  cover = GUNDAM::StarCoverByPath(g6, path_vec);
  ASSERT_FALSE(cover);
  return;
}

TEST(TestGUNDAM, TestCoverByPath) {
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

  TestCoverByPath<G1>();
  TestCoverByPath<G2>();
  TestCoverByPath<G3>();
  TestCoverByPath<G4>();
  TestCoverByPath<G5>();
  TestCoverByPath<G6>();
  TestCoverByPath<G7>();
}
