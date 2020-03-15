#ifndef _DATATYPE_H
#define _DATATYPE_H

#include "gundam/datetime.h"

#include <string>
#include <type_traits>

namespace GUNDAM {

namespace _datatype {
inline bool StringCompare(const char* a, const char* b) {
  while (*a != '\0' || *b != '\0') {
    if (*a != *b) return false;
    a++;
    b++;
  }
  return true;
}
}  // namespace _datatype

enum class BasicDataType : int {
  kTypeUnknown = -1,
  kTypeInt = 1,
  kTypeDouble = 2,
  kTypeString = 3,
  kTypeDateTime = 4,
};

inline BasicDataType StringToEnum(const char* type_str) {
  if (_datatype::StringCompare(type_str, "int"))
    return BasicDataType::kTypeInt;
  else if (_datatype::StringCompare(type_str, "double"))
    return BasicDataType::kTypeDouble;
  else if (_datatype::StringCompare(type_str, "string"))
    return BasicDataType::kTypeString;
  else if (_datatype::StringCompare(type_str, "datetime"))
    return BasicDataType::kTypeDateTime;

  return BasicDataType::kTypeUnknown;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, BasicDataType>::value,
                                  bool>::type = false>
inline const char* EnumToString(DataType data) {
  switch (data) {
    case BasicDataType::kTypeInt:
      return "int";
    case BasicDataType::kTypeDouble:
      return "double";
    case BasicDataType::kTypeString:
      return "string";
    case BasicDataType::kTypeDateTime:
      return "datetime";
    case BasicDataType::kTypeUnknown:
    default:
      return "unknown";      
  }
}

template <typename DataType,
          typename std::enable_if<std::is_integral<DataType>::value,
                                  bool>::type = false>
inline constexpr const char* TypeToString(const DataType& data) {
  return "int";
}

template <typename DataType,
          typename std::enable_if<std::is_integral<DataType>::value,
                                  bool>::type = false>
inline constexpr const char* TypeToString() {
  return "int";
}

template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
inline constexpr const char* TypeToString(const DataType& data) {
  return "double";
}
template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
inline constexpr const char* TypeToString() {
  return "double";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
inline constexpr const char* TypeToString(const DataType& data) {
  return "string";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
inline constexpr const char * TypeToString() {
  return "string";
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
inline constexpr const char * TypeToString(const DataType& data) {
  return "datetime";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
inline constexpr const char * TypeToString() {
  return "datetime";
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
inline constexpr const char * TypeToString(const DataType& data) {
  return "unknown";
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
inline constexpr const char *TypeToString() {
  return "unknown";
}

template <
    typename DataT,
    typename std::enable_if<std::is_integral<DataT>::value, bool>::type = false>
constexpr BasicDataType TypeToEnum(DataT data) {
  return BasicDataType::kTypeInt;
}

template <
    typename DataT,
    typename std::enable_if<std::is_integral<DataT>::value, bool>::type = false>
constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeInt;
}

template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
constexpr BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::kTypeDouble;
}

template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeDouble;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
constexpr BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::kTypeString;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeString;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
constexpr BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::kTypeDateTime;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeDateTime;
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
constexpr BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::kTypeUnknown;
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeUnknown;
}

}

#endif