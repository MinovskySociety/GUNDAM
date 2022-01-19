#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/is_star.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

template <class GraphType>
void ConstructGraph0(GraphType& g){
  // 1 -> 2
  // /\   |
  // |    |
  // 3 <---

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph1(GraphType& g){
  // 1 <- 2
  // |    /\
  // V    |
  // 3 ----

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 3, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 2, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph2(GraphType& g){
  // 1 <- 2
  // |    
  // V     
  // 3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 3, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph3(GraphType& g){
  // 1 -> 2 -> 4
  // |  / |  / |
  // V /  V /  V
  // 3 -> 5 -> 7
  // |  / |  / |
  // V /  V /  V
  // 6 -> 8 -> 9

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(6, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(7, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(8, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(9, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(1, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(2, 3, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 2, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(2, 4, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 5, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 5, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 6, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(4, 5, 42, 9);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 4, 42, 10);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 6, 42, 11);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 5, 42, 12);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(4, 7, 42, 13);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 7, 42, 14);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 8, 42, 15);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 8, 42, 16);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(7, 8, 42, 17);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(8, 7, 42, 18);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(7, 9, 42, 19);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(8, 9, 42, 20);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph4(GraphType& g){
  // 1 -> 2 -> 4
  // |    |    |
  // V    V    V
  // 3 -> 5 -> 7
  // |    |    |
  // V    V    V
  // 6 -> 8 -> 9

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 1);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(6, 2);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(7, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(8, 3);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(9, 4);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 2, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(1, 3, 42, 2);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(2, 4, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 5, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 5, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 6, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(4, 7, 42, 13);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 7, 42, 14);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 8, 42, 15);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 8, 42, 16);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);

  res2 = g.AddEdge(7, 9, 42, 19);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(8, 9, 42, 20);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph5(GraphType& g){
  // 1 -> 2
  // |    
  // V     
  // 3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(1, 3, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(1, 2, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph6(GraphType& g){
  // 1 <- 2
  // /\    
  // |     
  // 3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(3, 1, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph7(GraphType& g){
  // 1 <- 2 <- 4
  // /\    
  // |     
  // 3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(3, 1, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(4, 2, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph8(GraphType& g){
  // 5 -> 1 <- 2 <- 4
  //      /\     
  //       |     
  //       3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(3, 1, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(4, 2, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 1, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph9(GraphType& g){
  // 5 -> 1 <- 2 -> 4
  //      /\     
  //       |     
  //       3     

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(3, 1, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 4, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 1, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}

template <class GraphType>
void ConstructGraph10(GraphType& g){
  // 5 -> 1 <- 2 <- 4
  //      /\        /\
  //       |        |
  //       3-> 6 -> 7   

  // clear up
  g = GraphType();
  // AddVertex
  auto res1 = g.AddVertex(1, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(2, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(3, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(4, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(5, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(6, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  res1 = g.AddVertex(7, 0);
  ASSERT_TRUE(res1.first);
  ASSERT_TRUE(res1.second);

  // AddEdge
  auto res2 = g.AddEdge(3, 1, 42, 1);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(2, 1, 42, 3);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(4, 2, 42, 4);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(5, 1, 42, 5);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(3, 6, 42, 6);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(6, 7, 42, 7);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
  res2 = g.AddEdge(7, 4, 42, 8);
  ASSERT_TRUE(res2.first);
  ASSERT_TRUE(res2.second);
}


template <typename GraphType>
void TestStar() {

  GraphType g;

  std::cout << "GraphType: " << typeid(g).name() << std::endl;

  // complete the test here
  //ASSERT_TRUE(false);

  ConstructGraph0(g);
  // bool ret_non_bi_0 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_0,
  //   central_vertex_non_bi_0] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_0);
  // ASSERT_FALSE(!end_points_non_bi_0.empty());
  // ASSERT_FALSE(central_vertex_non_bi_0);

  bool ret_bi_0 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_0,
    central_vertex_bi_0] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_0);
  ASSERT_FALSE(!end_points_bi_0.empty());
  ASSERT_FALSE(central_vertex_bi_0);

  ConstructGraph1(g);
  // bool ret_non_bi_1 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_1,
  //   central_vertex_non_bi_1] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_1);
  // ASSERT_FALSE(!end_points_non_bi_1.empty());
  // ASSERT_FALSE(central_vertex_non_bi_1);

  bool ret_bi_1 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_1,
    central_vertex_bi_1] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_1);
  ASSERT_FALSE(!end_points_bi_1.empty());
  ASSERT_FALSE(central_vertex_bi_1);

  ConstructGraph2(g);
  // bool ret_non_bi_2 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_2,
  //   central_vertex_non_bi_2] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_TRUE(ret_non_bi_2);
  // ASSERT_TRUE(!end_points_non_bi_2.empty());
  // ASSERT_TRUE(central_vertex_non_bi_2);
//   ASSERT_TRUE((end_points_non_bi_2 == g.FindVertex(2)
//             && central_vertex_non_bi_2 == g.FindVertex(3))
//            || (end_points_non_bi_2 == g.FindVertex(3)
//             && central_vertex_non_bi_2 == g.FindVertex(2)));

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
  // bool ret_non_bi_3 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_3,
  //   central_vertex_non_bi_3] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_3);
  // ASSERT_FALSE(!end_points_non_bi_3.empty());
  // ASSERT_FALSE(central_vertex_non_bi_3);

  bool ret_bi_3 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_3,
    central_vertex_bi_3] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_3);
  ASSERT_FALSE(!end_points_bi_3.empty());
  ASSERT_FALSE(central_vertex_bi_3);

  ConstructGraph4(g);
  // bool ret_non_bi_4 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_4,
  //   central_vertex_non_bi_4] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_4);
  // ASSERT_FALSE(!end_points_non_bi_4.empty());
  // ASSERT_FALSE(central_vertex_non_bi_4);

  bool ret_bi_4 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_4,
    central_vertex_bi_4] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_4);
  ASSERT_FALSE(!end_points_bi_4.empty());
  ASSERT_FALSE(central_vertex_bi_4);

  ConstructGraph5(g);
  // bool ret_non_bi_5 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_5,
  //   central_vertex_non_bi_5] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_5);
  // ASSERT_FALSE(!end_points_non_bi_5.empty());
  // ASSERT_FALSE(central_vertex_non_bi_5);

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
  // bool ret_non_bi_6 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_6,
  //   central_vertex_non_bi_6] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_6);
  // ASSERT_FALSE(!end_points_non_bi_6.empty());
  // ASSERT_FALSE(central_vertex_non_bi_6);

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
  // bool ret_non_bi_7 = GUNDAM::IsStar<false>(g);
  // auto [end_points_non_bi_7,
  //   central_vertex_non_bi_7] = GUNDAM::StarEndPoints<false>(g);
  // ASSERT_FALSE(ret_non_bi_7);
  // ASSERT_FALSE(!end_points_non_bi_7.empty());
  // ASSERT_FALSE(central_vertex_non_bi_7);

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
  bool ret_bi_10 = GUNDAM::IsStar<true>(g);
  auto [end_points_bi_10,
    central_vertex_bi_10] = GUNDAM::StarEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_10);
  ASSERT_TRUE(end_points_bi_10.empty());
  ASSERT_FALSE(central_vertex_bi_10);
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