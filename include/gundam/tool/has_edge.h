#ifndef _GUNDAM_TOOL_HAS_EDGE_H
#define _GUNDAM_TOOL_HAS_EDGE_H

#include "include/gundam/tool/find_edge.h"

namespace GUNDAM {

template <typename GraphType,
          bool bidirectional = false>
inline bool HasEdge(typename VertexHandle<GraphType>::type src_handle,
                    typename    EdgeLabel<GraphType>::type edge_label,
                    typename VertexHandle<GraphType>::type dst_handle) {
  
  return FindEdge<GraphType,
              bidirectional>(src_handle,
                             edge_label,
                             dst_handle);
};

template <typename GraphType,
          bool bidirectional = false>
inline bool HasEdge(typename VertexHandle<GraphType>::type src_handle,
                    typename VertexHandle<GraphType>::type dst_handle) {
  
  return FindEdge<GraphType,
              bidirectional>(src_handle,
                             dst_handle);
};

};

#endif // _GUNDAM_TOOL_HAS_EDGE_H
