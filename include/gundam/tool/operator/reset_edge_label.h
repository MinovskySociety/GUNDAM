#ifndef _GUNDAM_TOOL_OPERATOR_RESET_EDGE_LABEL_H
#define _GUNDAM_TOOL_OPERATOR_RESET_EDGE_LABEL_H

namespace GUNDAM {

template <typename GraphType>
GraphType ResetEdgeLabel(const GraphType& graph,
        const std::map<std::pair<typename VertexLabel<GraphType>::type,
                                 typename VertexLabel<GraphType>::type>,
                                 typename   EdgeLabel<GraphType>::type>& edge_label_map) {
  GraphType new_graph;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    new_graph.AddVertex(vertex_it->id(),
                        vertex_it->label());
  }
  
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      auto edge_label_map_it = edge_label_map.find(std::pair(out_edge_it->src_handle()->label(),
                                                             out_edge_it->dst_handle()->label()));
      assert(edge_label_map_it != edge_label_map.end());
      new_graph.AddEdge(out_edge_it->src_handle()->id(),
                        out_edge_it->dst_handle()->id(),
                        edge_label_map_it->second,
                        out_edge_it->id());
    }
  }
  return new_graph;
}

template <typename GraphType>
GraphType ResetEdgeLabel(const GraphType& graph,
                         auto new_edge_label) {
  GraphType new_graph;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    new_graph.AddVertex(vertex_it->id(),
                        vertex_it->label());
  }
  
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    for (auto out_edge_it = vertex_it->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      new_graph.AddEdge(out_edge_it->src_handle()->id(),
                        out_edge_it->dst_handle()->id(),
                        new_edge_label,
                        out_edge_it->id());
    }
  }
  return new_graph;
}

};

#endif // _GUNDAM_TOOL_OPERATOR_RESET_EDGE_LABEL_H