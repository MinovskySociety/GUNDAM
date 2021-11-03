#ifndef _GUNDAM_TOOL_MAX_ID_H
#define _GUNDAM_TOOL_MAX_ID_H

#include "include/gundam/type_getter/edge_id.h"
#include "include/gundam/type_getter/vertex_id.h"

namespace GUNDAM {

template <typename GraphType>
inline typename VertexID<GraphType>::type 
             MaxVertexId(const GraphType& graph) {
  using VertexIDType = typename VertexID<GraphType>::type;
  VertexIDType max_vertex_id = 0;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    max_vertex_id = max_vertex_id > vertex_it->id()?
                    max_vertex_id : vertex_it->id();
  }
  return max_vertex_id;
}

template <typename GraphType>
inline typename EdgeID<GraphType>::type 
             MaxEdgeId(const GraphType& graph) {
  using EdgeIDType = typename EdgeID<GraphType>::type;
  EdgeIDType max_edge_id = 0;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      max_edge_id = max_edge_id > out_edge_it->id()?
                    max_edge_id : out_edge_it->id();
    }
  }
  return max_edge_id;
}
    
};

#endif // _GUNDAM_TOOL_MAX_ID_H