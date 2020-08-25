#ifndef _ITERATOR_H
#define _ITERATOR_H

#include "container.h"

namespace GUNDAM {

using IteratorDepthType = int8_t;

template <typename ContainerType_, bool is_const_, IteratorDepthType depth_>
class InnerIterator_;

template <typename ContainerType_, bool is_const_, IteratorDepthType depth_,
          IteratorDepthType now_depth_>
class _InnerIterator_;

/// the iterator of the container that only has one layer
template <enum ContainerType container_type_, enum SortType sort_type_,
          typename KeyType_, bool is_const_>
class InnerIterator_<Container<container_type_, sort_type_, KeyType_>,
                     is_const_, 1> {
 private:
  using ContainerType = Container<container_type_, sort_type_, KeyType_>;
  using ElementType = typename ContainerType::ElementType;

 protected:
  using IteratorType =
      typename std::conditional<is_const_,
                                typename ContainerType::const_iterator,
                                typename ContainerType::iterator>::type;

 private:
  IteratorType iterator_;
  IteratorType kIteratorEnd_;

 protected:
  inline const IteratorType& ConstInnerIterator() const {
    return this->iterator_;
  }

 public:
  InnerIterator_() : iterator_(), kIteratorEnd_(iterator_) {
    /// default iterator, return an empty iterator
    assert(this->IsDone());
    return;
  }
  InnerIterator_(const IteratorType& iterator_begin,
                 const IteratorType& iterator_end)
      : iterator_(iterator_begin), kIteratorEnd_(iterator_end) {
    return;
  }

 protected:
  inline bool IsDone() const { return this->iterator_ == this->kIteratorEnd_; }

  inline void ToNext() {
    assert(!this->IsDone());
    this->iterator_++;
    return;
  }

  template <typename ReturnIteratorType, IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline ReturnIteratorType& get_iterator() {
    /// static_assert check Type
    return this->iterator_;
  }
  template <typename ReturnIteratorType, IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline const ReturnIteratorType& get_const_iterator() const {
    /// static_assert check Type
    return this->iterator_;
  }
  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline ReturnType& get() {
    static_assert(
        std::is_same<ReturnType&,
                     decltype(this->iterator_.template get<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get<return_idx>());
  }

  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline const ReturnType& get_const() const {
    static_assert(
        std::is_same<const ReturnType&,
                     decltype(this->iterator_.template get_const<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get_const<return_idx>());
  }
};

/// the iterator of the container that only has one layer and more valuetype
template <enum ContainerType container_type_, enum SortType sort_type_,
          typename KeyType_, typename ValueType1_, typename... ValueType2_,
          bool is_const_>
class InnerIterator_<Container<container_type_, sort_type_, KeyType_,
                               ValueType1_, ValueType2_...>,
                     is_const_, 1> {
 private:
  using ContainerType = Container<container_type_, sort_type_, KeyType_,
                                  ValueType1_, ValueType2_...>;
  using ElementType = typename ContainerType::ElementType;

 protected:
  using IteratorType =
      typename std::conditional<is_const_,
                                typename ContainerType::const_iterator,
                                typename ContainerType::iterator>::type;

 private:
  IteratorType iterator_;
  IteratorType kIteratorEnd_;

 protected:
  inline const IteratorType& ConstInnerIterator() const {
    return this->iterator_;
  }

 public:
  InnerIterator_() : iterator_(), kIteratorEnd_(iterator_) {
    /// default iterator, return an empty iterator
    assert(this->IsDone());
    return;
  }
  InnerIterator_(const IteratorType& iterator_begin,
                 const IteratorType& iterator_end)
      : iterator_(iterator_begin), kIteratorEnd_(iterator_end) {
    return;
  }

  // protected:
  inline bool IsDone() const { return this->iterator_ == this->kIteratorEnd_; }

  inline void ToNext() {
    assert(!this->IsDone());
    this->iterator_++;
    return;
  }

  template <typename ReturnIteratorType,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline ReturnIteratorType& get_iterator() {
    /// static_assert check Type
    return this->iterator_;
  }
  template <typename ReturnIteratorType,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline const ReturnIteratorType& get_const_iterator() const {
    /// static_assert check Type
    return this->iterator_;
  }
  template <typename ReturnType,
            TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline ReturnType& get() {
    static_assert(
        std::is_same<ReturnType&,
                     decltype(this->iterator_.template get<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get<return_idx>());
  }

  template <typename ReturnType,
            TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline const ReturnType& get_const() const {
    static_assert(
         std::is_same<const ReturnType&,
                      decltype(this->iterator_.template get_const<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get_const<return_idx>());
  }
};

/// iterator of the container that has more than one layer
template <enum ContainerType container_type_, enum SortType sort_type_,
          typename KeyType_, typename InnerContainerType_,
          typename... ValueTypes_, bool is_const_, IteratorDepthType depth_>
class InnerIterator_<Container<container_type_, sort_type_, KeyType_,
                               InnerContainerType_, ValueTypes_...>,
                     is_const_, depth_>
    : protected _InnerIterator_<InnerContainerType_, is_const_, depth_, 2> {
  static_assert(depth_ >= 2, "Illegal depth");

 private:
  using ContainerType = Container<container_type_, sort_type_, KeyType_,
                                  InnerContainerType_, ValueTypes_...>;
  using ElementType = typename ContainerType::ElementType;

 protected:
  using IteratorType =
      typename std::conditional<is_const_,
                                typename ContainerType::const_iterator,
                                typename ContainerType::iterator>::type;

 private:
  static constexpr TupleIdxType kKeyIdx_ = 0;
  static constexpr TupleIdxType kInnerContainerIdx_ = 1;

  IteratorType iterator_;
  IteratorType kIteratorEnd_;

 protected:
  inline const IteratorType& ConstInnerIterator() const {
    return this->iterator_;
  }

  using InnerContainerType = InnerContainerType_;
  using InnerIteratorType =
      _InnerIterator_<InnerContainerType_, is_const_, depth_, 2>;

  template <bool judge = is_const_,
            typename std::enable_if<!judge, bool>::type = false>
  inline static typename InnerContainerType::iterator inner_iterator_begin(
      const IteratorType& container_iterator) {
    return (container_iterator.template get<kInnerContainerIdx_>()).begin();
  }

  template <bool judge = is_const_,
            typename std::enable_if<!judge, bool>::type = false>
  inline static typename InnerContainerType::iterator inner_iterator_end(
      const IteratorType& container_iterator) {
    return (container_iterator.template get<kInnerContainerIdx_>()).end();
  }

  template <bool judge = is_const_,
            typename std::enable_if<judge, bool>::type = false>
  inline static typename InnerContainerType::const_iterator
  inner_iterator_begin(const IteratorType& container_iterator) {
    return (container_iterator.template get_const<kInnerContainerIdx_>()).cbegin();
  }

  template <bool judge = is_const_,
            typename std::enable_if<judge, bool>::type = false>
  inline static typename InnerContainerType::const_iterator inner_iterator_end(
      const IteratorType& container_iterator) {
    return (container_iterator.template get_const<kInnerContainerIdx_>()).cend();
  }

  inline bool InnerIsDone() const {
    return InnerIteratorType::const_iterator() ==
           inner_iterator_end(this->iterator_);
  }

 public:
  InnerIterator_() : iterator_(), kIteratorEnd_(iterator_) {
    /// default iterator, return an empty iterator
    assert(this->IsDone());
    return;
  }
  InnerIterator_(const IteratorType& iterator_begin,
                 const IteratorType& iterator_end)
      : iterator_(iterator_begin), kIteratorEnd_(iterator_end) {
    if (iterator_begin != iterator_end) {
      /// the next layer iterator is not empty
      InnerIteratorType::set_iterator(inner_iterator_begin(iterator_begin));
      InnerIteratorType::set_lower_iterator();
    }
    return;
  }

 public:
  inline bool IsDone() const { return this->iterator_ == this->kIteratorEnd_; }

  inline void ToNext() {
    assert(!this->IsDone());
    assert(!this->InnerIsDone());
    if (!InnerIteratorType::_ToNext()) {
      /// this inner iterator has not moved
      return;
    }
    /// the inner iterator has moved
    if (!this->InnerIsDone()) {
      InnerIteratorType::set_lower_iterator();
      return;
    }
    /// the inner iterator is done
    this->iterator_++;
    if (this->IsDone()) return;
    InnerIteratorType::set_iterator(inner_iterator_begin(this->iterator_));
    InnerIteratorType::set_lower_iterator();
    return;
  }
  // add by wangyj
  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline ReturnType& get_iterator() {
    return this->iterator_;
  }
  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline const ReturnType& get_const_iterator() const {
    return this->iterator_;
  }

  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth != 0, bool>::type = false>
  inline ReturnType& get_iterator() {
    return InnerIteratorType::template get_iterator<ReturnType, depth>();
  }
  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth != 0, bool>::type = false>
  inline const ReturnType& get_const_iterator() const {
    return InnerIteratorType::template get_const_iterator<ReturnType, depth>();
  }
  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline ReturnType& get() {
    static_assert(
        std::is_same<ReturnType&,
                     decltype((this->iterator_.template get<return_idx>()))>::value,
        "Type mismatch");
    return (this->iterator_.template get<return_idx>());
  }

  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth != 0, bool>::type = false>
  inline ReturnType& get() {
    return InnerIteratorType::template get<ReturnType, return_idx, depth>();
  }

  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == 0, bool>::type = false>
  inline const ReturnType& get_const() const {
    static_assert(std::is_same<const ReturnType&,
                               decltype(this->iterator_.template get_const<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get_const<return_idx>());
  }

  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth != 0, bool>::type = false>
  inline const ReturnType& get_const() const {
    return InnerIteratorType::template get_const<ReturnType, return_idx,
                                                 depth>();
  }
};

template <enum ContainerType container_type_, enum SortType sort_type_,
          typename KeyType_, typename InnerContainerType_,
          typename... ValueTypes_, bool is_const_, IteratorDepthType depth_,
          IteratorDepthType now_depth_>
class _InnerIterator_<Container<container_type_, sort_type_, KeyType_,
                                InnerContainerType_, ValueTypes_...>,
                      is_const_, depth_, now_depth_>
    : protected _InnerIterator_<InnerContainerType_, is_const_, depth_,
                                now_depth_ + 1> {
  static_assert((now_depth_ > 1) && (now_depth_ < depth_), "Illegal depth");

 private:
  static constexpr TupleIdxType kKeyIdx_ = 0;
  static constexpr TupleIdxType kInnerContainerIdx_ = 1;

  using ContainerType = Container<container_type_, sort_type_, KeyType_,
                                  InnerContainerType_, ValueTypes_...>;
  using ElementType = typename ContainerType::ElementType;
  using IteratorType =
      typename std::conditional<is_const_,
                                typename ContainerType::const_iterator,
                                typename ContainerType::iterator>::type;

  using InnerContainerType = InnerContainerType_;
  using InnerIteratorType =
      _InnerIterator_<InnerContainerType_, is_const_, depth_, now_depth_ + 1>;
  IteratorType iterator_;

  template <bool judge = is_const_,
            typename std::enable_if<!judge, bool>::type = false>
  inline static typename InnerContainerType::iterator inner_iterator_begin(
      const IteratorType& container_iterator) {
    return (container_iterator.template get<kInnerContainerIdx_>()).begin();
  }

  template <bool judge = is_const_,
            typename std::enable_if<!judge, bool>::type = false>
  inline static typename InnerContainerType::iterator inner_iterator_end(
      const IteratorType& container_iterator) {
    return (container_iterator.template get<kInnerContainerIdx_>()).end();
  }

  template <bool judge = is_const_,
            typename std::enable_if<judge, bool>::type = false>
  inline static typename InnerContainerType::const_iterator
  inner_iterator_begin(const IteratorType& container_iterator) {
    return (container_iterator.template get_const<kInnerContainerIdx_>()).cbegin();
  }

  template <bool judge = is_const_,
            typename std::enable_if<judge, bool>::type = false>
  inline static typename InnerContainerType::const_iterator inner_iterator_end(
      const IteratorType& container_iterator) {
    return (container_iterator.template get_const<kInnerContainerIdx_>()).cend();
  }

  inline bool InnerIsDone() const {
    return InnerIteratorType::const_iterator() ==
           inner_iterator_end(this->iterator_);
  }

 protected:
  _InnerIterator_() = default;

  inline const IteratorType& const_iterator() const { return this->iterator_; }

  inline void set_iterator(const IteratorType& new_iterator) {
    this->iterator_ = new_iterator;
    return;
  }

  inline void set_lower_iterator() {
    InnerIteratorType::set_iterator(inner_iterator_begin(this->iterator_));
    InnerIteratorType::set_lower_iterator();
    return;
  }

  /// return whether iterator_ has moved
  inline bool _ToNext() {
    assert(!this->InnerIsDone());
    if (!InnerIteratorType::_ToNext()) {
      /// this inner iterator has not moved
      return false;  /// iterator_ has not moved
    }
    /// the inner iterator has moved
    if (!this->InnerIsDone()) {
      InnerIteratorType::set_lower_iterator();
      return false;  /// iterator_ has not moved
    }
    /// the inner iterator is done
    this->iterator_++;
    return true;
  }

  template <
      typename ReturnType, IteratorDepthType depth,
      typename std::enable_if<depth == now_depth_ - 1, bool>::type = false>
  inline ReturnType& get_iterator() {
    return this->iterator_;
  }
  template <
      typename ReturnType, IteratorDepthType depth,
      typename std::enable_if<depth == now_depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const_iterator() const {
    return this->iterator_;
  }
  template <
      typename ReturnType, IteratorDepthType depth,
      typename std::enable_if<depth != now_depth_ - 1, bool>::type = false>
  inline ReturnType& get_iterator() {
    return InnerIteratorType::template get_iterator<ReturnType, depth>();
  }
  template <
      typename ReturnType, IteratorDepthType depth,
      typename std::enable_if<depth != now_depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const_iterator() const {
    return InnerIteratorType::template get_const_iterator<ReturnType, depth>();
  }

  template <
      typename ReturnType, TupleIdxType return_idx, IteratorDepthType depth,
      typename std::enable_if<depth == now_depth_ - 1, bool>::type = false>
  inline ReturnType& get() {
    static_assert(
        std::is_same<ReturnType&,
                     decltype((this->iterator_.template get<return_idx>()))>::value,
        "Type mismatch");
    return (this->iterator_.template get<return_idx>());
  }

  template <
      typename ReturnType, TupleIdxType return_idx, IteratorDepthType depth,
      typename std::enable_if<depth != now_depth_ - 1, bool>::type = false>
  inline ReturnType& get() {
    return InnerIteratorType::template get<ReturnType, return_idx, depth>();
  }

  template <
      typename ReturnType, TupleIdxType return_idx, IteratorDepthType depth,
      typename std::enable_if<depth == now_depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const() const {
    static_assert(
          std::is_same<const ReturnType&, 
                       decltype(this->iterator_.template get_const<return_idx>())>::value,
        "Type mismatch");

    return (this->iterator_.template get_const<return_idx>());
  }

  template <
      typename ReturnType, TupleIdxType return_idx, IteratorDepthType depth,
      typename std::enable_if<depth != now_depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const() const {
    return InnerIteratorType::template get_const<ReturnType, return_idx,
                                                 depth>();
  }
};

template <enum ContainerType container_type_, enum SortType sort_type_,
          typename KeyType_, bool is_const_, IteratorDepthType depth_>
class _InnerIterator_<Container<container_type_, sort_type_, KeyType_>,
                      is_const_, depth_, depth_> {
  static_assert(depth_ > 1, "Illegal depth");

 private:
  using ContainerType = Container<container_type_, sort_type_, KeyType_>;
  using ElementType = typename ContainerType::ElementType;
  using IteratorType =
      typename std::conditional<is_const_,
                                typename ContainerType::const_iterator,
                                typename ContainerType::iterator>::type;

  IteratorType iterator_;

 protected:
  _InnerIterator_() = default;

  inline const IteratorType& const_iterator() const { return this->iterator_; }

  inline void set_iterator(const IteratorType& new_iterator) {
    this->iterator_ = new_iterator;
    return;
  }

  inline void set_lower_iterator() const { return; }

  /// return whether iterator_ has moved
  inline bool _ToNext() {
    this->iterator_++;
    return true;
  }

  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline ReturnType& get_iterator() {
    return this->iterator_;
  }
  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const_iterator() const {
    return this->iterator_;
  }
  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline ReturnType& get() {
    static_assert(
        std::is_same<ReturnType&,
                     decltype((this->iterator_.template get<return_idx>()))>::value,
        "Type mismatch");
    return (this->iterator_.template get<return_idx>());
  }

  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const() const {
    static_assert(
         std::is_same<const ReturnType&, 
                      decltype(this->iterator_.template get_const<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get_const<return_idx>());
  }
};
template <enum ContainerType container_type_, enum SortType sort_type_,
          typename KeyType_, typename ValueType_, typename... ValueTypes_,
          bool is_const_, IteratorDepthType depth_>
class _InnerIterator_<Container<container_type_, sort_type_, KeyType_,
                                ValueType_, ValueTypes_...>,
                      is_const_, depth_, depth_> {
  static_assert(depth_ > 1, "Illegal depth");

 private:
  using ContainerType = Container<container_type_, sort_type_, KeyType_,
                                  ValueType_, ValueTypes_...>;
  using ElementType = typename ContainerType::ElementType;
  using IteratorType =
      typename std::conditional<is_const_,
                                typename ContainerType::const_iterator,
                                typename ContainerType::iterator>::type;

  IteratorType iterator_;

 protected:
  _InnerIterator_() = default;

  inline const IteratorType& const_iterator() const { return this->iterator_; }

  inline void set_iterator(const IteratorType& new_iterator) {
    this->iterator_ = new_iterator;
    return;
  }

  inline void set_lower_iterator() const { return; }

  /// return whether iterator_ has moved
  inline bool _ToNext() {
    this->iterator_++;
    return true;
  }

  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline ReturnType& get_iterator() {
    return this->iterator_;
  }
  template <typename ReturnType, IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const_iterator() const {
    return this->iterator_;
  }
  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline ReturnType& get() {
    static_assert(
        std::is_same<ReturnType&,
                     decltype((this->iterator_.template get<return_idx>()))>::value,
        "Type mismatch");
    return (this->iterator_.template get<return_idx>());
  }

  template <typename ReturnType, TupleIdxType return_idx,
            IteratorDepthType depth,
            typename std::enable_if<depth == depth_ - 1, bool>::type = false>
  inline const ReturnType& get_const() const {
    static_assert(
         std::is_same<const ReturnType&, 
                      decltype(this->iterator_.template get_const<return_idx>())>::value,
        "Type mismatch");
    return (this->iterator_.template get_const<return_idx>());
  }
};

// Iterator_ "outter"
template <typename ContentIterator_>
class Iterator_ : protected ContentIterator_ {
 private:
  using ContentPtr = typename ContentIterator_ ::ContentPtr;

 public:
  using ContentIterator_::ContentIterator_;

  template <bool judge = ContentIterator_::kIsConst_,
            typename std::enable_if<!judge, bool>::type = false>
  inline ContentPtr operator->() {
    assert(!this->IsDone());
    return ContentIterator_::content_ptr();
  }

  template <bool judge = ContentIterator_::kIsConst_,
            typename std::enable_if<judge, bool>::type = false>
  inline ContentPtr operator->() const {
    assert(!this->IsDone());
    return ContentIterator_::content_ptr();
  }

  inline Iterator_ operator++() {
    /// prefix
    assert(!this->IsDone());
    ContentIterator_::ToNext();
    return *this;
  }
  inline Iterator_ operator++(int) {
    /// postfix
    assert(!this->IsDone());
    Iterator_ temp(*this);
    //++(*this);
    ContentIterator_::ToNext();
    return temp;
  }
  inline bool IsDone() const { return ContentIterator_::IsDone(); }
};

template <typename ContentIterator_>
class IteratorWithInstance_ : public Iterator_<ContentIterator_> {
 private:
  using IteratorType = Iterator_<ContentIterator_>;

  using ContentDataType = typename ContentIterator_
                                 ::ContentDataType;

 public:
  using IteratorType::IteratorType;

  template <bool judge = ContentIterator_::kIsConst_,
            typename std::enable_if<!judge, bool>::type = false>
  inline const ContentDataType& operator*() {
    assert(!this->IsDone());
    return ContentIterator_::instance();
  }

  template <bool judge = ContentIterator_::kIsConst_,
            typename std::enable_if<judge, bool>::type = false>
  inline const ContentDataType& operator*() const {
    assert(!this->IsDone());
    return ContentIterator_::instance();
  }
};

}  // namespace GUNDAM

#endif  // _ITERATOR_H
