#ifndef _GUNDAM_TOOL_MAP_EDGE_TO_H
#define _GUNDAM_TOOL_MAP_EDGE_TO_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

#include "gundam/match/match.h"

namespace GUNDAM {

// since there could be duplicated edges in the data 
// graph and one edge in graph pattern can therefore
// match to a set of edges in data graph, this method
// is not added in the Match class
template <typename GraphPatternType,
          typename    DataGraphType>
inline typename EdgeHandle<DataGraphType>::type MapEdgeTo(
       const Match<GraphPatternType,
                      DataGraphType>& match,
   const typename EdgeHandle<GraphPatternType>::type& edge_handle) {

  using GraphPatternEdgeHandleType 
         = typename EdgeHandle<GraphPatternType>::type;
  using GraphPatternVertexHandleType 
         = typename VertexHandle<GraphPatternType>::type;
  
  using DataGraphEdgeHandleType 
      = typename EdgeHandle<DataGraphType>::type;
  using DataGraphVertexHandleType 
      = typename VertexHandle<DataGraphType>::type;

  // cannot find the vertex corresponds to the src_handle of the input edge
  DataGraphVertexHandleType data_graph_src_handle
            = match.MapTo(edge_handle->src_handle());
  if (!data_graph_src_handle) {
    // match does not have match for the src_handle
    assert(!match.HasMap(edge_handle->src_handle()));
    assert(!match.HasMap(edge_handle->src_handle()->id()));
    assert(!DataGraphEdgeHandleType());
    // return null edge handle
    return DataGraphEdgeHandleType();
  }

  // cannot find the vertex corresponds to the dst_handle of the input edge
  DataGraphVertexHandleType data_graph_dst_handle 
            = match.MapTo(edge_handle->dst_handle());
  if (!data_graph_dst_handle) {
    // match does not have match for the src_handle
    assert(!match.HasMap(edge_handle->dst_handle()));
    assert(!match.HasMap(edge_handle->dst_handle()->id()));
    assert(!DataGraphEdgeHandleType());
    // return null edge handle
    return DataGraphEdgeHandleType();
  }

  for (auto out_edge_it = data_graph_src_handle->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    if (out_edge_it->dst_handle() 
       != data_graph_dst_handle) {
      continue;
    }
    if (out_edge_it->label() != edge_handle->label()) {
      continue;
    }
    // have found this edge 
    return out_edge_it;
  }

  // should have found that edge, otherwise this match is illegal
  assert(false);
  return DataGraphEdgeHandleType();
}

}; // namespace GUNDAM

#endif // _GUNDAM_TOOL_MAP_EDGE_TO_H