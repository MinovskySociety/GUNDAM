#ifndef _GUNDAM_UTIL_COMBINATION_H
#define _GUNDAM_UTIL_COMBINATION_H

#include <algorithm>
#include <vector>

namespace GUNDAM {

template<typename ElementType>
std::vector<
std::vector<ElementType>> Combination(
    const std::vector<ElementType>& unified_set,
    const size_t number_to_select) {
  std::vector<
  std::vector<ElementType>> combination_set;

  std::vector<bool> bitmask(number_to_select, 1); // K leading 1's
  bitmask.resize(unified_set.size(), 0);          // N-K trailing 0's

  // print integers and permute bitmask
  do {
      std::vector<ElementType> combination;
      combination.reserve(number_to_select);

      for (int i = 0; i < unified_set.size(); i++) { // [0..N-1] integers
          if (!bitmask[i]) {
            continue;
          }
          combination.emplace_back(unified_set[i]);
      }
      combination_set.emplace_back(std::move(combination));

  } while (std::prev_permutation(bitmask.begin(), bitmask.end()));


  return combination_set;
}

}; // namespace GUNDAM

#endif // _GUNDAM_UTIL_COMBINATION_H