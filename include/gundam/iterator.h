#ifndef _ITERATOR_H
#define _ITERATOR_H

#include "container.h"

namespace GUNDAM {
using IteratorDepthType = uint8_t;

template<typename  ContainerType_,
         bool           is_const_,
         IteratorDepthType depth_>
class InnerIterator_;

template<typename      ContainerType_,
         bool               is_const_,
         IteratorDepthType     depth_,
         IteratorDepthType now_depth_>
class _InnerIterator_;

template<typename ContentIterator_>
class Iterator_ : protected ContentIterator_{
 private:
  using ContentPtr = typename ContentIterator_
                            ::ContentPtr;

 public:
  using ContentIterator_::ContentIterator_;

  inline ContentPtr operator->(){
    std::cout<<"wenzhi test in \"->\" "<<std::endl;
    return ContentIterator_::content_ptr();
  }
  inline Iterator_ operator++(){
    /// prefix
    assert(!this->IsDone());
    ContentIterator_::ToNext();
    return *this;
  }
  inline Iterator_ operator++(int){
    /// postfix
    assert(!this->IsDone());
    Iterator_ temp(*this);
    ++(*this);
    return temp;
  }
  inline bool IsDone() const{
    return ContentIterator_::IsDone();
  }
};

/// the iterator of the container that only has one layer
template <enum ContainerType container_type_,
          enum SortType           sort_type_,
          typename                  KeyType_,
          typename                ValueType_,
          bool                     is_const_>
class InnerIterator_<Container<container_type_,
                                    sort_type_,
                                      KeyType_,
                                    ValueType_>,
                                     is_const_, 1>{
 private:
  using ContainerType = Container<container_type_,
                                       sort_type_,
                                         KeyType_,
                                       ValueType_>;

  using IteratorType = typename    std::conditional<is_const_,
                       typename ContainerType::const_iterator,
                       typename ContainerType::      iterator>::type;

        IteratorType  iterator_;
  const IteratorType kIteratorEnd_;

 public:
  InnerIterator_():iterator_(),
                  kIteratorEnd_(iterator_){
    /// default iterator, return an empty iterator
    assert(this->IsDone());
    return;
  }
  InnerIterator_(const IteratorType& iterator_begin,
                 const IteratorType& iterator_end)
                      :iterator_    (iterator_begin),
                      kIteratorEnd_ (iterator_end){
    std::cout<<"wenzhi in constructor of InnerIterator_"<<std::endl;
    return;
  }

 protected:
  inline bool IsDone() const{
    return this->iterator_ == this->kIteratorEnd_;
  }

  inline void ToNext(){
    assert(!this->IsDone());
    this->iterator_++;
    return;
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == 0, bool>::type = false>
  inline std::tuple<KeyType_,ValueType_>& get(){
    std::cout<<"wenzhi test in get"<<std::endl;
    assert(!this->IsDone());
    return *(this->iterator_);
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == 0, bool>::type = false>
  inline const std::tuple<KeyType_,ValueType_>& const_get() const{
    return *(this->iterator_);
  }
};

/// iterator of the container that has more than one layer
template <enum ContainerType container_type_,
          enum SortType           sort_type_,
          typename                  KeyType_,
          typename       InnerContainerType_,
          bool                     is_const_,
          IteratorDepthType           depth_>
class InnerIterator_<Container<container_type_,
                                    sort_type_,
                                      KeyType_,
                           InnerContainerType_>,
                                     is_const_,
                                        depth_>
              : protected _InnerIterator_<
                           InnerContainerType_,
                                     is_const_,
                                        depth_, 2>{
  static_assert(depth_ >= 2,"Illegal depth");
 private:
  using ContainerType = Container<container_type_,
                                       sort_type_,
                                         KeyType_,
                              InnerContainerType_>;

  using IteratorType = typename    std::conditional<is_const_,
                       typename ContainerType::const_iterator,
                       typename ContainerType::      iterator>::type;

  using InnerContainerType =  InnerContainerType_;
  using InnerIteratorType  = _InnerIterator_<
                              InnerContainerType_,
                                        is_const_,
                                           depth_, 2>;
        IteratorType  iterator_;
  const IteratorType kIteratorEnd_;

  template<bool judge = is_const_,
           typename std::enable_if<!judge,bool>::type = false>
  inline static typename InnerContainerType::iterator
    inner_iterator_begin(
                const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).begin();
  }

  template<bool judge = is_const_,
           typename std::enable_if<!judge,bool>::type = false>
  inline static typename InnerContainerType::iterator
    inner_iterator_end(
                const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).end();
  }

  template<bool judge = is_const_,
           typename std::enable_if<judge,bool>::type = false>
  inline static typename InnerContainerType::const_iterator
          inner_iterator_begin(
                     const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).cbegin();
  }

