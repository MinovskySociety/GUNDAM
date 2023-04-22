#ifndef _GUNDAM_TOOL_TOPOLOGICAL_CONSTRAINTS_HAS_DUPLICATED_EDGE_H
#define _GUNDAM_TOOL_TOPOLOGICAL_CONSTRAINTS_HAS_DUPLICATED_EDGE_H

#include "gundam/type_getter/vertex_id.h"

namespace GUNDAM {

// return true if there are multiple edges between a same 
// pair of vertexes
// if directed is marked as true, then there is allowed to
// has edge with different directions
template <bool directed = false,
          typename GraphType>
inline bool HasDuplicatedEdge(const GraphType& graph) {
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    std::unordered_set<typename VertexID<GraphType>::type> connected_vertex_id_set;

    for (auto out_edge_it = vertex_it->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      auto [it, ret] = connected_vertex_id_set.emplace(out_edge_it->dst_handle()->id());

      if (ret) {
        // added successfully, such a vertex has not been considered before
        continue;
      }
      // fail to add to set, exist duplicated edge, return true
      return true;
    }

    if constexpr (directed) {
      connected_vertex_id_set.clear();
    }

    for (auto in_edge_it = vertex_it->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      auto [it, ret] = connected_vertex_id_set.emplace( in_edge_it->src_handle()->id());

      if (ret) {
        // added successfully, such a vertex has not been considered before
        continue;
      }
      // fail to add to set, exist duplicated edge, return true
      return true;
    }
  }
  return false;
}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_CONSTRAINTS_HAS_DUPLICATED_EDGE_H