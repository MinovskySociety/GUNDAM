
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
void ParseCol(std::vector<std::string>& before_parse_col_name,
              std::vector<std::string>& after_parse_col_name,
              std::vector<std::string>& after_parse_value_type) {
  for (auto const& str : before_parse_col_name) {
    std::string col_name, value_type;
    int nameflag = 0;
    for (int i = 0; i < str.size(); i++) {
      if (str[i] == ':') {
        nameflag = 1;
        continue;
      }
      if (!nameflag) {
        col_name.push_back(str[i]);
      } else {
        value_type.push_back(str[i]);
      }
    }
    after_parse_col_name.push_back(col_name);
    after_parse_value_type.push_back(value_type);
  }
}
// check Attribute Key type is correct
// when no attribute after_parse_col_name.size()==begin_pos
template <typename AttributeKeyType>
bool CheckAttributeKeyIsCorrect(std::vector<std::string>& after_parse_col_name,
                                int begin_pos) {
  // check col name can change to attrkeytype
  for (int j = begin_pos; j < after_parse_col_name.size(); j++) {
    std::string str = after_parse_col_name[j];
    std::stringstream ss(str);
    AttributeKeyType check_flag;
    ss >> check_flag;
    std::stringstream s1;
    std::string str1;
    s1 << check_flag;
    s1 >> str1;
    if (str1 != str) {
      return false;
    }
  }
  return true;
}
template <
    template <typename...> class GraphType, typename... configures,
    typename std::enable_if<!GraphType<configures...>::vertex_has_attribute,
                            bool>::type = false>
void LoadVertexAttribue(GraphType<configures...>& graph,
                        typename GraphType<configures...>::VertexPtr& node_ptr,
                        rapidcsv::Document& node_file,
                        std::vector<std::string>& after_parse_col_name,
                        std::vector<std::string>& after_parse_value_type,
                        int row_pos) {
  return;
}
template <
    template <typename...> class GraphType, typename... configures,
    typename std::enable_if<GraphType<configures...>::vertex_has_attribute,
                            bool>::type = false>
void LoadVertexAttribue(GraphType<configures...>& graph,
                        typename GraphType<configures...>::VertexPtr& node_ptr,
                        rapidcsv::Document& node_file,
                        std::vector<std::string>& after_parse_col_name,
                        std::vector<std::string>& after_parse_value_type,
                        int row_pos) {
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  using AttributeType =
      typename GraphType<configures...>::VertexType::AttributeKeyType;
  int col_num = after_parse_col_name.size();
  for (int col_iter = 2; col_iter < col_num; col_iter++) {
    // key is col_iter
    std::stringstream ss(after_parse_col_name[col_iter]);
    AttributeType attr_key;
    ss >> attr_key;
    if (after_parse_value_type[col_iter] == "string") {
      auto cell_attr = node_file.GetCell<std::string>(col_iter, row_pos);
      node_ptr->AddAttribute(attr_key, cell_attr);
      node_ptr->SetValueType(attr_key, "string");
    } else if (after_parse_value_type[col_iter] == "int") {
      auto cell_attr = node_file.GetCell<int>(col_iter, row_pos);
      node_ptr->AddAttribute(attr_key, cell_attr);
      node_ptr->SetValueType(attr_key, "int");
    } else if (after_parse_value_type[col_iter] == "double") {
      auto cell_attr = node_file.GetCell<double>(col_iter, row_pos);
      node_ptr->AddAttribute(attr_key, cell_attr);
      node_ptr->SetValueType(attr_key, "double");
    } else if (after_parse_value_type[col_iter] == "short") {
      auto cell_attr = node_file.GetCell<short>(col_iter, row_pos);
      node_ptr->AddAttribute(attr_key, cell_attr);
      node_ptr->SetValueType(attr_key, "short");
    } else if (after_parse_value_type[col_iter] == "long") {
      auto cell_attr = node_file.GetCell<long long>(col_iter, row_pos);
      node_ptr->AddAttribute(attr_key, cell_attr);
      node_ptr->SetValueType(attr_key, "long");
    }
  }
  return;
}
template <template <typename...> class GraphType, typename... configures>
int ReadCSVVertexFile(GraphType<configures...>& graph, const char* v_file) {
  // read .v file(csv)
  //.v file format: (id,label,......)
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  using AttributeType =
      typename GraphType<configures...>::VertexType::AttributeKeyType;
  // check file exist
  std::ifstream check_node_file(v_file);
  if (!check_node_file) {
    std::cout << v_file << " not exist!" << std::endl;
    return -1;
  }
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
  // parse col
  std::vector<std::string> before_parse_col_name = node_file.GetColumnNames();
  std::vector<std::string> after_parse_col_name, after_parse_value_type;
  ParseCol(before_parse_col_name, after_parse_col_name, after_parse_value_type);
  // check attribute key type is correct
  if (CheckAttributeKeyIsCorrect<AttributeType>(after_parse_col_name, 2) ==
      false) {
    std::cout << "Attribute Key Type is not correct!" << std::endl;
    return -1;
  }
  std::vector<VertexIDType> node_id = node_file.GetColumn<VertexIDType>(0);
  std::vector<VertexLabelType> node_label =
      node_file.GetColumn<VertexLabelType>(1);
  size_t sz = node_id.size();
  for (size_t i = 0; i < sz; i++) {
    // add node_id and node_label;
    VertexPtr node_ptr =
        graph.AddVertex(node_id[i], VertexLabelType(node_label[i])).first;
    node_ptr->SetIDType(after_parse_value_type[0]);
    node_ptr->SetLabelType(after_parse_value_type[1]);
    // add other attribute
    if (graph.vertex_has_attribute) {
      LoadVertexAttribue(graph, node_ptr, node_file, after_parse_col_name,
                         after_parse_value_type, i);
    }
  }

  return static_cast<int>(sz);
}
template <template <typename...> class GraphType, typename... configures,
          typename std::enable_if<!GraphType<configures...>::edge_has_attribute,
                                  bool>::type = false>
