#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/graph.h"
#include "gundam/large_graph.h"
#include "gundam/large_graph2.h"

#include "gundam/attribute.h"
#include "gundam/attribute2.h"

template<typename TestAttributeType>
void TestAttribute(TestAttributeType& attr) {
  using namespace GUNDAM;

//   AbstractValue *v1 = new ConcreteValue<int>(1);
//   AbstractValue *v2 = new ConcreteValue<double>(2.1);

//   auto *v1c = dynamic_cast<ConcreteValue<int> *>(v1);
//   ASSERT_NE(nullptr, v1c);
//   ASSERT_EQ(1, v1c->const_value());
//   ASSERT_EQ("1", v1c->value_str());

//   auto *v2c = dynamic_cast<ConcreteValue<double> *>(v2);
//   ASSERT_NE(nullptr, v2c);
//   ASSERT_EQ(2.1, v2c->const_value());
//   ASSERT_EQ("2.1", v2c->value_str());

//   delete v1;
//   delete v2;

  auto ret_ptr = attr.FindAttributePtr("a");
  ASSERT_TRUE(ret_ptr.IsNull());

  auto ret = attr.AddAttribute("a", 1);
  ASSERT_TRUE(ret.second);
  ASSERT_EQ(ret.first->template const_value<int>(), 1);
  ret.first->template value<int>() = 2;
  ASSERT_EQ(ret.first->template const_value<int>(), 2);
  ASSERT_EQ(ret.first->key(), "a");  
  ASSERT_EQ(ret.first->value_str(), "2");
  ASSERT_EQ(ret.first->value_type(), BasicDataType::kTypeInt);

  ret = attr.AddAttribute("b", std::string("ABC"));
  ASSERT_TRUE(ret.second);
  ASSERT_EQ(ret.first->template value<std::string>(), std::string("ABC"));
  ASSERT_EQ(ret.first->key(), "b");
  ASSERT_EQ(ret.first->value_str(), std::string("ABC"));
  ASSERT_EQ(ret.first->value_type(), BasicDataType::kTypeString);
  
  ret = attr.AddAttribute("a", std::string("ABC"));
  ASSERT_FALSE(ret.second);
  ASSERT_EQ(ret.first->template value<int>(), 2);

  int count = 0;
  for (auto iter = attr.AttributeCBegin(); !iter.IsDone(); iter++) {
    std::cout << iter->key() << " " << iter->value_str() << std::endl;
    count++;
  }
  ASSERT_EQ(2, count);

  ASSERT_EQ(BasicDataType::kTypeInt, attr.attribute_value_type("a"));
  ASSERT_EQ(BasicDataType::kTypeString, attr.attribute_value_type("b"));

  ASSERT_EQ(std::string("int"), attr.attribute_value_type_name("a"));
  ASSERT_EQ(std::string("string"), attr.attribute_value_type_name("b"));

  ASSERT_TRUE(
      attr.AddAttribute("XXX1", BasicDataType::kTypeString, "sss").second);
  ASSERT_TRUE(attr.AddAttribute("XXX2", BasicDataType::kTypeInt, "-5").second);
  ASSERT_TRUE(
      attr.AddAttribute("XXX3", BasicDataType::kTypeInt64, "555").second);
  ASSERT_TRUE(
      attr.AddAttribute("XXX4", BasicDataType::kTypeFloat, "0.55").second);
  ASSERT_TRUE(
      attr.AddAttribute("XXX5", BasicDataType::kTypeDouble, "0.55").second);
  ASSERT_TRUE(attr.AddAttribute("XXX6", BasicDataType::kTypeDateTime,
                                "2000-1-1 10:10:10")
                  .second);
  ASSERT_FALSE(
      attr.AddAttribute("XXX6", BasicDataType::kTypeUnknown, "abc").second);


  class TestUnknownType{
   public:
    TestUnknownType(){
      return;
    }
  };

  TestUnknownType unknown_type_value;
  
  ASSERT_TRUE(
      attr.AddAttribute("XXX7", unknown_type_value).second);

  ASSERT_EQ(BasicDataType::kTypeUnknown, attr.attribute_value_type("XXX7"));
  
  ASSERT_EQ("unknown value type", attr.FindAttributePtr("XXX7")->value_str());
}


