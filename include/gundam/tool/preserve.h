#ifndef _GUNDAM_TOOL_PRESERVE_VERTEX_SET_H
#define _GUNDAM_TOOL_PRESERVE_VERTEX_SET_H

#include <vector>

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/edge_id.h"

#include "gundam/algorithm/dfs.h"

namespace GUNDAM {

// to construct a subgraph from input graph
// and 
template <typename GraphType>
inline GraphType PreserveVertexSet(GraphType& graph,
    const std::vector<typename VertexHandle<GraphType>::type>& vertex_set_to_preserve) {

  using VertexCounterType = typename  GraphType::VertexCounterType;
  using  VertexHandleType = typename VertexHandle<GraphType>::type;

  GraphType substructure;

  auto prune_nothing_callback 
    = [](const VertexHandleType vertex_handle){
    // prune nothing
    return false;
  };

  auto construct_substructure_callback 
    = [&substructure,
       &vertex_set_to_preserve](const VertexHandleType& vertex_handle){
    if (!std::binary_search(vertex_set_to_preserve.begin(),
                            vertex_set_to_preserve.end(),
                            vertex_handle)) {
      // this vertex is not contained in the vertex_handle_set
      // does not need to be preserved
      return true;
    }
    // this vertex is contained in the vertex_handle_set
    // needs to be preserved
    auto [substructure_vertex_handle,
          substructure_vertex_ret]
        = substructure.AddVertex(vertex_handle->id(),
                                 vertex_handle->label());
    // this vertex should not have been added 
    // into the substructure before
    assert(substructure_vertex_ret);
    assert(substructure_vertex_handle);

    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      GUNDAM::CopyAllAttributes(vertex_handle, 
                   substructure_vertex_handle);
    }

    for (auto out_edge_it = vertex_handle->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      auto substructure_vertex_handle
         = substructure.FindVertex(out_edge_it->dst_handle()->id());
      if (!substructure_vertex_handle){
        // this vertex cannot be found in substructure
        // this edge does not need to be added
        continue;
      }
      // this edge needs to be added
      auto [substructure_edge_handle, 
            substructure_edge_ret]
          = substructure.AddEdge(out_edge_it->src_handle()->id(),
                                 out_edge_it->dst_handle()->id(),
                                 out_edge_it->label(),
                                 out_edge_it->id());
      // should added successfully
      assert(substructure_edge_handle);
      assert(substructure_edge_ret);
      if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
        GUNDAM::CopyAllAttributes(out_edge_it, 
                         substructure_edge_handle);
      }
    }

    for (auto in_edge_it = vertex_handle->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      auto substructure_vertex_handle
         = substructure.FindVertex(in_edge_it->src_handle()->id());
      if (!substructure_vertex_handle){
        // this vertex cannot be found in substructure
        // this edge does not need to be added
        continue;
      }
      // this edge needs to be added
      auto [substructure_edge_handle, 
            substructure_edge_ret]
          = substructure.AddEdge(in_edge_it->src_handle()->id(),
                                 in_edge_it->dst_handle()->id(),
                                 in_edge_it->label(),
                                 in_edge_it->id());
      // should added successfully
      assert(substructure_edge_handle);
      assert(substructure_edge_ret);
      if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
        GUNDAM::CopyAllAttributes(in_edge_it, 
                        substructure_edge_handle);
      }
    }
    return true;
  };

  VertexCounterType vertex_num = 0;
  vertex_num = GUNDAM::Dfs<true>(graph, vertex_set_to_preserve.front(),
                                    construct_substructure_callback,
                                             prune_nothing_callback);
  assert(vertex_num == substructure.CountVertex());
  return substructure;
}

template <typename GraphType>
inline GraphType PreserveEdgeSet(GraphType& graph,
    const std::vector<typename EdgeHandle<GraphType>::type>& edge_set_to_preserve) {
  std::vector<typename EdgeID<GraphType>::type> edge_id_set_to_remove;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      typename EdgeHandle<GraphType>::type edge_handle = out_edge_it;
      if (std::binary_search(edge_set_to_preserve.begin(),
                             edge_set_to_preserve.end(),
                             edge_handle)){
        // this edge does not need to be removed
        continue;
      }
      // this edge needs to be removed
      edge_id_set_to_remove.emplace_back(edge_handle->id());
    }
  }
  GraphType remove_edge_graph(graph);
  for (const auto& edge_id : edge_id_set_to_remove) {
    auto ret = remove_edge_graph.EraseEdge(edge_id);
    assert(ret > 0);
  }
  return remove_edge_graph;
}

template <typename GraphType>
inline GraphType Preserve(GraphType& graph,
    const std::vector<typename VertexHandle<GraphType>::type>& vertex_set_to_preserve,
    const std::vector<typename   EdgeHandle<GraphType>::type>&   edge_set_to_preserve) {
  GraphType removed_edge = (GraphType)PreserveEdgeSet(graph, edge_set_to_preserve);
  return PreserveVertexSet(removed_edge, vertex_set_to_preserve);
}

};

#endif // _GUNDAM_TOOL_PRESERVE_VERTEX_SET_H