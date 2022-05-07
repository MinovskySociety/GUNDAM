#ifndef _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H
#define _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/util/util.h"

namespace GUNDAM {

enum class FilterType : uint8_t {
  kHigherOrEqualTo = 0,
  kHigherThan      = 1,
  kEqualTo         = 2,
  kLessOrEqualTo   = 3,
  kLessThan        = 4
};

template <enum FilterType filter_type,
          enum EdgeDirection edge_direction,
          class GraphType>
std::vector<typename VertexHandle<GraphType>::type>  // okey to return vector after C11
  VertexDegreeFilter(GraphType& graph, size_t degree) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::vector<VertexHandleType> vertex_handle_set(graph.CountVertex());
  size_t vertex_counter = 0;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    size_t degree_counter = 0;
    if constexpr (edge_direction == EdgeDirection::kOut
               || edge_direction == EdgeDirection::kInOut) {
      degree_counter += vertex_it->CountOutEdge();
    }
    if constexpr (edge_direction == EdgeDirection::kIn
               || edge_direction == EdgeDirection::kInOut) {
      degree_counter += vertex_it->CountInEdge();
    }
    // removed a if here 
    vertex_handle_set[vertex_counter] = vertex_it;
    bool preserve_this_vertex = false;
    if constexpr (filter_type == FilterType::kHigherOrEqualTo) {
      preserve_this_vertex = (degree_counter >= degree);
    }
    else if constexpr (filter_type == FilterType::kHigherThan) {
      preserve_this_vertex = (degree_counter >  degree);
    }
    else if constexpr (filter_type == FilterType::kEqualTo) {
      preserve_this_vertex = (degree_counter == degree);
    }
    else if constexpr (filter_type == FilterType::kLessOrEqualTo) {
      preserve_this_vertex = (degree_counter <= degree);
    }
    else {
      static_assert(filter_type == FilterType::kLessThan, "un-supported filter type");
      preserve_this_vertex = (degree_counter <  degree);
    }
    vertex_counter += preserve_this_vertex? 1: 0;
  }
  vertex_handle_set.resize(vertex_counter);
  return vertex_handle_set;
}

};

#endif // _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H