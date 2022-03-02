#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/tool/is_link.h"

#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/graph.h"

#include "test_data/test_pattern_set.h"

template <typename GraphType>
void TestLink() {

  GraphType g;

  std::cout << "GraphType: " << typeid(g).name() << std::endl;

  ConstructGraph0(g);
  bool ret_non_bi_0 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_0,
        dst_handle_non_bi_0] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_0);
  ASSERT_FALSE(src_handle_non_bi_0);
  ASSERT_FALSE(dst_handle_non_bi_0);

  bool ret_bi_0 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_0,
        dst_handle_bi_0] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_0);
  ASSERT_FALSE(src_handle_bi_0);
  ASSERT_FALSE(dst_handle_bi_0);

  ConstructGraph1(g);
  bool ret_non_bi_1 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_1,
        dst_handle_non_bi_1] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_1);
  ASSERT_FALSE(src_handle_non_bi_1);
  ASSERT_FALSE(dst_handle_non_bi_1);

  bool ret_bi_1 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_1,
        dst_handle_bi_1] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_1);
  ASSERT_FALSE(src_handle_bi_1);
  ASSERT_FALSE(dst_handle_bi_1);

  ConstructGraph2(g);
  bool ret_non_bi_2 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_2,
        dst_handle_non_bi_2] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_TRUE(ret_non_bi_2);
  ASSERT_TRUE(src_handle_non_bi_2);
  ASSERT_TRUE(dst_handle_non_bi_2);
  ASSERT_TRUE((src_handle_non_bi_2 == g.FindVertex(2)
            && dst_handle_non_bi_2 == g.FindVertex(3))
           || (src_handle_non_bi_2 == g.FindVertex(3)
            && dst_handle_non_bi_2 == g.FindVertex(2)));

  bool ret_bi_2 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_2,
        dst_handle_bi_2] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_2);
  ASSERT_TRUE(src_handle_bi_2);
  ASSERT_TRUE(dst_handle_bi_2);
  ASSERT_TRUE((src_handle_bi_2 == g.FindVertex(2)
            && dst_handle_bi_2 == g.FindVertex(3))
           || (src_handle_bi_2 == g.FindVertex(3)
            && dst_handle_bi_2 == g.FindVertex(2)));

  ConstructGraph3(g);
  bool ret_non_bi_3 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_3,
        dst_handle_non_bi_3] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_3);
  ASSERT_FALSE(src_handle_non_bi_3);
  ASSERT_FALSE(dst_handle_non_bi_3);

  bool ret_bi_3 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_3,
        dst_handle_bi_3] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_3);
  ASSERT_FALSE(src_handle_bi_3);
  ASSERT_FALSE(dst_handle_bi_3);

  ConstructGraph4(g);
  bool ret_non_bi_4 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_4,
        dst_handle_non_bi_4] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_4);
  ASSERT_FALSE(src_handle_non_bi_4);
  ASSERT_FALSE(dst_handle_non_bi_4);

  bool ret_bi_4 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_4,
        dst_handle_bi_4] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_FALSE(ret_bi_4);
  ASSERT_FALSE(src_handle_bi_4);
  ASSERT_FALSE(dst_handle_bi_4);

  ConstructGraph5(g);
  bool ret_non_bi_5 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_5,
        dst_handle_non_bi_5] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_5);
  ASSERT_FALSE(src_handle_non_bi_5);
  ASSERT_FALSE(dst_handle_non_bi_5);

  bool ret_bi_5 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_5,
        dst_handle_bi_5] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_5);
  ASSERT_TRUE(src_handle_bi_5);
  ASSERT_TRUE(dst_handle_bi_5);
  ASSERT_TRUE((src_handle_bi_5 == g.FindVertex(2)
            && dst_handle_bi_5 == g.FindVertex(3))
           || (src_handle_bi_5 == g.FindVertex(3)
            && dst_handle_bi_5 == g.FindVertex(2)));

  ConstructGraph6(g);
  bool ret_non_bi_6 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_6,
        dst_handle_non_bi_6] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_6);
  ASSERT_FALSE(src_handle_non_bi_6);
  ASSERT_FALSE(dst_handle_non_bi_6);

  bool ret_bi_6 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_6,
        dst_handle_bi_6] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_6);
  ASSERT_TRUE(src_handle_bi_6);
  ASSERT_TRUE(dst_handle_bi_6);
  ASSERT_TRUE((src_handle_bi_6 == g.FindVertex(2)
            && dst_handle_bi_6 == g.FindVertex(3))
           || (src_handle_bi_6 == g.FindVertex(3)
            && dst_handle_bi_6 == g.FindVertex(2)));

  ConstructGraph7(g);
  bool ret_non_bi_7 = GUNDAM::IsLink<false>(g);
  auto [src_handle_non_bi_7,
        dst_handle_non_bi_7] = GUNDAM::LinkEndPoints<false>(g);
  ASSERT_FALSE(ret_non_bi_7);
  ASSERT_FALSE(src_handle_non_bi_7);
  ASSERT_FALSE(dst_handle_non_bi_7);

  bool ret_bi_7 = GUNDAM::IsLink<true>(g);
  auto [src_handle_bi_7,
        dst_handle_bi_7] = GUNDAM::LinkEndPoints<true>(g);
  ASSERT_TRUE(ret_bi_7);
  ASSERT_TRUE(src_handle_bi_7);
  ASSERT_TRUE(dst_handle_bi_7);
  ASSERT_TRUE((src_handle_bi_7 == g.FindVertex(4)
            && dst_handle_bi_7 == g.FindVertex(3))
           || (src_handle_bi_7 == g.FindVertex(3)
            && dst_handle_bi_7 == g.FindVertex(4)));
}

TEST(TestGUNDAM, TestLink) {
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

  TestLink<G1>();
  TestLink<G2>();
  TestLink<G3>();
  TestLink<G4>();
  TestLink<G5>();
  TestLink<G6>();
  TestLink<G7>();
}