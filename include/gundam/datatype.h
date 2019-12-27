#ifndef _DATATYPE_H
#define _DATATYPE_H
#include <iostream>
#include <set>
#include "gundam/datetime.h"

enum class BasicDataType {
  int_,
  double_,
  long_,
  short_,
  string_,
  datetime_,
  unknown_type_
};
template <typename Str>
BasicDataType StringToEnum(Str str) {
  if (str == "int")
    return BasicDataType::int_;
  else if (str == "long")
    return BasicDataType::long_;
  else if (str == "double")
    return BasicDataType::double_;
  else if (str == "short")
    return BasicDataType::short_;
  else if (str == "string")
    return BasicDataType::string_;
  else if (str == "datetime")
    return BasicDataType::datetime_;
  return BasicDataType::unknown_type_;
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, BasicDataType>::value,
                                  bool>::type = false>
std::string EnumToString(DataType data) {
  std::string ret;

  switch (data) {
    case BasicDataType::int_: {
      ret = "int";
      break;
    }
    case BasicDataType::double_: {
      ret = "double";
      break;
    }
    case BasicDataType::long_: {
      ret = "long";
      break;
    }
    case BasicDataType::short_: {
      ret = "short";
      break;
    }
    case BasicDataType::string_: {
      ret = "string";
      break;
    }
    case BasicDataType::datetime_: {
      ret = "datetime";
      break;
    }
    case BasicDataType::unknown_type_: {
      ret = "unknown type";
      break;
    }
    default:
      ret = "";
  }
  return ret;
}
template <typename DataType,
          typename std::enable_if<std::is_integral<DataType>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString(const DataType& data) {
  return "int";
}
template <typename DataType,
          typename std::enable_if<std::is_integral<DataType>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString() {
  return "int";
}
template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString(const DataType& data) {
  return "double";
}
template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString() {
  return "double";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString(const DataType& data) {
  return "string";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString() {
  return "string";
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString(const DataType& data) {
  return "datetime";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
inline constexpr std::string TypeToString() {
  return "datetime";
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
inline constexpr std::string TypeToString(const DataType& data) {
  return "unknown type";
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
inline constexpr std::string TypeToString() {
  return "unknown type";
}

template <
    typename DataT,
    typename std::enable_if<std::is_integral<DataT>::value, bool>::type = false>
const BasicDataType TypeToEnum(DataT data) {
  return BasicDataType::int_;
}
template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
const BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::double_;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
const BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::string_;
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
const BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::datetime_;
}
template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
const BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::unknown_type_;
}

#endif