void LoadEdgeAttribue(GraphType<configures...>& graph,
                      typename GraphType<configures...>::EdgePtr& edge_ptr,
                      rapidcsv::Document& edge_file,
                      std::vector<std::string>& after_parse_col_name,
                      std::vector<std::string>& after_parse_value_type,
                      int row_pos) {
  return;
}
template <template <typename...> class GraphType, typename... configures>
int ReadCSVEdgeFile(GraphType<configures...>& graph, const char* e_file) {
  // read .e file(csv)
  //.e file format: (edge_id,from_id,to_id,edge_label,......)
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgePtr = typename GraphType<configures...>::EdgePtr;
  using AttributeType =
      typename GraphType<configures...>::EdgeType::AttributeKeyType;
  std::ifstream check_edge_file(e_file);
  if (!check_edge_file) {
    std::cout << e_file << " not exist!" << std::endl;
    return -1;
  }
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
  // parse col
  std::vector<std::string> before_parse_col_name = edge_file.GetColumnNames();
  std::vector<std::string> after_parse_col_name, after_parse_value_type;
  ParseCol(before_parse_col_name, after_parse_col_name, after_parse_value_type);
  // check col name can change to attrkeytype
  if (CheckAttributeKeyIsCorrect<AttributeType>(after_parse_col_name, 4) ==
      false) {
    std::cout << "Attribute type is not correct!" << std::endl;
    return -1;
  }

  edge_id = edge_file.GetColumn<VertexIDType>(0);
  from_id = edge_file.GetColumn<VertexIDType>(1);
  to_id = edge_file.GetColumn<VertexIDType>(2);
  edge_label = edge_file.GetColumn<EdgeLabelType>(3);
  size_t sz = from_id.size();
  for (size_t i = 0; i < sz; i++) {
    // std::cout << "j=" << i << std::endl;
    // std::cout<<from_id[i]<<" "<<to_id[i]<<" "<<edge_label[i]<<std::endl;
    EdgePtr edge_ptr = graph
                           .AddEdge(from_id[i], to_id[i],
                                    EdgeLabelType(edge_label[i]), edge_id[i])
                           .first;
    graph.SetEdgeIDType(after_parse_value_type[0]);
    graph.SetEdgeLabelType(after_parse_value_type[3]);
    edge_ptr->src_ptr()->SetIDType(after_parse_value_type[1]);
    edge_ptr->dst_ptr()->SetIDType(after_parse_value_type[2]);
    if (graph.edge_has_attribute) {
      for (int col_iter = 4; col_iter < col_num; col_iter++) {
        std::stringstream ss(after_parse_col_name[col_iter]);
        AttributeType attr_key;
        ss >> attr_key;
        if (after_parse_value_type[col_iter] == "string") {
          auto cell_attr = edge_file.GetCell<std::string>(col_iter, i);
          edge_ptr->AddAttribute(attr_key, cell_attr);
          edge_ptr->SetValueType(attr_key, "string");
        } else if (after_parse_value_type[col_iter] == "int") {
          auto cell_attr = edge_file.GetCell<int>(col_iter, i);
          edge_ptr->AddAttribute(attr_key, cell_attr);
          edge_ptr->SetValueType(attr_key, "int");
        } else if (after_parse_value_type[col_iter] == "double") {
          auto cell_attr = edge_file.GetCell<double>(col_iter, i);
          edge_ptr->AddAttribute(attr_key, cell_attr);
          edge_ptr->SetValueType(attr_key, "double");
        } else if (after_parse_value_type[col_iter] == "short") {
          auto cell_attr = edge_file.GetCell<short>(col_iter, i);
          edge_ptr->AddAttribute(attr_key, cell_attr);
          edge_ptr->SetValueType(attr_key, "short");
        } else if (after_parse_value_type[col_iter] == "long") {
          auto cell_attr = edge_file.GetCell<long long>(col_iter, i);
          edge_ptr->AddAttribute(attr_key, cell_attr);
          edge_ptr->SetValueType(attr_key, "long");
        }
      }
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
int WriteCSVGraph(const GraphType<configures...>& graph, const char* v_file,
                  const char* e_file) {
  using VertexType = typename GraphType<configures...>::VertexType;
  using EdgeType = typename GraphType<configures...>::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexPtr = typename GraphType<configures...>::VertexPtr;
  using VertexConstPtr = typename GraphType<configures...>::VertexConstPtr;
  using EdgeConstPtr = typename GraphType<configures...>::EdgeConstPtr;
  using AttributeKeyType = typename VertexType::AttributeKeyType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;
  // write .v file
  std::ofstream node_file(v_file);
  int count = 0;
  std::vector<std::string> node_col_name, col_value_type;
  std::vector<AttributeKeyType> node_attr_key;
  node_col_name.push_back("node_id");
  node_col_name.push_back("node_label");
  // get ID and Label Type
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    VertexConstPtr node_ptr = node_it;
    col_value_type.push_back(node_ptr->id_type());
    col_value_type.push_back(node_ptr->label_type());
    break;
  }
  // Get Attribute value type
  std::set<VertexLabelType> used_label;
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    if (used_label.count(node_it->label())) continue;
    used_label.insert(node_it->label());
    VertexConstPtr node_ptr = node_it;
    for (auto attr_it = node_ptr->AttributeCBegin(); !attr_it.IsDone();
         attr_it++) {
      AttributeKeyType attr_key = attr_it->key();
      std::string attr_value_type = node_ptr->GetValueType(attr_key);
      std::stringstream ss;
      ss << attr_key;
      std::string col_name;
      ss >> col_name;
      node_col_name.push_back(col_name);
      node_attr_key.push_back(attr_key);
      col_value_type.push_back(attr_value_type);
    }
  }
  // cout col
  for (int i = 0; i < node_col_name.size(); i++) {
    if (i) {
      node_file << "," << node_col_name[i] << ":" << col_value_type[i];
    } else {
      node_file << node_col_name[i] << ":" << col_value_type[i];
    }
  }
  node_file << std::endl;
  // cout each vertex
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    count++;
    std::vector<std::string> line_node_info;
    line_node_info.resize(node_col_name.size());
    if (col_value_type[0] == "string") {
      line_node_info[0] = node_it->id();
    } else {
      line_node_info[0] = std::to_string(node_it->id());
    }
    if (col_value_type[1] == "string") {
      line_node_info[1] = node_it->label();
    } else {
      line_node_info[1] = std::to_string(node_it->label());
    }
    VertexConstPtr node_ptr = node_it;
    for (auto attr_it = node_ptr->AttributeCBegin(); !attr_it.IsDone();
         attr_it++) {
      int attr_key_pos = 0;
      AttributeKeyType attr_key = attr_it->key();
      for (int j = 0; j < node_attr_key.size(); j++) {
        if (node_attr_key[j] == attr_key) {
          attr_key_pos = j + 2;
          break;
        }
      }
      if (col_value_type[attr_key_pos] == "string") {
        line_node_info[attr_key_pos] =
            attr_it->template const_value<std::string>();
      } else if (col_value_type[attr_key_pos] == "int") {
        line_node_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<int>());
      } else if (col_value_type[attr_key_pos] == "short") {
        line_node_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<short>());
      } else if (col_value_type[attr_key_pos] == "double") {
        line_node_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<double>());
      } else if (col_value_type[attr_key_pos] == "long") {
        line_node_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<long>());
      }
    }
    for (int j = 0; j < line_node_info.size(); j++) {
      if (j) {
        node_file << "," << line_node_info[j];
      } else
        node_file << line_node_info[j];
    }
    node_file << std::endl;
  }
  // write .e file
  std::ofstream edge_file(e_file);

  std::vector<std::string> edge_col_name, edge_col_value_type;
  std::vector<AttributeKeyType> edge_attr_key;
  edge_col_name.push_back("edge_id");
  edge_col_name.push_back("source_node_id");
  edge_col_name.push_back("target_node_id");
  edge_col_name.push_back("edge_label_id");
  // get ID and Label Type
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    int insert_edge_flag = 0;
    VertexConstPtr node_ptr = node_it;
    for (auto edge_it = node_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      EdgeConstPtr edge_ptr = edge_it;
      edge_col_value_type.push_back(graph.edge_id_type());
      edge_col_value_type.push_back(edge_ptr->const_src_ptr()->id_type());
      edge_col_value_type.push_back(edge_ptr->const_dst_ptr()->id_type());
      edge_col_value_type.push_back(graph.edge_label_type());
      insert_edge_flag = 1;
      break;
    }
    if (insert_edge_flag) break;
  }
  // std::cout << "size=" << edge_col_value_type.size() << std::endl;
  // Get Attribute value type
  std::set<EdgeLabelType> edge_used_label;
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    VertexConstPtr node_ptr = node_it;
    for (auto edge_it = node_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      if (edge_used_label.count(edge_it->label())) continue;
      edge_used_label.insert(edge_it->label());
      EdgeConstPtr edge_ptr = edge_it;
      for (auto edge_attr_it = edge_ptr->AttributeCBegin();
           !edge_attr_it.IsDone(); edge_attr_it++) {
        EdgeAttributeKeyType attr_key = edge_attr_it->key();
        std::string attr_value_type = edge_ptr->GetValueType(attr_key);
        std::stringstream ss;
        ss << attr_key;
        std::string col_name;
        ss >> col_name;
        edge_col_name.push_back(col_name);
        edge_attr_key.push_back(attr_key);
        edge_col_value_type.push_back(attr_value_type);
      }
    }
  }
  // std::cout << "size=" << edge_col_value_type.size() << std::endl;
  for (int i = 0; i < edge_col_value_type.size(); i++)
    //  std::cout << "type:" << edge_col_value_type[i] << std::endl;
    // std::cout << "size=" << edge_col_value_type.size() << std::endl;
    // cout col
    for (int i = 0; i < edge_col_name.size(); i++) {
      if (i) {
        edge_file << "," << edge_col_name[i] << ":" << edge_col_value_type[i];
      } else {
        edge_file << edge_col_name[i] << ":" << edge_col_value_type[i];
      }
    }
  edge_file << std::endl;
  // cout each edge
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    VertexConstPtr node_ptr = node_it;
    for (auto edge_it = node_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      count++;
      std::vector<std::string> line_edge_info;
      line_edge_info.resize(edge_col_name.size());
      if (edge_col_value_type[0] == "string") {
        line_edge_info[0] = edge_it->id();
      } else {
        line_edge_info[0] = std::to_string(edge_it->id());
      }
      if (edge_col_value_type[1] == "string") {
        line_edge_info[1] = edge_it->const_src_ptr()->id();
      } else {
        line_edge_info[1] = std::to_string(edge_it->const_src_ptr()->id());
      }
      if (edge_col_value_type[2] == "string") {
        line_edge_info[2] = edge_it->const_dst_ptr()->id();
      } else {
        line_edge_info[2] = std::to_string(edge_it->const_dst_ptr()->id());
      }
      if (edge_col_value_type[3] == "string") {
        line_edge_info[3] = edge_it->label();
      } else {
        line_edge_info[3] = std::to_string(edge_it->label());
      }
      EdgeConstPtr edge_ptr = edge_it;
      for (auto attr_it = edge_ptr->AttributeCBegin(); !attr_it.IsDone();
           attr_it++) {
        int attr_key_pos = 0;
        EdgeAttributeKeyType attr_key = attr_it->key();
        for (int j = 0; j < edge_attr_key.size(); j++) {
          if (edge_attr_key[j] == attr_key) {
            attr_key_pos = j + 4;
            break;
          }
        }
        if (edge_col_value_type[attr_key_pos] == "string") {
          line_edge_info[attr_key_pos] =
              attr_it->template const_value<std::string>();
        } else if (edge_col_value_type[attr_key_pos] == "int") {
          line_edge_info[attr_key_pos] =
              std::to_string(attr_it->template const_value<int>());
        } else if (edge_col_value_type[attr_key_pos] == "short") {
          line_edge_info[attr_key_pos] =
              std::to_string(attr_it->template const_value<short>());
        } else if (edge_col_value_type[attr_key_pos] == "double") {
          line_edge_info[attr_key_pos] =
              std::to_string(attr_it->template const_value<double>());
        } else if (edge_col_value_type[attr_key_pos] == "long") {
          line_edge_info[attr_key_pos] =
              std::to_string(attr_it->template const_value<long>());
        }
      }
      for (int j = 0; j < line_edge_info.size(); j++) {
        if (j) {
          edge_file << "," << line_edge_info[j];
        } else
          edge_file << line_edge_info[j];
      }
      edge_file << std::endl;
    }
  }
  return count;
}
}  // namespace GUNDAM

#endif
