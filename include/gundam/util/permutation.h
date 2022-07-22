#ifndef _GUNDAM_UTIL_PERMUTATION_H
#define _GUNDAM_UTIL_PERMUTATION_H

#include <numeric>
#include <algorithm>
#include <vector>

namespace GUNDAM {

template<typename ElementType>
std::vector<
std::vector<ElementType>> Permutation(
    const std::vector<ElementType>& unified_set,
    const size_t number_to_select) {
  std::vector<
  std::vector<ElementType>> permutation_set;

  std::vector<int> d(unified_set.size());
  std::iota(d.begin(), d.end(), 1);

  do {
    std::vector<ElementType> permutation;
    permutation.reserve(number_to_select);
    for (int i = 0; i < number_to_select; i++) {
      permutation.emplace_back(unified_set[d[i] - 1]);
    }
    permutation_set.emplace_back(std::move(permutation));
    std::reverse(d.begin() + number_to_select, 
                 d.end());
  } while (std::next_permutation(d.begin(),d.end()));

  return permutation_set;
}

}; // namespace GUNDAM

#endif // _GUNDAM_UTIL_PERMUTATION_H