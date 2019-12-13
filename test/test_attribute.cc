#include <iostream>
#include "gtest/gtest.h"
#include "gundam/graph.h"
TEST(TestGUNDAM, GoogleTest) { ASSERT_TRUE(true); }
TEST(TestGUNDAM, TestAttribute) {
  using namespace GUNDAM;
  Graph<> g;
  g.AddVertex(1, 1);
  g.FindVertex(1)->AddAttribute(1, 1);
  g.FindVertex(1)->AddAttribute(2, 2.1);
  g.FindVertex(1)->AddAttribute(3, (std::string) "33");
  std::cout << "iterator!" << std::endl;
  for (auto it = g.FindVertex(1)->AttributeBegin(); !it.IsDone(); it++) {
    std::cout << "key = " << it->key() << std::endl;
    std::cout << "value type =" << EnumToString(it->value_type_id())
              << std::endl;
  }
  std::cout << "ptr!" << std::endl;
  for (int i = 1; i <= 3; i++) {
    std::cout << "key = " << i << std::endl;
    std::cout << "value type = "
              << EnumToString(
                     g.FindVertex(1)->FindAttributePtr(i)->value_type_id())
              << std::endl;
  }
}