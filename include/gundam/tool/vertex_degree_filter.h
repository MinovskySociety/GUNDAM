#ifndef _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H
#define _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H

#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {

template <bool bidirectional = false,
          class GraphType>
std::vector<typename VertexHandle<GraphType>::type>  // okey to return vector after C11
  VertexDegreeFilter(GraphType& graph, size_t degree) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::vector<VertexHandleType> vertex_handle_set(graph.CountVertex());
  size_t vertex_counter = 0;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    size_t degree_counter = vertex_it->CountOutEdge();
    if constexpr (bidirectional) {
      degree_counter += vertex_it->CountInEdge();
    }
    // removed a if here 
    vertex_handle_set[vertex_counter] = vertex_it;
    vertex_counter += (degree_counter >= degree)? 1: 0;
  }
  vertex_handle_set.resize(vertex_counter);
  return vertex_handle_set;
}

};

#endif // _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H