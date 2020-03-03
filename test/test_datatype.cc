#include "gtest/gtest.h"

#include "gundam/datetime.h"
#include "gundam/datatype.h"

TEST(TestGUNDAM, TestDataType) {
  using namespace GUNDAM;

  DateTime t("1984-5-23 19:3:1");

  ASSERT_EQ("1984-05-23 19:03:01", t.to_string());

  ASSERT_EQ(BasicDataType::kTypeInt, StringToEnum("int"));
  ASSERT_EQ(BasicDataType::kTypeDouble, StringToEnum("double"));
  ASSERT_EQ(BasicDataType::kTypeString, StringToEnum("string"));
  ASSERT_EQ(BasicDataType::kTypeDateTime, StringToEnum("datetime"));
  ASSERT_EQ(BasicDataType::kTypeUnknown, StringToEnum("unknown"));
  ASSERT_EQ(BasicDataType::kTypeUnknown, StringToEnum("xxx"));

  ASSERT_EQ(std::string("int"), EnumToString(BasicDataType::kTypeInt));
  ASSERT_EQ(std::string("double"), EnumToString(BasicDataType::kTypeDouble));
  ASSERT_EQ(std::string("string"), EnumToString(BasicDataType::kTypeString));
  ASSERT_EQ(std::string("datetime"), EnumToString(BasicDataType::kTypeDateTime));
  ASSERT_EQ(std::string("unknown"), EnumToString(BasicDataType::kTypeUnknown));

  ASSERT_EQ(std::string("int"), TypeToString<int>());
  ASSERT_EQ(std::string("double"), TypeToString<double>());
  ASSERT_EQ(std::string("string"), TypeToString<std::string>());
  ASSERT_EQ(std::string("datetime"), TypeToString<DateTime>());
  ASSERT_EQ(std::string("unknown"), TypeToString<std::vector<int>>());

  ASSERT_EQ(std::string("int"), TypeToString(int()));
  ASSERT_EQ(std::string("double"), TypeToString(double()));
  ASSERT_EQ(std::string("string"), TypeToString(std::string()));
  ASSERT_EQ(std::string("datetime"), TypeToString(DateTime()));
  ASSERT_EQ(std::string("unknown"), TypeToString(std::vector<int>()));

  ASSERT_EQ(BasicDataType::kTypeInt, TypeToEnum<int>());
  ASSERT_EQ(BasicDataType::kTypeDouble, TypeToEnum<double>());
  ASSERT_EQ(BasicDataType::kTypeString, TypeToEnum<std::string>());
  ASSERT_EQ(BasicDataType::kTypeDateTime, TypeToEnum<DateTime>());
  ASSERT_EQ(BasicDataType::kTypeUnknown, TypeToEnum<std::vector<int>>());

  ASSERT_EQ(BasicDataType::kTypeInt, TypeToEnum(int()));
  ASSERT_EQ(BasicDataType::kTypeDouble, TypeToEnum(double()));
  ASSERT_EQ(BasicDataType::kTypeString, TypeToEnum(std::string()));
  ASSERT_EQ(BasicDataType::kTypeDateTime, TypeToEnum(DateTime()));
  ASSERT_EQ(BasicDataType::kTypeUnknown, TypeToEnum(std::vector<int>()));
}