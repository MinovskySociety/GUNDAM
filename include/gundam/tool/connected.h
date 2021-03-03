#ifndef _CONNECTED_H
#define _CONNECTED_H

#include "gundam/algorithm/dfs.h"

#include "gundam/graph_type/vertex_handle.h"

namespace GUNDAM {

template <typename GraphType>
inline bool Connected(GraphType& graph) {
  // just begin bfs at a random vertex and find whether
  // it can reach all vertexes
  typename VertexHandle<GraphType>::type
           vertex_handle = graph.VertexBegin();
  return GUNDAM::Dfs<true>(graph,vertex_handle)
                        == graph.CountVertex();
}

}  // namespace GUNDAM

#endif // _CONNECTED_H