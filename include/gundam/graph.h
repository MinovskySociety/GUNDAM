#ifndef _GRAPH_H
#define _GRAPH_H
#include <iostream>
#include <set>

#include "gundam/container.h"
#include "gundam/graph_configure.h"
#include "gundam/iterator.h"
#include "gundam/label.h"
namespace GUNDAM {
enum class BasicDataType { int_, double_, long_, short_, string_ };
template <typename DataType,
          typename std::enable_if<std::is_integral<DataType>::value,
                                  bool>::type = false>
std::string TypeToString(const DataType data) {
  return "int";
}
template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
std::string TypeToString(const DataType data) {
  return "double";
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
std::string TypeToString(const DataType data) {
  return "string";
}
template <typename DataT,
          typename std::enable_if<std::is_integral<DataT>::value, bool>::type =
              false>
const GUNDAM::BasicDataType TypeToEnum(DataT data) {
  return BasicDataType::int_;
}
template <typename DataType,
          typename std::enable_if<std::is_floating_point<DataType>::value,
                                  bool>::type = false>
const GUNDAM::BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::double_;
}
template <typename DataType,
          typename std::enable_if<std::is_same<DataType, std::string>::value,
                                  bool>::type = false>
const GUNDAM::BasicDataType TypeToEnum(DataType data) {
  return BasicDataType::string_;
}

enum class EdgeDirection : bool { InputEdge, OutputEdge };

const std::string EnumToString(const GUNDAM::BasicDataType data) {
  switch (data) {
    case BasicDataType::double_:
      return "double";
      break;
    case BasicDataType::int_:
      return "int";
      break;
    case BasicDataType::long_:
      return "long";
      break;
    case BasicDataType::short_:
      return "short";
      break;
    case BasicDataType::string_:
      return "string";
      break;
    default:
      return "";
      break;
  }
}

template <typename... configures>
class Graph {
 public:
  using Configures = GraphConfigures<configures...>;
  static constexpr bool vertex_has_attribute =
      Configures::vertex_has_static_attribute |
      Configures::vertex_has_dynamic_attribute;
  static constexpr bool edge_has_attribute =
      Configures::edge_has_static_attribute |
      Configures::edge_has_dynamic_attribute;

 private:
  /// only used in Graph class
  /// use Graph<>::VertexType::IDType outside Graph class instead
  using VertexIDType = typename Configures::VertexIDType;
  using VertexLabelType = typename Configures::VertexLabelType;
  using VertexAttributeKeyType = typename Configures::VertexAttributeKeyType;
  using VertexStaticAttributeType =
      typename Configures::VertexStaticAttributeType;
  using EdgeIDType = typename Configures::EdgeIDType;
  using EdgeLabelType = typename Configures::EdgeLabelType;
  using EdgeAttributeKeyType = typename Configures::EdgeAttributeKeyType;
  using EdgeStaticAttributeType = typename Configures::EdgeStaticAttributeType;

  static constexpr enum StoreData store_data = Configures::store_data;

  static constexpr bool vertex_label_is_const =
      Configures::vertex_label_is_const;
  static constexpr bool vertex_attribute_is_const =
      Configures::vertex_attribute_is_const;
  static constexpr bool vertex_has_static_attribute =
      Configures::vertex_has_static_attribute;
  static constexpr bool vertex_has_dynamic_attribute =
      Configures::vertex_has_dynamic_attribute;
  static constexpr bool edge_label_is_const = Configures::edge_label_is_const;
  static constexpr bool edge_attribute_is_const =
      Configures::edge_attribute_is_const;
  static constexpr bool edge_has_static_attribute =
      Configures::edge_has_static_attribute;
  static constexpr bool edge_has_dynamic_attribute =
      Configures::edge_has_dynamic_attribute;

  static constexpr bool allow_duplicate_edge = Configures::allow_duplicate_edge;

  static constexpr enum ContainerType vertex_attribute_container_type =
      Configures::vertex_attribute_container_type;
  static constexpr enum ContainerType edge_attribute_container_type =
      Configures::edge_attribute_container_type;
  static constexpr enum ContainerType decomposed_edge_container_type =
      Configures::decomposed_edge_container_type;
  static constexpr enum ContainerType vertex_ptr_container_type =
      Configures::vertex_ptr_container_type;
  static constexpr enum ContainerType edge_label_container_type =
      Configures::edge_label_container_type;
  static constexpr enum ContainerType vertex_id_container_type =
      Configures::vertex_id_container_type;
  static constexpr enum ContainerType vertex_label_container_type =
      Configures::vertex_label_container_type;

  static constexpr enum SortType vertex_attribute_container_sort_type =
      Configures::vertex_attribute_container_sort_type;
  static constexpr enum SortType edge_attribute_container_sort_type =
      Configures::edge_attribute_container_sort_type;
  static constexpr enum SortType decomposed_edge_container_sort_type =
      Configures::decomposed_edge_container_sort_type;
  static constexpr enum SortType vertex_ptr_container_sort_type =
      Configures::vertex_ptr_container_sort_type;
  static constexpr enum SortType edge_label_container_sort_type =
      Configures::edge_label_container_sort_type;
  static constexpr enum SortType vertex_id_container_sort_type =
      Configures::vertex_id_container_sort_type;
  static constexpr enum SortType vertex_label_container_sort_type =
      Configures::vertex_label_container_sort_type;

  template <typename IDType_>
  class WithID_ {
   private:
    const IDType_ id_;
    std::string id_type_;

   public:
    WithID_(const IDType_& id) : id_(id) { return; }
    inline void SetIDType(const std::string id_type) { id_type_ = id_type; }
    inline const std::string id_type() const { return id_type_; }
    inline const IDType_& id() const { return this->id_; }
  };

  template <typename LabelType_, bool is_const_>
  class WithLabel_ {
   private:
    typename std::conditional<is_const_, const LabelType_, LabelType_>::type
        label_;
    std::string label_type_;

   public:
    WithLabel_(const LabelType_& label) : label_(label) { return; }
    inline void SetLabelType(const std::string label_type) {
      label_type_ = label_type;
    }
    inline const std::string label_type() const { return label_type_; }
    inline const LabelType_& label() const { return this->label_; }
    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline void set_label(const LabelType_& label) {
      static_assert(judge == is_const_, "Illegal usage of this method");
      this->label_ = label;
      return;
    }
  };

  class AbstractValue {
   public:
    virtual ~AbstractValue() { return; }
  };

  template <typename ConcreteDataType_>
  class ConcreteValue : public AbstractValue {
   private:
    ConcreteDataType_ value_;

   public:
    ConcreteValue(const ConcreteDataType_& value) : value_(value) { return; }
    virtual ~ConcreteValue() { return; }
    inline const ConcreteDataType_& const_value() const { return this->value_; }
    inline ConcreteDataType_& value() { return this->value_; }
    inline void set_value(const ConcreteDataType_& value) {
      this->value_ = value;
      return;
    }
  };

  static constexpr TupleIdxType kAttributeKeyIdx = 0;
  static constexpr TupleIdxType kAttributeValuePtrIdx = 1;
  template <typename KeyType_, enum ContainerType container_type_,
            enum SortType sort_type_>
  using AttributeContainer =
      Container<container_type_, sort_type_, KeyType_, AbstractValue*>;

  template <typename AttributeType_, bool is_const_, bool is_dynamic_,
            typename KeyType_, enum ContainerType container_type_,
            enum SortType sort_type_>
  class WithAttribute_;

  template <typename AttributeContainerType, bool is_const_>
  class AttributeContentPtr_ {
   private:
    using IteratorType = typename std::conditional<
        is_const_, typename AttributeContainerType::const_iterator,
        typename AttributeContainerType::iterator>::type;

    bool is_null_;
    IteratorType iterator_;

   protected:
    inline bool IsNull() const { return this->is_null_; }

