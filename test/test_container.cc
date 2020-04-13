#include <iostream>
#include <string>
#include <tuple>

#include "gtest/gtest.h"

#include "gundam/container.h"

TEST(TestGUNDAM, TestContainer) {
  using namespace GUNDAM;

  Container<ContainerType::Vector, SortType::None, int, int, std::string> c1;

  auto r = c1.Insert(100);
  ASSERT_TRUE(r.second);
  ASSERT_EQ(100, std::get<0>(*r.first));

  ASSERT_TRUE(c1.Insert(101).second);
  ASSERT_TRUE(c1.Insert(102).second);
  ASSERT_TRUE(c1.Insert(103).second);
  ASSERT_TRUE(c1.Insert(104).second);

  r = c1.Insert(104);
  ASSERT_FALSE(r.second);
  ASSERT_EQ(104, std::get<0>(*r.first));

  std::get<1>(*r.first) = 1;
  std::get<2>(*r.first) = "AAA";
}