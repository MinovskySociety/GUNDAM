#ifndef _GUNDAM_TOOL_CONNECTED_COMPONENT_H
#define _GUNDAM_TOOL_CONNECTED_COMPONENT_H

#include "gundam/tool/k_hop.h"

#include <vector>
#include <unordered_set>

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
 * ##    decompose the input graph into  ## *
 * ##    set of connected components     ## *
 * ######################################## */
template <bool bidirectional = true,
          typename GraphType>
std::vector<GraphType>  // OK to return vector after C11
     ConnectedComponent(const GraphType& graph) {

  using VertexHandleType = typename VertexHandle<GraphType>::type;
  using VertexIDType = typename GraphType::VertexType::IDType;
  std::vector<GraphType> connected_components;
  connected_components.clear();

  /* ################################################# *
   * ##  wenzhi: optimize me                        ## *
   * ##    to support const GraphType in            ## *
   * ##    ConnectedComponent(graph, vertex_handle  ## *
   * ################################################# */
  GraphType temp_graph(graph);
  GraphType k_hop_subgraph;

  std::unordered_set<VertexIDType> processed_nodes;

  for (auto vertex_it = temp_graph.VertexBegin();
		  !vertex_it.IsDone();
		  vertex_it++) {
    if (processed_nodes.find(vertex_it->id())
		    != processed_nodes.end()) {
      continue;
    }

    k_hop_subgraph = GUNDAM::ConnectedComponent(temp_graph, vertex_it);
    connected_components.emplace_back(k_hop_subgraph);

    for (auto kh_vertex_it = k_hop_subgraph.VertexBegin();
		    !kh_vertex_it.IsDone();
		    kh_vertex_it++) {
     processed_nodes.insert(kh_vertex_it->id());
    }
  }


  assert(!connected_components.empty());
  return  connected_components;
}

};

#endif // _GUNDAM_TOOL_CONNECTED_COMPONENT_H
