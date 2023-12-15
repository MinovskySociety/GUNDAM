#ifndef _GUNDAM_TOOL_K_HOP_H
#define _GUNDAM_TOOL_K_HOP_H

#include "gundam/algorithm/bfs.h"

#include "gundam/type_getter/graph_parameter_getter.h"
#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {

template <bool bidirectional = true,
          typename GraphType>
std::vector<typename GUNDAM::VertexID<GraphType>::type> 
  KHopVertexId(GraphType& graph,
  const std::vector<typename GUNDAM::VertexID<GraphType>::type>& src_vertex_id_set,
             size_t k){

  using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;

  using VertexIDType = typename GUNDAM::VertexID<GraphType>::type;

  std::vector<VertexIDType> k_hop_vertex_id;

  auto hop_callback = [&k_hop_vertex_id
                      #ifndef NDEBUG
                      ,&k
                      #endif
                      ](
        VertexHandleType vertex_handle,
        uint32_t bfs_idx,
        uint32_t distance
      ){
    assert(distance <= k);

    k_hop_vertex_id.emplace_back(vertex_handle->id());
    
    return true;
  };

  GUNDAM::Bfs<bidirectional>(graph, src_vertex_id_set, hop_callback, k);

  return k_hop_vertex_id;
}

template <bool bidirectional = true,
          typename GraphType>
std::vector<typename GUNDAM::VertexHandle<GraphType>::type> 
  KHopVertex(GraphType& graph,
  const std::vector<typename GUNDAM::VertexID<GraphType>::type>& src_vertex_id_set,
             size_t k){

  using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;

  std::vector<VertexHandleType> k_hop_vertex;

  auto hop_callback = [&k_hop_vertex
                      #ifndef NDEBUG
                      ,&k
                      #endif
                      ](
        VertexHandleType vertex_handle,
        uint32_t bfs_idx,
        uint32_t distance
      ){
    assert(distance <= k);

    k_hop_vertex.emplace_back(vertex_handle);
    
    return true;
  };

  GUNDAM::Bfs<bidirectional>(graph, src_vertex_id_set, hop_callback, k);

  return k_hop_vertex;
}

template <bool bidirectional = true,
          typename GraphType>
GraphType KHop(GraphType& graph,
         const std::set<typename GUNDAM::VertexHandle<GraphType>::type>& src_handle_set,
               size_t k){

  using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;

  GraphType k_hop;

  auto hop_callback = [&k_hop
                      #ifndef NDEBUG
                      ,&k
                      #endif
                      ](
        VertexHandleType vertex_handle,
        uint32_t bfs_idx,
        uint32_t distance
      ){
    assert(distance <= k);

    auto [new_vertex_handle, ret] = k_hop.AddVertex(vertex_handle->id(),
                                                    vertex_handle->label());
    assert(new_vertex_handle);

    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      // add attribute to new_vertex
      for (auto attr_it = vertex_handle->AttributeBegin();
               !attr_it.IsDone();
                attr_it++){
        auto [attr_handle, ret] = new_vertex_handle->AddAttribute(
                                        attr_it->key(),
                                        attr_it->value_type(),
                                        attr_it->value_str());
      }
    }

    for (auto out_edge_it = vertex_handle->OutEdgeBegin();
             !out_edge_it.IsDone(); 
              out_edge_it++) {
      auto k_hop_vertex_handle 
         = k_hop.FindVertex(out_edge_it->dst_handle()->id());
      if (!k_hop_vertex_handle) {
        // the dst vertex does not exist in the k_hop, 
        // this edge does not need to be added into k_hop
        continue;
      }
      // this vertex should already be contained in sub graph
      auto [k_hop_edge_handle, ret] 
          = k_hop.AddEdge(out_edge_it->src_handle()->id(),
                          out_edge_it->dst_handle()->id(),
                          out_edge_it->label(),
                          out_edge_it->id());
      assert(k_hop_edge_handle);
      if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
        for (auto attr_it = out_edge_it->AttributeBegin();
                 !attr_it.IsDone();
                  attr_it++){
          auto [attr_handle, ret] = k_hop_edge_handle->AddAttribute(
                                          attr_it->key(),
                                          attr_it->value_type(),
                                          attr_it->value_str());
        }
      }
    }

    for (auto in_edge_it = vertex_handle->InEdgeBegin();
             !in_edge_it.IsDone(); 
              in_edge_it++) {
      auto k_hop_vertex_handle 
         = k_hop.FindVertex(in_edge_it->src_handle()->id());
      if (!k_hop_vertex_handle) {
        // the dst vertex does not exist in the k_hop, 
        // this edge does not need to be added into k_hop
        continue;
      }
      // this vertex should already be contained in sub graph
      auto [k_hop_edge_handle, ret] 
          = k_hop.AddEdge(in_edge_it->src_handle()->id(),
                          in_edge_it->dst_handle()->id(),
                          in_edge_it->label(),
                          in_edge_it->id());
      assert(k_hop_edge_handle);
      if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
        for (auto attr_it = in_edge_it->AttributeBegin();
                 !attr_it.IsDone();
                  attr_it++){
          auto [attr_handle, ret] = k_hop_edge_handle->AddAttribute(
                                          attr_it->key(),
                                          attr_it->value_type(),
                                          attr_it->value_str());
        }
      }
    }
    return true;
  };

  GUNDAM::Bfs<bidirectional>(graph, src_handle_set, hop_callback, k);

  return k_hop;
}

template <bool bidirectional = true,
          typename GraphType>
GraphType KHop(GraphType& graph,
  typename VertexHandle<GraphType>::type src_handle,
               size_t k){

  const std::set<typename GUNDAM::VertexHandle<GraphType>::type>
    src_handle_set = {src_handle};

  return KHop<bidirectional>(graph, src_handle_set, k);
}

}; // namespace GUNDAM

#endif // _GUNDAM_TOOL_K_HOP_H
