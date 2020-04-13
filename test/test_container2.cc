#include <iostream>
#include <string>
#include <tuple>

#include "gtest/gtest.h"

#include "gundam/container2.h"

template <class ContainerType>
inline void TestKVContainer(ContainerType &c) {
  c.Clear();
  ASSERT_EQ(0, c.Count());

  ASSERT_TRUE(c.Insert(5, "e").second);
  ASSERT_TRUE(c.Insert(1, "a").second);
  ASSERT_TRUE(c.Insert(3, "c").second);
  ASSERT_TRUE(c.Insert(2, "b").second);
  ASSERT_TRUE(c.Insert(4, "d").second);

  auto ret = c.Insert(5, "EE");
  ASSERT_FALSE(ret.second);
  ASSERT_EQ("e", ret.first->second);

  typename ContainerType::iterator it = c.Find(3);
  ASSERT_NE(c.end(), it);
  ASSERT_EQ(3, it->first);
  ASSERT_EQ("c", it->second);

  const ContainerType &c_const = c;
  auto it_const = c_const.Find(3);
  ASSERT_NE(c.cend(), it_const);
    
  ASSERT_TRUE(c.Erase(4));
  ASSERT_FALSE(c.Erase(4));
  ASSERT_TRUE(c.Insert(4, "D").second);

  ASSERT_EQ(5, c.Count());

  int sum_key = 0;
  int count = 0;
  for (auto it = c.cbegin(); it != c.cend(); ++it) {
    sum_key += it->first;
    ++count;

    std::cout << it->first << " " << it->second << std::endl;
  }
  ASSERT_EQ(15, sum_key);
  ASSERT_EQ(5, count);

  sum_key = 0;
  count = 0;
  for (auto it = c.begin(); it != c.end(); ++it) {
    sum_key += it->first;
    ++count;
  }
  ASSERT_EQ(15, sum_key);
  ASSERT_EQ(5, count);

  sum_key = 0;
  count = 0;
  for (auto p : c) {
    sum_key += p.first;
    ++count;
  }
  ASSERT_EQ(15, sum_key);
  ASSERT_EQ(5, count);

  sum_key = 0;
  count = 0;
  for (const auto &p : c) {
    sum_key += p.first;
    ++count;
  }
  ASSERT_EQ(15, sum_key);
  ASSERT_EQ(5, count);

  ContainerType c1{c};
  ASSERT_EQ(5, c1.Count());

  c1.Clear();
  ASSERT_EQ(0, c1.Count());

  ContainerType c2{std::move(c)};
  ASSERT_EQ(5, c2.Count());
}

TEST(TestGUNDAM, TestContainer2) {
  using namespace GUNDAM;

  using C1 = Vector<int, std::string>;
  using C2 = Map<int, std::string>;

  C1 c1;
  C2 c2;

  TestKVContainer(c1);
  TestKVContainer(c2);
}