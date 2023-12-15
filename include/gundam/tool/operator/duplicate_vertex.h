#ifndef _GUNDAM_TOOL_DUPLICATE_VERTEX_H
#define _GUNDAM_TOOL_DUPLICATE_VERTEX_H

#include "gundam/type_getter/vertex_id.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_id.h"
#include "gundam/tool/max_id.h"

namespace GUNDAM {  

namespace _duplicate_vertex {

template <bool copy_edge = true,
          typename GraphType>
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

  if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
    for (auto attr_it = vertex_handle_to_duplicate->AttributeBegin();
             !attr_it.IsDone(); 
              attr_it++){
      auto [attr_handle, attr_ret] 
        = new_vertex_handle->AddAttribute(attr_it->key(), 
                                          attr_it->value_type(), 
                                          attr_it->value_str());
      assert(!attr_handle.IsNull());
      assert(attr_ret);
    }
  }

  if constexpr (!copy_edge) {
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

    if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
      for (auto attr_it = edge_it->AttributeBegin();
               !attr_it.IsDone(); 
                attr_it++){
        auto [attr_handle, attr_ret] 
            = edge_handle->AddAttribute(attr_it->key(), 
                                        attr_it->value_type(), 
                                        attr_it->value_str());
        assert(!attr_handle.IsNull());
        assert(attr_ret);
      }
    }
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

    if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
      for (auto attr_it = edge_it->AttributeBegin();
               !attr_it.IsDone(); 
                attr_it++){
        auto [attr_handle, attr_ret] 
            = edge_handle->AddAttribute(attr_it->key(), 
                                        attr_it->value_type(), 
                                        attr_it->value_str());
        assert(!attr_handle.IsNull());
        assert(attr_ret);
      }
    }
  }

  assert( vertex_handle_to_duplicate->CountOutEdge()
                == new_vertex_handle->CountOutEdge() );

  assert( vertex_handle_to_duplicate->CountInEdge()
                == new_vertex_handle->CountInEdge() );

  new_vertex_id++;
  return new_vertex_handle;
}

}; // _duplicate_vertex

template <bool copy_edge = true,
          typename GraphType>
inline auto DuplicateVertex(GraphType& graph,
const typename VertexHandle<GraphType>::type& vertex_handle_to_duplicate) {
  assert(graph.FindVertex( vertex_handle_to_duplicate->id() )
                        == vertex_handle_to_duplicate );

  auto new_vertex_id = MaxVertexID(graph) + 1;
  auto new_edge_id   = MaxEdgeID(graph) + 1;

  return _duplicate_vertex
       ::_DuplicateVertex<copy_edge>(graph, vertex_handle_to_duplicate,
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

#endif // _GUNDAM_TOOL_DUPLICATE_VERTEX_H