#ifndef _GUNDAM_TOOL_FIND_EDGE_H
#define _GUNDAM_TOOL_FIND_EDGE_H

#include "include/gundam/type_getter/vertex_handle.h"
#include "include/gundam/type_getter/edge_handle.h"
#include "include/gundam/type_getter/edge_label.h"

namespace GUNDAM {

// return a handle to (one of) the edge(s) from 
// src_handle to dst_handle with label edge_label
template <typename GraphType,
          bool bidirectional = false>
inline typename EdgeHandle<GraphType>::type 
            FindEdge(typename VertexHandle<GraphType>::type src_handle,
                     typename    EdgeLabel<GraphType>::type edge_label,
                     typename VertexHandle<GraphType>::type dst_handle) {

  // wenzhi:
  // ## optimize me ##################################################
  // #  compare the number of the input edges of dst_handle and the  #
  // #  output edges of src_hanlde, iterate the smaller one          #
  // #################################################################

  assert(src_handle);
  assert(dst_handle);

  using EdgeHandleType = typename EdgeHandle<GraphType>::type;

  // wenzhi:
  // ## optimize me #############################
  // #  use vertex level EdgeLabelIdx if it has #
  // ############################################
  for (auto out_edge_it = src_handle->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    if (out_edge_it->label() != edge_label) {
      continue;
    }
    if (out_edge_it->dst_handle() != dst_handle) {
      continue;
    }
    return out_edge_it;
  }
  if constexpr (bidirectional) {
    for (auto in_edge_it = src_handle->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      if (in_edge_it->label() != edge_label) {
        continue;
      }
      if (in_edge_it->dst_handle() != dst_handle) {
        continue;
      }
      return in_edge_it;
    }
  }
  // does not find edge
  assert(!EdgeHandleType());
  return  EdgeHandleType();
}

// return a handle to (one of) the edge(s) from 
// src_handle to dst_handle with any edge_label
template <typename GraphType,
          bool bidirectional = false>
inline typename EdgeHandle<GraphType>::type 
            FindEdge(typename VertexHandle<GraphType>::type src_handle,
                     typename VertexHandle<GraphType>::type dst_handle) {

  // wenzhi:
  // ## optimize me ##################################################
  // #  compare the number of the input edges of dst_handle and the  #
  // #  output edges of src_hanlde, iterate the smaller one          #
  // #################################################################

  assert(src_handle);
  assert(dst_handle);

  using EdgeHandleType = typename EdgeHandle<GraphType>::type;

  for (auto out_edge_it = src_handle->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    if (out_edge_it->dst_handle() != dst_handle) {
      continue;
    }
    return out_edge_it;
  }
  if constexpr (bidirectional) {
    for (auto in_edge_it = src_handle->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      if (in_edge_it->dst_handle() != dst_handle) {
        continue;
      }
      return in_edge_it;
    }
  }
  // does not find edge
  assert(!EdgeHandleType());
  return  EdgeHandleType();
}

};

#endif // _GUNDAM_TOOL_FIND_EDGE_H