TEST(TestGUNDAM, TestAttribute) {
  using namespace GUNDAM;
  using AttributeType1 =
    WithAttribute_<std::string, false, true, std::string,
                   ContainerType::Vector, 
                        SortType::Default>;

  using AttributeType2 =
        Attribute_<AttributeType::kSeparated,
                   false, int32_t,
                   std::string,
                   ContainerType::Vector,
                        SortType::Default>;

  using AttributeType3 =
        Attribute_<AttributeType::kGrouped,
                   false, int32_t,
                   std::string,
                   ContainerType::Vector,
                        SortType::Default>;

  AttributeType1  attr1;
  AttributeType2  attr2(1);
  auto attr3_ptr = new AttributeType3(1), // container_id 0
       attr4_ptr = new AttributeType3(1), // container_id 1
       attr5_ptr = new AttributeType3(1); // container_id 2

  TestAttribute(attr1);
  TestAttribute(attr2);
  TestAttribute(*attr3_ptr);
  TestAttribute(*attr4_ptr);
  TestAttribute(*attr5_ptr);
  delete attr5_ptr;
   // container_id 2
  attr5_ptr = new AttributeType3(1);
  TestAttribute(*attr5_ptr);
  delete attr5_ptr;
  delete attr3_ptr;
   // container_id 0
  attr3_ptr = new AttributeType3(1);
  TestAttribute(*attr3_ptr);
  delete attr3_ptr;
  delete attr4_ptr;
   // container_id 0
  attr4_ptr = new AttributeType3(1);
  TestAttribute(*attr4_ptr);
   // container_id 1
  attr3_ptr = new AttributeType3(1);
   // container_id 2
  attr5_ptr = new AttributeType3(1);
  return;
}

