#ifndef _GUNDAM_TOOL_CONNECTED_COMPONENT_H
#define _GUNDAM_TOOL_CONNECTED_COMPONENT_H

#include "gundam/tool/k_hop.h"

#include <vector>

namespace GUNDAM {

template <bool bidirectional = true,
          typename GraphType>
GraphType ConnectedComponent(GraphType& graph,
       typename VertexHandle<GraphType>::type vertex_handle) {
  assert(vertex_handle);
  assert(graph.FindVertex(vertex_handle->id()));
  assert(graph.FindVertex(vertex_handle->id())
                       == vertex_handle);
  /* ####################################### *
   * ##  wenzhi:                          ## *
   * ##    to simplify the implemention,  ## *
   * ##    grab a vertex_handle-centered  ## *
   * ##    with a large enough k          ## *
   * ####################################### */
  return KHop<bidirectional>(graph, vertex_handle, 
                             graph.CountVertex());
}

/* ######################################## *
 * ##  wenzhi: todo:                     ## *
 * ##    decompose the input graph into  ## *
 * ##    set of connected components     ## *
 * ######################################## */
template <bool bidirectional = true,
          typename GraphType>
std::vector<GraphType>  // OK to return vector after C11
     ConnectedComponent(GraphType& graph) {
  assert(false);
  std::vector<GraphType> connected_components;
  assert(!connected_components.empty());
  /* ################################################################## *
   * ##  wenzhi: hint                                                ## *
   * ##    Call the method ConnectedComponent(graph, vertex_handle)  ## *
   * ##    above here                                                ## *
   * ################################################################## */
  return  connected_components;
}

};

#endif // _GUNDAM_TOOL_CONNECTED_COMPONENT_H