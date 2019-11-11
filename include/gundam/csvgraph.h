
#ifndef _CSVGRAPH_H
#define _CSVGRAPH_H

#include <iostream>
#include <sstream>
#include <string>
#include "graph.h"
#include "graph_configure.h"

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
      std::cout << edge_it->const_src_ptr()->id() << " "
                << edge_it->const_dst_ptr()->id() << " "
                << edge_it->label().to_string() << std::endl;
    }
  }
  return 1;
}

template <template <typename...> class GraphType, typename... configures>
ReturnType ReadVFile(GraphType<configures...>& graph, char* v_file) {
  // read .v file(csv)
  //.v file format: (id,label)
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  rapidcsv::Document node_file(v_file, rapidcsv::LabelParams(0, -1));
  int col_num = node_file.GetColumnCount();
  // check col num >= 2
  if (col_num < 2) {
    std::cout << "node file does not have node_id or node_label!" << std::endl;
    return 0;
  }
  // todo:check data type
  //.....
  std::vector<VertexIDType> node_id = node_file.GetColumn<VertexIDType>(0);
  std::vector<VertexLabelUnderlieType> node_label =
      node_file.GetColumn<VertexLabelUnderlieType>(1);
  size_t sz = node_id.size();
  for (size_t i = 0; i < sz; i++) {
    // add node_id and node_label;
    VertexPtr node_ptr =
        graph.AddVertex(node_id[i], VertexLabelType(node_label[i])).first;
    // add other attribute
    for (int col_iter = 2; col_iter < col_num; col_iter++) {
      // key is col_iter
      std::string cell_attr = node_file.GetCell<std::string>(col_iter, i);
      node_ptr->AddAttribute(col_iter, cell_attr);
    }
  }

  return 1;
}

template <template <typename...> class GraphType, typename... configures>
ReturnType ReadEFile(GraphType<configures...>& graph, char* e_file) {
  // read .e file(csv)
  //.e file format: (edge_id,from_id,to_id,edge_label)
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using EdgePtr = typename GraphType<configures...>::EdgePtr;
  rapidcsv::Document edge_file(e_file, rapidcsv::LabelParams(0, -1));
  std::vector<VertexIDType> from_id, to_id, edge_id;
  std::vector<EdgeLabelUnderlieType> edge_label;
  int col_num = edge_file.GetColumnCount();
  if (col_num < 4) {
    std::cout << "edge file is not correct!(col num must >=4)" << std::endl;
    return 0;
  }
  edge_id = edge_file.GetColumn<VertexIDType>(0);
  from_id = edge_file.GetColumn<VertexIDType>(1);
  to_id = edge_file.GetColumn<VertexIDType>(2);
  edge_label = edge_file.GetColumn<EdgeLabelUnderlieType>(3);
  size_t sz = from_id.size();
  for (size_t i = 0; i < sz; i++) {
    // std::cout<<from_id[i]<<" "<<to_id[i]<<" "<<edge_label[i]<<std::endl;
    EdgePtr edge_ptr =graph.AddEdge(from_id[i], to_id[i], EdgeLabelType(edge_label[i]),
                  edge_id[i]).first;
    for (int col_iter=4;col_iter<col_num;col_iter++){
        std::string cell=edge_file.GetCell<std::string>(col_iter,i);
        edge_ptr->AddAttribute(col_iter,cell);
    }
  }
  return 1;
}

template <template <typename...> class GraphType, typename... configures,
          typename FileNameList>
ReturnType ReadGraph(GraphType<configures...>& graph, FileNameList& v_list,
                     FileNameList& e_list) {
  for (auto v_file : v_list) {
    ReturnType flag = ReadVFile(graph, v_file);
    if (!flag) return 0;
  }
  for (auto e_file : e_list) {
    ReturnType flag = ReadEFile(graph, e_file);
    if (!flag) return 0;
  }
  return 1;
}
}  // namespace GUNDAM

#endif