template <typename GraphType>
void TestGraphAttribute() {
  using namespace GUNDAM;       

  GraphType g;
  bool res;

  typename GraphType::VertexPtr v1, v2;
  std::tie(v1, res) = g.AddVertex(1, "1");
  ASSERT_TRUE(res);
  ASSERT_TRUE(v1);

  std::tie(v2, res) = g.AddVertex(2, "1");
  ASSERT_TRUE(res);
  ASSERT_TRUE(v2);

  typename GraphType::EdgePtr e1, e2;
  std::tie(e1, res) = g.AddEdge(1, 2, "a", 1);
  ASSERT_TRUE(res);
  ASSERT_TRUE(e1);

  std::tie(e2, res) = g.AddEdge(2, 1, "b", 2);
  ASSERT_TRUE(res);
  ASSERT_TRUE(e1);

  typename GraphType::VertexType::AttributePtr va1;
  std::tie(va1, res) = v1->AddAttribute(1, 1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va1.IsNull());

  typename GraphType::VertexType::AttributePtr va2;
  std::tie(va2, res) = g.FindVertex(1)->template AddAttribute<double>(2, 2.1);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va2.IsNull());

  typename GraphType::VertexType::AttributePtr va3;
  const char *str = "33";
  std::tie(va3, res) = v1->template AddAttribute<std::string>(3, str);
  ASSERT_TRUE(res);
  ASSERT_FALSE(va3.IsNull());
  int count = 0;

  auto res_pair = v1->AddAttribute(4, std::string{"44"});
  ASSERT_TRUE(res_pair.second);
  ASSERT_FALSE(res_pair.first.IsNull());
  ASSERT_EQ(std::string{"44"}, res_pair.first->template const_value<std::string>());

  res_pair = v1->AddAttribute(1, 5);
  ASSERT_FALSE(res_pair.second);
  ASSERT_FALSE(res_pair.first.IsNull());
  ASSERT_EQ(1, res_pair.first->template const_value<int>());

  for (auto it = v1->AttributeBegin(); !it.IsDone(); it++) {
    std::cout << "key = " << it->key() << std::endl
              << "value type = " << EnumToString(it->value_type()) << std::endl;
    ++count;
  }
  ASSERT_EQ(4, count);

  for (const auto &key : {1, 2, 3}) {
    typename GraphType::VertexType::AttributePtr va = v1->FindAttributePtr(key);

    std::cout << "key = " << va->key() << std::endl
              << "value type = " << EnumToString(va->value_type()) << std::endl;
  }

  // Attributes
  ASSERT_TRUE(v1->AddAttribute(5, std::string("abc")).second);

  auto v_attr = v1->FindAttributePtr(5);
  ASSERT_FALSE(v_attr.IsNull());
  ASSERT_EQ(v_attr->template value<std::string>(), "abc");

  ASSERT_FALSE(v1->template AddAttribute<std::string>(5, "abcd").second);

  ASSERT_TRUE(v1->template SetAttribute<std::string>(5, "abcd").second);

  auto v_cattr = v1->FindConstAttributePtr(5);
  ASSERT_FALSE(v_cattr.IsNull());
  ASSERT_EQ(v_cattr->template const_value<std::string>(), "abcd");

  ASSERT_FALSE(v1->template SetAttribute<std::string>(6, "abcde").second);

  ASSERT_TRUE(e1->template AddAttribute<std::string>(5, "abc").second);

  auto e_attr = e1->FindAttributePtr(5);
  ASSERT_FALSE(e_attr.IsNull());
  ASSERT_EQ(e_attr->template value<std::string>(), "abc");

  ASSERT_FALSE(e1->template AddAttribute<std::string>(5, "abcd").second);

  ASSERT_TRUE(e1->template SetAttribute<std::string>(5, "abcd").second);

  auto e_cattr = e1->FindConstAttributePtr(5);
  ASSERT_FALSE(e_cattr.IsNull());
  ASSERT_EQ(e_cattr->template const_value<std::string>(), "abcd");

  ASSERT_FALSE(e1->template SetAttribute<std::string>(6, "abcde").second);
}

TEST(TestGUNDAM, TestGraphAttribute) {
  using namespace GUNDAM;

  using G1 = LargeGraph<uint32_t, std::string, int, 
                        uint64_t, std::string, int>;
                        
  using G2 = LargeGraph2<uint32_t, std::string, int, 
                         uint64_t, std::string, int>;
                         
  using G3 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<int>, 
                   SetVertexAttributeStoreType<AttributeType::kSeparated>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<int>,
                   SetEdgeAttributeStoreType<AttributeType::kSeparated>>;
                         
  using G4 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<int>, 
                   SetVertexAttributeStoreType<AttributeType::kGrouped>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<int>,
                   SetEdgeAttributeStoreType<AttributeType::kSeparated>>;
                         
  using G5 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<int>, 
                   SetVertexAttributeStoreType<AttributeType::kSeparated>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<int>,
                   SetEdgeAttributeStoreType<AttributeType::kGrouped>>;
                         
  using G6 = Graph<SetVertexIDType<uint32_t>, 
                   SetVertexLabelType<std::string>,
                   SetVertexAttributeKeyType<int>, 
                   SetVertexAttributeStoreType<AttributeType::kGrouped>,
                   SetEdgeIDType<uint64_t>,
                   SetEdgeLabelType<std::string>, 
                   SetEdgeAttributeKeyType<int>,
                   SetEdgeAttributeStoreType<AttributeType::kGrouped>>;

  TestGraphAttribute<G1>();
  TestGraphAttribute<G2>();
  TestGraphAttribute<G3>();
  TestGraphAttribute<G4>();
  TestGraphAttribute<G5>();
  TestGraphAttribute<G6>();
}