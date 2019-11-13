#include "gundam/csvgraph.h"
#include "gundam/util.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(TestGUNDAM, CSVGraph) {
  using namespace GUNDAM;
  std::vector<char *> v_list, e_list;
  v_list.push_back(
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-"
      "pro\\testdata\\liantong_n_attr_phone.csv");
  v_list.push_back(
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-"
      "pro\\testdata\\liantong_n_attr_user.csv");
  v_list.push_back(
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-"
      "pro\\testdata\\liantong_n_attr_web.csv");
  e_list.push_back(
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_"
      "e.csv");

  using GraphType = GUNDAM::Graph<>;
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;  
  using EdgeLabelType = typename EdgeType::LabelType;  
  using VertexPtr = typename GraphType::VertexPtr;
  using namespace GUNDAM;
  Graph<SetVertexHasAttribute<true>> g1;
  ReadCSVGraph(g1, v_list, e_list);
  PrintGraph(g1);
  std::cout << "Output end!\n";
  ASSERT_TRUE(true);
}