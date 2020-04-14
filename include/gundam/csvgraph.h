#ifndef _CSVGRAPH_H
#define _CSVGRAPH_H

#include <iostream>
#include <set>
#include <sstream>
#include <string>

#include "gundam/datatype.h"
#include "gundam/geneator.h"
#include "rapidcsv.h"

namespace GUNDAM {
using ReturnType = int;
// Parse col
// before: node_id:int
// after:  col_name: node_id  value_type:int
template <typename Data>
void ParseCol(std::vector<Data>& before_parse_col_name,
              std::vector<Data>& after_parse_col_name,
              std::vector<Data>& after_parse_value_type) {
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
    // std::cout << after_parse_col_name[j] << std::endl;
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
template <class GraphType,
          typename std::enable_if<!GraphType::vertex_has_attribute,
                                  bool>::type = false>
void LoadVertexAttribue(GraphType& graph,
                        typename GraphType::VertexPtr& node_ptr,
                        rapidcsv::Document& node_file,
                        std::vector<std::string>& after_parse_col_name,
                        std::vector<std::string>& after_parse_value_type,
                        int row_pos) {
  return;
}
// load Vertex Attribute
template <class GraphType,
          typename std::enable_if<GraphType::vertex_has_attribute, bool>::type =
              false>
void LoadVertexAttribue(GraphType& graph,
                        typename GraphType::VertexPtr& node_ptr,
                        rapidcsv::Document& node_file,
                        std::vector<std::string>& after_parse_col_name,
                        std::vector<std::string>& after_parse_value_type,
                        int row_pos) {
  using VertexType = typename GraphType::VertexType;
  using VertexAttributeKeyType = typename VertexType::AttributeKeyType;

  int col_num = static_cast<int>(after_parse_col_name.size());
  for (int col_iter = 2; col_iter < col_num; col_iter++) {
    // key is col_iter
    // std::cout << "attribute" << std::endl;
    std::stringstream ss(after_parse_col_name[col_iter]);
    VertexAttributeKeyType attr_key;
    ss >> attr_key;
    switch (StringToEnum(after_parse_value_type[col_iter].c_str())) {
      case BasicDataType::kTypeString: {
        std::pair<std::string, bool> cell_attr =
            node_file.GetCellNew<std::string>(col_iter, row_pos);
        if (cell_attr.second || cell_attr.first.empty()) continue;
        node_ptr->AddAttribute(attr_key, cell_attr.first);
        break;
      }
      case BasicDataType::kTypeInt: {
        std::pair<int, bool> cell_attr =
            node_file.GetCellNew<int>(col_iter, row_pos);
        if (cell_attr.second) continue;
        node_ptr->AddAttribute(attr_key, cell_attr.first);
        break;
      }
      case BasicDataType::kTypeDouble: {
        std::pair<double, bool> cell_attr =
            node_file.GetCellNew<double>(col_iter, row_pos);
        if (cell_attr.second) continue;
        node_ptr->AddAttribute(attr_key, cell_attr.first);
        break;
      }
      case BasicDataType::kTypeDateTime: {
        std::pair<std::string, bool> cell_attr =
            node_file.GetCellNew<std::string>(col_iter, row_pos);
        if (cell_attr.second || cell_attr.first.empty()) continue;
        DateTime date_time(cell_attr.first.c_str());
        node_ptr->AddAttribute(attr_key, date_time);
        break;
      }
      case BasicDataType::kTypeUnknown:
      default:
        break;
    }
  }
  return;
}

template <class GraphType, class VertexIDGen>
int ReadCSVVertexFile(GraphType& graph, const std::string& v_file,
                      VertexIDGen& vertex_id_gen) {
  // read .v file(csv)
  //.v file format: (id,label,......)
  using VertexType = typename GraphType::VertexType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using VertexPtr = typename GraphType::VertexPtr;
  using AttributeType = typename GraphType::VertexType::AttributeKeyType;

  std::cout << v_file << std::endl;
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
  for (int i = 0; i < sz; i++) {
    // add node_id and node_label;
    // std::cout << "i=" << i << std::endl;
    VertexPtr node_ptr =
        graph.AddVertex(node_id[i], VertexLabelType(node_label[i])).first;
    vertex_id_gen.UseID(node_id[i]);
    // add other attribute
    if (graph.vertex_has_attribute) {
      LoadVertexAttribue(graph, node_ptr, node_file, after_parse_col_name,
                         after_parse_value_type, i);
    }
  }

  return static_cast<int>(sz);
}
template <
    class GraphType,
    typename std::enable_if<!GraphType::edge_has_attribute, bool>::type = false>
void LoadEdgeAttribue(GraphType& graph, typename GraphType::EdgePtr& edge_ptr,
                      rapidcsv::Document& edge_file,
                      std::vector<std::string>& after_parse_col_name,
                      std::vector<std::string>& after_parse_value_type,
                      int row_pos) {
  return;
}
// load edge attribute
template <
    class GraphType,
    typename std::enable_if<GraphType::edge_has_attribute, bool>::type = false>
void LoadEdgeAttribue(GraphType& graph, typename GraphType::EdgePtr& edge_ptr,
                      rapidcsv::Document& edge_file,
                      std::vector<std::string>& after_parse_col_name,
                      std::vector<std::string>& after_parse_value_type,
                      int row_pos) {
  using EdgeType = typename GraphType::EdgeType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;

  int col_num = static_cast<int>(after_parse_col_name.size());
  for (int col_iter = 4; col_iter < col_num; col_iter++) {
    std::stringstream ss(after_parse_col_name[col_iter]);
    EdgeAttributeKeyType attr_key;
    ss >> attr_key;
    switch (StringToEnum(after_parse_value_type[col_iter].c_str())) {
      case BasicDataType::kTypeString: {
        auto cell_attr = edge_file.GetCellNew<std::string>(col_iter, row_pos);
        if (cell_attr.second || cell_attr.first.empty()) continue;
        edge_ptr->AddAttribute(attr_key, cell_attr.first);
        break;
      }
      case BasicDataType::kTypeInt: {
        auto cell_attr = edge_file.GetCellNew<int>(col_iter, row_pos);
        if (cell_attr.second) continue;
        edge_ptr->AddAttribute(attr_key, cell_attr.first);
        break;
      }
      case BasicDataType::kTypeDouble: {
        auto cell_attr = edge_file.GetCellNew<double>(col_iter, row_pos);
        if (cell_attr.second) continue;
        edge_ptr->AddAttribute(attr_key, cell_attr.first);
        break;
      }
      case BasicDataType::kTypeDateTime: {
        auto cell_attr = edge_file.GetCellNew<std::string>(col_iter, row_pos);
        if (cell_attr.second || cell_attr.first.empty()) continue;
        DateTime date_time(cell_attr.first.c_str());
        edge_ptr->AddAttribute(attr_key, date_time);
        break;
      }
      case BasicDataType::kTypeUnknown:
      default:
        break;
    }
  }
  return;
}
template <class GraphType, class EdgeIDGen>
int ReadCSVEdgeFile(GraphType& graph, const std::string& e_file,
                    EdgeIDGen& edge_id_gen) {
  // read .e file(csv)
  //.e file format: (edge_id,from_id,to_id,edge_label,......)

  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeAttributeKeyType = typename GraphType::EdgeType::AttributeKeyType;
  using EdgePtr = typename GraphType::EdgePtr;

  std::cout << e_file << std::endl;

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
  if (CheckAttributeKeyIsCorrect<EdgeAttributeKeyType>(after_parse_col_name,
                                                       4) == false) {
    std::cout << "Attribute type is not correct!" << std::endl;
    return -1;
  }

  edge_id = edge_file.GetColumn<VertexIDType>(0);
  from_id = edge_file.GetColumn<VertexIDType>(1);
  to_id = edge_file.GetColumn<VertexIDType>(2);
  edge_label = edge_file.GetColumn<EdgeLabelType>(3);
  for (const auto& it : edge_id) {
    edge_id_gen.UseID(it);
  }
  size_t sz = from_id.size();
  for (int i = 0; i < sz; i++) {
    // std::cout << "j=" << i << std::endl;
    // std::cout<<from_id[i]<<" "<<to_id[i]<<" "<<edge_label[i]<<std::endl;

    EdgePtr edge_ptr;
    bool res;
    std::tie(edge_ptr, res) = graph.AddEdge(
        from_id[i], to_id[i], EdgeLabelType(edge_label[i]), edge_id[i]);
    // wangyj:delete !edge_ptr
    if (!res) {
      std::cout << "Failed to add edge from " << from_id[i] << " to "
                << to_id[i] << ", label: " << EdgeLabelType(edge_label[i])
                << ", id: " << edge_id[i] << std::endl;

      continue;
    }

    if (graph.edge_has_attribute) {
      LoadEdgeAttribue(graph, edge_ptr, edge_file, after_parse_col_name,
                       after_parse_value_type, i);
    }
  }
  return static_cast<int>(sz);
}
template <class GraphType>
int ReadCSVGraph(GraphType& graph, const char* v_file, const char* e_file) {
  SimpleArithmeticIDEmptyGenerator<typename GraphType::VertexType::IDType>
      empty_vertex_id_type;
  SimpleArithmeticIDEmptyGenerator<typename GraphType::EdgeType::IDType>
      empty_edge_id_type;
  return ReadCSVGraph(graph, v_file, e_file, empty_vertex_id_type,
                      empty_edge_id_type);
}
template <class GraphType, class VertexIDGen, class EdgeIDGen>
int ReadCSVGraph(GraphType& graph, const char* v_file, const char* e_file,
                 VertexIDGen& vertex_id_gen, EdgeIDGen& edge_id_gen) {
  int count = 0;
  int res = ReadCSVVertexFile(graph, v_file, vertex_id_gen);
  if (res < 0) return res;
  count += res;
  res = ReadCSVEdgeFile(graph, e_file, edge_id_gen);
  if (res < 0) return res;
  count += res;
  return count;
}
template <class GraphType, typename VertexFileList, typename EdgeFileList>
int ReadCSVGraph(GraphType& graph, const VertexFileList& v_list,
                 const EdgeFileList& e_list) {
  int count = 0;
  for (const auto& v_file : v_list) {
    SimpleArithmeticIDEmptyGenerator<typename GraphType::VertexType::IDType>
        empty_vertex_id_type;
    int res = ReadCSVVertexFile(graph, v_file, empty_vertex_id_type);
    if (res < 0) return res;
    count += res;
  }
  for (const auto& e_file : e_list) {
    SimpleArithmeticIDEmptyGenerator<typename GraphType::EdgeType::IDType>
        empty_edge_id_type;
    int res = ReadCSVEdgeFile(graph, e_file, empty_edge_id_type);
    if (res < 0) return res;
    count += res;
  }
  return count;
}
// Write CSV
// ToString
template <typename ElementType>
std::string ToString(ElementType element) {
  std::stringstream ss;
  ss << element;
  std::string ret;
  ss >> ret;
  return ret;
}
// Write CSV col
template <typename StreamType>
void WriteCSVCol(StreamType& file_stream, std::vector<std::string>& col_name,
                 std::vector<std::string>& value_type) {
  for (int i = 0; i < col_name.size(); i++) {
    if (i) {
      file_stream << "," << col_name[i] << ":" << value_type[i];
    } else {
      file_stream << col_name[i] << ":" << value_type[i];
    }
  }
  file_stream << std::endl;
}
template <typename StreamType>
void WriteLine(StreamType& file_stream, std::vector<std::string>& line_info) {
  for (int j = 0; j < line_info.size(); j++) {
    if (j) {
      file_stream << "," << line_info[j];
    } else
      file_stream << line_info[j];
  }
  file_stream << std::endl;
}

template <class GraphType,
          typename std::enable_if<!GraphType::vertex_has_attribute,
                                  bool>::type = false>
void GetVertexAttributeValueType(
    const GraphType& graph, std::vector<std::string>& node_col_name,
    std::vector<std::string>& col_value_type,
    std::vector<typename GraphType::VertexType::AttributeKeyType>&
        node_attr_key) {
  return;
}
template <class GraphType,
          typename std::enable_if<GraphType::vertex_has_attribute, bool>::type =
              false>
void GetVertexAttributeValueType(
    const GraphType& graph, std::vector<std::string>& node_col_name,
    std::vector<std::string>& col_value_type,
    std::vector<typename GraphType::VertexType::AttributeKeyType>&
        node_attr_key) {
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using EdgeIDType = typename EdgeType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;
  using VertexAttributeKeyType = typename VertexType::AttributeKeyType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;

  std::set<VertexLabelType> used_label;
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    if (used_label.count(node_it->label())) continue;
    used_label.insert(node_it->label());
    VertexConstPtr node_ptr = node_it;
    for (auto attr_it = node_ptr->AttributeCBegin(); !attr_it.IsDone();
         attr_it++) {
      VertexAttributeKeyType attr_key = attr_it->key();
      if (std::find(node_attr_key.begin(), node_attr_key.end(), attr_key) !=
          node_attr_key.end())
        continue;
      std::string attr_value_type =
          node_ptr->attribute_value_type_name(attr_key);
      std::stringstream ss;
      ss << attr_key;
      std::string col_name;
      ss >> col_name;
      node_col_name.push_back(col_name);
      node_attr_key.push_back(attr_key);
      col_value_type.push_back(attr_value_type);
    }
  }
  return;
}

template <
    class GraphType,
    typename std::enable_if<!GraphType::edge_has_attribute, bool>::type = false>
void GetEdgeAttributeValueType(
    const GraphType& graph, std::vector<std::string>& edge_col_name,
    std::vector<std::string>& edge_col_value_type,
    std::vector<typename GraphType::EdgeType::AttributeKeyType>&
        edge_attr_key) {
  return;
}

template <
    class GraphType,
    typename std::enable_if<GraphType::edge_has_attribute, bool>::type = false>
void GetEdgeAttributeValueType(
    const GraphType& graph, std::vector<std::string>& edge_col_name,
    std::vector<std::string>& edge_col_value_type,
    std::vector<typename GraphType::EdgeType::AttributeKeyType>&
        edge_attr_key) {
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexPtr = typename GraphType::VertexPtr;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;
  using AttributeKeyType = typename VertexType::AttributeKeyType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;
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
        if (std::find(edge_attr_key.begin(), edge_attr_key.end(), attr_key) !=
            edge_attr_key.end())
          continue;
        std::string attr_value_type =
            edge_ptr->attribute_value_type_name(attr_key);
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
  return;
}
template <class GraphType,
          typename std::enable_if<!GraphType::vertex_has_attribute,
                                  bool>::type = false>
void WriteVertexAttribute(
    const GraphType& graph, typename GraphType::VertexConstPtr& node_ptr,
    std::vector<typename GraphType::VertexType::AttributeKeyType>&
        node_attr_key,
    std::vector<std::string>& col_value_type,
    std::vector<std::string>& line_node_info) {
  return;
}
template <class GraphType,
          typename std::enable_if<GraphType::vertex_has_attribute, bool>::type =
              false>
void WriteVertexAttribute(
    const GraphType& graph, typename GraphType::VertexConstPtr& node_ptr,
    std::vector<typename GraphType::VertexType::AttributeKeyType>&
        node_attr_key,
    std::vector<std::string>& col_value_type,
    std::vector<std::string>& line_node_info) {
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexPtr = typename GraphType::VertexPtr;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;
  using AttributeKeyType = typename VertexType::AttributeKeyType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;
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
    enum BasicDataType value_type_id =
        node_ptr->attribute_value_type_id(attr_key);
    switch (value_type_id) {
      case BasicDataType::kTypeInt:
        line_node_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<int>());
        break;
      case BasicDataType::kTypeDouble:
        line_node_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<double>());
        break;
      case BasicDataType::kTypeString:
        line_node_info[attr_key_pos] =
            attr_it->template const_value<std::string>();
        break;
      case BasicDataType::kTypeDateTime:
        line_node_info[attr_key_pos] =
            (attr_it->template const_value<DateTime>()).to_string();
        break;
      case BasicDataType::kTypeUnknown:
      default:
        break;
    }
  }
  return;
}

