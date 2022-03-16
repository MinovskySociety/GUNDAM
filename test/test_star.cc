#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/topological/star/is_star.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template <typename GraphType>
void TestStar() {
{
  GraphType g0;

  // 1(label 2) -> 2(label 4) -> 3 (label 5)

  auto res1 = g0.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g0.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  auto res2 = g0.AddEdge(1, 2, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g0.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  bool ret_no_bi_0 = GUNDAM::IsStar<false>(g0);
  auto [end_points_no_bi_0, centre_vertex_no_bi_0] =
      GUNDAM::StarEndPoints<false>(g0);
  ASSERT_TRUE(ret_no_bi_0);
  ASSERT_EQ(end_points_no_bi_0.size(), 2);
  ASSERT_FALSE(centre_vertex_no_bi_0);

  bool ret_bi_0 = GUNDAM::IsStar<true>(g0);
  auto [end_points_bi_0, centre_vertex_bi_0] =
      GUNDAM::StarEndPoints<true>(g0);
  ASSERT_TRUE(ret_bi_0);
  ASSERT_EQ(end_points_bi_0.size(), 2);
  ASSERT_FALSE(centre_vertex_bi_0);

  GraphType g1;

  // 1(label 2) <- 2(label 4) -> 3 (label 5)
  res1 = g1.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g1.AddVertex(3, 5);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g1.AddEdge(2, 1, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g1.AddEdge(2, 3, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  bool ret_no_bi_1 = GUNDAM::IsStar<false>(g1);
  auto [end_points_no_bi_1, centre_vertex_no_bi_1] =
      GUNDAM::StarEndPoints<false>(g1);
  ASSERT_TRUE(ret_no_bi_1);
  ASSERT_EQ(end_points_no_bi_1.size(), 2);
  ASSERT_TRUE(centre_vertex_no_bi_1);

  bool ret_bi_1 = GUNDAM::IsStar<true>(g1);
  auto [end_points_bi_1, centre_vertex_bi_1] =
      GUNDAM::StarEndPoints<true>(g1);
  ASSERT_TRUE(ret_bi_1);
  ASSERT_EQ(end_points_bi_1.size(), 2);
  ASSERT_FALSE(centre_vertex_bi_1);


  GraphType g2;

  // 1(label 2) <- 2(label 4) 
  //                  /\  |
  //                  |---|
  res1 = g2.AddVertex(1, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);
  res1 = g2.AddVertex(2, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res2 = g2.AddEdge(2, 1, 4, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g2.AddEdge(2, 2, 4, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);


  bool ret_no_bi_2 = GUNDAM::IsStar<false>(g2);
  auto [end_points_no_bi_2, centre_vertex_no_bi_2] =
      GUNDAM::StarEndPoints<false>(g2);
  ASSERT_FALSE(ret_no_bi_2);
  ASSERT_EQ(end_points_no_bi_2.size(), 0);
  ASSERT_FALSE(centre_vertex_no_bi_2);

  bool ret_bi_2 = GUNDAM::IsStar<true>(g2);
  auto [end_points_bi_2, centre_vertex_bi_2] =
      GUNDAM::StarEndPoints<true>(g2);
  ASSERT_FALSE(ret_bi_2);
  ASSERT_EQ(end_points_bi_2.size(), 0);
  ASSERT_FALSE(centre_vertex_bi_2);
}

  GraphType g;

  std::cout << "GraphType: " << typeid(g).name() << std::endl;

  // complete the test here
  //ASSERT_TRUE(false);

  ConstructGraph0(g);
  bool ret_non_bi_0 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_0,
    central_vertex_non_bi_0] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_0);
  ASSERT_TRUE(end_points_non_bi_0.empty());
  ASSERT_FALSE(central_vertex_non_bi_0);

  bool ret_bi_0 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_0,
    central_vertex_bi_0] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_0);
  ASSERT_FALSE(!end_points_bi_0.empty());
  ASSERT_FALSE(central_vertex_bi_0);

  ConstructGraph1(g);
  bool ret_non_bi_1 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_1,
    central_vertex_non_bi_1] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_1);
  ASSERT_TRUE(end_points_non_bi_1.empty());
  ASSERT_FALSE(central_vertex_non_bi_1);

  bool ret_bi_1 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_1,
    central_vertex_bi_1] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_1);
  ASSERT_FALSE(!end_points_bi_1.empty());
  ASSERT_FALSE(central_vertex_bi_1);

  ConstructGraph2(g);
  bool ret_non_bi_2 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_2,
    central_vertex_non_bi_2] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_TRUE(ret_non_bi_2);
  ASSERT_TRUE(!end_points_non_bi_2.empty());
  ASSERT_TRUE(!central_vertex_non_bi_2);
