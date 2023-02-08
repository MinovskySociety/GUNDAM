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

    inline size_t size() const {
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
    for (auto& [child_element, child_ptr] : root_.children_) {
      assert(child_ptr != nullptr);
      delete child_ptr;
    }
    return;
  }

  set_ptr_t GetJoinRelation(std::vector<ElementType>& set) {

    assert(std::is_sorted(set.begin(), set.end(), 
                          SetManager::comp));

    set_ptr_t info = &this->root_;

    // now look it up in the tree
    for (const auto& element : set) {
      auto child_it = info->children_.find(element);
      if (child_it == info->children_.end()) {
        // node not found, create it
        child_it = info->children_.emplace_hint(child_it, element, new Set());
      }
      // move to the next level of node
      info = child_it->second;
    }

    // now check if the Set has already been created
    if (info->element_set_.empty()) {
      // if it hasn't we need to create it
      info->element_set_.swap(set);
    }
    return info;
  }

  inline set_ptr_t Union(set_const_ptr_t set_0_ptr,
                         set_const_ptr_t set_1_ptr) {
    // auto relations = unique_ptr<idx_t[]>(new idx_t[left->count + right->count]);
    std::vector<ElementType> union_set;
    union_set.reserve(set_0_ptr->size() + set_1_ptr->size());

    std::merge(set_0_ptr->children_.begin(), set_0_ptr->children_.end(), 
               set_1_ptr->children_.begin(), set_1_ptr->children_.end(), 
               std::back_inserter(union_set), SetManager::comp);

    assert(union_set.size() == set_0_ptr->size() + set_1_ptr->size());

    assert(std::is_sorted(union_set.begin(),
                          union_set.end(), SetManager::comp));

    return this->GetJoinRelation(std::move(union_set));
  }

 private:
  Set root_;
};

}; 

#endif // _GUNDAM_COMPONENT_SET_MANAGER_H