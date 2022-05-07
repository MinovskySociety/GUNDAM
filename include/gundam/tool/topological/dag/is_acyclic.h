#ifndef _GUNDAM_TOOL_TOPOLOGICAL_DAG_IS_ACYCLIC_H
#define _GUNDAM_TOOL_TOPOLOGICAL_DAG_IS_ACYCLIC_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_id.h"
#include <unordered_map>
#include <queue>

namespace GUNDAM {

template <typename GraphType>
inline bool IsAcyclic(const GraphType& graph) {
  using VertexHandleType  = typename VertexHandle<const GraphType>::type;
  using VertexIDType      = typename VertexID   <       GraphType>::type;
  using VertexCounterType = size_t;
  if (graph.CountVertex() == 0
   || graph.CountEdge()   == 0) {
    return false;
  }
  std::unordered_map<VertexIDType,
                     VertexCounterType> in_degree_counter;
  in_degree_counter.reserve(graph.CountVertex());
  std::queue<VertexHandleType> queue_of_leaf;
  VertexCounterType removed_vertex_size = 0;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    if (vertex_it->CountInEdge() == 0) {
      removed_vertex_size++;
      queue_of_leaf.emplace(vertex_it);
      continue;
    }
    in_degree_counter.emplace(vertex_it->id(),
                              vertex_it->CountInEdge());
  }
  while (!queue_of_leaf.empty()) {
    auto leaf_handle = queue_of_leaf.front();
    queue_of_leaf.pop();
    for (auto out_edge_it = leaf_handle->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
       auto  in_degree_it  = in_degree_counter.find(out_edge_it->dst_handle()->id());
      assert(in_degree_it != in_degree_counter.end());
      assert(in_degree_it->second > 0);
      in_degree_it->second--;
      if (in_degree_it->second != 0) {
        continue;
      }
      // is a leaf now, add to queue
      queue_of_leaf.emplace(out_edge_it->dst_handle());
      removed_vertex_size++;
    }
  }
  if (removed_vertex_size == graph.CountVertex()) {
    return true;
  }
  return false;
}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_DAG_IS_ACYCLIC_H