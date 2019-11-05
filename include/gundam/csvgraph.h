
#ifndef _CSVGRAPH_H
#define _CSVGRAPH_H

#include "graph.h"

#include <iostream>
#include <sstream>
#include <string>

#include "rapidcsv.h"

namespace GUNDAM {
using ReturnType = int;

// node :has label
// edge: has label but not have attribute
template <template <typename...> class GraphType, typename... configures>
ReturnType ReadCSVGraph(GraphType<configures...>& graph, char* v_file,
                        char* e_file) {
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  // read .v file(csv)
  //.v file format: (id,label)
  rapidcsv::Document node_file(v_file, rapidcsv::LabelParams(0, -1));
  std::vector<VertexIDType> node_id = node_file.GetColumn<VertexIDType>(0);
  std::vector<VertexLabelUnderlieType> node_label =
      node_file.GetColumn<VertexLabelUnderlieType>(1);
  size_t sz = node_id.size();
  for (int i = 0; i < sz; i++) {
    // std::cout<<node_id[i]<<" "<<node_label[i]<<std::endl;
    graph.AddVertex(node_id[i], VertexLabelType(node_label[i]));
  }
  // read .e file(csv)
  //.e file format: (edge_id,from_id,to_id,edge_label)
  rapidcsv::Document edge_file(e_file, rapidcsv::LabelParams(0, -1));
  std::vector<VertexIDType> from_id, to_id, edge_id;
  std::vector<EdgeLabelUnderlieType> edge_label;
  edge_id = edge_file.GetColumn<VertexIDType>(0);
  from_id = edge_file.GetColumn<VertexIDType>(1);
  to_id = edge_file.GetColumn<VertexIDType>(2);
  edge_label = edge_file.GetColumn<EdgeLabelUnderlieType>(3);
  sz = from_id.size();
  for (int i = 0; i < sz; i++) {
    // std::cout<<from_id[i]<<" "<<to_id[i]<<" "<<edge_label[i]<<std::endl;
    graph.AddEdge(from_id[i], to_id[i], EdgeLabelType(edge_label[i]),
                  edge_id[i]);
  }
  return 1;
}

template <template <typename...> class GraphType, typename... configures>
ReturnType OutputGraph(const GraphType<configures...>& graph) {
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using VertexPtr = const VertexType*;
  std::cout << "node label:\n";
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    std::cout << it->id() << " " << it->label().to_string() << std::endl;
  }
  std::cout << "src_id dst_id edge_label:\n";
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    for (auto edge_it = graph.FindConstVertex(it->id())->OutEdgeCBegin();
         !edge_it.IsDone(); edge_it++) {
      std::cout << edge_it->const_src_ptr()->id() << " " << edge_it->const_dst_ptr()->id() << " "
                << edge_it->label().to_string() << std::endl;
    }
  }
  return 1;
}
}  // namespace GUNDAM

#endif