#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <sstream>
#include <string>

#include "gundam/container.h"
#include "gundam/datatype.h"
#include "gundam/iterator.h"

namespace GUNDAM {

// Value
class AbstractValue {
 public:
  virtual ~AbstractValue() {}

  virtual std::string value_str() const = 0;
};

template <typename ConcreteDataType_>
class ConcreteValue : public AbstractValue {
 public:
  ConcreteValue(const ConcreteDataType_& value) : value_(value) {}

  virtual ~ConcreteValue() override {}

  virtual std::string value_str() const override {
    std::stringstream ss;
    ss << value_;
    return ss.str();
  }

  const ConcreteDataType_& const_value() const { return this->value_; }

  ConcreteDataType_& value() { return this->value_; }

  void set_value(const ConcreteDataType_& value) { this->value_ = value; }

 private:
  ConcreteDataType_ value_;
};

// Attribute
constexpr TupleIdxType kAttributeKeyIdx = 0;
constexpr TupleIdxType kAttributeValuePtrIdx = 1;
constexpr TupleIdxType kAttributeValueTypeIdx = 2;

template <typename KeyType_, enum ContainerType container_type_,
          enum SortType sort_type_>
using AttributeContainer = Container<container_type_, sort_type_, KeyType_,
                                     AbstractValue*, enum BasicDataType>;

template <typename AttributeType_, bool is_const_, bool is_dynamic_,
          typename KeyType_, enum ContainerType container_type_,
          enum SortType sort_type_>
class WithAttribute_;

// This"Ptr" is iterator!
template <typename AttributeContainerType, bool is_const_>
class AttributeContentPtr_ {
 private:
  using IteratorType = typename std::conditional<
      is_const_, typename AttributeContainerType::const_iterator,
      typename AttributeContainerType::iterator>::type;

 protected:
  bool is_null_;
  IteratorType iterator_;
  bool IsNull() const { return this->is_null_; }

