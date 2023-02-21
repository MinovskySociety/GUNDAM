#ifndef _GUNDAM_TOOL_EDGE_CONNECTS_H
#define _GUNDAM_TOOL_EDGE_CONNECTS_H

#include "include/gundam/type_getter/vertex_handle.h"
#include "include/gundam/type_getter/edge_handle.h"

#include <vector>
#include <unordered_set>

namespace GUNDAM {

// wenzhi: to test & optimize me!
// ok to return vector after C11
template <typename GraphType, bool bidirectional = false>
std::vector<typename EdgeHandle<GraphType>::type> 
  EdgesConnect(const std::vector<typename VertexHandle<GraphType>::type>& src_handle_set,
        const std::unordered_set<typename VertexHandle<GraphType>::type>& dst_handle_set) {
  std::vector<typename EdgeHandle<GraphType>::type> connected_edge_set;
  for (const auto& src_handle : src_handle_set) {
    // first consider the edges from src_handle_set toward dst_handle_set
    for (auto out_edge_it = src_handle->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      if (dst_handle_set.find(out_edge_it->dst_handle())
       == dst_handle_set.end()) {
        // this edge does not connect the two set of vertex set
        continue;
      }
      connected_edge_set.emplace_back(out_edge_it);
    }
    
    if constexpr (bidirectional) {
      // consider the edges from dst_handle_set toward src_handle_set
      for (auto in_edge_it = src_handle->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        if (dst_handle_set.find(in_edge_it->src_handle())
         == dst_handle_set.end()) {
          // this edge does not connect the two set of vertex set
          continue;
        }
        connected_edge_set.emplace_back(in_edge_it);
      }
    }
  }
  return connected_edge_set;
}

template <bool bidirectional = false, typename GraphType>
std::vector<typename EdgeHandle<GraphType>::type> 
  EdgesConnect(GraphType& graph,
               // there are not suppose to have duplicated vertexes in both src_id_set and dst_id_set
               const std::vector<typename VertexID<GraphType>::type>& src_id_set,
               const std::vector<typename VertexID<GraphType>::type>& dst_id_set) {

         std::vector<typename VertexHandle<GraphType>::type> src_handle_set;
  std::unordered_set<typename VertexHandle<GraphType>::type> dst_handle_set;

  #ifndef NDEBUG
  std::unordered_set<typename VertexID<GraphType>::type> temp_src_id_set;
  #endif // NDEBUG

  for (const auto& src_id : src_id_set) {
    #ifndef NDEBUG
    auto [it, ret] = temp_src_id_set.emplace(src_id);
    assert(ret);
    #endif // NDEBUG

    auto src_handle = graph.FindVertex(src_id);
    assert(src_handle);
    src_handle_set.emplace_back(src_handle);
  }

  for (const auto& dst_id : dst_id_set) {
    auto dst_handle = graph.FindVertex(src_id);
    assert(dst_handle);
    auto [it, ret] = dst_handle_set.emplace(dst_handle);
    assert(ret);
  }

  return EdgesConnect<GraphType, bidirectional>(src_handle_set, dst_handle_set);
}

};

#endif // _GUNDAM_TOOL_EDGE_CONNECTS_H