   public:
    AttributeContentPtr_() : is_null_(true), iterator_() { return; }
    AttributeContentPtr_(const IteratorType& iterator)
        : is_null_(false), iterator_(iterator) {
      return;
    }
    inline const typename AttributeContainerType::KeyType& key() const {
      assert(!this->is_null_);
      return std::get<kAttributeKeyIdx>(*(this->iterator_));
    }
    template <typename ConcreteDataType>
    inline const ConcreteDataType& const_value() const {
      assert(!this->is_null_);
      return static_cast<const ConcreteValue<ConcreteDataType>*>(
                 std::get<kAttributeValuePtrIdx>(*(this->iterator_)))
          ->const_value();
    }
    template <typename ConcreteDataType>
    inline ConcreteDataType& value() {
      assert(!this->is_null_);
      return static_cast<ConcreteValue<ConcreteDataType>*>(
                 std::get<kAttributeValuePtrIdx>(*(this->iterator_)))
          ->value();
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

    inline bool operator==(const AttributePtr_& attribute_ptr) const {
      return this->ptr_ == attribute_ptr.ptr_;
    }
    inline bool operator!=(const AttributePtr_& attribute_ptr) const {
      return this->ptr_ != attribute_ptr.ptr_;
    }
    inline bool operator<(const AttributePtr_& attribute_ptr) const {
      return this->ptr_ < attribute_ptr.ptr_;
    }
    inline bool operator>(const AttributePtr_& attribute_ptr) const {
      return this->ptr_ > attribute_ptr.ptr_;
    }
    inline bool operator<=(const AttributePtr_& attribute_ptr) const {
      return this->ptr_ <= attribute_ptr.ptr_;
    }
    inline bool operator>=(const AttributePtr_& attribute_ptr) const {
      return this->ptr_ >= attribute_ptr.ptr_;
    }
    inline AttributePtr_& operator=(const AttributePtr_& attribute_ptr) {
      this->ptr_ = attribute_ptr.ptr_;
      return *this;
    }

    template <const bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline AttributeContentPtr operator->() {
      static_assert(judge == is_const_, "Illegal usage of this method");
      AttributeContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

    template <const bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline AttributeContentPtr operator->() const {
      static_assert(judge == is_const_, "Illegal usage of this method");
      AttributeContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

    inline bool IsNull() const { return AttributeContentPtrType::IsNull(); }
  };

  class InnerVertex_;

  using VertexAttributeType =
      WithAttribute_<VertexStaticAttributeType, vertex_attribute_is_const,
                     vertex_has_dynamic_attribute, VertexAttributeKeyType,
                     vertex_attribute_container_type,
                     vertex_attribute_container_sort_type>;

  using EdgeAttributeType =
      WithAttribute_<EdgeStaticAttributeType, edge_attribute_is_const,
                     edge_has_dynamic_attribute, EdgeAttributeKeyType,
                     edge_attribute_container_type,
                     edge_attribute_container_sort_type>;

  /// these ContentIterator classes are transparent to programmers
  template <typename KeyType_, typename ContainerType_, bool is_const_,
            IteratorDepthType depth_, IteratorDepthType begin_depth_,
            TupleIdxType key_idx_, TupleIdxType value_ptr_idx_>
  class AttributeContentIterator_
      : protected InnerIterator_<ContainerType_, is_const_, depth_> {
   private:
    using InnerIteratorType = InnerIterator_<ContainerType_, is_const_, depth_>;

    friend typename VertexAttributeType::AttributeIterator
    VertexAttributeType::EraseAttribute(
        const typename VertexAttributeType::AttributeIterator&
            attribute_iterator);
    friend typename EdgeAttributeType::AttributeIterator
    EdgeAttributeType::EraseAttribute(
        const typename EdgeAttributeType::AttributeIterator&
            attribute_iterator);

    inline const typename InnerIteratorType::IteratorType& ConstInnerIterator()
        const {
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
    inline ContentPtr content_ptr() {
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline ContentPtr content_ptr() const {
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

   public:
    using InnerIteratorType::InnerIteratorType;

    inline const KeyType_& key() const {
      assert(!this->IsDone());
      return InnerIteratorType::template get_const<KeyType_, key_idx_,
                                                   begin_depth_>();
    }
    template <typename ConcreteDataType>
    inline const ConcreteDataType& const_value() const {
      assert(!this->IsDone());
      return static_cast<ConcreteValue<const ConcreteDataType>*>(
                 InnerIteratorType::template get_const<
                     AbstractValue*, value_ptr_idx_, begin_depth_>())
          ->const_value();
    }
    template <typename ConcreteDataType>
    inline ConcreteDataType& value() {
      assert(!this->IsDone());
      return static_cast<ConcreteValue<ConcreteDataType>*>(
                 InnerIteratorType::template get<AbstractValue*, value_ptr_idx_,
                                                 begin_depth_>())
          ->value();
    }
  };

  /// non-dynamic attribute
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
      static_assert(judge_is_const == is_const_,
                    "Illegal usage of this method");
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
      static_assert(judge_is_const == is_const_,
                    "Illegal usage of this method");
      /// the constant attribute should not be modified
      return this->attribute_;
    }

    template <bool judge_is_const = is_const_,
              typename std::enable_if<!judge_is_const, bool>::type = false>
    inline void set_attribute(const AttributeType_& attribute) {
      static_assert(judge_is_const == is_const_,
                    "Illegal usage of this method");
      /// only the non-constant non-dynamic attribute could be set
      this->attribute_ = attribute;
      return;
    }
  };

  /// dynamic attribute, holds a container for the attributes instead of
  /// a single attribute
  template <typename AttributeType_, bool is_const_, typename KeyType_,
            enum ContainerType container_type_, enum SortType sort_type_>
  class WithAttribute_<AttributeType_, is_const_, true, KeyType_,
                       container_type_, sort_type_> {
   private:
    using AttributeContainerType =
        AttributeContainer<KeyType_, container_type_, sort_type_>;
    AttributeContainerType attributes_;

    using AttributeContentIterator =
        AttributeContentIterator_<KeyType_, AttributeContainerType, false, 1, 0,
                                  kAttributeKeyIdx, kAttributeValuePtrIdx>;

    std::map<KeyType_, enum BasicDataType> key_to_value_type_map;

   public:
    using AttributePtr = AttributePtr_<AttributeContainerType, false>;
    using AttributeConstPtr = AttributePtr_<AttributeContainerType, true>;

    using AttributeIterator = Iterator_<AttributeContentIterator>;
    using AttributeConstIterator = Iterator_<
        AttributeContentIterator_<KeyType_, AttributeContainerType, true, 1, 0,
                                  kAttributeKeyIdx, kAttributeValuePtrIdx>>;

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

    inline bool SetValueType(const KeyType_& key,
                             enum BasicDataType value_type) {
      if (key_to_value_type_map.find(key) != key_to_value_type_map.end()) {
        return false;
      }
      key_to_value_type_map.insert(std::make_pair(key, value_type));
      return true;
    }
    inline const enum BasicDataType GetAttributeValueTypeID(const KeyType_& key) const {
      assert(key_to_value_type_map.find(key) != key_to_value_type_map.end());
      return key_to_value_type_map.find(key)->second;
    }
    inline const std::string GetAttributeValueTypeName(const KeyType_& key) const {
      assert(key_to_value_type_map.find(key) != key_to_value_type_map.end());
      return EnumToString(key_to_value_type_map.find(key)->second);
    }
    inline AttributeIterator AttributeBegin() {
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
      enum BasicDataType value_type_id = TypeToEnum(value);
      this->SetValueType(key, value_type_id);
      std::get<kAttributeValuePtrIdx>(*(ret.first)) =
          new ConcreteValue<ConcreteDataType>(value);
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

 public:
  using VertexAttributePtr = typename VertexAttributeType ::AttributePtr;
  using VertexAttributeIterator =
      typename VertexAttributeType ::AttributeIterator;
  using VertexAttributeConstPtr =
      typename VertexAttributeType ::AttributeConstPtr;
  using VertexAttributeConstIterator =
      typename VertexAttributeType ::AttributeConstIterator;
  using EdgeAttributePtr = typename EdgeAttributeType ::AttributePtr;
  using EdgeAttributeIterator = typename EdgeAttributeType ::AttributeIterator;
  using EdgeAttributeConstPtr = typename EdgeAttributeType ::AttributeConstPtr;
  using EdgeAttributeConstIterator =
      typename EdgeAttributeType ::AttributeConstIterator;

 private:
  template <typename ContainerType_, bool is_const_, IteratorDepthType depth_,
            IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
            TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
            TupleIdxType edge_attribute_ptr_idx_>
  class EdgeContentIterator_
      : protected InnerIterator_<ContainerType_, is_const_, depth_> {
   public:
    friend typename InnerVertex_::EdgeIterator InnerVertex_::EraseEdge(
        typename InnerVertex_::EdgeIterator& edge_iterator);
    friend typename InnerVertex_::EdgeIteratorSpecifiedEdgeLabel
    InnerVertex_::EraseEdge(
        typename InnerVertex_::EdgeIteratorSpecifiedEdgeLabel& edge_iterator);
    friend typename InnerVertex_::template EdgePtr_<is_const_>;

   protected:
    using InnerIteratorType = InnerIterator_<ContainerType_, is_const_, depth_>;
    using VertexPtr = typename InnerVertex_::VertexPtr;
    using VertexConstPtr = typename InnerVertex_::VertexConstPtr;
    using VertexPtrType =
        typename std::conditional<is_const_, VertexConstPtr, VertexPtr>::type;
    using VertexPtrIteratorType =
        typename InnerVertex_::VertexPtrContainerType::iterator;
    using DecomposedEdgeIteratorType =
        typename InnerVertex_::DecomposedEdgeIteratorType;

    enum EdgeDirection direction_;
    VertexPtrType vertex_ptr_;

    inline const enum EdgeDirection& const_direction() const {
      return this->direction_;
    }
    inline enum EdgeDirection& direction() { return this->direction_; }
    inline const VertexPtrType& const_vertex_ptr() const {
      return this->vertex_ptr_;
    }
    inline VertexPtrType& vertex_ptr() { return this->vertex_ptr_; }

    inline EdgeAttributeType& _attribute() {
      assert(!this->IsDone());
      return *(
          InnerIteratorType::template get<
              EdgeAttributeType*, edge_attribute_ptr_idx_, begin_depth_ + 2>());
    }

    inline const EdgeAttributeType& _const_attribute() const {
      assert(!this->IsDone());
      return *(
          InnerIteratorType::template get_const<
              EdgeAttributeType*, edge_attribute_ptr_idx_, begin_depth_ + 2>());
    }

    inline VertexPtrType& VertexPtrContainerElement() {
      assert(!this->IsDone());
      return InnerIteratorType::template get<VertexPtr, dst_ptr_idx_,
                                             begin_depth_ + 1>();
    }

    inline VertexConstPtr VertexPtrContainerConstElement() const {
      assert(!this->IsDone());
      VertexPtr temp_vertex_ptr =
          InnerIteratorType::template get_const<VertexPtr, dst_ptr_idx_,
                                                begin_depth_ + 1>();
      return VertexConstPtr(temp_vertex_ptr);
    }

   protected:
    using InnerIteratorType::IsDone;
    using InnerIteratorType::ToNext;
    using ContentPtr =
        typename std::conditional<is_const_, const EdgeContentIterator_*,
                                  EdgeContentIterator_*>::type;
    static constexpr bool kIsConst_ = is_const_;

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline ContentPtr content_ptr() {
      assert(!this->IsDone());
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }
    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline ContentPtr content_ptr() const {
      assert(!this->IsDone());
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

   public:
    EdgeContentIterator_() : InnerIteratorType(), direction_(), vertex_ptr_() {
      return;
    }
    template <typename... ParameterTypes>
    EdgeContentIterator_(const enum EdgeDirection direction,
                         const VertexPtrType& vertex_ptr,
                         const ParameterTypes&... parameters)
        : InnerIteratorType(parameters...),
          direction_(direction),
          vertex_ptr_(vertex_ptr) {
      return;
    }
    inline VertexPtrIteratorType& vertex_ptr_iterator() {
      return this
          ->template get_iterator<VertexPtrIteratorType, begin_depth_ + 1>();
    }
    inline DecomposedEdgeIteratorType& decomposed_edge_iterator() {
      return this->template get_iterator<DecomposedEdgeIteratorType,
                                         begin_depth_ + 2>();
    }
    inline VertexPtrType& src_ptr() {
      assert(!this->IsDone());
      if (this->direction_ == EdgeDirection::OutputEdge)
        return this->vertex_ptr_;
      return this->VertexPtrContainerElement();
    }
    inline VertexPtrType& dst_ptr() {
      assert(!this->IsDone());
      if (this->direction_ == EdgeDirection::OutputEdge)
        return this->VertexPtrContainerElement();
      return this->vertex_ptr_;
    }
    inline const VertexConstPtr const_src_ptr() const {
      assert(!this->IsDone());
      if (this->direction_ == EdgeDirection::OutputEdge)
        return this->vertex_ptr_;
      return this->VertexPtrContainerConstElement();
    }
    inline const VertexConstPtr const_dst_ptr() const {
      assert(!this->IsDone());
      if (this->direction_ == EdgeDirection::OutputEdge)
        return this->VertexPtrContainerConstElement();
      return this->vertex_ptr_;
    }
    inline const EdgeLabelType& label() const {
      assert(!this->IsDone());
      return InnerIteratorType::template get_const<
          EdgeLabelType, edge_label_idx_, begin_depth_>();
    }
    inline const EdgeIDType& id() const {
      assert(!this->IsDone());
      return InnerIteratorType::template get_const<EdgeIDType, edge_id_idx_,
                                                   begin_depth_ + 2>();
    }
    template <typename ConcreteDataType>
    inline ConcreteDataType& attribute(const EdgeAttributeKeyType& key) {
      return this->_attribute().template attribute<ConcreteDataType>(key);
    }
    template <typename ConcreteDataType>
    inline const ConcreteDataType& const_attribute(
        const EdgeAttributeKeyType& key) const {
      return this->_const_attribute()
          .template _const_attribute<ConcreteDataType>(key);
    }
    inline EdgeAttributeIterator AttributeBegin() {
      assert(!this->IsDone());
      return this->_attribute().AttributeBegin();
    }
    inline EdgeAttributeConstIterator AttributeCBegin() const {
      assert(!this->IsDone());
      return this->_const_attribute().AttributeCBegin();
    }
    inline EdgeAttributePtr FindAttributePtr(const EdgeAttributeKeyType& key) {
      assert(!this->IsDone());
      return this->_attribute().FindAttributePtr(key);
    }
    inline EdgeAttributeConstPtr FindConstAttributePtr(
        const EdgeAttributeKeyType& key) const {
      assert(!this->IsDone());
      return this->_const_attribute().FindConstAttributePtr(key);
    }
    inline const std::string GetAttributeValueTypeName(
        const EdgeAttributeKeyType& key) const {
      return this->_const_attribute().GetAttributeValueTypeName(key);
    }
    inline const enum BasicDataType GetAttributeValueTypeID(
        const EdgeAttributeKeyType& key) const {
      return this->_const_attribute().GetAttributeValueTypeID(key);
    }
    template <typename ConcreteDataType>
    inline std::pair<EdgeAttributePtr, bool> AddAttribute(
        const EdgeAttributeKeyType& key, const ConcreteDataType& value) {
      assert(!this->IsDone());
      return this->_attribute().AddAttribute(key, value);
    }

    template <typename ConcreteDataType>
    inline std::pair<EdgeAttributePtr, bool> SetAttribute(
        const EdgeAttributeKeyType& key, const ConcreteDataType& value) {
      assert(!this->IsDone());
      return this->_attribute().SetAttribute(key, value);
    }

    inline EdgeAttributeIterator EraseAttribute(
        const EdgeAttributeIterator& attribute_iterator) {
      return this->_attribute().EraseAttribute(attribute_iterator);
    }

    inline bool EraseAttribute(const EdgeAttributeKeyType& key) {
      return this->_attribute().EraseAttribute(key);
    }
  };

  template <typename ContainerType_, bool is_const_, IteratorDepthType depth_,
            IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
            TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
            TupleIdxType edge_attribute_ptr_idx_>
  class EdgeContentIteratorSpecifiedEdgeLabel_
      : public EdgeContentIterator_<ContainerType_, is_const_, depth_,
                                    begin_depth_ - 1, edge_label_idx_,
                                    dst_ptr_idx_, edge_id_idx_,
                                    edge_attribute_ptr_idx_> {
   private:
    friend typename InnerVertex_::EdgeIteratorSpecifiedEdgeLabel
    InnerVertex_::EraseEdge(
        typename InnerVertex_::EdgeIteratorSpecifiedEdgeLabel& edge_iterator);

    using InnerIteratorType =
        EdgeContentIterator_<ContainerType_, is_const_, depth_,
                             begin_depth_ - 1, edge_label_idx_, dst_ptr_idx_,
                             edge_id_idx_, edge_attribute_ptr_idx_>;
    using VertexPtr = typename InnerVertex_::VertexPtr;
    //   const EdgeLabelType edge_label_;
    using EdgeLabelIterator = typename std::conditional<
        is_const_,
        typename InnerVertex_::EdgeLabelContainerType::const_iterator,
        typename InnerVertex_::EdgeLabelContainerType::iterator>::type;
    EdgeLabelIterator edge_label_iterator_;

   protected:
    using InnerIteratorType::IsDone;
    using InnerIteratorType::ToNext;
    using ContentPtr = typename std::conditional<
        is_const_, const EdgeContentIteratorSpecifiedEdgeLabel_*,
        EdgeContentIteratorSpecifiedEdgeLabel_*>::type;
    static constexpr bool kIsConst_ = is_const_;

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline ContentPtr content_ptr() {
      assert(!this->IsDone());
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }
    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline ContentPtr content_ptr() const {
      assert(!this->IsDone());
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }
    EdgeLabelIterator get_iterator() { return this->edge_label_iterator_; }

   public:
    EdgeContentIteratorSpecifiedEdgeLabel_()
        : InnerIteratorType(), edge_label_iterator_() {
      return;
    }
    template <typename... ParameterTypes>
    EdgeContentIteratorSpecifiedEdgeLabel_(
        const EdgeLabelIterator& edge_label_iterator,
        const ParameterTypes&... parameters)
        : InnerIteratorType(parameters...),
          edge_label_iterator_(edge_label_iterator) {
      return;
    }
    inline const EdgeLabelType& label() const {
      assert(!this->IsDone());
      return std::get<InnerVertex_::kEdgeLabelIdx>(
          *(this->edge_label_iterator_));
    }
  };

  template <typename ContainerType_, bool is_const_, IteratorDepthType depth_,
            TupleIdxType vertex_ptr_idx_>
  class VertexContentIterator_
      : protected InnerIterator_<ContainerType_, is_const_, depth_> {
   private: /*
     template<typename        _ContainerType_,
            IteratorDepthType         _depth_,
            TupleIdxType     _vertex_ptr_idx_>
     friend void InnerVertex_::template VertexPtr_<is_const_>::Construct(
       const Iterator_<VertexContentIterator_<_ContainerType_,
                                                    is_const_,
                                                      _depth_,
                                             _vertex_ptr_idx_>>&
     vertex_ptr_iterator);*/
    friend typename InnerVertex_::template VertexPtr_<is_const_>;

    using VertexPtr = typename InnerVertex_::VertexPtr;
    using VertexConstPtr = typename InnerVertex_::VertexConstPtr;
    using VertexPtrType =
        typename std::conditional<is_const_, const VertexConstPtr,
                                  VertexPtr>::type;
    using InnerIteratorType = InnerIterator_<ContainerType_, is_const_, depth_>;

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline VertexPtr& vertex_ptr() {
      return InnerIteratorType::template get<ContentPtr, vertex_ptr_idx_,
                                             depth_ - 1>();
    }

    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline VertexConstPtr vertex_ptr() const {
      auto temp_vertex_ptr =
          InnerIteratorType::template get_const<VertexPtr, vertex_ptr_idx_,
                                                depth_ - 1>();
      return ContentPtr(temp_vertex_ptr);
    }

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline const VertexPtr& const_vertex_ptr() const {
      return InnerIteratorType::template get_const<VertexPtr, vertex_ptr_idx_,
                                                   depth_ - 1>();
    }

    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline VertexConstPtr const_vertex_ptr() const {
      auto temp_vertex_ptr =
          InnerIteratorType::template get_const<VertexPtr, vertex_ptr_idx_,
                                                depth_ - 1>();
      return VertexConstPtr(temp_vertex_ptr);
    }

   protected:
    using InnerIteratorType::InnerIteratorType;
    using InnerIteratorType::IsDone;
    using InnerIteratorType::ToNext;
    using ContentPtr = VertexPtrType;
    static constexpr bool kIsConst_ = is_const_;

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline ContentPtr& content_ptr() {
      assert(!this->IsDone());
      return this->vertex_ptr();
    }

    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline ContentPtr content_ptr() const {
      assert(!this->IsDone());
      return this->vertex_ptr();
    }
  };

 public:
  template <typename ContainerType_, bool is_const_, IteratorDepthType depth_,
            TupleIdxType edge_label_idx_>
  class EdgeLabelContentIterator_
      : protected InnerIterator_<ContainerType_, is_const_, depth_> {
   private:
    using InnerIteratorType = InnerIterator_<ContainerType_, is_const_, depth_>;

   protected:
    using InnerIteratorType::InnerIteratorType;
    using InnerIteratorType::IsDone;
    using InnerIteratorType::ToNext;
    using ContentPtr =
        typename std::conditional<is_const_, const EdgeLabelContentIterator_*,
                                  EdgeLabelContentIterator_*>::type;
    static constexpr bool kIsConst_ = is_const_;

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline ContentPtr content_ptr() {
      assert(!this->IsDone());
      ContentPtr const temp_this_ptr = this;
      return this;
    }

    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline ContentPtr content_ptr() const {
      assert(!this->IsDone());
      ContentPtr const temp_this_ptr = this;
      return temp_this_ptr;
    }

   public:
    inline const EdgeLabelType& label() const {
      assert(!this->IsDone());
      return InnerIteratorType::template get_const<
          EdgeLabelType, edge_label_idx_, depth_ - 1>();
    }
  };

 public:
  /// the class that provides the basic information about both vertex and edge
  template <typename IDType_, typename LabelType_, bool has_static_attribtue_,
            typename StaticAttributeType_, bool has_dynamic_attribtue_,
            typename AttributeKeyType_>
  class Type_ {
    /// trick compiler, same to static_assert(false);
    static_assert(has_static_attribtue_, "Unsupported configuration");
    static_assert(!has_static_attribtue_, "Unsupported configuration");
  };
  template <typename IDType_, typename LabelType_,
            typename StaticAttributeType_, typename AttributeKeyType_>
  class Type_<IDType_, LabelType_, false, StaticAttributeType_, false,
              AttributeKeyType_> {
   public:
    using IDType = IDType_;
    using LabelType = LabelType_;
  };
  template <typename IDType_, typename LabelType_,
            typename StaticAttributeType_, typename AttributeKeyType_>
  class Type_<IDType_, LabelType_, true, StaticAttributeType_, false,
              AttributeKeyType_>
      : public Type_<IDType_, LabelType_, false, StaticAttributeType_, false,
                     AttributeKeyType_> {
   public:
    using StaticAttributeType = StaticAttributeType_;
  };
  template <typename IDType_, typename LabelType_, bool has_static_attribtue_,
            typename StaticAttributeType_, typename AttributeKeyType_>
  class Type_<IDType_, LabelType_, has_static_attribtue_, StaticAttributeType_,
              true, AttributeKeyType_>
      : public Type_<IDType_, LabelType_, has_static_attribtue_,
                     StaticAttributeType_, false, AttributeKeyType_> {
   public:
    using AttributeKeyType = AttributeKeyType_;
  };

 private:
  using VertexWithIDType = WithID_<VertexIDType>;
  using VertexWithLabelType =
      WithLabel_<VertexLabelType, vertex_label_is_const>;
  /// this class is transparent to user
  class InnerVertex_ : public VertexWithIDType,
                       public VertexWithLabelType,
                       public VertexAttributeType {
   public:
    template <bool is_const_>
    class VertexPtr_ {
     private:
      friend class VertexPtr_<!is_const_>;

      using VertexPtrType =
          typename std::conditional<is_const_, const InnerVertex_*,
                                    InnerVertex_*>::type;
      VertexPtrType ptr_;

      template <typename _ContainerType_, IteratorDepthType _depth_,
                TupleIdxType _vertex_ptr_idx_>
      using FriendVertexContentIterator =
          VertexContentIterator_<_ContainerType_, is_const_, _depth_,
                                 _vertex_ptr_idx_>;

      template <typename _ContainerType_, IteratorDepthType _depth_,
                TupleIdxType _vertex_ptr_idx_>
      using FriendVertexIterator =
          Iterator_<FriendVertexContentIterator<_ContainerType_, _depth_,
                                                _vertex_ptr_idx_>>;

      template <typename _ContainerType_, IteratorDepthType _depth_,
                TupleIdxType _vertex_ptr_idx_>
      inline void Construct(
          const FriendVertexIterator<_ContainerType_, _depth_,
                                     _vertex_ptr_idx_>& vertex_ptr_iterator) {
        const void* const ptr = &vertex_ptr_iterator;

        this->ptr_ = (static_cast<const FriendVertexContentIterator<
                          _ContainerType_, _depth_, _vertex_ptr_idx_>*>(ptr))
                         ->const_vertex_ptr()
                         .ptr_;
        return;
      }

     public:
      VertexPtr_() : ptr_(nullptr) { return; }

      VertexPtr_(VertexPtrType const ptr) : ptr_(ptr) { return; }

      template <typename _ContainerType_, IteratorDepthType _depth_,
                TupleIdxType _vertex_ptr_idx_>
      VertexPtr_(
          const FriendVertexIterator<_ContainerType_, _depth_,
                                     _vertex_ptr_idx_>& vertex_ptr_iterator) {
        this->Construct(vertex_ptr_iterator);
        return;
      }

      template <typename _ContainerType_, IteratorDepthType _depth_,
                TupleIdxType _vertex_ptr_idx_>
      inline VertexPtr_& operator=(
          const FriendVertexIterator<_ContainerType_, _depth_,
                                     _vertex_ptr_idx_>& vertex_ptr_iterator) {
        this->Construct(vertex_ptr_iterator);
        return *this;
      }

      template <bool input_is_const_, bool judge = is_const_,
                typename std::enable_if<judge, bool>::type = false>
      VertexPtr_(const VertexPtr_<input_is_const_>& vertex_ptr)
          : ptr_(vertex_ptr.ptr_) {
        return;
      }

      template <bool input_is_const_, bool judge = is_const_,
                typename std::enable_if<!judge, bool>::type = false>
      VertexPtr_(VertexPtr_<input_is_const_>& vertex_ptr)
          : ptr_(vertex_ptr.ptr_) {
        return;
      }

      inline bool operator==(const VertexPtr_& ptr) const {
        assert(this->ptr_ != ptr.ptr_ || this->ptr_->id() == ptr.ptr_->id());
        return this->ptr_ == ptr.ptr_;
      }
      inline bool operator!=(const VertexPtr_& ptr) const {
        return this->ptr_ != ptr.ptr_;
      }
      inline bool operator<(const VertexPtr_& ptr) const {
        return this->ptr_ < ptr.ptr_;
      }
      inline bool operator>(const VertexPtr_& ptr) const {
        return this->ptr_ > ptr.ptr_;
      }
      inline bool operator<=(const VertexPtr_& ptr) const {
        return this->ptr_ <= ptr.ptr_;
      }
      inline bool operator>=(const VertexPtr_& ptr) const {
        return this->ptr_ >= ptr.ptr_;
      }

      inline VertexPtr_& operator=(const VertexPtr_& ptr) {
        this->ptr_ = ptr.ptr_;
        return *this;
      }

      template <const bool judge = is_const_,
                typename std::enable_if<!judge, bool>::type = false>
      inline VertexPtrType operator->() {
        static_assert(judge == is_const_, "Illegal usage of this method");
        return this->ptr_;
      }

      template <const bool judge = is_const_,
                typename std::enable_if<judge, bool>::type = false>
      inline VertexPtrType operator->() const {
        static_assert(judge == is_const_, "Illegal usage of this method");
        return this->ptr_;
      }

      inline void Release() {
        delete this->ptr_;
        return;
      }

      inline bool IsNull() const { return this->ptr_ == nullptr; }
    };

    using VertexPtr = VertexPtr_<false>;
    using VertexConstPtr = VertexPtr_<true>;

   public:
    static constexpr TupleIdxType kEdgeIDIdx = 0;
    static constexpr TupleIdxType kEdgeAttributePtrIdx = 1;
    using DecomposedEdgeContainerType =
        Container<decomposed_edge_container_type,
                  decomposed_edge_container_sort_type, EdgeIDType,
                  EdgeAttributeType*>;
    using DecomposedEdgeIteratorType =
        typename DecomposedEdgeContainerType::iterator;

    static constexpr TupleIdxType kVertexPtrIdx = 0;
    static constexpr TupleIdxType kDecomposedEdgeContainerIdx = 1;
    using VertexPtrContainerType =
        Container<vertex_ptr_container_type, vertex_ptr_container_sort_type,
                  VertexPtr, DecomposedEdgeContainerType>;
    using VertexPtrIteratorType = typename VertexPtrContainerType::iterator;
    static constexpr TupleIdxType kEdgeLabelIdx = 0;
    static constexpr TupleIdxType kVertexPtrContainerIdx = 1;
    using EdgeLabelContainerType =
        Container<edge_label_container_type, edge_label_container_sort_type,
                  EdgeLabelType, VertexPtrContainerType>;
    using EdgeLabelIteratorType = typename EdgeLabelContainerType::iterator;

    //    static constexpr TupleIdxType kEdgeIsOutputIdx = 0;
    //    static constexpr TupleIdxType kEdgeLabelContainerIdx = 1;
    //    using EdgeContainerType =
    //        Container<ContainerType::Vector,
    //                  SortType::None,
    //                  bool,
    //                  EdgeLabelContainerType>;

    template <bool is_const_, bool with_const_methods_,
              bool meaning_less_ = true>
    class EdgePtrContent_ {
     private:
      /// trick the compiler, same to static_assert(false)
      static_assert(is_const_, "Illegal configuration");
      static_assert(!is_const_, "Illegal configuration");
    };

    /// content pointer with only constant method
    template <bool is_const_, bool meaning_less_>
    class EdgePtrContent_<is_const_, false, meaning_less_> {
      static_assert(meaning_less_, "Illegal usage of this method");

     public:
     protected:
      using EdgeLabelIteratorType = typename std::conditional<
          is_const_, typename EdgeLabelContainerType::const_iterator,
          typename EdgeLabelContainerType::iterator>::type;
      using VertexPtrIteratorType = typename std::conditional<
          is_const_, typename VertexPtrContainerType::const_iterator,
          typename VertexPtrContainerType::iterator>::type;
      using DecomposedEdgeIteratorType = typename std::conditional<
          is_const_, typename DecomposedEdgeContainerType::const_iterator,
          typename DecomposedEdgeContainerType::iterator>::type;

      using VertexPtrType =
          typename std::conditional<is_const_, VertexConstPtr, VertexPtr>::type;

     public:
      enum EdgeDirection direction_;
      VertexPtrType vertex_ptr_;

      EdgeLabelIteratorType edge_label_iterator_;
      VertexPtrIteratorType vertex_ptr_iterator_;
      DecomposedEdgeIteratorType decomposed_edge_iterator_;

      inline const VertexConstPtr VertexPtrContainerConstElement() const {
        return std::get<kVertexPtrIdx>(*(this->vertex_ptr_iterator_));
      }

      inline const EdgeAttributeType& const_attribute() const {
        return *(
            std::get<kEdgeAttributePtrIdx>(*(this->decomposed_edge_iterator_)));
      }

      inline bool IsNull() const { return this->vertex_ptr_.IsNull(); }

     public:
      EdgePtrContent_()
          : direction_(),
            vertex_ptr_(),
            edge_label_iterator_(),
            vertex_ptr_iterator_(),
            decomposed_edge_iterator_() {
        return;
      }

      EdgePtrContent_(
          const enum EdgeDirection& direction, const VertexPtrType& vertex_ptr,
          const EdgeLabelIteratorType& edge_label_iterator,
          const VertexPtrIteratorType& vertex_ptr_iterator,
          const DecomposedEdgeIteratorType& decomposed_edge_iterator)
          : direction_(direction),
            vertex_ptr_(vertex_ptr),
            edge_label_iterator_(edge_label_iterator),
            vertex_ptr_iterator_(vertex_ptr_iterator),
            decomposed_edge_iterator_(decomposed_edge_iterator) {
        return;
      }

      /// public methods same to ContentEdgeIterator_
      inline const EdgeLabelType& label() const {
        return std::get<kEdgeLabelIdx>(*(this->edge_label_iterator_));
      }
      inline const EdgeIDType& id() const {
        return std::get<kEdgeIDIdx>(*(this->decomposed_edge_iterator_));
      }

      inline const VertexConstPtr const_src_ptr() const {
        if (this->direction_ == EdgeDirection::OutputEdge)
          return this->vertex_ptr_;
        return this->VertexPtrContainerConstElement();
      }

      inline const VertexConstPtr const_dst_ptr() const {
        if (this->direction_ == EdgeDirection::OutputEdge)
          return this->VertexPtrContainerConstElement();
        return this->vertex_ptr_;
      }

      template <typename ConcreteDataType>
      inline const ConcreteDataType& const_attribute(
          const EdgeAttributeKeyType& key) const {
        return this->const_attribute()
            .template const_attribute<ConcreteDataType>(key);
      }

      inline EdgeAttributeConstIterator AttributeCBegin() const {
        return this->const_attribute().AttributeCBegin();
      }

      inline EdgeAttributeConstPtr FindConstAttributePtr(
          const EdgeAttributeKeyType& key) const {
        return this->const_attribute().FindConstAttributePtr(key);
      }
      inline const std::string GetAttributeValueTypeName(
          const EdgeAttributeKeyType& key) const {
        return this->const_attribute().GetAttributeValueTypeName(key);
      }
      inline const enum BasicDataType GetAttributeValueTypeID(
          const EdgeAttributeKeyType& key) const {
        return this->const_attribute().GetAttributeValueTypeID(key);
      }
    };

    /// content pointer with non-constant method
    template <bool meaning_less_>
    class EdgePtrContent_<false, true, meaning_less_>
        : public EdgePtrContent_<false, false, meaning_less_> {
      static_assert(meaning_less_, "Illegal usage of this method");

     private:
      using EdgePtrContent = EdgePtrContent_<false, false, meaning_less_>;

      inline VertexPtr& VertexPtrContainerElement() {
        return std::get<kVertexPtrIdx>(*(EdgePtrContent::vertex_ptr_iterator_));
      }

      inline EdgeAttributeType& _attribute() {
        return *(std::get<kEdgeAttributePtrIdx>(
            *(EdgePtrContent::decomposed_edge_iterator_)));
      }

     public:
      using EdgePtrContent::EdgePtrContent;
      inline VertexPtr& src_ptr() {
        if (EdgePtrContent::direction_ == EdgeDirection::OutputEdge)
          return EdgePtrContent::vertex_ptr_;
        return this->VertexPtrContainerElement();
      }

      inline VertexPtr& dst_ptr() {
        if (EdgePtrContent::direction_ == EdgeDirection::OutputEdge)
          return this->VertexPtrContainerElement();
        return EdgePtrContent::vertex_ptr_;
      }
      template <typename ConcreteDataType>
      inline ConcreteDataType& attribute(const EdgeAttributeKeyType& key) {
        return this->_attribute().template attribute<ConcreteDataType>(key);
      }

      inline EdgeAttributeIterator AttributeBegin() {
        return this->_attribute().AttributeBegin();
      }

      inline EdgeAttributePtr FindAttributePtr(
          const EdgeAttributeKeyType& key) {
        return this->_attribute().FindAttributePtr(key);
      }
      /*
      inline bool SetValueType(const EdgeAttributeKeyType& key,
                               const std::string value_type) {
        return this->_attribute().SetValueType(key, value_type);
      }
      */
      template <typename ConcreteDataType>
      inline std::pair<EdgeAttributePtr, bool> AddAttribute(
          const EdgeAttributeKeyType& key, const ConcreteDataType& value) {
        return this->_attribute().AddAttribute(key, value);
      }

      template <typename ConcreteDataType>
      inline std::pair<EdgeAttributePtr, bool> SetAttribute(
          const EdgeAttributeKeyType& key, const ConcreteDataType& value) {
        return this->_attribute().SetAttribute(key, value);
      }

      inline EdgeAttributeIterator EraseAttribute(
          const EdgeAttributeIterator& attribute_iterator) {
        return this->_attribute().EraseAttribute(attribute_iterator);
      }

      inline bool EraseAttribute(const EdgeAttributeKeyType& key) {
        return this->_attribute().EraseAttribute(key);
      }
    };

   public:
    template <bool is_const_>
    class EdgePtr_ : protected EdgePtrContent_<is_const_, !is_const_> {
     private:
      friend class EdgePtr_<!is_const_>;
      using EdgePtrContentType = EdgePtrContent_<is_const_, !is_const_>;

      using EdgePtrContentTypePtrType =
          typename std::conditional<is_const_, const EdgePtrContentType*,
                                    EdgePtrContentType*>::type;

      // add by wangyj
      template <typename ContainerType_, IteratorDepthType depth_,
                IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
                TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
                TupleIdxType edge_attribute_ptr_idx_>
      using FriendEdgeContentIterator =
          EdgeContentIterator_<ContainerType_, is_const_, depth_, begin_depth_,
                               edge_label_idx_, dst_ptr_idx_, edge_id_idx_,
                               edge_attribute_ptr_idx_>;
      template <typename ContainerType_, IteratorDepthType depth_,
                IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
                TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
                TupleIdxType edge_attribute_ptr_idx_>
      using FriendEdgeIterator = Iterator_<FriendEdgeContentIterator<
          ContainerType_, depth_, begin_depth_, edge_label_idx_, dst_ptr_idx_,
          edge_id_idx_, edge_attribute_ptr_idx_>>;
      template <typename ContainerType_, IteratorDepthType depth_,
                IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
                TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
                TupleIdxType edge_attribute_ptr_idx_>
      inline void Construct(
          const FriendEdgeIterator<ContainerType_, depth_, begin_depth_,
                                   edge_label_idx_, dst_ptr_idx_, edge_id_idx_,
                                   edge_attribute_ptr_idx_>&
              vertex_ptr_iterator) {
        using FriendEdgeConstPtrType = const FriendEdgeContentIterator<
            ContainerType_, depth_, begin_depth_, edge_label_idx_, dst_ptr_idx_,
            edge_id_idx_, edge_attribute_ptr_idx_>*;

        const void* const ptr = &vertex_ptr_iterator;
        this->direction_ =
            (static_cast<FriendEdgeConstPtrType>(ptr))->const_direction();
        this->vertex_ptr_ =
            (static_cast<FriendEdgeConstPtrType>(ptr))->const_vertex_ptr();

        this->edge_label_iterator_ =
            (static_cast<FriendEdgeConstPtrType>(ptr))
                ->template get_const_iterator<
                    typename EdgePtrContentType ::EdgeLabelIteratorType, 0>();

        this->vertex_ptr_iterator_ =
            (static_cast<FriendEdgeConstPtrType>(ptr))
                ->template get_const_iterator<
                    typename EdgePtrContentType ::VertexPtrIteratorType, 1>();
        this->decomposed_edge_iterator_ =
            (static_cast<FriendEdgeConstPtrType>(ptr))
                ->template get_const_iterator<
                    typename EdgePtrContentType ::DecomposedEdgeIteratorType,
                    2>();

        return;
      }

     public:
      using EdgePtrContentType::IsNull;

      template <typename... ParameterTypes>
      EdgePtr_(const ParameterTypes&... parameters)
          : EdgePtrContentType(parameters...) {
        return;
      }
      template <typename ContainerType_, IteratorDepthType depth_,
                IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
                TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
                TupleIdxType edge_attribute_ptr_idx_>
      EdgePtr_(const FriendEdgeIterator<ContainerType_, depth_, begin_depth_,
                                        edge_label_idx_, dst_ptr_idx_,
                                        edge_id_idx_, edge_attribute_ptr_idx_>&
                   vertex_ptr_iterator) {
        this->Construct(vertex_ptr_iterator);
        return;
      }
      template <typename ContainerType_, IteratorDepthType depth_,
                IteratorDepthType begin_depth_, TupleIdxType edge_label_idx_,
                TupleIdxType dst_ptr_idx_, TupleIdxType edge_id_idx_,
                TupleIdxType edge_attribute_ptr_idx_>
      inline EdgePtr_& operator=(
          const FriendEdgeIterator<ContainerType_, depth_, begin_depth_,
                                   edge_label_idx_, dst_ptr_idx_, edge_id_idx_,
                                   edge_attribute_ptr_idx_>&
              vertex_ptr_iterator) {
        this->Construct(vertex_ptr_iterator);
        return *this;
      }

      template <bool input_is_const_>
      inline bool operator==(const EdgePtr_<input_is_const_>& edge_ptr) const {
        using InputEdgePtrType = EdgePtr_<input_is_const_>;
        using InputEdgePtrContentType =
            typename InputEdgePtrType::EdgePtrContentType;
        assert((edge_ptr.InputEdgePtrContentType::id() !=
                EdgePtrContentType::id()) ||
               !((edge_ptr.InputEdgePtrContentType::label() !=
                  EdgePtrContentType::label()) ||
                 (edge_ptr.InputEdgePtrContentType::const_src_ptr() !=
                  EdgePtrContentType::const_src_ptr()) ||
                 (edge_ptr.InputEdgePtrContentType::const_dst_ptr() !=
                  EdgePtrContentType::const_dst_ptr())));
        return edge_ptr.InputEdgePtrContentType::id() ==
               EdgePtrContentType::id();
      }
      template <bool judge = is_const_,
                typename std::enable_if<judge, bool>::type = false>
      inline EdgePtrContentTypePtrType operator->() const {
        static_assert(judge == is_const_, "Illegal usage of this method");
        EdgePtrContentTypePtrType const temp_ptr = this;
        return temp_ptr;
      }

      template <bool judge = is_const_,
                typename std::enable_if<!judge, bool>::type = false>
      inline EdgePtrContentTypePtrType operator->() {
        static_assert(judge == is_const_, "Illegal usage of this method");
        EdgePtrContentTypePtrType const temp_ptr = this;
        return temp_ptr;
      }
    };

    using EdgePtr = EdgePtr_<false>;
    using EdgeConstPtr = EdgePtr_<true>;

   private:
    template <enum StoreData store_data, bool meaning_less>
    class Edges;

    template <bool meaning_less>
    class Edges<StoreData::OutEdge, meaning_less> {
     private:
      static_assert(meaning_less, "Illegal configuration");
      EdgeLabelContainerType out_edges_;

     public:
      inline const EdgeLabelContainerType& const_out_edges() const {
        return this->out_edges_;
      }
      inline EdgeLabelContainerType& out_edges() { return this->out_edges_; }
    };

    template <bool meaning_less>
    class Edges<StoreData::InOutEdge, meaning_less>
        : public Edges<StoreData::OutEdge, meaning_less> {
     private:
      static_assert(meaning_less, "Illegal configuration");
      EdgeLabelContainerType in_edges_;

     public:
      inline const EdgeLabelContainerType& const_in_edges() const {
        return this->in_edges_;
      }
      inline EdgeLabelContainerType& in_edges() { return this->in_edges_; }
    };

    Edges<store_data, true> edges_;
    inline void AddInEdge(const VertexPtr& dst_ptr,
                          const EdgeLabelType& edge_label,
                          const EdgeIDType& edge_id,
                          EdgeAttributeType* const edge_attribute_ptr) {
      auto edge_label_it = this->edges_.in_edges().Insert(edge_label).first;
      auto vertex_ptr_it = std::get<kVertexPtrContainerIdx>(*edge_label_it)
                               .Insert(dst_ptr)
                               .first;
      auto edge_it = std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it)
                         .Insert(edge_id)
                         .first;
      std::get<kEdgeAttributePtrIdx>(*edge_it) = edge_attribute_ptr;
      return;
    }

    inline std::pair<EdgePtr, bool> AddOutEdge(
        const VertexPtr& dst_ptr, const EdgeLabelType& edge_label,
        const EdgeIDType& edge_id,
        EdgeAttributeType* const edge_attribute_ptr) {
      auto edge_label_it = this->edges_.out_edges().Insert(edge_label).first;
      auto vertex_ptr_it = std::get<kVertexPtrContainerIdx>(*edge_label_it)
                               .Insert(dst_ptr)
                               .first;
      auto edge_it = std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it)
                         .Insert(edge_id)
                         .first;
      std::get<kEdgeAttributePtrIdx>(*edge_it) = edge_attribute_ptr;
      InnerVertex_* const temp_this_ptr = this;
      VertexPtr temp_vertex_ptr(temp_this_ptr);
      assert(allow_duplicate_edge);
      /// otherwise, needs to decide the second element in the pair
      return std::pair<EdgePtr, bool>(
          EdgePtr(EdgeDirection::OutputEdge,
                  temp_vertex_ptr,  /// src_ptr
                  edge_label_it, vertex_ptr_it, edge_it),
          true);
    }

    /*
    inline bool EraseOutEdge(const VertexPtr& dst_ptr,
                             const EdgeLabelType& edge_label,
                             const EdgeIDType& edge_id) {
      auto& out_edges_container = this->edges_.out_edges();
      auto edge_label_it = out_edges_container.Find(edge_label).first;
      auto& vertex_ptr_container =
          std::get<kVertexPtrContainerIdx>(*edge_label_it);
      auto vertex_ptr_it = vertex_ptr_container.Find(dst_ptr).first;
      auto& edge_id_container =
          std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it);
      auto edge_it = edge_id_container.Find(edge_id).first;
      // erase edge_attr
      delete std::get<kEdgeAttributePtrIdx>(*edge_it);
      // erase edge_id
      edge_id_container.Erase(edge_it);
      // erase dst_ptr
      if (edge_id_container.empty()) {
        vertex_ptr_container.Erase(vertex_ptr_it);
      }
      // erase edge_label
      if (vertex_ptr_container.empty()) {
        out_edges_container.Erase(edge_label_it);
      }
      return true;
    }
    */

   public:
    InnerVertex_(const VertexIDType& id, const VertexLabelType& label)
        : VertexWithIDType(id),
          VertexWithLabelType(label),
          VertexAttributeType() {
      return;
    }

    ~InnerVertex_() {
      for (auto& edge_label_it : this->edges_.out_edges()) {
        for (auto& vertex_ptr_it :
             std::get<kVertexPtrContainerIdx>(edge_label_it)) {
          for (auto& decomposed_edge_it :
               std::get<kDecomposedEdgeContainerIdx>(vertex_ptr_it)) {
            delete std::get<kEdgeAttributePtrIdx>(decomposed_edge_it);
          }
        }
      }
      return;
    }
    /*
    inline bool EraseEdge(VertexPtr& dst_ptr, const EdgeLabelType& edge_label,
                          const EdgeIDType& edge_id) {
      auto ret = this->FindOutEdge(edge_id);
      if (ret.IsNull()) {
        return false;
      }
      InnerVertex_* const temp_this_ptr = this;
      VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
      dst_ptr->EraseInEdge(temp_vertex_ptr, edge_label, edge_id);
      this->EraseOutEdge(dst_ptr, edge_label, edge_id);
      return true;
    }*/

   private:
    inline void EraseEdge(const enum EdgeDirection& edge_direction,
                          const EdgeLabelType& edge_label,
                          const VertexPtr& vertex_ptr,
                          const EdgeIDType& edge_id) {
      // first level container, decide which container to remove
      EdgeLabelContainerType* edge_label_container_ptr = nullptr;
      if (edge_direction == EdgeDirection::OutputEdge)
        edge_label_container_ptr = &this->edges_.out_edges();
      else
        edge_label_container_ptr = &this->edges_.in_edges();
      assert(edge_label_container_ptr != nullptr);
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container_ptr->Find(edge_label);
      assert(edge_label_ret.second);
      // second level container
      VertexPtrContainerType& vertex_ptr_container =
          std::get<kVertexPtrContainerIdx>(*(edge_label_ret.first));
      /// <iterator of VertexPtrContainer, bool>
      auto vertex_ptr_ret = vertex_ptr_container.Find(vertex_ptr);
      assert(vertex_ptr_ret.second);
      // third level container
      DecomposedEdgeContainerType& decomposed_edge_container =
          std::get<kDecomposedEdgeContainerIdx>(*(vertex_ptr_ret.first));
      /// <iterator of DecomposedEdgeContainer, bool>
      auto decomposed_edge_ret = decomposed_edge_container.Find(edge_id);
      assert(decomposed_edge_ret.second);

      decomposed_edge_container.Erase(decomposed_edge_ret.first);
      if (!decomposed_edge_container.empty()) return;
      // the decomposed_edge_container is empty,
      // therefore, there are no edge point to vertex_ptr
      // remove it from the vertex_ptr_container
      vertex_ptr_container.Erase(vertex_ptr_ret.first);
      if (!vertex_ptr_container.empty()) return;
      // the vertex_ptr_container is empty,
      // therefore, there are no edge has Label edge_label
      // remove it from the edge_label_container
      edge_label_container_ptr->Erase(edge_label_ret.first);
      return;
    }

    inline bool EraseEdge(EdgeLabelContainerType& edge_label_contaienr,
                          const enum EdgeDirection& edge_direction,
                          const EdgeIDType& edge_id) {
      for (auto edge_label_it = edge_label_contaienr.begin();
           edge_label_it != edge_label_contaienr.end(); edge_label_it++) {
        VertexPtrContainerType& vertex_ptr_container =
            std::get<kVertexPtrContainerIdx>(*edge_label_it);
        for (auto vertex_ptr_it = vertex_ptr_container.begin();
             vertex_ptr_it != vertex_ptr_container.end(); vertex_ptr_it++) {
          DecomposedEdgeContainerType& decomposed_edge_container =
              std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it);
          auto edge_id_ret = decomposed_edge_container.Find(edge_id);
          if (!edge_id_ret.second) {
            //  not found edge_id in this container
            continue;
          }
          delete std::get<kEdgeAttributePtrIdx>(*(edge_id_ret.first));
          decomposed_edge_container.Erase(edge_id_ret.first);
          InnerVertex_* temp_this_ptr = this;
          VertexPtr temp_vertex_ptr(temp_this_ptr);
          std::get<kVertexPtrIdx>(*vertex_ptr_it)
              ->EraseEdge(edge_direction == EdgeDirection::InputEdge
                              ? EdgeDirection::OutputEdge
                              : EdgeDirection::InputEdge,
                          std::get<kEdgeLabelIdx>(*edge_label_it),
                          temp_vertex_ptr, edge_id);

          if (!decomposed_edge_container.empty()) return true;
          // the decomposed_edge_container is empty,
          // therefore, there are no edge point to vertex_ptr
          // remove it from the vertex_ptr_container
          vertex_ptr_container.Erase(vertex_ptr_it);
          if (!vertex_ptr_container.empty()) return true;
          // the vertex_ptr_container is empty,
          // therefore, there are no edge has Label edge_label
          // remove it from the edge_label_container
          edge_label_contaienr.Erase(edge_label_it);
          return true;
        }
      }
      return false;
    }

   public:
    inline bool EraseEdge(const EdgeIDType& edge_id) {
      if (this->EraseEdge(this->edges_.out_edges(), EdgeDirection::OutputEdge,
                          edge_id))
        return true;
      return this->EraseEdge(this->edges_.in_edges(), EdgeDirection::InputEdge,
                             edge_id);
    }
    /*
    inline EdgeIterator EraseEdge(EdgeIterator& edge_iterator) {
      // check IsDone
      if (!edge_iterator.IsDone()) {
        return edge_iterator;
      }
      std::pair<EdgeIterator, bool> erase_res = this->EraseEdge(
          this->edges_.out_edges(), EdgeDirection::OutputEdge, edge_iterator);
      if (erase_res.second) return erase_res.first;
      erase_res = this->EraseEdge(this->edges_.in_edges(),
                                  EdgeDirection::InputEdge, edge_iterator);
      return erase_res.first;
    }
    */
    /*
    inline bool EraseOutEdge(const EdgeIDType& edge_id) {
      return this->EraseEdge(this->edges_.out_edges(), edge_id);
    }
    inline bool EraseInEdge(const EdgeIDType& edge_id) {
      return this->EraseEdge(this->edges_.in_edges(), edge_id);
    }*/

    inline std::pair<EdgePtr, bool> AddEdge(VertexPtr& dst_ptr,
                                            const EdgeLabelType& edge_label,
                                            const EdgeIDType& edge_id) {
      auto ret = this->FindOutEdge(edge_id);
      if (!ret.IsNull()) {
        /// this Edge has already been existed
        return std::pair<EdgePtr, bool>(ret, false);
      }
      EdgeAttributeType* edge_attribute_ptr = new EdgeAttributeType();
      InnerVertex_* const temp_this_ptr = this;
      VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
      dst_ptr->AddInEdge(temp_vertex_ptr, edge_label, edge_id,
                         edge_attribute_ptr);
      return this->AddOutEdge(dst_ptr, edge_label, edge_id, edge_attribute_ptr);
    }
    /// possible extension:
    ///     AddEdge(dst_ptr,edge_label);
    ///         if duplicate edge is not allowed

   public:
    /// access auxiliary data, will not be implement in this beggar version
    //    const VertexAuxiliaryType& const_auxiliary() const;
    //    VertexAuxiliaryType& auxiliary();

    /// access element, will not be implement in this beggar version
    //    EdgeLabelConstPtr FindConstOutEdgeLabel(const EdgeLabelType&
    //    edge_label) const; EdgeLabelConstPtr
    //    FindConstInEdgeLabel(const EdgeLabelType& edge_label) const;

   private:
    VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                         const VertexPtr& dst_ptr) {
      for (auto& edge_label_it : edge_label_container) {
        /// <iterator, bool>
        auto ret =
            std::get<kVertexPtrContainerIdx>(edge_label_it).Find(dst_ptr);
        if (ret.second) {
          /// found it
          return std::get<kVertexPtrIdx>(*ret.first);
        }
      }
      return VertexPtr();
    }

    VertexConstPtr FindConstVertex(
        const EdgeLabelContainerType& edge_label_container,
        const VertexPtr& dst_ptr) const {
      for (auto edge_label_it = edge_label_container.cbegin();
           edge_label_it != edge_label_container.cend(); ++edge_label_it) {
        /// <iterator, bool>
        auto ret =
            std::get<kVertexPtrContainerIdx>(*edge_label_it).FindConst(dst_ptr);
        if (ret.second) {
          /// found it
          return std::get<kVertexPtrIdx>(*ret.first);
        }
      }
      return VertexConstPtr();
    }

    VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                         const VertexIDType& dst_id) {
      for (auto& edge_label_it : edge_label_container) {
        for (auto& vertex_it :
             std::get<kVertexPtrContainerIdx>(edge_label_it)) {
          VertexPtr vertex_ptr = std::get<kVertexPtrIdx>(vertex_it);
          if (vertex_ptr->id() == dst_id) return vertex_ptr;  /// found it
        }
      }
      /// not found
      return VertexPtr();
    }

    VertexConstPtr FindConstVertex(
        const EdgeLabelContainerType& edge_label_container,
        const VertexIDType& dst_id) const {
      for (auto edge_label_it = edge_label_container.cbegin();
           edge_label_it != edge_label_container.cend(); ++edge_label_it) {
        for (auto vertex_it =
                 std::get<kVertexPtrContainerIdx>(*edge_label_it).cbegin();
             vertex_it !=
             std::get<kVertexPtrContainerIdx>(*edge_label_it).cend();
             ++vertex_it) {
          VertexConstPtr vertex_ptr = std::get<kVertexPtrIdx>(*vertex_it);
          if (vertex_ptr->id() == dst_id) return vertex_ptr;  /// found it
        }
      }
      /// not found
      return VertexConstPtr();
    }

    inline VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                                const EdgeLabelType& edge_label,
                                const VertexPtr& dst_ptr) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second)  /// does not have edge_label
        return VertexPtr();
      /// <iterator of VertexPtrContainer, bool>
      auto vertex_ptr_ret =
          std::get<kVertexPtrContainerIdx>(*edge_label_ret.first).Find(dst_ptr);
      if (!vertex_ptr_ret.second) return VertexPtr();
      /// found it
      return std::get<kVertexPtrIdx>(*vertex_ptr_ret.first);
    }