template <
    class GraphType,
    typename std::enable_if<!GraphType::edge_has_attribute, bool>::type = false>
void WriteEdgeAttribute(
    const GraphType& graph, typename GraphType::EdgeConstPtr& edge_ptr,
    std::vector<typename GraphType::EdgeType::AttributeKeyType>& edge_attr_key,
    std::vector<std::string>& edge_col_value_type,
    std::vector<std::string>& line_edge_info) {
  return;
}

template <
    class GraphType,
    typename std::enable_if<GraphType::edge_has_attribute, bool>::type = false>
void WriteEdgeAttribute(
    const GraphType& graph, typename GraphType::EdgeConstPtr& edge_ptr,
    std::vector<typename GraphType::EdgeType::AttributeKeyType>& edge_attr_key,
    std::vector<std::string>& edge_col_value_type,
    std::vector<std::string>& line_edge_info) {
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexPtr = typename GraphType::VertexPtr;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;
  using AttributeKeyType = typename VertexType::AttributeKeyType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;
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
    enum BasicDataType value_type_id =
        edge_ptr->attribute_value_type_id(attr_key);
    switch (value_type_id) {
      case BasicDataType::kTypeDouble:
        line_edge_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<double>());
        break;
      case BasicDataType::kTypeInt:
        line_edge_info[attr_key_pos] =
            std::to_string(attr_it->template const_value<int>());
        break;
      case BasicDataType::kTypeString:
        line_edge_info[attr_key_pos] =
            attr_it->template const_value<std::string>();
        break;
      case BasicDataType::kTypeDateTime:
        line_edge_info[attr_key_pos] =
            (attr_it->template const_value<DateTime>()).to_string();
        break;
      case BasicDataType::kTypeUnknown:
      default:
        break;
    }
  }
  return;
}