  template<bool judge = is_const_,
           typename std::enable_if<judge,bool>::type = false>
  inline static typename InnerContainerType::const_iterator
          inner_iterator_end(
                     const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).cend();
  }

  inline bool InnerIsDone() const{
    return InnerIteratorType::const_iterator()
        == inner_iterator_end(this->iterator_);
  }

 public:
  InnerIterator_(const IteratorType&  iterator_begin,
                 const IteratorType&  iterator_end)
                 :InnerIteratorType (
                 inner_iterator_begin(iterator_begin)),
                       iterator_     (iterator_begin),
                      kIteratorEnd_  (iterator_end){
    return;
  }

 protected:
  using InnerIteratorType::get;

  inline bool IsDone() const{
    return this->iterator_ == this->kIteratorEnd_;
  }

  inline void ToNext(){
    assert(!this->IsDone());
    assert(!this->InnerIsDone());
    if (!InnerIteratorType::_ToNext()){
      /// this inner iterator has not moved
      return;
    }
    /// the inner iterator has moved
    if (!this->InnerIsDone()){
      InnerIteratorType::set_lower_iterator();
      return;
    }
    /// the inner iterator is done
    this->iterator_++;
    if (this->IsDone())
      return;
    InnerIteratorType::set_iterator(inner_iterator_begin(this->iterator_));
    InnerIteratorType::set_lower_iterator();
    return;
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == 0, bool>::type = false>
  inline KeyType_& get(){
    return std::get<0>(*(this->iterator_));
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == 0, bool>::type = false>
  inline const KeyType_& const_get() const{
    return std::get<0>(*(this->iterator_));
  }
};

template <enum ContainerType container_type_,
          enum SortType           sort_type_,
          typename                  KeyType_,
          typename       InnerContainerType_,
          bool                     is_const_,
          IteratorDepthType           depth_,
          IteratorDepthType       now_depth_>
class _InnerIterator_<Container<container_type_,
                                     sort_type_,
                                       KeyType_,
                            InnerContainerType_>,
                                      is_const_,
                                         depth_,
                                     now_depth_>
               : protected _InnerIterator_<
                            InnerContainerType_,
                                      is_const_,
                                         depth_,
                                     now_depth_ + 1>{
  static_assert((now_depth_ > 1)
             && (now_depth_ < depth_),"Illegal depth");
 private:
  using ContainerType = Container<container_type_,
                                       sort_type_,
                                         KeyType_,
                              InnerContainerType_>;

  using IteratorType = typename    std::conditional<is_const_,
                       typename ContainerType::const_iterator,
                       typename ContainerType::      iterator>::type;

  using InnerContainerType =  InnerContainerType_;
  using InnerIteratorType  = _InnerIterator_<InnerContainerType_,
                                                       is_const_,
                                                          depth_,
                                                      now_depth_ + 1>;
  IteratorType iterator_;

  template<bool judge = is_const_,
           typename std::enable_if<!judge,bool>::type = false>
  inline static typename InnerContainerType::iterator
    inner_iterator_begin(
               const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).begin();
  }

  template<bool judge = is_const_,
           typename std::enable_if<!judge,bool>::type = false>
  inline static typename InnerContainerType::iterator
    inner_iterator_end(
               const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).end();
  }

  template<bool judge = is_const_,
           typename std::enable_if<judge,bool>::type = false>
  inline static typename InnerContainerType::const_iterator
          inner_iterator_begin(
                     const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).cbegin();
  }

  template<bool judge = is_const_,
           typename std::enable_if<judge,bool>::type = false>
  inline static typename InnerContainerType::const_iterator
          inner_iterator_end(
                     const IteratorType& container_iterator){
    return std::get<1>(*(container_iterator)).cend();
  }


  inline bool InnerIsDone() const{
    return InnerIteratorType::const_iterator()
                     == this->inner_iterator_end();
  }

 protected:
  using InnerIteratorType::get;

  _InnerIterator_(const IteratorType& iterator_begin)
                           :iterator_(iterator_begin),
                    InnerIteratorType(
                          this->inner_iterator_begin()){
    return;
  }

  inline const IteratorType& const_iterator() const{
    return this->iterator_;
  }

  inline void set_iterator(const IteratorType& new_iterator){
    this->iterator_ = new_iterator;
    return;
  }

  inline void set_lower_iterator(){
    InnerIteratorType::set_iterator(
                     inner_iterator_begin(this->iterator_));
    InnerIteratorType::set_lower_iterator();
    return;
  }

  /// return whether iterator_ has moved
  inline bool _ToNext(){
    assert(!this->IsDone());
    assert(!this->InnerIsDone());
    if (!InnerIteratorType::_ToNext()){
      /// this inner iterator has not moved
      return false; /// iterator_ has not moved
    }
    /// the inner iterator has moved
    if (!this->InnerIsDone()){
      InnerIteratorType::set_lower_iterator();
      return false; /// iterator_ has not moved
    }
    /// the inner iterator is done
    this->iterator_++;
    return true;
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == now_depth_-1, bool>::type = false>
  inline KeyType_& get(){
    return std::get<0>(*(this->iterator_));
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == now_depth_-1, bool>::type = false>
  inline const KeyType_& const_get() const{
    return std::get<0>(*(this->iterator_));
  }
};

template <enum ContainerType container_type_,
          enum SortType           sort_type_,
          typename                  KeyType_,
          typename                ValueType_,
          bool                     is_const_,
          IteratorDepthType           depth_>
class _InnerIterator_<Container<container_type_,
                                     sort_type_,
                                       KeyType_,
                                     ValueType_>,
                                      is_const_,
                                         depth_,
                                         depth_>{
  static_assert(depth_ > 1, "Illegal depth");
 private:
  using ContainerType = Container<container_type_,
                                       sort_type_,
                                         KeyType_,
                                       ValueType_>;

  using IteratorType = typename    std::conditional<is_const_,
                       typename ContainerType::const_iterator,
                       typename ContainerType::      iterator>::type;

  IteratorType iterator_;

 protected:
  _InnerIterator_(const IteratorType& iterator_begin)
                           :iterator_(iterator_begin){
    return;
  }

  inline const IteratorType& const_iterator() const{
    return this->iterator_;
  }

  inline void set_iterator(const IteratorType& new_iterator){
    this->iterator_ = new_iterator;
    return;
  }

  inline void set_lower_iterator() const{
    return;
  }

  /// return whether iterator_ has moved
  inline bool _ToNext(){
    this->iterator_++;
    return true;
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == depth_-1, bool>::type = false>
  inline std::tuple<KeyType_,ValueType_>& get(){
    return (*(this->iterator_));
  }

  template<IteratorDepthType depth,
           typename std::enable_if<depth == depth_-1, bool>::type = false>
  inline const std::tuple<KeyType_,ValueType_>& const_get() const{
    return (*(this->iterator_));
  }
};

}

#endif  // _ITERATOR_H
