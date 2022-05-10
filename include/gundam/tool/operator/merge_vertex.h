#ifndef _GUNDAM_TOOL_OPERATOR_MERGE_VERTEX_H
#define _GUNDAM_TOOL_OPERATOR_MERGE_VERTEX_H

#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"
#include "gundam/tool/max_id.h"

#include <vector>
#include <set>
#include <map>
#include <iostream>

namespace GUNDAM {

/* ##################################################### *
 * ##  merge the input set of graph, join the set of  ## *
 * ##  vertex with given id                           ## *
 * ##################################################### */
template <typename GraphType>
inline auto MergeVertex(GraphType& input_graph,
  const 
  std::vector<
  std::vector<
  typename VertexHandle<GraphType>::type>>& set_of_vertex_set_to_merge) {

  if (set_of_vertex_set_to_merge.empty()) {
    return GraphType();
  }

  using VertexHandleType = typename VertexHandle<GraphType>::type;

  GraphType merged_graph(input_graph);

  for (const auto& vertex_set_to_merge
          : set_of_vertex_set_to_merge) {
    assert(vertex_set_to_merge.size() >= 1);
     auto  merge_to_vertex = merged_graph.FindVertex(vertex_set_to_merge[0]->id());
    assert(merge_to_vertex); // should have this vertex
     auto  vertex_set_to_it = vertex_set_to_merge.begin();
    assert(vertex_set_to_it
        != vertex_set_to_merge.end());
    vertex_set_to_it++;
    for (;vertex_set_to_it != vertex_set_to_merge.end();
          vertex_set_to_it++) {
      auto  vertex_to_erase = *vertex_set_to_it;
      // does not support merge vertex attribute now
      if constexpr (GUNDAM::GraphParameter<GraphType>::vertex_has_attribute) {
        assert(vertex_to_erase->AttributeBegin().IsDone());
      }
      merged_graph.EraseVertex(vertex_to_erase->id());
      assert(merge_to_vertex->label() 
          == vertex_to_erase->label());
      assert(vertex_to_erase);
      for (auto out_edge_it = vertex_to_erase->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        if (out_edge_it->src_handle()->id()
         == out_edge_it->dst_handle()->id()) {
          // self loop
          auto [ edge_handle,
                 edge_ret ] = merged_graph.AddEdge(merge_to_vertex->id(),
                                                   merge_to_vertex->id(),
                                                       out_edge_it->label(),
                                                       out_edge_it->id());
          assert(edge_ret);
          if constexpr (GUNDAM::GraphParameter<GraphType>::edge_has_attribute) {
            CopyAllAttributes(out_edge_it, edge_handle);
          }
          continue;
        }
        auto [ edge_handle,
               edge_ret ] = merged_graph.AddEdge(merge_to_vertex->id(),
                                       out_edge_it->dst_handle()->id(),
                                       out_edge_it->label(),
                                       out_edge_it->id());
        assert(edge_ret);
        if constexpr (GUNDAM::GraphParameter<GraphType>::edge_has_attribute) {
          CopyAllAttributes(out_edge_it, edge_handle);
        }
      }
      for (auto in_edge_it = vertex_to_erase->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        auto [ edge_handle,
               edge_ret ] = merged_graph.AddEdge(in_edge_it->src_handle()->id(),
                                                          merge_to_vertex->id(),
                                                 in_edge_it->label(),
                                                 in_edge_it->id());
        assert(edge_ret);
        if constexpr (GUNDAM::GraphParameter<GraphType>::edge_has_attribute) {
          CopyAllAttributes(in_edge_it, edge_handle);
        }
      }
    }
  }
  return merged_graph;
}

template <typename GraphType>
inline auto MergeVertex(GraphType& input_graph,
  const std::vector<
  typename VertexHandle<GraphType>::type>& vertex_set_to_merge) {

  const 
  std::vector<
  std::vector<
  typename VertexHandle<GraphType>::type>> set_of_vertex_set_to_merge 
                                               = {vertex_set_to_merge};

  return MergeVertex(input_graph, set_of_vertex_set_to_merge);
}

}
#endif // _GUNDAM_TOOL_OPERATOR_MERGE_VERTEX_H
