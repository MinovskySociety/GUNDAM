#include <iostream>

#include "gtest/gtest.h"
#include "gundam/graph.h"

TEST(TestGUNDAM, TestAttribute) {
  using namespace GUNDAM;

  using G = Graph<>;

  G g;
  bool res;

  G::VertexPtr v1;
  std::tie(v1, res) = g.AddVertex(1, 1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(v1.IsNull());

  G::VertexAttributePtr va1;
  std::tie(va1, res) = v1->AddAttribute(1, 1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va1.IsNull());

  G::VertexAttributePtr va2;
  std::tie(va2, res) = g.FindVertex(1)->AddAttribute(2, 2.1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va2.IsNull());

  G::VertexAttributePtr va3;
  const char* str = "33";
  std::tie(va3, res) = v1->AddAttribute(3, str);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va3.IsNull());
  int count = 0;

  auto res_pair = v1->AddAttribute(4, std::string{"44"});
  ASSERT_TRUE(res_pair.second);
  ASSERT_FALSE(res_pair.first.IsNull());
  
  res_pair = v1->AddAttribute(1, 5);
  ASSERT_FALSE(res_pair.second);
  ASSERT_FALSE(res_pair.first.IsNull());
  ASSERT_EQ(1, res_pair.first->const_value<int>());

  for (G::EdgeAttributeIterator it = v1->AttributeBegin(); !it.IsDone(); it++) {
    std::cout << "key = " << it->key() << std::endl
              << "value type =" << EnumToString(it->value_type_id())
              << std::endl;
    ++count;
  }
  ASSERT_EQ(4, count);

  for (const auto& key : {1, 2, 3}) {
    G::VertexAttributePtr va = v1->FindAttributePtr(key);

    std::cout << "key = " << va->key() << std::endl
              << "value type = " << EnumToString(va->value_type_id())
              << std::endl;
  }
}