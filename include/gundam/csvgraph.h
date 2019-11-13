
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

template <template <typename...> class GraphType, typename... configures>
int ReadCSVVertexFile(GraphType<configures...>& graph, const char* v_file) {
  // read .v file(csv)
  //.v file format: (id,label)
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;  
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  rapidcsv::Document node_file(v_file, rapidcsv::LabelParams(0, -1));
  size_t col_num = node_file.GetColumnCount();
  // check col num >= 2
  if (col_num < 2) {
    std::cout << "node file does not have node_id or node_label!" << std::endl;
    return -1;
  }
  // check attribute correct
  if (col_num >= 3 && !graph.vertex_has_attribute) {
    std::cout << "node file has attribute but graph does not support!"
              << std::endl;
    return -1;
  }
  // todo:check data type
  //.....
  std::vector<VertexIDType> node_id = node_file.GetColumn<VertexIDType>(0);
  std::vector<VertexLabelType> node_label =
      node_file.GetColumn<VertexLabelType>(1);
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

  return static_cast<int>(sz);
}

template <template <typename...> class GraphType, typename... configures>
int ReadCSVEdgeFile(GraphType<configures...>& graph, const char* e_file) {
  // read .e file(csv)
  //.e file format: (edge_id,from_id,to_id,edge_label)
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;  
  using EdgePtr = typename GraphType<configures...>::EdgePtr;
  rapidcsv::Document edge_file(e_file, rapidcsv::LabelParams(0, -1));
  std::vector<VertexIDType> from_id, to_id, edge_id;
  std::vector<EdgeLabelType> edge_label;
  size_t col_num = edge_file.GetColumnCount();
  // check col_num >= 4
  if (col_num < 4) {
    std::cout << "edge file is not correct!(col num must >=4)" << std::endl;
    return -1;
  }
  if (col_num >= 5 && !graph.edge_has_attribute) {
    std::cout << "edge file has attribute but graph does not support!"
              << std::endl;
    return -1;
  }
  edge_id = edge_file.GetColumn<VertexIDType>(0);
  from_id = edge_file.GetColumn<VertexIDType>(1);
  to_id = edge_file.GetColumn<VertexIDType>(2);
  edge_label = edge_file.GetColumn<EdgeLabelType>(3);
  size_t sz = from_id.size();
  for (size_t i = 0; i < sz; i++) {
    // std::cout<<from_id[i]<<" "<<to_id[i]<<" "<<edge_label[i]<<std::endl;
    EdgePtr edge_ptr = graph
                           .AddEdge(from_id[i], to_id[i],
                                    EdgeLabelType(edge_label[i]), edge_id[i])
                           .first;
    for (int col_iter = 4; col_iter < col_num; col_iter++) {
      std::string cell = edge_file.GetCell<std::string>(col_iter, i);
      edge_ptr->AddAttribute(col_iter, cell);
    }
  }
  return static_cast<int>(sz);
}

template <template <typename...> class GraphType, typename... configures,
          typename VertexFileList, typename EdgeFileList>
int ReadCSVGraph(GraphType<configures...>& graph, const VertexFileList& v_list,
                 const EdgeFileList& e_list) {
  int count = 0;
  for (const auto& v_file : v_list) {
    int res = ReadCSVVertexFile(graph, v_file);
    if (res < 0) return res;
    count += res;
  }
  for (const auto& e_file : e_list) {
    int res = ReadCSVEdgeFile(graph, e_file);
    if (res < 0) return res;
    count += res;
  }
  return count;
}

template <template <typename...> class GraphType, typename... configures>
int WriteCsvGraph(const GraphType<configures...>& graph, const char* v_file,
                  const char* e_file) {
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;  
  using EdgeLabelType = typename EdgeType::LabelType;  
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  std::ofstream node_file(v_file);
  node_file << "node_id,node_label" << std::endl;
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    node_file << it->id() << " " << it->label() << std::endl;
  }

  return 1;
}
}  // namespace GUNDAM

#endif
