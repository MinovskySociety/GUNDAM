#ifndef _DATATYPE_H
#define _DATATYPE_H

#include <cstdint>
#include <string>
#include <type_traits>

#include "gundam/datetime.h"

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
  kTypeString = 1,
  kTypeInt = 2,
  kTypeInt64 = 3,
  kTypeFloat = 4,
  kTypeDouble = 5,
  kTypeDateTime = 6,
};

inline BasicDataType StringToEnum(const char* type_str) {
  if (_datatype::StringCompare(type_str, "string"))
    return BasicDataType::kTypeString;
  else if (_datatype::StringCompare(type_str, "int"))
    return BasicDataType::kTypeInt;
  else if (_datatype::StringCompare(type_str, "int64"))
    return BasicDataType::kTypeInt64;
  else if (_datatype::StringCompare(type_str, "float"))
    return BasicDataType::kTypeFloat;
  else if (_datatype::StringCompare(type_str, "double"))
    return BasicDataType::kTypeDouble;
  else if (_datatype::StringCompare(type_str, "datetime"))
    return BasicDataType::kTypeDateTime;

  return BasicDataType::kTypeUnknown;
}

inline const char* EnumToString(enum BasicDataType data_type) {
  switch (data_type) {
    case BasicDataType::kTypeString:
      return "string";
    case BasicDataType::kTypeInt:
      return "int";
    case BasicDataType::kTypeInt64:
      return "int64";
    case BasicDataType::kTypeFloat:
      return "float";
    case BasicDataType::kTypeDouble:
      return "double";
    case BasicDataType::kTypeDateTime:
      return "datetime";
    case BasicDataType::kTypeUnknown:
    default:
      return "unknown";
  }
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeString;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, int8_t>::value ||
                                      std::is_same<DataType, uint8_t>::value ||
                                      std::is_same<DataType, int16_t>::value ||
                                      std::is_same<DataType, uint16_t>::value ||
                                      std::is_same<DataType, int32_t>::value ||
                                      std::is_same<DataType, uint32_t>::value,
                                  bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeInt;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, int64_t>::value ||
                                      std::is_same<DataType, uint64_t>::value,
                                  bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeInt64;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, float>::value,
                                  bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeFloat;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, double>::value,
                                  bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeDouble;
}

template <typename DataType,
          typename std::enable_if<std::is_same<DataType, DateTime>::value,
                                  bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeDateTime;
}

template <
    typename DataType,
    typename std::enable_if<!std::is_integral<DataType>::value &&
                                !std::is_floating_point<DataType>::value &&
                                !std::is_same<DataType, std::string>::value &&
                                !std::is_same<DataType, DateTime>::value,
                            bool>::type = false>
inline constexpr BasicDataType TypeToEnum() {
  return BasicDataType::kTypeUnknown;
}

template <typename DataType>
inline constexpr BasicDataType TypeToEnum(const DataType&) {
  return TypeToEnum<DataType>();
}

template <typename DataType>
inline constexpr const char* TypeToString(const DataType &data) {
  return EnumToString(TypeToEnum<DataType>());
}

template <typename DataType>
inline constexpr const char* TypeToString() {
  return EnumToString(TypeToEnum<DataType>());
}

}  // namespace GUNDAM

#endif
