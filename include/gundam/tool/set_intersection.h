#ifndef _GUNDAM_TOOL_SET_INTERSECTION_H
#define _GUNDAM_TOOL_SET_INTERSECTION_H
#include <iterator>
#include <set>
namespace GUNDAM {
constexpr int set_intersction_threhold = 32;
template <class Container, class OutputIt>
OutputIt SetIntersectionByBinarySearch(Container &a, Container &b,
                                       OutputIt output) {
  /* a.size is smaller than b.size */
  for (auto &it : a) {
    if (std::binary_search(std::begin(b), std::end(b), it)) {
      *output++ = it;
    }
  }
  return output;
}
// adaptive set intersection
template <class Container, class OutputIt>
OutputIt SetIntersection(Container &a, Container &b, OutputIt output) {
  auto a_size = a.size();
  auto b_size = b.size();
  if (std::max(a_size, b_size) >
      set_intersction_threhold * std::min(a.size(), b.size())) {
    /* using binary search */
    return SetIntersectionByBinarySearch(a.size() <= b.size() ? a : b,
                                         a.size() > b.size() ? a : b, output);
  } else {
    /* using linear merge */
    return std::set_intersection(std::begin(a), std::end(a), std::begin(b),
                                 std::end(b), output);
  }
}

/* TODO: simd set intersection when inner id can use */

}  // namespace GUNDAM

#endif