//  ASSERT_TRUE((end_points_non_bi_2 == g.FindVertex(2)
//            && central_vertex_non_bi_2 == g.FindVertex(3))
//            || (end_points_non_bi_2 == g.FindVertex(3)
//           && central_vertex_non_bi_2 == g.FindVertex(2)));

  bool ret_bi_2 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_2,
    central_vertex_bi_2] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_2);
  ASSERT_TRUE(!end_points_bi_2.empty());
  ASSERT_TRUE(!central_vertex_bi_2);
//   ASSERT_TRUE((end_points_bi_2 == g.FindVertex(2)
//             && central_vertex_bi_2 == g.FindVertex(3))
//            || (end_points_bi_2 == g.FindVertex(3)
//             && central_vertex_bi_2 == g.FindVertex(2)));

  ConstructGraph3(g);
  bool ret_non_bi_3 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_3,
    central_vertex_non_bi_3] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_3);
  ASSERT_TRUE(end_points_non_bi_3.empty());
  ASSERT_FALSE(central_vertex_non_bi_3);

  bool ret_bi_3 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_3,
    central_vertex_bi_3] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_3);
  ASSERT_FALSE(!end_points_bi_3.empty());
  ASSERT_FALSE(central_vertex_bi_3);

  ConstructGraph4(g);
  bool ret_non_bi_4 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_4,
    central_vertex_non_bi_4] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_4);
  ASSERT_TRUE(end_points_non_bi_4.empty());
  ASSERT_FALSE(central_vertex_non_bi_4);

  bool ret_bi_4 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_4,
    central_vertex_bi_4] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_4);
  ASSERT_FALSE(!end_points_bi_4.empty());
  ASSERT_FALSE(central_vertex_bi_4);

  ConstructGraph5(g);
  bool ret_non_bi_5 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_5,
    central_vertex_non_bi_5] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_TRUE(ret_non_bi_5);
  ASSERT_FALSE(end_points_non_bi_5.empty());
  ASSERT_TRUE(central_vertex_non_bi_5);

  bool ret_bi_5 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_5,
    central_vertex_bi_5] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_5);
  ASSERT_TRUE(!end_points_bi_5.empty());
  ASSERT_TRUE(!central_vertex_bi_5);
//   ASSERT_TRUE((end_points_bi_5 == g.FindVertex(2)
//             && central_vertex_bi_5 == g.FindVertex(3))
//            || (end_points_bi_5 == g.FindVertex(3)
//             && central_vertex_bi_5 == g.FindVertex(2)));

  ConstructGraph6(g);
  bool ret_non_bi_6 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_6,
    central_vertex_non_bi_6] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_6);
  ASSERT_TRUE(end_points_non_bi_6.empty());
  ASSERT_FALSE(central_vertex_non_bi_6);

  bool ret_bi_6 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_6,
    central_vertex_bi_6] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_6);
  ASSERT_TRUE(!end_points_bi_6.empty());
  ASSERT_TRUE(!central_vertex_bi_6);
//   ASSERT_TRUE((end_points_bi_6 == g.FindVertex(2)
//             && central_vertex_bi_6 == g.FindVertex(3))
//            || (end_points_bi_6 == g.FindVertex(3)
//             && central_vertex_bi_6 == g.FindVertex(2)));

  ConstructGraph7(g);
  bool ret_non_bi_7 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_7,
    central_vertex_non_bi_7] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_7);
  ASSERT_TRUE(end_points_non_bi_7.empty());
  ASSERT_FALSE(central_vertex_non_bi_7);

  bool ret_bi_7 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_7,
    central_vertex_bi_7] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_7);
  ASSERT_TRUE(!end_points_bi_7.empty());
  ASSERT_TRUE(!central_vertex_bi_7);
