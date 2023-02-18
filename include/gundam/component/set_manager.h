#ifndef _GUNDAM_COMPONENT_SET_MANAGER_H
#define _GUNDAM_COMPONENT_SET_MANAGER_H

#include <functional>
#include <vector>
#include <unordered_map>
#include <memory>

namespace GUNDAM {

template <typename ElementType,
          typename ElementComp  = std::less    <ElementType>,
          typename Hash         = std::hash    <ElementType>,
          typename ElementEqual = std::equal_to<ElementType>>
class SetManager {
 private:
  static constexpr ElementComp comp = ElementComp();

  class Set;

 public:
  using set_ptr_t       =       Set*;
  using set_const_ptr_t = const Set*;

 private:
  class Set {
   private:
    friend class SetManager;

   public:
    Set() = default;

    Set(const Set &) = delete;
    Set(Set &&) = default;

    Set &operator=(const Set &) = delete;
    Set &operator=(Set &&) = default;

    inline const auto& element_set() const {
      return this->element_set_;
    }

    inline const ElementType& element_at(size_t element_idx) const {
      return this->element_set_[element_idx];
    }

    inline size_t element_num() const {
      return this->element_set_.size();
    }

   private:
    std::vector<ElementType> element_set_;

		std::unordered_map<ElementType, set_ptr_t,
                       Hash, ElementEqual> children_;
  };
 
 public:
  SetManager() = default;

  SetManager(const SetManager &) = delete;
  SetManager(SetManager &&) = default;

  SetManager &operator=(const SetManager &) = delete;
  SetManager &operator=(SetManager &&) = default;

  ~SetManager() {
    for (auto& child_ptr : this->all_allocated_set_ptr_) {
      assert(child_ptr != nullptr);
      delete child_ptr;
    }
    return;
  }
  
  inline set_ptr_t GetJoinRelation(const ElementType& element) {
    const std::vector<ElementType> set = {element};
    return this->GetJoinRelation(set);
  }

  template <bool is_sorted  = true> // to mark whether the input "set" is sorted
  set_ptr_t GetJoinRelation(const std::vector<ElementType>& set) {

    if constexpr (!is_sorted) {
      std::vector<ElementType> sorted_set = set;
      std::sort(sorted_set.begin(), sorted_set.end(), SetManager::comp);
      return this->template GetJoinRelation<true>(sorted_set);
    }

    assert(std::is_sorted(set.begin(), set.end(), SetManager::comp));

    set_ptr_t info = &this->root_;

    // now look it up in the tree
    for (const auto& element : set) {
      auto child_it = info->children_.find(element);
      if (child_it == info->children_.end()) {
        // node not found, create it
        child_it = info->children_.emplace_hint(child_it, element, new Set());
        this->all_allocated_set_ptr_.emplace_back(child_it->second);
      }
      // move to the next level of node
      info = child_it->second;
    }

    // now check if the Set has already been created
    if (info->element_set_.empty()) {
      // if it hasn't we need to create it
      info->element_set_ = set;
    }
    return info;
  }

  template <bool is_sorted = true> // to mark whether the input "set" is sorted
  inline size_t EnumerateSubset(const std::vector<ElementType>& set) const {

    if constexpr (!is_sorted) {
      std::vector<ElementType> sorted_set = set;
      std::sort(sorted_set.begin(), sorted_set.end(), SetManager::comp);
      return this->template EnumerateSubset<true>(sorted_set);
    }

    assert(std::is_sorted(set.begin(), set.end(), SetManager::comp));

    size_t subset_count = 0;
    
    for (size_t i = 0; i < set.size(); i++) {
      // try to enumerate the subset: {set[i], ..., set[set.size() - 1]}
      Set* current_set_ptr = &(this->root_);
      for (size_t j = i; j < set.size(); j++) {
        auto entry = current_set_ptr->children_.find(set[j]);
        if (entry == current_set_ptr->children_.end()) {
          // does not have such a child, e.g. cannot find a larger subset
          break;
        }
        current_set_ptr = entry->second;
        subset_count++;
      }
    }
    return subset_count;
  }

  inline set_ptr_t Union(set_const_ptr_t set_0_ptr,
                         set_const_ptr_t set_1_ptr) {
    // auto relations = unique_ptr<idx_t[]>(new idx_t[left->count + right->count]);
    std::vector<ElementType> union_set;
    union_set.reserve(set_0_ptr->size() + set_1_ptr->size());

    std::merge(set_0_ptr->element_set_.begin(), set_0_ptr->element_set_.end(), 
               set_1_ptr->element_set_.begin(), set_1_ptr->element_set_.end(), 
               std::back_inserter(union_set), SetManager::comp);

    assert(union_set.size() == set_0_ptr->size() + set_1_ptr->size());

    assert(std::is_sorted(union_set.begin(),
                          union_set.end(), SetManager::comp));

    return this->GetJoinRelation(std::move(union_set));
  }

 private:
  Set root_;

  std::vector<Set*> all_allocated_set_ptr_;
};

}; 

#endif // _GUNDAM_COMPONENT_SET_MANAGER_H