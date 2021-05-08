#ifndef _GUNDAM_GRAPH_TYPE_GRAPH_PARAMETER_H
#define _GUNDAM_GRAPH_TYPE_GRAPH_PARAMETER_H

namespace GUNDAM{

class GraphParameter{
 public:
  // vertex level index
  static constexpr bool vertex_level_edge_label_index = false;

  // vertex level count methods
  static constexpr bool vertex_level_count_edge_label = false;
  static constexpr bool vertex_level_count_vertex     = false;
  static constexpr bool vertex_level_count_edge       = false;

  // vertex level vertex iterator
  static constexpr bool vertex_level_vertex_iterator = false;

  // vertex level find methods
  static constexpr bool vertex_level_find_vertex = false;
  static constexpr bool vertex_level_find_edge = false;

  // vertex level add/erase edge
  static constexpr bool vertex_level_add_edge = false;
  static constexpr bool vertex_level_erase_edge = false;

  static constexpr bool duplicate_edge = false;

  // attribute
  static constexpr bool vertex_has_attribute = false;
  static constexpr bool   edge_has_attribute = false;

  // graph level index
  static constexpr bool graph_level_vertex_label_index = false;

  // graph level count methods
  static constexpr bool graph_level_count_vertex = false;
  static constexpr bool graph_level_count_edge = false;

  // graph level erase methods
  static constexpr bool graph_level_erase_vertex = false;
  static constexpr bool graph_level_erase_edge = false;

  // graph level edge iterator
  static constexpr bool graph_level_edge_iterator = false;

  // graph level find methods
  static constexpr bool graph_level_find_vertex = false;
  static constexpr bool graph_level_find_edge = false;
};

};

#endif // _GUNDAM_GRAPH_TYPE_GRAPH_PARAMETER_H