 public:
  AttributeContentPtr_() : is_null_(true), iterator_() { return; }
  AttributeContentPtr_(const IteratorType& iterator)
      : is_null_(false), iterator_(iterator) {
    return;
  }
  const typename AttributeContainerType::KeyType& key() const {
    assert(!this->is_null_);
    return std::get<kAttributeKeyIdx>(*(this->iterator_));
  }
  template <typename ConcreteDataType>
  const ConcreteDataType& const_value() const {
    assert(!this->is_null_);
    return static_cast<const ConcreteValue<ConcreteDataType>*>(
               std::get<kAttributeValuePtrIdx>(*(this->iterator_)))
        ->const_value();
  }
  template <typename ConcreteDataType>
  ConcreteDataType& value() {
    assert(!this->is_null_);
    return static_cast<ConcreteValue<ConcreteDataType>*>(
               std::get<kAttributeValuePtrIdx>(*(this->iterator_)))
        ->value();
  }
  std::string value_str() const {
    assert(!this->is_null_);
    return std::get<kAttributeValuePtrIdx>(*(this->iterator_))->value_str();
  }
  BasicDataType value_type() const {
    assert(!this->is_null_);
    return std::get<kAttributeValueTypeIdx>(*(this->iterator_));
  }
};

template <typename AttributeContainerType, bool is_const_>
class AttributePtr_
    : protected AttributeContentPtr_<AttributeContainerType, is_const_> {
 private:
  friend class AttributePtr_<AttributeContainerType, !is_const_>;

  using AttributeContentPtrType =
      AttributeContentPtr_<AttributeContainerType, is_const_>;

  using AttributeContentPtr =
      typename std::conditional<is_const_, const AttributeContentPtrType*,
                                AttributeContentPtrType*>::type;

 public:
  using AttributeContentPtrType::AttributeContentPtrType;

  bool operator==(const AttributePtr_& attribute_ptr) const {
    return this->iterator_ == attribute_ptr.iterator_;
  }
  bool operator!=(const AttributePtr_& attribute_ptr) const {
    return this->iterator_ != attribute_ptr.iterator_;
  }
  bool operator<(const AttributePtr_& attribute_ptr) const {
    return this->iterator_ < attribute_ptr.iterator_;
  }
  bool operator>(const AttributePtr_& attribute_ptr) const {
    return this->iterator_ > attribute_ptr.iterator_;
  }
  bool operator<=(const AttributePtr_& attribute_ptr) const {
    return this->iterator_ <= attribute_ptr.iterator_;
  }
  bool operator>=(const AttributePtr_& attribute_ptr) const {
    return this->iterator_ >= attribute_ptr.iterator_;
  }
  AttributePtr_& operator=(const AttributePtr_& attribute_ptr) {
    this->iterator_ = attribute_ptr.iterator_;
    this->is_null_ = attribute_ptr.is_null_;
    return *this;
  }

  template <const bool judge = is_const_,
            typename std::enable_if<!judge, bool>::type = false>
  AttributeContentPtr operator->() {
    static_assert(judge == is_const_, "Illegal usage of this method");
    AttributeContentPtr const temp_this_ptr = this;
    return temp_this_ptr;
  }

  template <const bool judge = is_const_,
            typename std::enable_if<judge, bool>::type = false>
  AttributeContentPtr operator->() const {
    static_assert(judge == is_const_, "Illegal usage of this method");
    AttributeContentPtr const temp_this_ptr = this;
    return temp_this_ptr;
  }

  bool IsNull() const { return AttributeContentPtrType::IsNull(); }
};

// class InnerVertex_;
//
// using VertexAttributeType =
//    WithAttribute_<VertexStaticAttributeType, vertex_attribute_is_const,
//                   vertex_has_dynamic_attribute, VertexAttributeKeyType,
//                   vertex_attribute_container_type,
//                   vertex_attribute_container_sort_type>;
//
// using EdgeAttributeType =
//    WithAttribute_<EdgeStaticAttributeType, edge_attribute_is_const,
//                   edge_has_dynamic_attribute, EdgeAttributeKeyType,
//                   edge_attribute_container_type,
//                   edge_attribute_container_sort_type>;

/// these ContentIterator classes are transparent to programmers
template <typename KeyType_, typename ContainerType_, bool is_const_,
          IteratorDepthType depth_, IteratorDepthType begin_depth_,
          TupleIdxType key_idx_, TupleIdxType value_ptr_idx_,
          TupleIdxType value_typex_>
class AttributeContentIterator_
    : protected InnerIterator_<ContainerType_, is_const_, depth_> {
 private:
  using InnerIteratorType = InnerIterator_<ContainerType_, is_const_, depth_>;

  // friend typename VertexAttributeType::AttributeIterator
  // VertexAttributeType::EraseAttribute(
  //    const typename VertexAttributeType::AttributeIterator&
  //        attribute_iterator);
  // friend typename EdgeAttributeType::AttributeIterator
  // EdgeAttributeType::EraseAttribute(
  //    const typename EdgeAttributeType::AttributeIterator&
  //    attribute_iterator);

  const typename InnerIteratorType::IteratorType& ConstInnerIterator() const {
    return InnerIteratorType::ConstInnerIterator();
  }

 protected:
  using InnerIteratorType::IsDone;
  using InnerIteratorType::ToNext;
  using ContentPtr =
      typename std::conditional<is_const_, const AttributeContentIterator_*,
                                AttributeContentIterator_*>::type;
  static constexpr bool kIsConst_ = is_const_;

  template <bool judge = is_const_,
            typename std::enable_if<!judge, bool>::type = false>
  ContentPtr content_ptr() {
    ContentPtr const temp_this_ptr = this;
    return temp_this_ptr;
  }

  template <bool judge = is_const_,
            typename std::enable_if<judge, bool>::type = false>
  ContentPtr content_ptr() const {
    ContentPtr const temp_this_ptr = this;
    return temp_this_ptr;
  }

 public:
  using InnerIteratorType::InnerIteratorType;

  const KeyType_& key() const {
    assert(!this->IsDone());
    return InnerIteratorType::template get_const<KeyType_, key_idx_,
                                                 begin_depth_>();
  }

  template <typename ConcreteDataType>
  const ConcreteDataType& const_value() const {
    assert(!this->IsDone());
    return static_cast<ConcreteValue<const ConcreteDataType>*>(
               InnerIteratorType::template get_const<
                   AbstractValue*, value_ptr_idx_, begin_depth_>())
        ->const_value();
  }

  template <typename ConcreteDataType>
  ConcreteDataType& value() {
    assert(!this->IsDone());
    return static_cast<ConcreteValue<ConcreteDataType>*>(
               InnerIteratorType::template get<AbstractValue*, value_ptr_idx_,
                                               begin_depth_>())
        ->value();
  }

  std::string value_str() const {
    assert(!this->IsDone());
    return (InnerIteratorType::template get_const<
                AbstractValue*, value_ptr_idx_, begin_depth_>())
        ->value_str();
  }

  enum BasicDataType value_type() const {
    assert(!this->IsDone());
    return InnerIteratorType::template get_const<
        enum BasicDataType, value_typex_, begin_depth_>();
  }
};

// non-dynamic attribute
template <typename AttributeType_, bool is_const_, typename KeyType_,
          enum ContainerType container_type_, enum SortType sort_type_>
class WithAttribute_<AttributeType_, is_const_, false, KeyType_,
                     container_type_, sort_type_> {
 private:
  typename std::conditional<is_const_, const AttributeType_,
                            AttributeType_>::type attribute_;

 public:
  WithAttribute_(const AttributeType_& attribute) : attribute_(attribute) {
    return;
  }

  template <bool judge_is_const = is_const_,
            typename std::enable_if<!judge_is_const, bool>::type = false>
  WithAttribute_() : attribute_() {
    static_assert(judge_is_const == is_const_, "Illegal usage of this method");
    /// the constant non-dynamic attribute should be
    /// initialized in the construct method
    return;
  }

  inline const AttributeType_& const_attribute() const {
    return this->attribute_;
  }

  template <bool judge_is_const = is_const_,
            typename std::enable_if<!judge_is_const, bool>::type = false>
  inline AttributeType_& attribute() {
    static_assert(judge_is_const == is_const_, "Illegal usage of this method");
    /// the constant attribute should not be modified
    return this->attribute_;
  }

  template <bool judge_is_const = is_const_,
            typename std::enable_if<!judge_is_const, bool>::type = false>
  inline void set_attribute(const AttributeType_& attribute) {
    static_assert(judge_is_const == is_const_, "Illegal usage of this method");
    /// only the non-constant non-dynamic attribute could be set
    this->attribute_ = attribute;
    return;
  }
};

/// dynamic attribute, holds a container for the attributes instead of
/// a single attribute
template <typename AttributeType_, bool is_const_, typename KeyType_,
          enum ContainerType container_type_, enum SortType sort_type_>
class WithAttribute_<AttributeType_, is_const_, true, KeyType_, container_type_,
                     sort_type_> {
 private:
  using AttributeContainerType =
      AttributeContainer<KeyType_, container_type_, sort_type_>;
  AttributeContainerType attributes_;

  using AttributeContentIterator =
      AttributeContentIterator_<KeyType_, AttributeContainerType, false, 1, 0,
                                kAttributeKeyIdx, kAttributeValuePtrIdx,
                                kAttributeValueTypeIdx>;

  std::map<KeyType_, enum BasicDataType> key_to_value_type_map;
  inline bool SetValueType(const KeyType_& key, enum BasicDataType value_type) {
    if (key_to_value_type_map.find(key) != key_to_value_type_map.end()) {
      return false;
    }
    key_to_value_type_map.insert(std::make_pair(key, value_type));
    return true;
  }

 public:
  using AttributePtr = AttributePtr_<AttributeContainerType, false>;
  using AttributeConstPtr = AttributePtr_<AttributeContainerType, true>;

  using AttributeIterator = Iterator_<AttributeContentIterator>;

  using AttributeConstIterator = Iterator_<AttributeContentIterator_<
      KeyType_, AttributeContainerType, true, 1, 0, kAttributeKeyIdx,
      kAttributeValuePtrIdx, kAttributeValueTypeIdx>>;

  WithAttribute_() : attributes_() {
    key_to_value_type_map.clear();
    return;
  }

  ~WithAttribute_() {
    for (auto& it : this->attributes_)
      delete std::get<kAttributeValuePtrIdx>(it);
    key_to_value_type_map.clear();
    return;
  }

  BasicDataType attribute_value_type(const KeyType_& key) const {
    assert(key_to_value_type_map.find(key) != key_to_value_type_map.end());
    return key_to_value_type_map.find(key)->second;
  }

  const char* attribute_value_type_name(const KeyType_& key) const {
    assert(key_to_value_type_map.find(key) != key_to_value_type_map.end());
    // return "";
    return EnumToString(key_to_value_type_map.find(key)->second);
  }

  AttributeIterator AttributeBegin() {
    return AttributeIterator(this->attributes_.begin(),
                             this->attributes_.end());
  }

  inline AttributeConstIterator AttributeCBegin() const {
    return AttributeConstIterator(this->attributes_.cbegin(),
                                  this->attributes_.cend());
  }

  inline AttributePtr FindAttributePtr(const KeyType_& key) {
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Find(key);
    if (!ret.second) return AttributePtr();
    return AttributePtr(ret.first);
  }

  inline AttributeConstPtr FindConstAttributePtr(const KeyType_& key) const {
    /// <constant iterator of attribute container, bool>
    auto ret = this->attributes_.FindConst(key);
    if (!ret.second) return AttributeConstPtr();
    return AttributeConstPtr(ret.first);
  }

  template <typename ConcreteDataType>
  inline ConcreteDataType& attribute(const KeyType_& key) {
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Find(key);
    assert(ret.second);
    return static_cast<ConcreteValue<ConcreteDataType>*>(
               std::get<kAttributeValuePtrIdx>(*(ret.first)))
        ->value();
  }

  template <typename ConcreteDataType>
  inline const ConcreteDataType& const_attribute(const KeyType_& key) const {
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.FindConst(key);
    assert(ret.second);
    return static_cast<const ConcreteValue<ConcreteDataType>*>(
               std::get<kAttributeValuePtrIdx>(*(ret.first)))
        ->const_value();
  }

  template <typename ConcreteDataType>
  inline std::pair<AttributePtr, bool> AddAttribute(
      const KeyType_& key, const ConcreteDataType& value) {
    /// the constant attribute should not be modified
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Insert(key);
    if (!ret.second) {
      /// has already existed in the Container
      return std::pair<AttributePtr, bool>(AttributePtr(ret.first), false);
    }
    enum BasicDataType value_type = TypeToEnum(value);
    this->SetValueType(key, value_type);
    std::get<kAttributeValuePtrIdx>(*(ret.first)) =
        new ConcreteValue<ConcreteDataType>(value);
    std::get<kAttributeValueTypeIdx>(*ret.first) = value_type;
    return std::pair<AttributePtr, bool>(AttributePtr(ret.first), true);
  }

  template <typename ConcreteDataType>
  inline std::pair<AttributePtr, bool> SetAttribute(
      const KeyType_& key, const ConcreteDataType& value) {
    /// the constant attribute should not be modified
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Find(key);
    if (!ret.second) {
      /// not find that attribute key in the container
      return std::pair<AttributePtr, bool>(AttributePtr(), false);
    }
    static_cast<ConcreteValue<ConcreteDataType>*>(
        std::get<kAttributeValuePtrIdx>(*(ret.first)))
        ->set_value(value);
    enum BasicDataType value_type = TypeToEnum(value);
    std::get<kAttributeValueTypeIdx>(*ret.first) = value_type;
    return std::pair<AttributePtr, bool>(AttributePtr(ret.first), true);
  }

  inline AttributeIterator EraseAttribute(
      const AttributeIterator& attribute_iterator) {
    const void* const ptr = &attribute_iterator;
    /// iterator of AttributeContainer
    auto it = this->attributes_.Erase(
        static_cast<const AttributeContentIterator*>(ptr)
            ->ConstInnerIterator());
    return AttributeIterator(it, this->attributes_.end());
  }

  inline bool EraseAttribute(const KeyType_& key) {
    return this->attributes_.Erase(key);
  }
};

}  // namespace GUNDAM
#endif  // _GRAPH_H
