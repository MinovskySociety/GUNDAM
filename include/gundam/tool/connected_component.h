#ifndef _GUNDAM_TOOL_CONNECTED_COMPONENT_H
#define _GUNDAM_TOOL_CONNECTED_COMPONENT_H

#include "include/gundam/tool/k_hop.h"

#include <vector>

namespace GUNDAM {

template <bool bidirectional = true,
          typename GraphType>
GraphType ConnectedComponent(GraphType& graph,
       typename VertexHandle<GraphType>::type vertex_handle) {
  assert(graph.FindVertx(vertex_handle->id())
                      == vertex_handle);
  return KHop<bidirectional>(graph, vertex_handle, 
                             graph.CountVertex());
}

};

#endif // _GUNDAM_TOOL_CONNECTED_COMPONENT_H