#ifndef _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H
#define _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H

namespace GUNDAM {

template <bool bidirectional = false,
          class GraphType>
std::vector<typename VertexHandle<GraphType>::type>  // okey to return vector after C11
  VertexDegreeFilter(GraphType& graph, size_t degree) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::vector<VertexHandleType> vertex_handle_set;
  return vertex_handle_set;
}

};

#endif // _GUNDAM_TOOL_VERTEX_DEGREE_FILTER_H