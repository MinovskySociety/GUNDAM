#ifndef _GUNDAM_TOOL_OPERATOR_RESET_EDGE_LABEL_H
#define _GUNDAM_TOOL_OPERATOR_RESET_EDGE_LABEL_H

namespace GUNDAM {

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