    inline VertexConstPtr FindConstVertex(
        const EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label, const VertexPtr& dst_ptr) const {
      /// <iterator of EdgeLabelContainer, bool>
      const auto edge_label_ret = edge_label_container.FindConst(edge_label);
      if (!edge_label_ret.second)  /// does not have edge_label
        return VertexPtr();
      /// <iterator of VertexContainer, bool>
      const auto vertex_ptr_ret =
          std::get<kVertexPtrContainerIdx>(*edge_label_ret.first)
              .FindConst(dst_ptr);
      if (!vertex_ptr_ret.second) return VertexPtr();
      /// found it
      return std::get<kVertexPtrIdx>(*vertex_ptr_ret.first);
    }

    VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                         const EdgeLabelType& edge_label,
                         const VertexIDType& dst_id) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second)  /// does not have edge_label
        return VertexPtr();
      for (auto& vertex_it :
           std::get<kVertexPtrContainerIdx>(*edge_label_ret.first)) {
        VertexPtr vertex_ptr = std::get<kVertexPtrIdx>(vertex_it);
        if (vertex_ptr->id() == dst_id) return vertex_ptr;  /// found it
      }
      /// not found
      return VertexPtr();
    }

    VertexConstPtr FindConstVertex(
        const EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label, const VertexIDType& dst_id) const {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.FindConst(edge_label);
      if (!edge_label_ret.second)  /// does not have edge_label
        return VertexPtr();
      for (auto vertex_it =
               std::get<kVertexPtrContainerIdx>(*edge_label_ret.first).cbegin();
           vertex_it !=
           std::get<kVertexPtrContainerIdx>(*edge_label_ret.first).cend();
           ++vertex_it) {
        VertexConstPtr vertex_ptr = std::get<kVertexPtrIdx>(*vertex_it);
        if (vertex_ptr->id() == dst_id) return vertex_ptr;  /// found it
      }
      /// not found
      return VertexConstPtr();
    }

   public:
    /// out vertex
    inline VertexPtr FindOutVertex(const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.out_edges(), dst_ptr);
    }
    inline VertexPtr FindOutVertex(const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.out_edges(), dst_id);
    }
    inline VertexPtr FindOutVertex(const EdgeLabelType& edge_label,
                                   const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.out_edges(), edge_label, dst_ptr);
    }
    inline VertexPtr FindOutVertex(const EdgeLabelType& edge_label,
                                   const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.out_edges(), edge_label, dst_id);
    }
    /// const out vertex
    inline VertexConstPtr FindConstOutVertex(const VertexPtr& dst_ptr) const {
      return this->FindConstVertex(this->edges_.const_out_edges(), dst_ptr);
    }
    inline VertexConstPtr FindConstOutVertex(const VertexIDType& dst_id) const {
      return this->FindConstVertex(this->edges_.const_out_edges(), dst_id);
    }
    inline VertexConstPtr FindConstOutVertex(const EdgeLabelType& edge_label,
                                             const VertexPtr& dst_ptr) const {
      return this->FindConstVertex(this->edges_.const_out_edges(), edge_label,
                                   dst_ptr);
    }
    inline VertexConstPtr FindConstOutVertex(const EdgeLabelType& edge_label,
                                             const VertexIDType& dst_id) const {
      return this->FindConstVertex(this->edges_.const_out_edges(), edge_label,
                                   dst_id);
    }
    /// in vertex
    inline VertexPtr FindInVertex(const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.in_edges(), dst_ptr);
    }
    inline VertexPtr FindInVertex(const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.in_edges(), dst_id);
    }
    inline VertexPtr FindInVertex(const EdgeLabelType& edge_label,
                                  const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.in_edges(), edge_label, dst_ptr);
    }
    inline VertexPtr FindInVertex(const EdgeLabelType& edge_label,
                                  const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.in_edges(), edge_label, dst_id);
    }
    /// const in vertex:
    ///    unimplemented:
    ///    inline VertexConstPtr FindConstInVertex(const VertexPtr& dst_ptr)
    ///    const; inline VertexConstPtr FindConstInVertex(const VertexIDType&
    ///    dst_id ) const; inline VertexConstPtr FindConstInVertex(const
    ///    EdgeLabelType& edge_label,
    ///                                            const VertexPtr& dst_ptr)
    ///                                            const;
    ///    inline VertexConstPtr FindConstInVertex(const EdgeLabelType&
    ///    edge_label,
    ///                                            const VertexIDType& dst_id)
    ///                                            const;
   private:
    inline typename VertexPtrContainerType::size_type CountVertex(
        const EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label) const {
      /// <constant iterator of EdgeLabelContainer, bool>
      auto ret = edge_label_container.FindConst(edge_label);
      if (!ret.second) return 0;
      return std::get<kVertexPtrContainerIdx>(*(ret.first)).size();
    }

   public:
    inline typename EdgeLabelContainerType::size_type CountOutEdgeLabel()
        const {
      return this->edges_.const_out_edges().size();
    }
    inline typename EdgeLabelContainerType::size_type CountInEdgeLabel() const {
      return this->edges_.const_in_edges().size();
    }

    ///    unimplemented:
    ///    inline typename VertexPtrContainerType::size_type
    ///           CountOutVertex() const;
    ///    inline typename VertexPtrContainerType::size_type
    ///           CountInVertex() const;

    inline typename VertexPtrContainerType::size_type CountOutVertex() const {
      if (allow_duplicate_edge) {
        std::set<VertexConstPtr> erase_multi_result;
        for (auto it = this->OutEdgeCBegin(); !it.IsDone(); it++) {
          erase_multi_result.insert(it->const_dst_ptr());
        }
        return static_cast<typename VertexPtrContainerType::size_type>(
            erase_multi_result.size());
      }
      typename VertexPtrContainerType::size_type out_vertex_num = 0;
      for (auto it = this->edges_.const_out_edges().cbegin();
           it != this->edges_.const_out_edges().cend(); it++) {
        const EdgeLabelType edge_label =
            std::get<InnerVertex_::kEdgeLabelIdx>(*(it));
        out_vertex_num += this->CountOutVertex(edge_label);
      }
      return out_vertex_num;
    }
    inline typename VertexPtrContainerType::size_type CountOutEdge() const {
      typename VertexPtrContainerType::size_type out_edge_num = 0;
      for (auto it = this->OutEdgeCBegin(); !it.IsDone(); it++) {
        out_edge_num++;
      }
      return out_edge_num;
    }
    inline typename VertexPtrContainerType::size_type CountInEdge() const {
      typename VertexPtrContainerType::size_type in_edge_num = 0;
      for (auto it = this->InEdgeCBegin(); !it.IsDone(); it++) {
        in_edge_num++;
      }
      return in_edge_num;
    }
    inline typename VertexPtrContainerType::size_type CountInVertex() const {
      if (allow_duplicate_edge) {
        std::set<VertexConstPtr> erase_multi_result;
        for (auto it = this->InEdgeCBegin(); !it.IsDone(); it++) {
          erase_multi_result.insert(it->const_src_ptr());
        }
        return static_cast<typename VertexPtrContainerType::size_type>(
            erase_multi_result.size());
      }
      typename VertexPtrContainerType::size_type in_vertex_num = 0;
      for (auto it = this->edges_.const_in_edges().cbegin();
           it != this->edges_.const_in_edges().cend(); it++) {
        const EdgeLabelType edge_label =
            std::get<InnerVertex_::kEdgeLabelIdx>(*(it));
        in_vertex_num += this->CountInVertex(edge_label);
      }
      return in_vertex_num;
    }

    inline typename VertexPtrContainerType::size_type CountOutVertex(
        const EdgeLabelType& edge_label) const {
      return this->CountVertex(this->edges_.const_out_edges(), edge_label);
    }
    inline typename VertexPtrContainerType::size_type CountInVertex(
        const EdgeLabelType& edge_label) const {
      return this->CountVertex(this->edges_.const_in_edges(), edge_label);
    }
    ///    unimplemented:
    ///    inline typename VertexPtrContainerType::size_type
    ///           CountOutVertex(const   EdgeLabelType&   edge_label,
    ///                          const VertexLabelType& vertex_label) const;
    ///    inline typename VertexPtrContainerType::size_type
    ///           CountInVertex(const   EdgeLabelType&   edge_label,
    ///                         const VertexLabelType& vertex_label) const;

    ///    unimplemented:
    ///    inline typename DecomposedEdgeContainerType::size_type
    ///           CountOutEdge() const;
    ///    inline typename DecomposedEdgeContainerType::size_type
    ///           CountInEdge() const;
    ///    inline typename DecomposedEdgeContainerType::size_type
    ///           CountOutEdge(const EdgeLabelType& edge_label) const;
    ///    inline typename DecomposedEdgeContainerType::size_type
    ///           CountInEdge(const EdgeLabelType& edge_label) const;
    ///    inline typename DecomposedEdgeContainerType::size_type
    ///           CountOutEdge(const   EdgeLabelType&   edge_label,
    ///                        const VertexLabelType& vertex_label) const;
    ///    inline typename DecomposedEdgeContainerType::size_type
    ///           CountInEdge(const   EdgeLabelType&   edge_label,
    ///                       const VertexLabelType& vertex_label) const;

   private:
    /// constant methods:
    inline EdgeConstPtr FindConstEdge(
        const EdgeLabelContainerType& edge_label_container,
        const enum EdgeDirection& edge_direction,
        const EdgeIDType& edge_id) const {
      for (auto edge_label_it = edge_label_container.cbegin();
           edge_label_it != edge_label_container.cend(); ++edge_label_it) {
        for (auto vertex_ptr_it =
                 std::get<kVertexPtrContainerIdx>(*edge_label_it).cbegin();
             vertex_ptr_it !=
             std::get<kVertexPtrContainerIdx>(*edge_label_it).cend();
             ++vertex_ptr_it) {
          auto decomposed_edge_ret =
              std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it)
                  .FindConst(edge_id);
          if (!decomposed_edge_ret.second) {
            /// not found
            continue;
          }
          const InnerVertex_* const temp_this_ptr = this;
          const VertexConstPtr temp_vertex_ptr = VertexConstPtr(temp_this_ptr);
          return EdgeConstPtr(edge_direction, temp_vertex_ptr, edge_label_it,
                              vertex_ptr_it, decomposed_edge_ret.first);
        }
      }
      return EdgeConstPtr();
    }

    /// non-constant methods:
    inline EdgePtr FindEdge(EdgeLabelContainerType& edge_label_container,
                            const enum EdgeDirection& edge_direction,
                            const EdgeIDType& edge_id) {
      for (auto edge_label_it = edge_label_container.begin();
           edge_label_it != edge_label_container.end(); ++edge_label_it) {
        for (auto vertex_ptr_it =
                 std::get<kVertexPtrContainerIdx>(*edge_label_it).begin();
             vertex_ptr_it !=
             std::get<kVertexPtrContainerIdx>(*edge_label_it).end();
             ++vertex_ptr_it) {
          auto decomposed_edge_ret =
              std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it)
                  .Find(edge_id);
          if (!decomposed_edge_ret.second) {
            /// not found
            continue;
          }
          InnerVertex_* const temp_this_ptr = this;
          VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
          return EdgePtr(edge_direction, temp_vertex_ptr, edge_label_it,
                         vertex_ptr_it, decomposed_edge_ret.first);
        }
      }
      return EdgePtr();
    }

    inline EdgePtr FindEdge(EdgeLabelContainerType& edge_label_container,
                            const enum EdgeDirection& edge_direction,
                            const EdgeLabelType& edge_label,
                            const EdgeIDType& edge_id) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second) {
        /// not found this edge label
        return EdgePtr();
      }
      /// found this edge label
      for (auto vertex_ptr_it =
               std::get<kVertexPtrContainerIdx>(*(edge_label_ret.first))
                   .begin();
           vertex_ptr_it !=
           std::get<kVertexPtrContainerIdx>(*(edge_label_ret.first)).end();
           ++vertex_ptr_it) {
        auto decomposed_edge_ret =
            std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it).Find(edge_id);
        if (!decomposed_edge_ret.second) {
          /// not found
          continue;
        }
        InnerVertex_* const temp_this_ptr = this;
        VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
        return EdgePtr(edge_direction, temp_vertex_ptr, edge_label_ret.first,
                       vertex_ptr_it, decomposed_edge_ret.first);
      }
      return EdgePtr();
    }

    inline EdgePtr FindEdge(EdgeLabelContainerType& edge_label_container,
                            const enum EdgeDirection& edge_direction,
                            const EdgeLabelType& edge_label,
                            const VertexPtr& dst_ptr,
                            const EdgeIDType& edge_id) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second)  /// does not have this edge label
        return EdgePtr();
      /// <iterator of VertexContainer, bool>
      auto vertex_ptr_ret =
          std::get<kVertexPtrContainerIdx>(*(edge_label_ret.first))
              .Find(dst_ptr);
      if (!vertex_ptr_ret.second)  /// does not have this VertexPtr
        return EdgePtr();
      /// <iterator of DecomposedEdgeContainer, bool>
      auto decomposed_edge_ret =
          std::get<kDecomposedEdgeContainerIdx>(*(vertex_ptr_ret.first))
              .Find(edge_id);
      if (!decomposed_edge_ret.second)  /// does not find it
        return EdgePtr();
      InnerVertex_* const temp_this_ptr = this;
      VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
      return EdgePtr(edge_direction, temp_vertex_ptr, edge_label_ret.first,
                     vertex_ptr_ret.first, decomposed_edge_ret.first);
    }

   public:
    inline EdgePtr FindOutEdge(const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.out_edges(), EdgeDirection::OutputEdge,
                            edge_id);
    }
    inline EdgeConstPtr FindConstOutEdge(const EdgeIDType& edge_id) const {
      return this->FindConstEdge(this->edges_.const_out_edges(),
                                 EdgeDirection::OutputEdge, edge_id);
    }
    inline EdgePtr FindOutEdge(const EdgeLabelType& edge_label,
                               const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.out_edges(), EdgeDirection::OutputEdge,
                            edge_label, edge_id);
    }
    inline EdgePtr FindOutEdge(const EdgeLabelType& edge_label,
                               const VertexPtr& dst_ptr,
                               const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.out_edges(), EdgeDirection::OutputEdge,
                            edge_label, dst_ptr, edge_id);
    }
    ///    unimplemented:
    ///    inline EdgeConstPtr FindConstOutEdge(const EdgeIDType& edge_id);
    ///    inline EdgeConstPtr FindConstOutEdge(const EdgeLabelType&
    ///    edge_label,
    ///                                         const EdgeIDType& edge_id);
    ///    inline EdgeConstPtr FindConstOutEdge(const EdgeLabelType&
    ///    edge_label,
    ///                                         const VertexPtr& dst_ptr,
    ///                                         const EdgeIDType& edge_id);
    /// possible extension:
    ///     FindOutEdge(edge_label,dst_ptr)
    ///         when there are not duplicate edge
    ///     FindOutEdge(dst_ptr)
    ///         when there are not duplicate edge
    ///         and only has one edge label type

    inline EdgePtr FindInEdge(const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.in_edges(), EdgeDirection::InputEdge,
                            edge_id);
    }
    inline EdgePtr FindInEdge(const EdgeLabelType& edge_label,
                              const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.in_edges(), EdgeDirection::InputEdge,
                            edge_label, edge_id);
    }
    inline EdgePtr FindInEdge(const EdgeLabelType& edge_label,
                              const VertexPtr& src_ptr,
                              const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.in_edges(), EdgeDirection::InputEdge,
                            edge_label, src_ptr, edge_id);
    }
    ///    unimplemented:
    ///    inline EdgeConstPtr FindConstInEdge(const EdgeIDType& edge_id);
    ///    inline EdgeConstPtr FindConstInEdge(const EdgeLabelType&
    ///    edge_label,
    ///                                        const EdgeIDType& edge_id);
    ///    inline EdgeConstPtr FindConstInEdge(const EdgeLabelType&
    ///    edge_label,
    ///                                        const VertexPtr& dst_ptr,
    ///                                        const EdgeIDType& edge_id);
    /// possible extension:
    ///     FindInEdge(edge_label,dst_ptr)
    ///         when there are not duplicate edge
    ///     FindInEdge(dst_ptr)
    ///         when there are not duplicate edge
    ///         and only has one edge label type

    using EdgeLabelIterator =
        Iterator_<EdgeLabelContentIterator_<EdgeLabelContainerType, true, 1,
                                            kEdgeLabelIdx>>;
    using VertexIterator =
        Iterator_<VertexContentIterator_<EdgeLabelContainerType, false, 2,
                                         kVertexPtrIdx>>;
    using VertexConstIterator = Iterator_<
        VertexContentIterator_<EdgeLabelContainerType, true, 2, kVertexPtrIdx>>;
    using VertexIteratorSpecifiedEdgeLabel =
        Iterator_<VertexContentIterator_<VertexPtrContainerType, false, 1,
                                         kVertexPtrIdx>>;
    using VertexConstIteratorSpecifiedEdgeLabel = Iterator_<
        VertexContentIterator_<VertexPtrContainerType, true, 1, kVertexPtrIdx>>;
    using EdgeContentIterator =
        EdgeContentIterator_<EdgeLabelContainerType, false, 3, 0, kEdgeLabelIdx,
                             kVertexPtrIdx, kEdgeIDIdx, kEdgeAttributePtrIdx>;
    using EdgeIterator = Iterator_<
        EdgeContentIterator_<EdgeLabelContainerType, false, 3, 0, kEdgeLabelIdx,
                             kVertexPtrIdx, kEdgeIDIdx, kEdgeAttributePtrIdx>>;
    using EdgeConstIterator = Iterator_<
        EdgeContentIterator_<EdgeLabelContainerType, true, 3, 0, kEdgeLabelIdx,
                             kVertexPtrIdx, kEdgeIDIdx, kEdgeAttributePtrIdx>>;
    using EdgeIteratorSpecifiedEdgeLabel =
        Iterator_<EdgeContentIteratorSpecifiedEdgeLabel_<
            VertexPtrContainerType, false, 2, 0, kEdgeLabelIdx, kVertexPtrIdx,
            kEdgeIDIdx, kEdgeAttributePtrIdx>>;
    using EdgeConstIteratorSpecifiedEdgeLabel =
        Iterator_<EdgeContentIteratorSpecifiedEdgeLabel_<
            VertexPtrContainerType, true, 2, 0, kEdgeLabelIdx, kVertexPtrIdx,
            kEdgeIDIdx, kEdgeAttributePtrIdx>>;
    using EdgeContentIteratorSpecifiedEdgeLabel =
        EdgeContentIteratorSpecifiedEdgeLabel_<
            VertexPtrContainerType, false, 2, 0, kEdgeLabelIdx, kVertexPtrIdx,
            kEdgeIDIdx, kEdgeAttributePtrIdx>;

   public:
    inline EdgeIteratorSpecifiedEdgeLabel EraseEdge(
        EdgeIteratorSpecifiedEdgeLabel& edge_iterator) {
      void* ptr = &edge_iterator;
      EdgeContentIteratorSpecifiedEdgeLabel* edge_it_ptr =
          static_cast<EdgeContentIteratorSpecifiedEdgeLabel*>(ptr);
      enum EdgeDirection edge_direction = edge_it_ptr->direction();
      auto edge_label_iterator = edge_it_ptr->get_iterator();
      VertexPtrContainerType& vertex_ptr_container =
          std::get<kVertexPtrContainerIdx>(*edge_label_iterator);
      auto vertex_ptr_iterator = edge_it_ptr->vertex_ptr_iterator();
      DecomposedEdgeContainerType& decomposed_edge_container =
          std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_iterator);
      auto decomposed_edge_iterator = edge_it_ptr->decomposed_edge_iterator();
      // erase dst_ptr iterator
      EdgeLabelType edge_label = edge_iterator->label();
      EdgeIDType edge_id = edge_iterator->id();
      InnerVertex_* temp_this_ptr = this;
      VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
      VertexPtr dst_ptr;
      if (edge_direction == EdgeDirection::OutputEdge) {
        dst_ptr = edge_it_ptr->dst_ptr();
      } else {
        dst_ptr = edge_it_ptr->src_ptr();
      }
      enum EdgeDirection reverse_edge_direction;
      if (edge_direction == EdgeDirection::OutputEdge) {
        reverse_edge_direction = EdgeDirection::InputEdge;
      } else {
        reverse_edge_direction = EdgeDirection::OutputEdge;
      }
      // debug
      /*
      std::cout<<"dst ptr id = "<<dst_ptr->id()<<std::endl;
      std::cout<<"label = "<<edge_label<<std::endl;
      std::cout<<"src ptr id = "<<temp_vertex_ptr->id()<<std::endl;
      */
      dst_ptr->EraseEdge(reverse_edge_direction, edge_label, temp_vertex_ptr,
                         edge_id);
      EdgeIteratorSpecifiedEdgeLabel ret_iterator = edge_iterator;
      void* ret_ptr = &ret_iterator;
      EdgeContentIteratorSpecifiedEdgeLabel* ret_it_ptr =
          static_cast<EdgeContentIteratorSpecifiedEdgeLabel*>(ret_ptr);
      auto& ret_vertex_ptr_iterator = ret_it_ptr->vertex_ptr_iterator();
      auto& ret_decomposed_edge_iterator =
          ret_it_ptr->decomposed_edge_iterator();
      // erase decomposed edge
      decomposed_edge_iterator =
          decomposed_edge_container.Erase(decomposed_edge_iterator);
      if (!decomposed_edge_container.empty()) {
        // decomposed_edge_container is not empty and iter is not end
        // return new iterator
        if (decomposed_edge_iterator != decomposed_edge_container.end()) {
          ret_decomposed_edge_iterator = decomposed_edge_iterator;
          return ret_iterator;
        }
        // else add vertex_ptr to next iter
        vertex_ptr_iterator++;
      } else {
        // decomposed_edge_container is empty
        // erase vertex_ptr_iterator
        vertex_ptr_iterator = vertex_ptr_container.Erase(vertex_ptr_iterator);
      }
      if ((!vertex_ptr_container.empty()) &&
          (vertex_ptr_iterator != vertex_ptr_container.end())) {
        auto& decomposed_edge_container_ref =
            std::get<kDecomposedEdgeContainerIdx>(*(vertex_ptr_iterator));
        ret_vertex_ptr_iterator = vertex_ptr_iterator;
        ret_decomposed_edge_iterator = decomposed_edge_container_ref.begin();
        return ret_iterator;
      } else {
        return EdgeIteratorSpecifiedEdgeLabel();
      }
    }

    inline EdgeIterator EraseEdge(EdgeIterator& edge_iterator) {
      // get direction

      void* ptr = &edge_iterator;
      EdgeContentIterator* edge_content_iterator_ptr =
          static_cast<EdgeContentIterator*>(ptr);

      enum EdgeDirection edge_direction =
          edge_content_iterator_ptr->direction();
      // get container and iterator
      EdgeLabelContainerType* edge_label_container = nullptr;
      if (edge_direction == EdgeDirection::OutputEdge)
        edge_label_container = &this->edges_.out_edges();
      else
        edge_label_container = &this->edges_.in_edges();
      VertexPtrContainerType& vertex_ptr_container =
          edge_content_iterator_ptr->template get<VertexPtrContainerType,
                                                  kVertexPtrContainerIdx, 0>();
      DecomposedEdgeContainerType& decomposed_edge_container =
          edge_content_iterator_ptr->template get<
              DecomposedEdgeContainerType, kDecomposedEdgeContainerIdx, 1>();
      auto edge_label_iterator =
          edge_content_iterator_ptr
              ->template get_iterator<EdgeLabelIteratorType, 0>();
      auto vertex_ptr_iterator =
          edge_content_iterator_ptr
              ->template get_iterator<VertexPtrIteratorType, 1>();
      auto decomposed_edge_iterator =
          edge_content_iterator_ptr
              ->template get_iterator<DecomposedEdgeIteratorType, 2>();

      // erase dst_ptr iterator
      EdgeLabelType edge_label = edge_iterator->label();
      EdgeIDType edge_id = edge_iterator->id();
      InnerVertex_* temp_this_ptr = this;
      VertexPtr temp_vertex_ptr = VertexPtr(temp_this_ptr);
      VertexPtr dst_ptr;
      if (edge_direction == EdgeDirection::OutputEdge) {
        dst_ptr = edge_content_iterator_ptr->dst_ptr();
      } else {
        dst_ptr = edge_content_iterator_ptr->src_ptr();
      }
      enum EdgeDirection reverse_edge_direction;
      if (edge_direction == EdgeDirection::OutputEdge) {
        reverse_edge_direction = EdgeDirection::InputEdge;
      } else {
        reverse_edge_direction = EdgeDirection::OutputEdge;
      }
      dst_ptr->EraseEdge(reverse_edge_direction, edge_label, temp_vertex_ptr,
                         edge_id);
      // build return iterator
      EdgeIterator ret_iterator = edge_iterator;
      void* ret_ptr = &ret_iterator;
      EdgeContentIterator* ret_it = static_cast<EdgeContentIterator*>(ret_ptr);
      auto& ret_edge_label_iterator =
          ret_it->template get_iterator<EdgeLabelIteratorType, 0>();
      auto& ret_vertex_ptr_iterator =
          ret_it->template get_iterator<VertexPtrIteratorType, 1>();
      auto& ret_decomposed_edge_iterator =
          ret_it->template get_iterator<DecomposedEdgeIteratorType, 2>();
      // erase decomposed edge
      decomposed_edge_iterator =
          decomposed_edge_container.Erase(decomposed_edge_iterator);

      if (!decomposed_edge_container.empty()) {
        // decomposed_edge_container is not empty and iter is not end
        // return new iterator
        if (decomposed_edge_iterator != decomposed_edge_container.end()) {
          ret_decomposed_edge_iterator = decomposed_edge_iterator;
          return ret_iterator;
        }
        // else add vertex_ptr to next iter
        vertex_ptr_iterator++;
      } else {
        // decomposed_edge_container is empty
        // erase vertex_ptr_iterator
        vertex_ptr_iterator = vertex_ptr_container.Erase(vertex_ptr_iterator);
      }
      if (!vertex_ptr_container.empty()) {
        if (vertex_ptr_iterator != vertex_ptr_container.end()) {
          auto& decomposed_edge_container_ref =
              std::get<kDecomposedEdgeContainerIdx>(*(vertex_ptr_iterator));
          ret_vertex_ptr_iterator = vertex_ptr_iterator;
          ret_decomposed_edge_iterator = decomposed_edge_container_ref.begin();
          return ret_iterator;
        }
        edge_label_iterator++;
      } else {
        edge_label_iterator = edge_label_container->Erase(edge_label_iterator);
      }
      if (!edge_label_container->empty() &&
          edge_label_iterator != edge_label_container->end()) {
        vertex_ptr_iterator =
            std::get<kVertexPtrContainerIdx>(*(edge_label_iterator)).begin();
        decomposed_edge_iterator =
            std::get<kDecomposedEdgeContainerIdx>(*(vertex_ptr_iterator))
                .begin();
        ret_edge_label_iterator = edge_label_iterator;
        ret_vertex_ptr_iterator = vertex_ptr_iterator;
        ret_it->template get_iterator<DecomposedEdgeIteratorType, 2>() =
            decomposed_edge_iterator;
        return ret_iterator;
      }
      /// is done
      return EdgeIterator();
    }

   private:
    inline EdgeLabelIterator EdgeLabelBegin(
        const EdgeLabelContainerType& edge_label_container) const {
      return EdgeLabelIterator(edge_label_container.cbegin(),
                               edge_label_container.cend());
    }

    inline VertexIterator VertexBegin(
        EdgeLabelContainerType& edge_label_container) {
      return VertexIterator(edge_label_container.begin(),
                            edge_label_container.end());
    }
    inline VertexConstIterator VertexCBegin(
        const EdgeLabelContainerType& edge_label_container) const {
      return VertexConstIterator(edge_label_container.cbegin(),
                                 edge_label_container.cend());
    }
    inline VertexIteratorSpecifiedEdgeLabel VertexBegin(
        EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label) {
      auto ret = edge_label_container.Find(edge_label);
      if (!ret.second) {
        /// not find
        return VertexIteratorSpecifiedEdgeLabel();
      }
      return VertexIteratorSpecifiedEdgeLabel(
          (std::get<kVertexPtrContainerIdx>(*(ret.first))).begin(),
          (std::get<kVertexPtrContainerIdx>(*(ret.first))).end());
    }
    inline VertexConstIteratorSpecifiedEdgeLabel VertexCBegin(
        const EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label) const {
      auto ret = edge_label_container.FindConst(edge_label);
      if (!ret.second) {
        /// not find
        return VertexConstIteratorSpecifiedEdgeLabel();
      }
      return VertexConstIteratorSpecifiedEdgeLabel(
          (std::get<kVertexPtrContainerIdx>(*(ret.first))).cbegin(),
          (std::get<kVertexPtrContainerIdx>(*(ret.first))).cend());
    }
    inline EdgeIterator EdgeBegin(
        enum EdgeDirection direction,
        EdgeLabelContainerType& edge_label_container) {
      InnerVertex_* const temp_this_ptr = this;
      VertexPtr temp_vertex_ptr(temp_this_ptr);
      return EdgeIterator(direction, temp_vertex_ptr,
                          edge_label_container.begin(),
                          edge_label_container.end());
    }
    inline EdgeConstIterator EdgeCBegin(
        enum EdgeDirection direction,
        const EdgeLabelContainerType& edge_label_container) const {
      const InnerVertex_* const temp_this_ptr = this;
      const VertexConstPtr temp_vertex_ptr(temp_this_ptr);
      return EdgeConstIterator(direction, temp_vertex_ptr,
                               edge_label_container.cbegin(),
                               edge_label_container.cend());
    }
    inline EdgeIteratorSpecifiedEdgeLabel EdgeBegin(
        enum EdgeDirection direction,
        EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label) {
      /// <iterator of EdgeLabelContainerType, bool>
      auto ret = edge_label_container.Find(edge_label);
      if (!ret.second) {
        /// not found
        return EdgeIteratorSpecifiedEdgeLabel();
      }
      InnerVertex_* const temp_this_ptr = this;
      const VertexPtr temp_vertex_ptr(temp_this_ptr);
      return EdgeIteratorSpecifiedEdgeLabel(
          ret.first, direction, temp_vertex_ptr,
          std::get<kVertexPtrContainerIdx>(*(ret.first)).begin(),
          std::get<kVertexPtrContainerIdx>(*(ret.first)).end());
    }
    inline EdgeConstIteratorSpecifiedEdgeLabel EdgeCBegin(
        enum EdgeDirection direction,
        const EdgeLabelContainerType& edge_label_container,
        const EdgeLabelType& edge_label) const {
      /// <iterator of EdgeLabelContainerType, bool>
      auto ret = edge_label_container.FindConst(edge_label);
      if (!ret.second) {
        /// not found
        return EdgeConstIteratorSpecifiedEdgeLabel();
      }
      const InnerVertex_* const temp_this_ptr = this;
      VertexConstPtr temp_vertex_ptr(temp_this_ptr);
      return EdgeConstIteratorSpecifiedEdgeLabel(
          ret.first, direction, temp_vertex_ptr,
          std::get<kVertexPtrContainerIdx>(*(ret.first)).cbegin(),
          std::get<kVertexPtrContainerIdx>(*(ret.first)).cend());
    }

   public:
    /// EdgeLabel Iterators:
    inline EdgeLabelIterator OutEdgeLabelBegin() const {
      return this->EdgeLabelBegin(this->edges_.const_out_edges());
    }
    inline EdgeLabelIterator InEdgeLabelBegin() const {
      return this->EdgeLabelBegin(this->edges_.const_in_edges());
    }

    /// Vertex Iterators:
    /// output edge:
    inline VertexIterator OutVertexBegin() {
      return this->VertexBegin(this->edges_.out_edges());
    }
    inline VertexConstIterator OutVertexCBegin() const {
      return this->VertexCBegin(this->edges_.const_out_edges());
    }
    inline VertexIteratorSpecifiedEdgeLabel OutVertexBegin(
        const EdgeLabelType& edge_label) {
      return this->VertexBegin(this->edges_.out_edges(), edge_label);
    }
    inline VertexConstIteratorSpecifiedEdgeLabel OutVertexCBegin(
        const EdgeLabelType& edge_label) const {
      return this->VertexCBegin(this->edges_.const_out_edges(), edge_label);
    }
    /// possible extension:
    ///     OutVertexBegin (edge_label,vertex_label)
    ///     OutVertexCBegin(edge_label,vertex_label)

    /// input edge:
    inline VertexIterator InVertexBegin() {
      return this->VertexBegin(this->edges_.in_edges());
    }
    inline VertexConstIterator InVertexCBegin() const {
      return this->VertexCBegin(this->edges_.const_in_edges());
    }
    inline VertexIteratorSpecifiedEdgeLabel InVertexBegin(
        const EdgeLabelType& edge_label) {
      return this->VertexBegin(this->edges_.in_edges(), edge_label);
    }
    inline VertexConstIteratorSpecifiedEdgeLabel InVertexCBegin(
        const EdgeLabelType& edge_label) const {
      return this->VertexCBegin(this->edges_.const_in_edges(), edge_label);
    }
    /// possible extension:
    ///     InVertexBegin (edge_label,vertex_label)
    ///     InVertexCBegin(edge_label,vertex_label)

    /// Edge Iterators:
    /// output edge:
    inline EdgeIterator OutEdgeBegin() {
      return this->EdgeBegin(EdgeDirection::OutputEdge,
                             this->edges_.out_edges());
    }
    inline EdgeConstIterator OutEdgeCBegin() const {
      return this->EdgeCBegin(EdgeDirection::OutputEdge,
                              this->edges_.const_out_edges());
    }
    inline EdgeIteratorSpecifiedEdgeLabel OutEdgeBegin(
        const EdgeLabelType& edge_label) {
      return this->EdgeBegin(EdgeDirection::OutputEdge,
                             this->edges_.out_edges(), edge_label);
    }
    inline EdgeConstIteratorSpecifiedEdgeLabel OutEdgeCBegin(
        const EdgeLabelType& edge_label) const {
      return this->EdgeCBegin(EdgeDirection::OutputEdge,
                              this->edges_.const_out_edges(), edge_label);
    }
    /// possible extension:
    ///     OutEdgeBegin (edge_label,vertex_label)
    ///     OutEdgeCBegin(edge_label,vertex_label)

    /// input edge:
    inline EdgeIterator InEdgeBegin() {
      return this->EdgeBegin(EdgeDirection::InputEdge, this->edges_.in_edges());
    }
    inline EdgeConstIterator InEdgeCBegin() const {
      return this->EdgeCBegin(EdgeDirection::InputEdge,
                              this->edges_.const_in_edges());
    }
    inline EdgeIteratorSpecifiedEdgeLabel InEdgeBegin(
        const EdgeLabelType& edge_label) {
      return this->EdgeBegin(EdgeDirection::InputEdge, this->edges_.in_edges(),
                             edge_label);
    }
    inline EdgeConstIteratorSpecifiedEdgeLabel InEdgeCBegin(
        const EdgeLabelType& edge_label) const {
      return this->EdgeCBegin(EdgeDirection::InputEdge,
                              this->edges_.const_in_edges(), edge_label);
    }
    /// possible extension:
    ///     InEdgeBegin (edge_label,vertex_label)
    ///     InEdgeCBegin(edge_label,vertex_label)

    /// unimplemented:
    /// inline VertexIterator EraseVertex(const VertexIterator& iterator);
    /// inline VertexIteratorSpecifiedEdgeLabel
    ///                       EraseVertex(const
    ///                       VertexIteratorSpecifiedEdgeLabel& iterator);
    /// inline EdgeIterator EraseEdge(const EdgeIterator& iterator);
    /// inline EdgeIteratorSpecifiedEdgeLabel
    ///                     EraseEdge(const EdgeIteratorSpecifiedEdgeLabel&
    ///                     iterator);

    /// inline EdgePtr SetLabel(EdgePtr&       edge_ptr,
    ///                   const EdgeLabelType& edge_label);
    /// inline EdgePtr SetLabel(const EdgeIDType&    edge_id,
    ///                         const EdgeLabelType& edge_label);
    /// inline EdgePtr SetSrc(EdgePtr&     edge_ptr,
    ///                 const VertexPtr& vertex_ptr);
    /// inline EdgePtr SetSrc(const EdgeIDType& edge_id,
    ///                       const VertexPtr&  vertex_ptr);
    /// inline EdgePtr SetDst(EdgePtr&     edge_ptr,
    ///                 const VertexPtr& vertex_ptr);
    /// inline EdgePtr SetDst(const EdgeIDType& edge_id,
    ///                       const VertexPtr&  vertex_ptr);
  };

  using InnerVertexType = InnerVertex_;

 public:
  using EdgeType = Type_<EdgeIDType, EdgeLabelType, edge_has_static_attribute,
                         EdgeStaticAttributeType, edge_has_dynamic_attribute,
                         EdgeAttributeKeyType>;
  using VertexType =
      Type_<VertexIDType, VertexLabelType, vertex_has_static_attribute,
            VertexStaticAttributeType, vertex_has_dynamic_attribute,
            VertexAttributeKeyType>;
  /// non-constant pointer
  using EdgePtr = typename InnerVertexType::EdgePtr;
  using VertexPtr = typename InnerVertexType::VertexPtr;
  /// constant pointer
  using EdgeConstPtr = typename InnerVertexType::EdgeConstPtr;
  using VertexConstPtr = typename InnerVertexType::VertexConstPtr;

 private:
  static constexpr TupleIdxType kVertexIDIdx = 0;
  static constexpr TupleIdxType kVertexPtrIdx = 1;
  using VertexIDContainerType =
      Container<vertex_id_container_type, vertex_id_container_sort_type,
                VertexIDType, VertexPtr>;

  static constexpr TupleIdxType kVertexLabelIdx = 0;
  static constexpr TupleIdxType kVertexIDContainerIdx = 1;
  using VertexLabelContainerType =
      Container<vertex_label_container_type, vertex_label_container_sort_type,
                VertexLabelType, VertexIDContainerType>;

 public:
  using VertexSizeType = typename VertexIDContainerType::size_type;

 private:
  VertexLabelContainerType vertexes_;

  inline void Deconstruct() {
    for (auto& vertex_label_it : this->vertexes_) {
      for (auto& vertex_ptr_it :
           std::get<kVertexIDContainerIdx>(vertex_label_it)) {
        std::get<kVertexPtrIdx>(vertex_ptr_it).Release();
      }
      std::get<kVertexIDContainerIdx>(vertex_label_it).clear();
    }
    this->vertexes_.clear();
    return;
  }

  template <template <typename...> class GraphType_, typename... configures_>
  inline void CanConvertTo(const GraphType_<configures_...>& graph) const {
    static_assert(std::is_base_of<Graph<configures_...>,
                                  GraphType_<configures_...>>::value,
                  "Illegal input parameter, should be a graph");
    using ConvertToGraphType = GraphType_<configures_...>;
    static_assert(Graph::allow_duplicate_edge ||
                      !ConvertToGraphType::allow_duplicate_edge,
                  "Cannot convert from a graph allows duplicate edge\
                 to a graph does not allow duplicate edge");
    /// wenzhi ToDo:
    ///     complete the checking process of the parameters,
    ///     such as the above statement, if this graph type does
    ///     not allow duplicate edges but the InputGraphType allows
    ///     that, then this convert would be illegal
    return;
  }

  template <template <typename...> class GraphType_, typename... configures_>
  inline void Construct(const GraphType_<configures_...>& graph) {
    for (auto vit = graph.VertexCBegin(); !vit.IsDone(); vit++)
      this->AddVertex(vit->id(), vit->label());
    for (auto vit = graph.VertexCBegin(); !vit.IsDone(); vit++) {
      for (auto eit = vit->OutEdgeCBegin(); !eit.IsDone(); eit++) {
        this->AddEdge(eit->const_src_ptr()->id(), eit->const_dst_ptr()->id(),
                      eit->label(), eit->id());
      }
    }
    return;
  }

 public:
  Graph() = default;

  ~Graph() {
    this->Deconstruct();
    return;
  }

  Graph(const Graph& graph) {
    this->CanConvertTo(graph);
    this->Construct(graph);
    return;
  }

  Graph& operator=(const Graph& graph) {
    this->CanConvertTo(graph);
    this->Deconstruct();
    this->Construct(graph);
    return *this;
  }

  inline void Clear() {
    this->Deconstruct();
    return;
  }

  inline std::pair<VertexPtr, bool> AddVertex(
      const typename VertexType::IDType& id,
      const typename VertexType::LabelType& label) {
    const VertexPtr ret = this->FindVertex(id);
    if (!ret.IsNull()) {
      /// already exist
      return std::pair<VertexPtr, bool>(ret, false);
    }
    InnerVertex_* const temp_inner_vertex_ptr = new InnerVertex_(id, label);
    VertexPtr temp_vertex_ptr(temp_inner_vertex_ptr);
    /// vertex label iterator
    auto vertex_label_it = this->vertexes_.Insert(label).first;
    /// vertex ID iterator
    auto vertex_id_it =
        std::get<kVertexIDContainerIdx>(*vertex_label_it).Insert(id).first;
    temp_vertex_ptr->SetIDType(GUNDAM::TypeToString(id));
    temp_vertex_ptr->SetLabelType(GUNDAM::TypeToString(label));
    std::get<kVertexPtrIdx>(*vertex_id_it) = temp_vertex_ptr;
    return std::pair<VertexPtr, bool>(std::get<kVertexPtrIdx>(*vertex_id_it),
                                      true);
  }
  /// possible variant:
  ///     AddVertex(id)
  ///     AddVertex(id,label)
  ///     AddVertex(id,label,attribute)
  ///     AddVertex(id,label,attribute0,attribute1,...)
  ///     AddVertex(id,attribute)
  ///     AddVertex(id,attribute0,attribute1,...)
  ///     ...

  inline bool EraseEdge(const typename EdgeType::IDType& edge_id) {
    for (auto vertex_it = this->VertexBegin(); !vertex_it.IsDone();
         vertex_it++) {
      if (vertex_it->EraseEdge(edge_id)) return true;
    }
    return false;
  }
  /*
  bool find_flag = false;

      for (auto vertex_ptr_it = this->VertexBegin(); !vertex_ptr_it.IsDone();
           ++vertex_ptr_it) {
        EdgePtr edge_ptr = vertex_ptr_it->FindOutEdge(edge_id);
        VertexPtr src_ptr = vertex_ptr_it;
        VertexPtr dst_ptr = edge_ptr->dst_ptr();
        EdgeLabelType edge_label = edge_ptr->label();
        if (!edge_ptr.IsNull()) {
          find_flag = true;
          src_ptr->EraseEdge(dst_ptr, edge_label, edge_id);
          break;
        }
      }
      return find_flag;*/

  inline std::pair<EdgePtr, bool> AddEdge(
      const typename VertexType::IDType& src_id,
      const typename VertexType::IDType& dst_id,
      const typename EdgeType::LabelType& edge_label,
      const typename EdgeType::IDType& edge_id) {
    VertexPtr src_ptr = this->FindVertex(src_id);
    VertexPtr dst_ptr = this->FindVertex(dst_id);
    this->SetEdgeIDType(TypeToString(edge_id));
    this->SetEdgeLabelType(TypeToString(edge_label));
    if (src_ptr.IsNull() || dst_ptr.IsNull()) {
      /// the src vertex or the dst vertex does not exist
      return std::pair<EdgePtr, bool>(EdgePtr(), false);
    }
    for (auto vertex_ptr_it = this->VertexBegin(); !vertex_ptr_it.IsDone();
         ++vertex_ptr_it) {
      EdgePtr const edge_ptr = vertex_ptr_it->FindOutEdge(edge_id);
      if (!edge_ptr.IsNull()) {
        /// the edge with this edge_id has already existed
        return std::pair<EdgePtr, bool>(edge_ptr, false);
      }
    }
    /// assume that the duplicate edge is allowed
    return src_ptr->AddEdge(dst_ptr, edge_label, edge_id);
  }
  /// possible variant:
  ///     AddEdge(src_id,dst_id)
  ///     AddEdge(src_id,dst_id,edge_label)
  ///     AddEdge(src_id,dst_id,edge_label,edge_id)
  ///     AddEdge(src_id,dst_id,edge_label,edge_id,edge_attribute)
  ///     ...

  inline VertexPtr FindVertex(const typename VertexType::IDType& id) {
    for (auto& vertex_label_it : this->vertexes_) {
      /// <iterator of ID container, bool>
      auto ret = std::get<kVertexIDContainerIdx>(vertex_label_it).Find(id);
      if (ret.second) {
        /// found it
        return std::get<kVertexPtrIdx>(*(ret.first));
      }
    }
    /// not found
    return VertexPtr();
  }

  inline VertexConstPtr FindConstVertex(
      const typename VertexType::IDType& id) const {
    for (auto vertex_label_it = this->vertexes_.cbegin();
         vertex_label_it != this->vertexes_.cend(); ++vertex_label_it) {
      /// <iterator of ID container, bool>
      const auto ret =
          std::get<kVertexIDContainerIdx>(*vertex_label_it).FindConst(id);
      if (ret.second) {
        /// found it
        return std::get<kVertexPtrIdx>(*(ret.first));
      }
    }
    /// not found
    return VertexConstPtr();
  }

  inline VertexPtr FindVertex(const typename VertexType::IDType& id,
                              const typename VertexType::LabelType& label) {
    /// <iterator of VertexLabelContainer, bool>
    auto vertex_label_ret = this->vertexes_.Find(label);
    if (!vertex_label_ret.second) {
      /// not have this vertex label
      return VertexPtr();
    }
    auto vertex_id_ret =
        std::get<kVertexIDContainerIdx>(*(vertex_label_ret.first)).Find(id);
    if (!vertex_id_ret.second) {
      /// not have this vertex id
      return VertexPtr();
    }
    return std::get<kVertexPtrIdx>(*(vertex_id_ret.first));
  }

  inline VertexConstPtr FindConstVertex(
      const typename VertexType::IDType& id,
      const typename VertexType::LabelType& label) const {
    /// <iterator of VertexLabelContainer, bool>
    auto vertex_label_ret = this->vertexes_.FindConst(label);
    if (!vertex_label_ret.second) {
      /// not have this vertex label
      return VertexPtr();
    }
    auto vertex_id_ret =
        std::get<kVertexIDContainerIdx>(*(vertex_label_ret.first))
            .FindConst(id);
    if (!vertex_id_ret.second) {
      /// not have this vertex id
      return VertexConstPtr();
    }
    return std::get<kVertexPtrIdx>(*(vertex_id_ret.first));
  }

  inline EdgePtr FindEdge(const typename EdgeType::IDType& edge_id) {
    for (auto vertex_ptr_it = this->VertexBegin(); !vertex_ptr_it.IsDone();
         vertex_ptr_it++) {
      EdgePtr const edge_ptr = vertex_ptr_it->FindOutEdge(edge_id);
      if (!edge_ptr.IsNull()) {
        /// the edge with this edge_id has already existed
        return edge_ptr;
      }
    }
    return EdgePtr();
  }
  ///  unimplemented:
  ///  inline EdgeConstPtr FindConstEdge(
  ///           const typename EdgeType::IDType& edge_id) const;
  ///  inline EdgeConstPtr FindConstEdge(
  ///           const typename EdgeType::   IDType& edge_id,
  ///           const typename EdgeType::LabelType& edge_label) const;
  ///  inline EdgeConstPtr FindConstEdge(
  ///                const typename EdgeType::IDType& edge_id) const;
  ///  inline EdgeConstPtr FindConstEdge(
  ///                const typename EdgeType::   IDType& edge_id,
  ///                const typename EdgeType::LabelType& edge_label) const;

  VertexSizeType CountVertex() const {
    VertexSizeType vertex_num = 0;
    for (auto vertex_label_it = this->vertexes_.cbegin();
         vertex_label_it != this->vertexes_.cend(); vertex_label_it++) {
      vertex_num += std::get<kVertexIDContainerIdx>(*vertex_label_it).size();
    }
    return vertex_num;
  }

  VertexSizeType CountVertex(
      const typename VertexType::LabelType& label) const {
    /// <iterator of VertexLabelContainer, bool>
    auto ret = this->vertexes_.FindConst(label);
    if (!ret.second) {
      /// does not have this vertex label
      return 0;
    }
    return std::get<kVertexIDContainerIdx>(*(ret.first)).size();
  }

  using VertexIterator =
      Iterator_<VertexContentIterator_<VertexLabelContainerType, false, 2,
                                       kVertexPtrIdx>>;
  using VertexConstIterator = Iterator_<
      VertexContentIterator_<VertexLabelContainerType, true, 2, kVertexPtrIdx>>;
  using VertexIteratorSpecifiedLabel = Iterator_<
      VertexContentIterator_<VertexIDContainerType, false, 1, kVertexPtrIdx>>;
  using VertexConstIteratorSpecifiedLabel = Iterator_<
      VertexContentIterator_<VertexIDContainerType, true, 1, kVertexPtrIdx>>;

  inline VertexIterator VertexBegin() {
    return VertexIterator(this->vertexes_.begin(), this->vertexes_.end());
  }
  inline VertexConstIterator VertexCBegin() const {
    return VertexConstIterator(this->vertexes_.cbegin(),
                               this->vertexes_.cend());
  }

  inline VertexIteratorSpecifiedLabel VertexBegin(
      typename VertexType::LabelType label) {
    /// <iterator of VertexLabelContainer, bool>
    auto ret = this->vertexes_.Find(label);
    if (!ret.second)  /// does not have this vertex label
      return VertexIteratorSpecifiedLabel();
    return VertexIteratorSpecifiedLabel(
        std::get<kVertexIDContainerIdx>(*(ret.first)).begin(),
        std::get<kVertexIDContainerIdx>(*(ret.first)).end());
  }
  inline VertexConstIteratorSpecifiedLabel VertexCBegin(
      typename VertexType::LabelType label) const {
    /// <iterator of VertexLabelContainer, bool>
    auto ret = this->vertexes_.FindConst(label);
    if (!ret.second)  /// does not have this vertex label
      return VertexConstIteratorSpecifiedLabel();
    return VertexConstIteratorSpecifiedLabel(
        std::get<kVertexIDContainerIdx>(*(ret.first)).cbegin(),
        std::get<kVertexIDContainerIdx>(*(ret.first)).cend());
  }

  /// unimplemented:
  /// inline VertexIterator EraseVertex(const VertexIterator& iterator);
  /// inline VertexIteratorSpecifiedLabel
  ///                       EraseVertex(const VertexIteratorSpecifiedLabel&
  ///                       iterator);

  /// unimplemented:
  /// inline VertexPtr SetLabel(VertexPtr&       vertex_ptr,
  ///                     const VertexLabelType& vertex_label);
  /// inline VertexPtr SetLabel(const VertexIDType&    vertex_id,
  ///                           const VertexLabelType& vertex_label);
  /// inline   EdgePtr SetLabel(EdgePtr&       edge_ptr,
  ///                     const EdgeLabelType& edge_label);
  /// inline   EdgePtr SetLabel(const   EdgeIDType&      edge_id,
  ///                           const   EdgeLabelType&   edge_label);
  /// inline   EdgePtr SetSrc  (  EdgePtr&   edge_ptr,
  ///                           VertexPtr& vertex_ptr);
  /// inline   EdgePtr SetDst  (  EdgePtr&   edge_ptr,
  ///                           VertexPtr& vertex_ptr);
  /// inline   EdgePtr SetSrc (const EdgeIDType& edge_id,
  ///                          VertexPtr& vertex_ptr);
  /// inline   EdgePtr SetDst (const EdgeIDType& edge_id,
  ///            G             VertexPtr& vertex_ptr);
 private:
  std::string edge_label_type_;
  std::string edge_id_type_;

 public:
  void SetEdgeLabelType(const std::string& label_type) {
    this->edge_label_type_ = label_type;
  }
  void SetEdgeIDType(const std::string& id_type) {
    this->edge_id_type_ = id_type;
  }
  const std::string edge_id_type() const { return this->edge_id_type_; }
  const std::string edge_label_type() const { return this->edge_label_type_; }
};  // namespace GUNDAM
}  // namespace GUNDAM
   // es _GRAPH_G# // es _GRAPH_G#
#endif  // _GRAPH_H