template <class GraphType>
int WriteCSVGraph(const GraphType& graph, const char* v_file,
                  const char* e_file) {
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using EdgeIDType = typename EdgeType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;
  using VertexAttributeKeyType = typename VertexType::AttributeKeyType;
  using EdgeAttributeKeyType = typename EdgeType::AttributeKeyType;

  // write .v file
  std::ofstream node_file(v_file);
  int count = 0;
  std::vector<std::string> node_col_name, col_value_type;
  std::vector<VertexAttributeKeyType> node_attr_key;
  node_col_name.push_back("node_id");
  node_col_name.push_back("node_label");
  // get ID and Label Type
  int has_vertex_flag = 0;
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    VertexConstPtr node_ptr = node_it;
    col_value_type.push_back(TypeToString<VertexIDType>());
    col_value_type.push_back(TypeToString<VertexLabelType>());
    has_vertex_flag = 1;
    break;
  }
  if (!has_vertex_flag) {
    return 0;
  }
  // Get Attribute value type
  if (graph.vertex_has_attribute) {
    GetVertexAttributeValueType(graph, node_col_name, col_value_type,
                                node_attr_key);
  }
  // cout col
  WriteCSVCol(node_file, node_col_name, col_value_type);
  // cout each vertex
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    count++;
    std::vector<std::string> line_node_info;
    line_node_info.resize(node_col_name.size());
    line_node_info[0] = ToString(node_it->id());
    line_node_info[1] = ToString(node_it->label());
    VertexConstPtr node_ptr = node_it;
    if (graph.vertex_has_attribute) {
      WriteVertexAttribute(graph, node_ptr, node_attr_key, col_value_type,
                           line_node_info);
    }
    WriteLine(node_file, line_node_info);
  }
  // write .e file
  std::ofstream edge_file(e_file);

  std::vector<std::string> edge_col_name, edge_col_value_type;
  std::vector<EdgeAttributeKeyType> edge_attr_key;
  edge_col_name.push_back("edge_id");
  edge_col_name.push_back("source_node_id");
  edge_col_name.push_back("target_node_id");
  edge_col_name.push_back("edge_label_id");
  // get ID and Label Type
  int insert_edge_flag = 0;
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    VertexConstPtr node_ptr = node_it;
    for (auto edge_it = node_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      EdgeConstPtr edge_ptr = edge_it;
      edge_col_value_type.push_back(TypeToString<EdgeIDType>());
      edge_col_value_type.push_back(TypeToString<VertexIDType>());
      edge_col_value_type.push_back(TypeToString<VertexIDType>());
      edge_col_value_type.push_back(TypeToString<EdgeLabelType>());
      insert_edge_flag = 1;
      break;
    }
    if (insert_edge_flag) break;
  }
  if (!insert_edge_flag) {
    return count;
  }
  // std::cout << "size=" << edge_col_value_type.size() << std::endl;
  // Get Attribute value type
  if (graph.edge_has_attribute) {
    GetEdgeAttributeValueType(graph, edge_col_name, edge_col_value_type,
                              edge_attr_key);
  }
  // cout col
  WriteCSVCol(edge_file, edge_col_name, edge_col_value_type);
  // cout each edge
  for (auto node_it = graph.VertexCBegin(); !node_it.IsDone(); node_it++) {
    VertexConstPtr node_ptr = node_it;
    for (auto edge_it = node_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      count++;
      std::vector<std::string> line_edge_info;
      line_edge_info.resize(edge_col_name.size());
      line_edge_info[0] = ToString(edge_it->id());
      line_edge_info[1] = ToString(edge_it->const_src_ptr()->id());
      line_edge_info[2] = ToString(edge_it->const_dst_ptr()->id());
      line_edge_info[3] = ToString(edge_it->label());
      EdgeConstPtr edge_ptr = edge_it;
      if (graph.edge_has_attribute) {
        WriteEdgeAttribute(graph, edge_ptr, edge_attr_key, edge_col_value_type,
                           line_edge_info);
      }
      WriteLine(edge_file, line_edge_info);
    }
  }
  return count;
}
}  // namespace GUNDAM

#endif
