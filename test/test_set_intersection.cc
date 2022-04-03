#include <iostream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "gundam/tool/set_intersection.h"

void TestInterSectionWhenExceedThrehold() {
  /* manual case */
  using namespace GUNDAM;
  std::vector<int32_t> small, big;
  int32_t small_size = 50, big_size = 50000;
  int32_t max_value = 100000;
  for (int i = 0; i < small_size; i++) small.emplace_back(max_value - i);
  for (int i = 0; i < big_size; i++) big.emplace_back(max_value - i);
  std::reverse(small.begin(), small.end());
  std::reverse(big.begin(), big.end());

  std::vector<int32_t> res1, res2;
  auto tim_begin = clock();

  SetIntersection(small, big, std::inserter(res1, res1.begin()));
  auto tim_end = clock();
  std::cout << "GUNDAM set intersection time is "
            << (1.0 * tim_end - tim_begin) / CLOCKS_PER_SEC << std::endl;
  tim_begin = clock();
  std::set_intersection(small.begin(), small.end(), big.begin(), big.end(),
                        std::inserter(res2, res2.begin()));
  tim_end = clock();
  std::cout << "std set intersection time is "
            << (1.0 * tim_end - tim_begin) / CLOCKS_PER_SEC << std::endl;
  ASSERT_EQ(res1.size(), res2.size());

  // random case
}
TEST(TestGUNDAM, TestInterSectionWhenExceedThrehold) {
  TestInterSectionWhenExceedThrehold();
}