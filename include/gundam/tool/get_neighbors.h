#ifndef _GUNDAM_TOOL_GET_NEIGHBORS_H
#define _GUNDAM_TOOL_GET_NEIGHBORS_H

#include "gundam/algorithm/dfs.h"

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_id.h"

namespace GUNDAM {

namespace _get_neighbors {

template <typename ParameterType>
class default_prune_callback {
 public:
  inline bool operator()(const ParameterType&) const {
    return false;
  }
};

};

// ok to return vector after C11
template <typename GraphType, bool bidirectional = false,
          typename PruneCallbackType = _get_neighbors::default_prune_callback<typename VertexHandle<GraphType>::type>>
std::vector<typename VertexHandle<GraphType>::type> 
  GetNeighborVertexSet(const std::vector<typename VertexHandle<GraphType>::type>& source_vertex_set,
                       PruneCallbackType prune_callback = _get_neighbors::default_prune_callback<typename VertexHandle<GraphType>::type>()) {
  
  std::vector<typename VertexHandle<GraphType>::type> neighbors;

  for (const auto& source_vertex 
                 : source_vertex_set) {

    for (auto out_edge_it = source_vertex->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      if (prune_callback(out_edge_it->dst_handle())) {
        continue;
      }
      neighbors.emplace_back(out_edge_it->dst_handle());
    }

    if constexpr (bidirectional)  {
      // consider also in-ward edges
      for (auto in_edge_it = source_vertex->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        if (prune_callback(in_edge_it->src_handle())) {
          continue;
        }
        neighbors.emplace_back(in_edge_it->src_handle());
      }
    }
  }

  std::sort( neighbors.begin(), 
             neighbors.end() );

  neighbors.erase( std::unique( neighbors.begin(),
                                neighbors.end() ),
                                neighbors.end() );
                                
  return neighbors;
}

// ok to return vector after C11
template <typename GraphType, bool bidirectional = false,
          typename PruneCallbackType = _get_neighbors::default_prune_callback<typename EdgeHandle<GraphType>::type>>
std::vector<typename EdgeHandle<GraphType>::type> 
  GetNeighborEdgeSet(const std::vector<typename VertexHandle<GraphType>::type>& source_vertex_set,
                       PruneCallbackType prune_callback = _get_neighbors::default_prune_callback<typename EdgeHandle<GraphType>::type>()) {
  
  std::vector<typename EdgeHandle<GraphType>::type> neighbor_edges;

  for (const auto& source_vertex : source_vertex_set) {

    for (auto out_edge_it = source_vertex->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      if (prune_callback(out_edge_it)) {
        continue;
      }
      neighbor_edges.emplace_back(out_edge_it);
    }

    if constexpr (bidirectional)  {
      // consider also in-ward edges
      for (auto in_edge_it = source_vertex->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        if (prune_callback(in_edge_it)) {
          continue;
        }
        neighbor_edges.emplace_back(in_edge_it);
      }
    }
  }

  std::sort( neighbor_edges.begin(), 
             neighbor_edges.end() );

  neighbor_edges.erase( std::unique( neighbor_edges.begin(),
                                     neighbor_edges.end() ),
                                     neighbor_edges.end() );
                                
  return neighbor_edges;
}

template <typename GraphType,
          bool bidirectional = false,
          typename PruneCallbackType = _get_neighbors::default_prune_callback<typename VertexHandle<GraphType>::type>>
inline std::vector<typename VertexHandle<GraphType>::type> 
  GetNeighborVertexSetExclude(const std::vector<typename VertexHandle<GraphType>::type>&    source_vertex_set,
                       const std::unordered_set<typename VertexHandle<GraphType>::type>& exclusion_vertex_set) {
  
  auto exclude_vertex_callback = [&exclusion_vertex_set](
      const typename VertexHandle<GraphType>::type& vertex_handle) -> bool {
    return exclusion_vertex_set.find(vertex_handle)
        != exclusion_vertex_set.end();
  };
                                
  return GetNeighborVertexSet(source_vertex_set,  exclude_vertex_callback);
}

template <typename GraphType,
          bool bidirectional = false>
inline std::vector<typename VertexHandle<GraphType>::type> 
  GetNeighborVertexSetExclude(const std::vector<typename VertexHandle<GraphType>::type>&    source_vertex_set,
                       const std::unordered_set<typename VertexID    <GraphType>::type>& exclusion_vertex_set) {
  
  auto exclude_vertex_callback = [&exclusion_vertex_set](
      const typename VertexHandle<GraphType>::type& vertex_handle) -> bool {
    return exclusion_vertex_set.find(vertex_handle->id())
        != exclusion_vertex_set.end();
  };
                                
  return GetNeighborVertexSet<GraphType, bidirectional>(source_vertex_set,  exclude_vertex_callback);
}

template <bool bidirectional = false, typename GraphType>
inline std::vector<typename VertexHandle<GraphType>::type> 
  GetNeighborVertexSetExclude(GraphType& graph,
                              const std::vector<typename VertexID<GraphType>::type>&    source_vertex_set,
                       const std::unordered_set<typename VertexID<GraphType>::type>& exclusion_vertex_set) {

  std::vector<typename VertexHandle<GraphType>::type> source_vertex_handle_set;
  source_vertex_handle_set.reserve(source_vertex_set.size());

  for (const auto& vertex_id : source_vertex_set) {
    auto vertex_handle = graph.FindVertex(vertex_id);
    if (!vertex_handle) {
      assert(false);
      return std::vector<typename VertexHandle<GraphType>::type>();
    } 
    source_vertex_handle_set.emplace_back(vertex_handle);
  }
                                
  return GetNeighborVertexSetExclude<GraphType, bidirectional>(source_vertex_handle_set,  
                                                            exclusion_vertex_set);
}

template <bool bidirectional = false, typename GraphType>
inline std::vector<typename VertexHandle<GraphType>::type> 
  GetNeighborVertexSetExclude(GraphType& graph,
                       const std::unordered_set<typename VertexID<GraphType>::type>&    source_vertex_set,
                       const std::unordered_set<typename VertexID<GraphType>::type>& exclusion_vertex_set) {

  std::vector<typename VertexHandle<GraphType>::type> source_vertex_handle_set;
  source_vertex_handle_set.reserve(source_vertex_set.size());

  for (const auto& vertex_id : source_vertex_set) {
    auto vertex_handle = graph.FindVertex(vertex_id);
    if (!vertex_handle) {
      assert(false);
      return std::vector<typename VertexHandle<GraphType>::type>();
    } 
    source_vertex_handle_set.emplace_back(vertex_handle);
  }
                                
  return GetNeighborVertexSet<GraphType>(source_vertex_handle_set,  
                                      exclusion_vertex_set);
}

}  // namespace GUNDAM

#endif // _GUNDAM_TOOL_GET_NEIGHBORS_H