#ifndef _GUNDAM_UTIL_ENUMERATE_SUBSET_H
#define _GUNDAM_UTIL_ENUMERATE_SUBSET_H

#include <vector>

namespace GUNDAM {

namespace _enumerate_subset {

template <bool specified_element_num,
          typename  ElementType,
          typename CallbackType>
bool Search(const std::vector<ElementType>& set,
                  std::vector<ElementType>& subset,
                                    size_t& subset_count, size_t low, size_t element_num,
                        const CallbackType& callback) {

  assert(low >= 0 && low <= set.size());

  if (low == set.size()) {
    if constexpr (specified_element_num) {
      if (subset.size() != element_num) {
        // continue searching
        return true;
      }
    }

    if (subset.empty()) [[unlikely]] {
      return true;
    }
    // found a subset
    subset_count++;
    return callback(subset);
  }
  if constexpr (!specified_element_num) {
    // include k in the subset
    subset.emplace_back(set[low]);
    if (!Search<specified_element_num>(set, subset, subset_count, low + 1, element_num, callback)) {
      return false;
    }
    subset.pop_back();
    // don't include k in the subset
    return Search<specified_element_num>(set, subset, subset_count, low + 1, element_num, callback);
  }
  else {
    if (subset.size() < element_num) {
      // include k in the subset
      subset.emplace_back(set[low]);
      if (!Search<specified_element_num>(set, subset, subset_count, low + 1, element_num, callback)) {
        return false;
      }
      subset.pop_back();
    }
    // don't include k in the subset
    return Search<specified_element_num>(set, subset, subset_count, low + 1, element_num, callback);
  }
}

}; // namespace _enumerate_subset


template <bool ordered_by_element_num = false,
          typename  ElementType,
          typename CallbackType>
size_t EnumerateSubset(const std::vector<ElementType>& set,
                       const CallbackType& callback) {
  std::vector<ElementType> subset;
  subset.reserve(set.size());
  size_t subset_count = 0;
  if constexpr (!ordered_by_element_num) {
    _enumerate_subset::Search<false>(set, subset, subset_count, 0, set.size(), callback);
    assert(subset.empty());
  }
  else {

    for (int element_num = 1; element_num <= set.size(); element_num++) {
      assert(subset.empty());
      _enumerate_subset::Search<true>(set, subset, subset_count, 0, element_num, callback);
      assert(subset.empty());
    }
  }
  return subset_count;
}

};

#endif // _GUNDAM_UTIL_ENUMERATE_SUBSET_H