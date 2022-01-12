#ifndef _GUNDAM_IS_STAR_H
#define _GUNDAM_IS_STAR_H

#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {

template<bool bidirectional = false,
         typename GraphType>
std::pair<std::vector<typename VertexHandle<GraphType>::type>, // set of end points, return an empty vector if this not a star
                                                               // okey to return STL container after c11
                      typename VertexHandle<GraphType>::type>  // the central vertex, if this pattern is a star with more than two paths, else return nullptr
              StarEndPoints(GraphType& graph) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::vector<VertexHandleType> end_points;
  /* ############################# *
   * #                           # *
   * #    complete logic here    # *
   * #                           # *
   * ############################# */

  // fail to find the end point, is not a star
  return std::pair(end_points, VertexHandleType());
}

template<bool bidirectional = false,
         typename GraphType> 
inline bool IsStar(GraphType& graph) {
  const auto [end_points,
              central_vertex] = StarEndPoints<bidirectional>(graph);
  if (!end_points.empty()) {
    assert(end_points.size() >= 2);
    assert(end_points.size() == 2 || central_vertex);
    return true;
  }
  assert(!central_vertex);
  return false;
}

};

#endif // _GUNDAM_IS_STAR_H