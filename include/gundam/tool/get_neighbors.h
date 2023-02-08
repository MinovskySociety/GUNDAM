#ifndef _GUNDAM_TOOL_CONNECTED_H
#define _GUNDAM_TOOL_CONNECTED_H

#include "gundam/algorithm/dfs.h"

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_id.h"

namespace GUNDAM {

// ok to return vector after C11
template <typename GraphType,
          bool bidirectional = false>
std::vector<typename VertexHandle<GraphType>::type> 
  GetNeighbors(const std::vector<typename VertexHandle<GraphType>::type>& source_vertex_set) {
  
  std::vector<typename VertexHandle<GraphType>::type> neighbors;

  for (const auto& source_vertex 
                 : source_vertex_set) {

    for (auto out_edge_it = source_vertex->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      neighbors.emplace_back(out_edge_it->dst_handle());
    }

    if constexpr (bidirectional)  {
      // consider also in-ward edges
      for (auto in_edge_it = source_vertex->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
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

}  // namespace GUNDAM

#endif // _GUNDAM_TOOL_CONNECTED_H