//   ASSERT_TRUE((end_points_bi_7 == g.FindVertex(4)
//             && central_vertex_bi_7 == g.FindVertex(3))
//            || (end_points_bi_7 == g.FindVertex(3)
//             && central_vertex_bi_7 == g.FindVertex(4)));



  ConstructGraph8(g);
  bool ret_non_bi_8 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_8,
    central_vertex_non_bi_8] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_8);
  ASSERT_TRUE(end_points_non_bi_8.empty());
  ASSERT_FALSE(central_vertex_non_bi_8);

  bool ret_bi_8 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_8,
    central_vertex_bi_8] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_8);
  ASSERT_TRUE(!end_points_bi_8.empty());
  ASSERT_TRUE( central_vertex_bi_8);
  ASSERT_EQ( end_points_bi_8.size(), 3);
  ASSERT_EQ( central_vertex_bi_8->id(), 1);
  std::sort(end_points_bi_8.begin(),
            end_points_bi_8.end());
  ASSERT_TRUE(std::binary_search(end_points_bi_8.begin(),
                                 end_points_bi_8.end(),
                                 g.FindVertex(3)));
  ASSERT_TRUE(std::binary_search(end_points_bi_8.begin(),
                                 end_points_bi_8.end(),
                                 g.FindVertex(4)));
  ASSERT_TRUE(std::binary_search(end_points_bi_8.begin(),
                                 end_points_bi_8.end(),
                                 g.FindVertex(5)));
//   ASSERT_TRUE((end_points_bi_7 == g.FindVertex(4)
//             && central_vertex_bi_7 == g.FindVertex(3))
//            || (end_points_bi_7 == g.FindVertex(3)
//             && central_vertex_bi_7 == g.FindVertex(4)));

  ConstructGraph9(g);
  bool ret_non_bi_9 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_9,
    central_vertex_non_bi_9] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_9);
  ASSERT_TRUE(end_points_non_bi_9.empty());
  ASSERT_FALSE(central_vertex_non_bi_9);

  bool ret_bi_9 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_9,
    central_vertex_bi_9] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_9);
  ASSERT_TRUE(!end_points_bi_9.empty());
  ASSERT_TRUE( central_vertex_bi_9);
  ASSERT_EQ( end_points_bi_9.size(), 3);
  ASSERT_EQ( central_vertex_bi_9->id(), 1);
  std::sort(end_points_bi_9.begin(),
            end_points_bi_9.end());
  ASSERT_TRUE(std::binary_search(end_points_bi_9.begin(),
                                 end_points_bi_9.end(),
                                 g.FindVertex(3)));
  ASSERT_TRUE(std::binary_search(end_points_bi_9.begin(),
                                 end_points_bi_9.end(),
                                 g.FindVertex(4)));
  ASSERT_TRUE(std::binary_search(end_points_bi_9.begin(),
                                 end_points_bi_9.end(),
                                 g.FindVertex(5)));


  ConstructGraph10(g);
  bool ret_non_bi_10 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_10,
    central_vertex_non_bi_10] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_10);
  ASSERT_TRUE(end_points_non_bi_10.empty());
  ASSERT_FALSE(central_vertex_non_bi_10);

  bool ret_bi_10 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_10,
    central_vertex_bi_10] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_10);
  ASSERT_TRUE(end_points_bi_10.empty());
  ASSERT_FALSE(central_vertex_bi_10);

  ConstructGraph11(g);
  bool ret_non_bi_11 = GUNDAM::IsStar<false>(g);
  auto [end_points_non_bi_11,
    central_vertex_non_bi_11] = GUNDAM::StarEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_11);
  ASSERT_TRUE(end_points_non_bi_11.empty());
  ASSERT_FALSE(central_vertex_non_bi_11);

  bool ret_bi_11 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_11,
    central_vertex_bi_11] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_11);
  ASSERT_TRUE(end_points_bi_11.empty());
  ASSERT_FALSE(central_vertex_bi_11);

}


TEST(TestGUNDAM, TestStar) {
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

  TestStar<G1>();
  TestStar<G2>();
  TestStar<G3>();
  TestStar<G4>();
  TestStar<G5>();
  TestStar<G6>();
  TestStar<G7>();
  TestStar<G8>();
}