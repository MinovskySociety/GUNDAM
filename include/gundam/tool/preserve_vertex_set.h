#ifndef _GUNDAM_TOOL_PRESERVE_VERTEX_SET_H
#define _GUNDAM_TOOL_PRESERVE_VERTEX_SET_H

#include <vector>
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

namespace GUNDAM {

// construct a 
template <typename GraphType>
inline GraphType PreserveVertexSet(GraphType& data_graph,
    const std::vector<typename GUNDAM::VertexHandle<GraphType>::type>& vertex_set_to_preserve) {

  using VertexCounterType = typename GraphType::VertexCounterType;
  using DataGraphVertexHandle = typename VertexHandle<GraphType>::type;

  GraphType substructure;

  auto prune_not_in_substructure_callback 
    = [](const DataGraphVertexHandle vertex_handle){
    return false;
  };

  auto construct_substructure_callback 
    = [&substructure](DataGraphVertexHandle vertex_handle){
    auto [substructure_vertex_handle,
          substructure_vertex_ret]
        = substructure.AddVertex(vertex_handle->id(),
                                 vertex_handle->label());
    // this vertex should not have been added 
    // into the substructure before
    assert(substructure_vertex_ret);
    assert(substructure_vertex_handle);

    GUNDAM::CopyAllAttributes(vertex_handle, 
                  substructure_vertex_handle);

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
      if constexpr () {
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
      GUNDAM::CopyAllAttributes(in_edge_it, 
                      substructure_edge_handle);
    }
    return true;
  };

  VertexCounterType vertex_num = 0;
  vertex_num += GUNDAM::Dfs<true>(data_graph, vertex_set_to_preserve.front(),
                                     construct_substructure_callback,
                                  prune_not_in_substructure_callback);
  assert(vertex_num == substructure.CountVertex());
  return substructure;
}

};

#endif // _GUNDAM_TOOL_PRESERVE_VERTEX_SET_H