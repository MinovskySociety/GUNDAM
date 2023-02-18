#ifndef _GUNDAM_UTIL_ENUMERATE_SUBSET_H
#define _GUNDAM_UTIL_ENUMERATE_SUBSET_H

#include <vector>

namespace GUNDAM {

namespace _enumerate_subset {

template <typename  ElementType,
          typename CallbackType>
bool Search(const std::vector<ElementType>& set,
                  std::vector<ElementType>& subset,
                                    size_t& subset_count, size_t low,
                        const CallbackType& callback) {

  assert(low >= 0 && low <= set.size());

  if (low == set.size()) {
    if (subset.empty()) [[unlikely]] {
      return true;
    }
    // found a subset
    subset_count++;
    return callback(subset);
  }
  // include k in the subset
  subset.emplace_back(set[low]);
  if (!Search(set, subset, subset_count, low + 1, callback)) {
    return false;
  }
  subset.pop_back();
  // don't include k in the subset
  return Search(set, subset, subset_count, low + 1, callback);
}

}; // namespace _enumerate_subset

// wenzhi: optimize me!
template <typename  ElementType,
          typename CallbackType>
size_t EnumerateSubset(const std::vector<ElementType>& set,
                       const CallbackType& callback) {
  std::vector<ElementType> subset;
  subset.reserve(set.size());
  size_t subset_count = 0;
  _enumerate_subset::Search(set, subset, subset_count, 0, callback);
  return subset_count;
}

};

#endif // _GUNDAM_UTIL_ENUMERATE_SUBSET_H