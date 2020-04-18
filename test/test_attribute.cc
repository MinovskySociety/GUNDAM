#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/attribute.h"

TEST(TestGUNDAM, TestAttribute) {
  using namespace GUNDAM;

  AbstractValue *v1 = new ConcreteValue<int>(1);
  AbstractValue *v2 = new ConcreteValue<double>(2.1);

  auto *v1c = dynamic_cast<ConcreteValue<int> *>(v1);
  ASSERT_NE(nullptr, v1c);
  ASSERT_EQ(1, v1c->const_value());
  ASSERT_EQ("1", v1c->value_str());

  auto *v2c = dynamic_cast<ConcreteValue<double> *>(v2);
  ASSERT_NE(nullptr, v2c);
  ASSERT_EQ(2.1, v2c->const_value());
  ASSERT_EQ("2.1", v2c->value_str());

  delete v1;
  delete v2;

  using TestAttributeType =
      WithAttribute_<std::string, false, true, std::string,
                     ContainerType::Vector, SortType::None>;

  TestAttributeType attr;
  auto ret = attr.AddAttribute<int>("a", 1);
  ASSERT_TRUE(ret.second);
  ASSERT_EQ(ret.first->const_value<int>(), 1);
  ret.first->value<int>() = 2;
  ASSERT_EQ(ret.first->const_value<int>(), 2);
  ASSERT_EQ(ret.first->key(), "a");  
  ASSERT_EQ(ret.first->value_str(), "2");
  ASSERT_EQ(ret.first->value_type_id(), BasicDataType::kTypeInt);

  ret = attr.AddAttribute<std::string>("b", "ABC");
  ASSERT_TRUE(ret.second);
  ASSERT_EQ(ret.first->value<std::string>(), "ABC");
  ASSERT_EQ(ret.first->key(), "b");
  ASSERT_EQ(ret.first->value_str(), "ABC");
  ASSERT_EQ(ret.first->value_type_id(), BasicDataType::kTypeString);
  
  ret = attr.AddAttribute<std::string>("a", "ABC");
  ASSERT_FALSE(ret.second);
  ASSERT_EQ(ret.first->value<int>(), 2);

  int count = 0;
  for (auto iter = attr.AttributeCBegin(); !iter.IsDone(); iter++) {
    std::cout << iter->key() << " " << iter->value_str() << std::endl;        
    count++;
  }
  ASSERT_EQ(2, count);

  ASSERT_EQ(BasicDataType::kTypeInt, attr.attribute_value_type_id("a"));
  ASSERT_EQ(BasicDataType::kTypeString, attr.attribute_value_type_id("b"));

  ASSERT_EQ(std::string("int"), attr.attribute_value_type_name("a"));
  ASSERT_EQ(std::string("string"), attr.attribute_value_type_name("b"));
}