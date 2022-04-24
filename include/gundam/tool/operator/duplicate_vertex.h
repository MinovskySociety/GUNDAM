#ifndef _GUNDAM_TOOL_OPERATOR_MERGE_BY_ID_H
#define _GUNDAM_TOOL_OPERATOR_MERGE_BY_ID_H

#include "gundam/type_getter/vertex_id.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_id.h"
#include "gundam/tool/max_id.h"

namespace GUNDAM {  

namespace _duplicate_vertex {

template <typename GraphType>
inline auto _DuplicateVertex(GraphType& graph,
 const typename VertexHandle<GraphType>::type&     vertex_handle_to_duplicate,
       typename VertexID    <GraphType>::type& new_vertex_id,
       typename   EdgeID    <GraphType>::type& new_edge_id) {
  
  auto [ new_vertex_handle,
         new_vertex_ret ] = graph.AddVertex(new_vertex_id,
                                                vertex_handle_to_duplicate->label());

  if (!new_vertex_handle) {
    return new_vertex_handle;
  }

  for (auto edge_it = vertex_handle_to_duplicate->OutEdgeBegin();
           !edge_it.IsDone();
            edge_it++) {
    auto [edge_handle,
          edge_ret] = graph.AddEdge(new_vertex_id,
                                    edge_it->dst_handle()->id(),
                                    edge_it->label(),
                                    new_edge_id++);
    assert(edge_handle);
    assert(edge_ret);
  }

  for (auto edge_it = vertex_handle_to_duplicate->InEdgeBegin();
           !edge_it.IsDone();
            edge_it++) {
    auto [edge_handle,
          edge_ret] = graph.AddEdge(edge_it->src_handle()->id(),
                                    new_vertex_id,
                                    edge_it->label(),
                                    new_edge_id++);
    assert(edge_handle);
    assert(edge_ret);
  }

  assert( vertex_handle_to_duplicate->CountOutEdge()
                == new_vertex_handle->CountOutEdge() );

  assert( vertex_handle_to_duplicate->CountInEdge()
                == new_vertex_handle->CountInEdge() );

  new_vertex_id++;
  return new_vertex_handle;
}

}; // _duplicate_vertex

template <typename GraphType>
inline bool DuplicateVertex(GraphType& graph,
const typename VertexHandle<GraphType>::type& vertex_handle_to_duplicate) {
  assert(graph.FindVertex( vertex_handle_to_duplicate->id() )
                        == vertex_handle_to_duplicate );

  auto new_vertex_id = MaxVertexID(graph) + 1;
  auto new_edge_id   = MaxEdgeID(graph) + 1;

  return _duplicate_vertex
       ::_DuplicateVertex(graph, vertex_handle_to_duplicate,
                          new_vertex_id,
                          new_edge_id);
}

template <typename GraphType>
inline auto DuplicateVertex(GraphType& graph,
      const std::vector<
      typename VertexHandle<GraphType>::type>& vertex_handle_set_to_duplicate) {

  auto new_vertex_id = MaxVertexId(graph) + 1;
  auto new_edge_id   = MaxEdgeId  (graph) + 1;

  using VertexHandleType = typename VertexHandle<GraphType>::type;

  std::vector<std::pair<VertexHandleType,
                        VertexHandleType>> vertex_id_map;
  vertex_id_map.reserve(vertex_handle_set_to_duplicate.size());

  for (const auto& vertex_handle_to_duplicate
                 : vertex_handle_set_to_duplicate) {
    assert(graph.FindVertex( vertex_handle_to_duplicate->id() )
                          == vertex_handle_to_duplicate );
    auto vertex_handle = _duplicate_vertex
                       ::_DuplicateVertex(graph, vertex_handle_to_duplicate,
                                          new_vertex_id,
                                          new_edge_id);
    if (vertex_handle) {
      vertex_id_map.emplace_back(vertex_handle_to_duplicate,
                                 vertex_handle);
      continue;
    }
    return std::vector<std::pair<VertexHandleType,
                                 VertexHandleType>>{};
  }
  return vertex_id_map;
}

};

#endif // _GUNDAM_TOOL_OPERATOR_MERGE_BY_ID_H