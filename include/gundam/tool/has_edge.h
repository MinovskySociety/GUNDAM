#ifndef _GUNDAM_TOOL_HAS_EDGE_H
#define _GUNDAM_TOOL_HAS_EDGE_H

#include "include/gundam/type_getter/vertex_handle.h"
#include "include/gundam/type_getter/edge_handle.h"
#include "include/gundam/type_getter/edge_label.h"

namespace GUNDAM {

// wenzhi:
// ## optimize me ##################################################
// #  compare the number of the input edges of dst_handle and the  #
// #  output edges of src_hanlde, iterate the smaller one          #
// #################################################################
template <typename GraphType>
inline bool HasEdge(typename VertexHandle<GraphType>::type src_handle,
                    typename    EdgeLabel<GraphType>::type edge_label,
                    typename VertexHandle<GraphType>::type dst_handle) {
  
  assert(src_handle);
  assert(dst_handle);

  using EdgeHandleType = typename EdgeHandle<GraphType>::type;

  for (auto out_edge_it = src_handle->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    if (out_edge_it->label() != edge_label) {
      continue;
    }
    if (out_edge_it->dst_handle() != dst_handle) {
      continue;
    }
    return true;
  }
  // does not find edge
  return false;
};

};

#endif // _GUNDAM_TOOL_HAS_EDGE_H
