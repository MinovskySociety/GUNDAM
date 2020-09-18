#ifndef _ATTRIBUTE_H
#define _ATTRIBUTE_H

#include <sstream>
#include <string>

#include "gundam/container.h"
#include "gundam/datatype.h"
#include "gundam/iterator.h"

namespace GUNDAM {
  
template <typename            AttributeType_, 
          bool                     is_const_, 
          bool                   is_dynamic_,
          typename                  KeyType_, 
          enum ContainerType container_type_,
          enum SortType           sort_type_>
class WithAttribute_;

// non-dynamic attribute
template <typename            AttributeType_, 
          bool                     is_const_, 
          typename                  KeyType_,
          enum ContainerType container_type_, 
          enum SortType           sort_type_>
class WithAttribute_<AttributeType_, 
                          is_const_, 
                              false, 
                           KeyType_,
                    container_type_, 
                         sort_type_> {
 private:
  typename std::conditional<is_const_, 
                 const AttributeType_,
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

  const AttributeType_& const_attribute() const { return this->attribute_; }

  template <bool judge_is_const = is_const_,
            typename std::enable_if<!judge_is_const, bool>::type = false>
  AttributeType_& attribute() {
    static_assert(judge_is_const == is_const_, "Illegal usage of this method");
    /// the constant attribute should not be modified
    return this->attribute_;
  }

  template <bool judge_is_const = is_const_,
            typename std::enable_if<!judge_is_const, bool>::type = false>
  void set_attribute(const AttributeType_& attribute) {
    static_assert(judge_is_const == is_const_, "Illegal usage of this method");
    /// only the non-constant non-dynamic attribute could be set
    this->attribute_ = attribute;
    return;
  }
};

/// dynamic attribute, holds a container for the attributes instead of
/// a single attribute
template <typename            AttributeType_, 
          bool                     is_const_, 
          typename                  KeyType_,
          enum ContainerType container_type_, 
          enum      SortType      sort_type_>
class WithAttribute_<AttributeType_, 
                          is_const_, 
                               true, 
                           KeyType_, 
                    container_type_,
                         sort_type_> {
 private:
  // Value
  class AbstractValue {
  public:
    virtual ~AbstractValue() {}

    virtual std::string value_str() const = 0;
  };

  template <typename ConcreteDataType_>
  class ConcreteValue : public AbstractValue {
   public:
    ConcreteValue(const ConcreteDataType_& value)
                                : AbstractValue(),
                                    value_(value) {
      return;
    }

    virtual ~ConcreteValue() override {
      return;
    }

    virtual std::string value_str() const override {
      std::stringstream ss;
      ss << value_;
      return ss.str();
    }

    const ConcreteDataType_& const_value() const { 
      return this->value_; 
    }

    ConcreteDataType_& value() { 
      return this->value_; 
    }

    void set_value(const ConcreteDataType_& value) { 
      this->value_ = value; 
      return;
    }

   private:
    ConcreteDataType_ value_;
  };

  // Attribute
  static constexpr TupleIdxType kAttributeKeyIdx = 0;
  static constexpr TupleIdxType kAttributeValuePtrIdx = 1;
  static constexpr TupleIdxType kAttributeValueTypeIdx = 2;

  template <typename                  _KeyType_, 
            enum ContainerType _container_type_,
            enum SortType           _sort_type_>
  using AttributeContainer = Container<_container_type_, 
                                            _sort_type_, 
                                              _KeyType_,
                                         AbstractValue*, 
                                    enum BasicDataType>;

  // This"Ptr" is iterator!
  template <typename AttributeContainerType, 
            bool _is_const_>
  class AttributeContentPtr_ {
   private:
    using IteratorType = typename std::conditional<_is_const_, 
              typename AttributeContainerType::const_iterator,
              typename AttributeContainerType::      iterator>::type;

   protected:
    bool is_null_;
    IteratorType iterator_;

    inline bool IsNull() const { 
      return this->is_null_; 
    }

   public:
    AttributeContentPtr_() : is_null_(true), iterator_() { 
      return; 
    }

    AttributeContentPtr_(const IteratorType& iterator)
                : is_null_(false), iterator_(iterator) {
      return;
    }

    inline const typename AttributeContainerType::KeyType& key() const {
      assert(!this->is_null_);
      return this->iterator_.template get<kAttributeKeyIdx>();
    }

    template <typename ConcreteDataType>
    inline const ConcreteDataType& const_value() const {
      assert(!this->is_null_);
      return static_cast<const ConcreteValue<ConcreteDataType>*>(
                this->iterator_.template get_const<kAttributeValuePtrIdx>())
          ->const_value();
    }

    template <typename ConcreteDataType>
    inline ConcreteDataType& value() {
      assert(!this->is_null_);
      return static_cast<ConcreteValue<ConcreteDataType>*>(
                this->iterator_.template get<kAttributeValuePtrIdx>())
          ->value();
    }

    inline std::string value_str() const {
      assert(!this->is_null_);
      return this->iterator_.template get_const<kAttributeValuePtrIdx>()->value_str();
    }

    inline BasicDataType value_type() const {
      assert(!this->is_null_);
      return this->iterator_.template get_const<kAttributeValueTypeIdx>();
    }
  };

  template <typename AttributeContainerType,
            bool _is_const_>
  class AttributePtr_
      : protected AttributeContentPtr_<AttributeContainerType, _is_const_> {
   private:
    friend class AttributePtr_<AttributeContainerType, !_is_const_>;

    using AttributeContentPtrType
        = AttributeContentPtr_<AttributeContainerType, _is_const_>;

    using AttributeContentPtr = typename std::conditional<_is_const_, 
                                      const AttributeContentPtrType*,
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
    
    inline bool IsNull() const { 
      return AttributeContentPtrType::IsNull(); 
    }
  };

  /// these ContentIterator classes are transparent to programmers
  template <typename             _KeyType_, 
            typename        ContainerType_, 
            bool                _is_const_,
            IteratorDepthType       depth_, 
            IteratorDepthType begin_depth_,
            TupleIdxType          key_idx_, 
            TupleIdxType    value_ptr_idx_,
            TupleIdxType      value_typex_>
  class AttributeContentIterator_
      : protected InnerIterator_<ContainerType_, _is_const_, depth_> {
   private:
    using InnerIteratorType = InnerIterator_<ContainerType_, _is_const_, depth_>;

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
    using ContentPtr = typename std::conditional<_is_const_, 
                            const AttributeContentIterator_*,
                                  AttributeContentIterator_*>::type;
    static constexpr bool kIsConst_ = _is_const_;

    template <bool judge = _is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    ContentPtr content_ptr() {
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

    template <bool judge = _is_const_,
              typename std::enable_if<judge, bool>::type = false>
    ContentPtr content_ptr() const {
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

   public:
    using InnerIteratorType::InnerIteratorType;

    const _KeyType_& key() const {
      assert(!this->IsDone());
      return InnerIteratorType::template get_const<_KeyType_, 
                                                    key_idx_,
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

  using AttributeContainerType
      = AttributeContainer<KeyType_, container_type_, sort_type_>;

  using AttributeContentIterator 
      = AttributeContentIterator_<KeyType_, 
                   AttributeContainerType, false, 1, 0,
                  kAttributeKeyIdx, 
                  kAttributeValuePtrIdx,
                  kAttributeValueTypeIdx>;

 public:
  using AttributePtr      = AttributePtr_<AttributeContainerType, false>;
  using AttributeConstPtr = AttributePtr_<AttributeContainerType,  true>;

  using AttributeIterator = Iterator_<AttributeContentIterator>;

  using AttributeConstIterator 
      = Iterator_<AttributeContentIterator_<KeyType_, 
                  AttributeContainerType, true, 1, 0,
                 kAttributeKeyIdx,
                 kAttributeValuePtrIdx, 
                 kAttributeValueTypeIdx>>;

  WithAttribute_() : attributes_() {
    return;
  }

  ~WithAttribute_() {
    for (auto it  = this->attributes_.begin();
              it != this->attributes_.end(); it++)
      delete it.template get<kAttributeValuePtrIdx>();
    return;
  }

  BasicDataType attribute_value_type(const KeyType_& key) const {
    auto ret = this->attributes_.FindConst(key);
    assert(ret.second);
    return ret.first.template get_const<kAttributeValueTypeIdx>();
  }

  const char* attribute_value_type_name(const KeyType_& key) const {
    return EnumToString(this->attribute_value_type(key));
  }

  AttributeIterator AttributeBegin() {
    return AttributeIterator(this->attributes_.begin(),
                             this->attributes_.end());
  }

  AttributeConstIterator AttributeCBegin() const {
    return AttributeConstIterator(this->attributes_.cbegin(),
                                  this->attributes_.cend());
  }

  AttributePtr FindAttributePtr(const KeyType_& key) {
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Find(key);
    if (!ret.second) return AttributePtr();
    return AttributePtr(ret.first);
  }

  AttributeConstPtr FindConstAttributePtr(const KeyType_& key) const {
    /// <constant iterator of attribute container, bool>
    auto ret = this->attributes_.FindConst(key);
    if (!ret.second) return AttributeConstPtr();
    return AttributeConstPtr(ret.first);
  }

  template <typename ConcreteDataType>
  ConcreteDataType& attribute(const KeyType_& key) {
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Find(key);
    assert(ret.second);
    return static_cast<ConcreteValue<ConcreteDataType>*>(
               ret.first.template get<kAttributeValuePtrIdx>())
        ->value();
  }

  template <typename ConcreteDataType>
  const ConcreteDataType& const_attribute(const KeyType_& key) const {
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.FindConst(key);
    assert(ret.second);
    return static_cast<const ConcreteValue<ConcreteDataType>*>(
               ret.first.template get_const<kAttributeValuePtrIdx>())
        ->const_value();
  }

  template <typename ConcreteDataType>
  std::pair<AttributePtr, bool> AddAttribute(const KeyType_& key,
                                             const ConcreteDataType& value) {
    /// the constant attribute should not be modified
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Insert(key);
    if (!ret.second) {
      /// has already existed in the Container
      return std::pair<AttributePtr, bool>(AttributePtr(ret.first), false);
    }
    const enum BasicDataType value_type = TypeToEnum<ConcreteDataType>();
    ret.first.template get<kAttributeValuePtrIdx>()
      = new ConcreteValue<ConcreteDataType>(value);
    ret.first.template get<kAttributeValueTypeIdx>() = value_type;
    return std::pair<AttributePtr, bool>(AttributePtr(ret.first), true);
  }

  std::pair<AttributePtr, bool> AddAttribute(
      const KeyType_& key, const enum BasicDataType& data_type,
      const std::string& value_str) {
    switch (data_type) {
      case BasicDataType::kTypeString:
        return AddAttribute<std::string>(key, value_str);
      case BasicDataType::kTypeInt:
        return AddAttribute<int>(key, std::stoi(value_str));
      case BasicDataType::kTypeInt64:
        return AddAttribute<int64_t>(key, std::stoll(value_str));
      case BasicDataType::kTypeFloat:
        return AddAttribute<float>(key, std::stof(value_str));
      case BasicDataType::kTypeDouble:
        return AddAttribute<double>(key, std::stod(value_str));
      case BasicDataType::kTypeDateTime:
        return AddAttribute<DateTime>(key, DateTime(value_str));
      case BasicDataType::kTypeUnknown:
      default:
        break;
    }
    return std::make_pair(AttributePtr(), false);
  }

  template <typename ConcreteDataType>
  std::pair<AttributePtr, bool> SetAttribute(const KeyType_& key,
                                             const ConcreteDataType& value) {
    /// the constant attribute should not be modified
    /// <iterator of attribute container, bool>
    auto ret = this->attributes_.Find(key);
    if (!ret.second) {
      /// not find that attribute key in the container
      return std::pair<AttributePtr, bool>(AttributePtr(), false);
    }
    const enum BasicDataType value_type = TypeToEnum<ConcreteDataType>();
    if (ret.first.template get<kAttributeValueTypeIdx>() == value_type){
      /// the new added value have the same type
      static_cast<ConcreteValue<ConcreteDataType>*>(
          ret.first.template get<kAttributeValuePtrIdx>())
          ->set_value(value);
      return std::pair<AttributePtr, bool>(AttributePtr(ret.first), true);
    }
    /// the new value does not have the same type
    /// needs to delete the ptr and "new" a new one
    delete ret.first.template get<kAttributeValuePtrIdx>();
    ret.first.template get<kAttributeValuePtrIdx>()
      = new ConcreteValue<ConcreteDataType>(value);
    ret.first.template get<kAttributeValueTypeIdx>() = value_type;
    return std::pair<AttributePtr, bool>(AttributePtr(ret.first), true);
  }

  AttributeIterator EraseAttribute(
      const AttributeIterator& attribute_iterator) {
    const void* const ptr = &attribute_iterator;
    /// iterator of AttributeContainer
    auto it = this->attributes_.Erase(
        static_cast<const AttributeContentIterator*>(ptr)
            ->ConstInnerIterator());
    return AttributeIterator(it, this->attributes_.end());
  }

  size_t EraseAttribute(const KeyType_& key) {
    return this->attributes_.Erase(key) ? 1 : 0;
  }

 private:
  AttributeContainerType attributes_;
};

template <class A, class B>
inline int CopyAllAttributes(A& from, B& to) {
  int count = 0;
  for (auto it_a = from->AttributeCBegin(); !it_a.IsDone(); ++it_a) {
    switch (it_a->value_type()) {
      case GUNDAM::BasicDataType::kTypeInt:
        if (!to->template AddAttribute<int>(it_a->key(),
                                            it_a->template const_value<int>())
                 .second) {
          return -1;
        }
        break;
      case GUNDAM::BasicDataType::kTypeInt64:
        if (!to->template AddAttribute<int64_t>(
                   it_a->key(), it_a->template const_value<int64_t>())
                 .second) {
          return -1;
        }
        break;
      case GUNDAM::BasicDataType::kTypeDouble:
        if (!to->template AddAttribute<double>(
                   it_a->key(), it_a->template const_value<double>())
                 .second) {
          return -1;
        }
        break;
      case GUNDAM::BasicDataType::kTypeString:
        if (!to->template AddAttribute<std::string>(
                   it_a->key(), it_a->template const_value<std::string>())
                 .second) {
          return -1;
        }
        break;
      case GUNDAM::BasicDataType::kTypeDateTime:
        if (!to->template AddAttribute<GUNDAM::DateTime>(
                   it_a->key(), it_a->template const_value<GUNDAM::DateTime>())
                 .second) {
          return -1;
        }
        break;
      case GUNDAM::BasicDataType::kTypeUnknown:
      default:
        return -2;
    }
    ++count;
  }
  return count;
}

}  // namespace GUNDAM
#endif  // _GRAPH_H
