#ifndef _CONTAINER_H
#define _CONTAINER_H

#include <assert.h>
#include <vector>
#include <map>

namespace GUNDAM {

using TupleIdxType = uint8_t;

enum class ContainerType:uint8_t{
    List,   /// for graph with few  elements (Edge types or associated Vertexes)
    Vector, /// for static graph
    Array,  /// for static graph that can statically set the maximum size of neighbors
    Map,    /// for graph with lots elements (Edge types or associated Vertexes)
    Set,    ///
/// unimplemented feature
    Adaptive/// use list when this vertex have few  elements (Edge types or associated Vertexes)
            /// use map  when this vertex have lots elements (Edge types or associated Vertexes)
            /// for dynamic graph with non-balanced elements distribution
};

enum class SortType:uint8_t{
    /// how the vertexes connected by the same type of edge
    /// are sorted
       None, /// not sorted
     Ascend, /// sorted by  ascend order
    Descend  /// sorted by descend order
};

template <enum ContainerType container_type_,
          enum SortType sort_type_,
          typename        KeyType_,
          typename...   ValueTypes>
class Container {
  /// trick the compiler, same to static_assert(false)
  static_assert( std::is_object<KeyType_>::value, "Unsupported configuration");
  static_assert(!std::is_object<KeyType_>::value, "Unsupported configuration");
};

template <enum SortType sort_type_,
          typename        KeyType_,
          typename...   ValueTypes_>
class Container<ContainerType::Vector,
                    sort_type_,
                      KeyType_,
                    ValueTypes_...> {
 public:
  static constexpr uint8_t kKeyIdx = 0;
  using     KeyType = KeyType_;
  using ElementType = std::tuple<KeyType_,
                               ValueTypes_...>;

 private:
  using InnerContainerType = std::vector<ElementType>;
  InnerContainerType container_;

  static_assert(sort_type_ == SortType::None,
               "other sorting type are not supported yet");

 public:
  using const_iterator = typename InnerContainerType::const_iterator;
  using       iterator = typename InnerContainerType::      iterator;
  using      size_type = typename InnerContainerType::size_type;

  inline const_iterator cbegin() const {
    return this->container_.cbegin();
  }
  inline const_iterator cend() const {
    return this->container_.cend();
  }
  inline iterator begin() {
    return this->container_.begin();
  }
  inline iterator end() {
    return this->container_.end();
  }
  inline size_type size() const {
    return this->container_.size();
  }
  inline bool empty() const {
    return this->container_.empty();
  }
  inline void clear() {
    this->container_.clear();
    return;
  }
  inline void shrink_to_fit() const {
    this->container_.shrink_to_fit();
    return;
  }

  inline std::pair<iterator, bool> Insert(const KeyType_& key) {
    if (sort_type_ == SortType::None) {
      for (iterator it  = this->container_.begin();
                    it != this->container_. end ();++it){
        if ((std::get<kKeyIdx>(*it)) == key)
          return std::pair<iterator, bool>(it, false);
      }
      this->container_.emplace_back(
          std::tuple_cat(std::tuple<KeyType_>(key),
                         std::tuple<ValueTypes_...>()));
      return std::pair<iterator, bool>(this->container_.end() - 1, true);
    }
    /// other sorting type are not implemented
    assert(false);
    return std::pair<iterator, bool>(this->container_.end(), false);
  }

  inline std::pair<iterator, bool> Find(const KeyType& key) {
    if (sort_type_ == SortType::None) {
      for (iterator it  = this->container_.begin();
                    it != this->container_. end ();++it){
        if ((std::get<kKeyIdx>(*it)) == key)
          return std::pair<iterator, bool>(it, true);
      }
      return std::pair<iterator, bool>(this->container_.end(), false);
    }
    /// other sorting type are not implemented
    assert(false);
    return std::pair<iterator, bool>(this->container_.end(), false);
  }

  inline std::pair<const_iterator, bool> FindConst(const KeyType& key) const {
    if (sort_type_ == SortType::None) {
      for (const_iterator it  = this->container_.cbegin();
                          it != this->container_. cend (); ++it){
        if ((std::get<kKeyIdx>(*it)) == key)
          return std::pair<const_iterator, bool>(it, true);
      }
      return std::pair<const_iterator, bool>(this->container_.cend(), false);
    }
    /// other sorting type are not implemented
    assert(false);
    return std::pair<const_iterator, bool>(this->container_.cend(), false);
  }
};

}

#endif  // _CONTAINER_H
