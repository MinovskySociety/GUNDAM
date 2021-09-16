#ifndef _GUNDAM_COMPONENT_DISJOINT_SET_H
#define _GUNDAM_COMPONENT_DISJOINT_SET_H

namespace GUNDAM {

template<typename ElementType,
         bool kElementTypeConvertable = true>
class DisjointSet {
 private:
  // static constexpr bool kElementTypeConvertable
  //  = std::is_convertible<ElementType, typename std::vector<ElementType>::size_type>::value;

  // return the father of x
  inline ElementType& GetFather(const ElementType& x) {
    if constexpr (kElementTypeConvertable) {
      // this->father_ is a vector
      assert(x >= 0);
      // added an element that have not been considered before
      if (x >= this->father_.size()) {
        const auto kOldSize = this->father_.size();
        this->father_.reserve(x + 1);
        for (ElementType i = kOldSize; i <= x; i++) {
          this->father_.emplace_back(i);
        }
      }
      return this->father_[x];
    }
    if constexpr (!kElementTypeConvertable) {
      // this->father_ is a map
      auto father_it = this->father_.find(x);
      if (father_it != this->father_.end()) {
        // has this element
        return father_it->second;
      }
      // does not have such an element
      auto father_emplaced_it = this->father_.emplace_hint(father_it, x, x);
      assert(father_emplaced_it != this->father_.end());
      assert(father_emplaced_it->second == x);
      return father_emplaced_it->second;
    }
  }

 public:
  DisjointSet() = default;

  DisjointSet(size_t expand_node_size) {
    if constexpr (kElementTypeConvertable) {
      // this->father_ is a vector
      this->father_.resize(expand_node_size);
      for (ElementType i = 0; i < expand_node_size; i++) {
        this->father_[i] = i;
      }
    }
    return;
  }

  inline const ElementType& Find(const ElementType& x) {
    return x == this->GetFather(x) ? 
           x : (this->GetFather(x) = this->Find(this->GetFather(x)));
  }

  inline void Merge(const ElementType& x,
                    const ElementType& y) {
    this->GetFather(this->Find(x)) = this->Find(y);
    return;
  }

 private:
  typename std::conditional<kElementTypeConvertable, 
                            std::vector<ElementType>,
                            std::  map <ElementType,
                                        ElementType> >::type father_;
};

};


#endif // _GUNDAM_COMPONENT_DISJOINT_SET_H