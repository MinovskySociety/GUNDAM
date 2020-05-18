#ifndef _MATCH_H
#define _MATCH_H
#pragma once
#include <iostream>
#include <set>
#include <vector>

namespace GUNDAM {
/// can be configured to much different type
///     constant src graph to     constant dst graph
///     constant src graph to non-constant dst graph
/// non-constant src graph to     constant dst graph
/// non-constant src graph to non-constant dst graph
template<typename SrcGraphType,
         typename DstGraphType,
         enum ContainerType map_container_type
                               = ContainerType::Vector,
         enum SortType map_container_sort_type
                                    = SortType::None>
class Match{
 private:
  static constexpr bool
       kSrcIsConst_ = std::is_const<SrcGraphType>::value;
  using SrcVertexIDType = typename SrcGraphType
                                   ::VertexType
                                       ::IDType;
  using SrcVertexPtrType
    = typename std::conditional<kSrcIsConst_,
                        typename SrcGraphType
                             ::VertexConstPtr,
                        typename SrcGraphType
                                  ::VertexPtr>::type;

  static constexpr bool
       kDstIsConst_ = std::is_const<DstGraphType>::value;
  using DstVertexIDType = typename DstGraphType
                                   ::VertexType
                                       ::IDType;
  using DstVertexPtrType
    = typename std::conditional<kDstIsConst_,
                        typename DstGraphType
                             ::VertexConstPtr,
                        typename DstGraphType
                                  ::VertexPtr>::type;

  static constexpr TupleIdxType kSrcVertexPtrIdx = 0;
  static constexpr TupleIdxType kDstVertexPtrIdx = 1;

  using MapContainerType
         = Container<map_container_type,
                     map_container_sort_type,
                     SrcVertexPtrType,
                     DstVertexPtrType>;

  MapContainerType match_container_;

 public:
  using size_type = typename MapContainerType::size_type;

  Match():match_container_(){
    return;
  }

  inline size_type size() const{
    return this->match_container_.size();
  }

  inline bool HasMap(SrcVertexPtrType& src_ptr) const{
    /// <iterator, bool>
    auto ret = this->match_container_.FindConst(src_ptr);
    return ret.second;
  }

  /// if such a match does not exist, add this src-dst
  /// pair to current match then return true
  /// else if the match for this src vertex has already
  /// exisited then return false
  inline bool AddMap(SrcVertexPtrType& src_ptr,
                     DstVertexPtrType& dst_ptr){
    /// <iterator, bool>
    auto ret = this->match_container_.Insert(src_ptr);
    if (!ret.second){
      /// has already existed in the current match
      return false;
    }
    /// has not added match for that vertex
    std::get<kDstVertexPtrIdx>(*(ret.first)) = dst_ptr;
    return true;
  }

  /// constant dst
  inline DstVertexPtrType MapTo(const SrcVertexPtrType& src_ptr) const {
    /// <iterator, bool>
    auto ret = this->match_container_.FindConst(src_ptr);
    if (!ret.second){
      /// does not find src_ptr
      /// this just a partial match and does not contain
      return DstVertexPtrType();
    }
    /// found that
    return std::get<kDstVertexPtrIdx>(*(ret.first));
  }

  bool operator==(const Match& match) const {
    for (auto it  = this->match_container_.cbegin();
              it != this->match_container_.cend();it++){
      if (std::get<kDstVertexPtrIdx>(*it) != match.MapTo(std::get<kSrcVertexPtrIdx>(*it))){
        /// not equal
        return false;
      }
    }
    return true;
  }
};

template<typename SrcGraphType,
         typename DstGraphType,
         enum ContainerType match_container_type
                                 = ContainerType::Vector,
         enum SortType match_container_sort_type
                                      = SortType::None>
class MatchSet{
 private:
  static constexpr bool
       kSrcIsConst_ = std::is_const<SrcGraphType>::value;
  static constexpr bool
       kDstIsConst_ = std::is_const<DstGraphType>::value;

  using SrcVertexPtrType
    = typename std::conditional<kSrcIsConst_,
                        typename SrcGraphType
                             ::VertexConstPtr,
                        typename SrcGraphType
                                  ::VertexPtr>::type;
  using DstVertexPtrType
    = typename std::conditional<kDstIsConst_,
                        typename DstGraphType
                             ::VertexConstPtr,
                        typename DstGraphType
                                  ::VertexPtr>::type;

  using MatchType
      = Match<SrcGraphType,
              DstGraphType>;

  static constexpr TupleIdxType kMatchIdx = 0;

  using MatchContainerType
           = Container<match_container_type,
                       match_container_sort_type,
                       MatchType>;

  template <typename  ContainerType_,
            bool           is_const_,
            IteratorDepthType depth_,
            TupleIdxType    key_idx_>
  class MatchContentIterator_
      : protected InnerIterator_<ContainerType_,
                                      is_const_,
                                         depth_> {
   private:
    using InnerIteratorType = InnerIterator_<ContainerType_,
                                                  is_const_,
                                                     depth_>;

   protected:
    using ContentPtr = MatchType*;
    using InnerIteratorType::IsDone;
    using InnerIteratorType::ToNext;
    static constexpr bool kIsConst_ = is_const_;

    template <bool judge = is_const_,
              typename std::enable_if<judge, bool>::type = false>
    inline ContentPtr const content_ptr() const {
      assert(!this->IsDone());
      return &(InnerIteratorType::template get_const<MatchType,
                                                      key_idx_,
                                                    depth_ - 1>());
    }

    template <bool judge = is_const_,
              typename std::enable_if<!judge, bool>::type = false>
    inline ContentPtr content_ptr() {
      assert(!this->IsDone());
      return &(InnerIteratorType::template get<MatchType,
                                                key_idx_,
                                              depth_ - 1>());
    }

   public:
    using InnerIteratorType::InnerIteratorType;
  };

  MatchContainerType match_set;

 public:
  using MatchIterator
           = Iterator_<MatchContentIterator_<
                       MatchContainerType,false,1,kMatchIdx>>;
  using MatchConstIterator
           = Iterator_<MatchContentIterator_<
                       MatchContainerType, true,1,kMatchIdx>>;

  using size_type = typename MatchContainerType::size_type;

  MatchSet():match_set(){
    return;
  }

  inline size_type size() const{
    return this->match_set.size();
  }

  inline MatchIterator MatchBegin(){
    return MatchIterator(this->match_set.begin(),
                         this->match_set.end());
  }

  inline MatchConstIterator MatchCBegin() const{
    return MatchConstIterator(this->match_set.cbegin(),
                              this->match_set.cend());
  }

  inline std::pair<MatchIterator, bool> AddMatch(const MatchType& match) {
    /// <iterator, bool>
    auto ret = this->match_set.Insert(match);
    assert(ret.second
        || ret.first == this->match_set.end());
    return std::pair<MatchIterator, bool>
                    (MatchIterator(ret.first,this->match_set.end()),
                                   ret.second);
  }
};

}  // namespace GUNDAM

#endif
