#include "gundam/csvgraph.h"
#include "gundam/util.h"

#include <iostream>
#include <string>

#include "gtest/gtest.h"

// TEST(TestGUNDAM, CSVGraph) {
//  using namespace GUNDAM;
//  std::vector<const char *> v_list, e_list;
//  v_list.push_back(
//      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-"
//      "pro\\testdata\\liantong_n_attr_phone.csv");
//  v_list.push_back(
//      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-"
//      "pro\\testdata\\liantong_n_attr_user.csv");
//  v_list.push_back(
//      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-"
//      "pro\\testdata\\liantong_n_attr_web.csv");
//  e_list.push_back(
//      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_"
//      "e.csv");
//
//  using GraphType = GUNDAM::Graph<>;
//  using VertexType = typename GraphType::VertexType;
//  using EdgeType = typename GraphType::EdgeType;
//  using VertexIDType = typename VertexType::IDType;
//  using VertexLabelType = typename VertexType::LabelType;
//  using EdgeLabelType = typename EdgeType::LabelType;
//  using VertexPtr = typename GraphType::VertexPtr;
//  using namespace GUNDAM;
//  Graph<SetVertexHasAttribute<true>> g1;
//  ReadCSVGraph(g1, v_list, e_list);
//  PrintGraph(g1);
//  std::cout << "Output end!\n";
//  ASSERT_TRUE(true);
//}

TEST(TestGUNDAM, WriteCSVGraph) {
  using namespace GUNDAM;

  using GraphType1 =
      Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint16_t>,
            SetEdgeIDType<uint32_t>, SetEdgeLabelType<std::string>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>,
            SetVertexAttributeKeyType<std::string>,
            SetEdgeAttributeKeyType<uint32_t>>;

  GraphType1 g1, g1a;

  // g1
  ASSERT_TRUE(g1.AddVertex(1, 0).second);
  ASSERT_TRUE(g1.AddVertex(2, 1).second);
  ASSERT_TRUE(g1.AddVertex(3, 1).second);
  ASSERT_TRUE(g1.AddEdge(1, 2, "10", 1).second);
  ASSERT_TRUE(g1.AddEdge(1, 3, "10", 2).second);

  auto v1 = g1.FindVertex(1);
  ASSERT_FALSE(v1.IsNull());
  ASSERT_TRUE(v1->AddAttribute<std::string>("name", "v1").second);

  auto e1 = g1.FindEdge(1);
  ASSERT_FALSE(e1.IsNull());
  ASSERT_TRUE(e1->AddAttribute<std::string>(100, "e1").second);

  PrintGraph(g1);

  ASSERT_TRUE(WriteCSVGraph(g1, "g1_v.csv", "g1_e.csv") >= 0);

  ASSERT_TRUE(ReadCSVGraph(g1a, std::vector<const char *>{"g1_v.csv"},
                           std::vector<const char *>{"g1_e.csv"}) >= 0);

  PrintGraph(g1a);
}