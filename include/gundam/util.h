#ifndef _UTIL_H
#define _UTIL_H

#include "graph.h"

#include <iostream>

namespace GUNDAM {

template <template <typename...> class GraphType, typename... configures>
void PrintGraph(const GraphType<configures...>& graph) {
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
  std::cout << "vertex:\n";
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    std::cout << it->id() << " " << it->label() << " ";
    VertexConstPtr node_ptr = it;
    if (graph.vertex_has_attribute) {
      for (auto attr_it = node_ptr->AttributeCBegin(); !attr_it.IsDone();
           attr_it++) {
        enum BasicDataType value_type = it->GetAttributeValueTypeID(attr_it->key());
        std::cout << attr_it->key() << ":";
        switch (value_type) {
          case BasicDataType::int_:
            std::cout << attr_it->template const_value<int>() << " ";
            break;
          case BasicDataType::double_:
            std::cout << attr_it->template const_value<double>() << " ";
            break;
          case BasicDataType::long_:
            std::cout << attr_it->template const_value<long long>() << " ";
            break;
          case BasicDataType::short_:
            std::cout << attr_it->template const_value<short>() << " ";
            break;
          case BasicDataType::string_:
            std::cout << attr_it->template const_value<std::string>() << " ";
            break;
          default:
            break;
        }
      }
    }
    std::cout << std::endl;
  }
  std::cout << "edge:\n";
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    for (auto edge_it = graph.FindConstVertex(it->id())->OutEdgeCBegin();
         !edge_it.IsDone(); edge_it++) {
      std::cout << edge_it->const_src_ptr()->id() << " "
                << edge_it->const_dst_ptr()->id() << " " << edge_it->label()
                << " ";
      if (graph.edge_has_attribute) {
        EdgeConstPtr edge_ptr = edge_it;
        for (auto attr_it = edge_ptr->AttributeCBegin(); !attr_it.IsDone();
             attr_it++) {
          enum BasicDataType value_type =
              edge_it->GetAttributeValueTypeID(attr_it->key());
          std::cout << attr_it->key() << ":";
          switch (value_type) {
            case BasicDataType::int_:
              std::cout << attr_it->template const_value<int>() << " ";
              break;
            case BasicDataType::double_:
              std::cout << attr_it->template const_value<double>() << " ";
              break;
            case BasicDataType::long_:
              std::cout << attr_it->template const_value<long long>() << " ";
              break;
            case BasicDataType::short_:
              std::cout << attr_it->template const_value<short>() << " ";
              break;
            case BasicDataType::string_:
              std::cout << attr_it->template const_value<std::string>() << " ";
              break;
            default:
              break;
          }
        }
      }
      std::cout << std::endl;
    }
  }
}
}  // namespace GUNDAM

#endif