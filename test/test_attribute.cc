#include <iostream>
#include <string>

#include "gtest/gtest.h"

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