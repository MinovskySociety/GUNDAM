#include "gundam/csvgraph.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(TestGUNDAM, CSVGraph) {
  using namespace GUNDAM;
<<<<<<< HEAD
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
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using VertexPtr = typename GraphType::VertexPtr;
  using namespace GUNDAM;
  Graph<SetVertexHasAttribute<true>> g1;
  GUNDAM::ReadGraph(g1, v_list, e_list);
  GUNDAM::OutputGraph(g1);
  std::cout << "Output end!\n";
  ASSERT_TRUE(true);
=======
  
  char e_file[1000] =
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_e.csv";
  char v_file[1000] =
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_n.csv";
  
  Graph<> test_graph;
  int res = ReadCSVGraph(test_graph, v_file, e_file);
  
  ASSERT_EQ(res, 1);
>>>>>>> 8529d596831ced853948d00e80a7fe8aeb60923b
}