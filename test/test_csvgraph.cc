#include "gundam/csvgraph.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(TestGUNDAM, CSVGraph) {
  using namespace GUNDAM;
  
  char e_file[1000] =
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_e.csv";
  char v_file[1000] =
      "C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_n.csv";
  
  Graph<> test_graph;
  int res = ReadCSVGraph(test_graph, v_file, e_file);
  
  ASSERT_EQ(res, 1);
}