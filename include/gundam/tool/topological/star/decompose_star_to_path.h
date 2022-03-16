#ifndef _GUNDAM_TOOL_TOPOLOGICAL_STAR_DECOMPOSE_STAR_TO_PATH_H
#define _GUNDAM_TOOL_TOPOLOGICAL_STAR_DECOMPOSE_STAR_TO_PATH_H


#include "gundam/tool/topological/star/is_star.h"
#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"

#include <vector>
#include <set>
#include <iostream>

namespace GUNDAM {

template <bool bidirectional = false,
          typename GraphType>
std::vector<GraphType> DecomposeStarToPath(GraphType& input_star) {
  // static constexpr bool bidirectional = true;
  static_assert(bidirectional, "have not implemented bidirectional=false yet!");  // currently supports undirected-graph only

  using VertexHandleType = typename VertexHandle<GraphType>::type;
  using EdgeHandleType = typename EdgeHandle<GraphType>::type;

  std::vector<GraphType> ret_paths;

  auto [leaf_vertex_handle_vec, centre_vertex_handle] =
        StarEndPoints<bidirectional>(input_star);

  if (leaf_vertex_handle_vec.empty()) {
    assert(!centre_vertex_handle);
    return ret_paths;
  }

  if (!centre_vertex_handle) {
    ret_paths.emplace_back(input_star);
    return ret_paths;
  }

  std::set<VertexHandleType> leaf_vertex_handle_set(
             leaf_vertex_handle_vec.begin(),
             leaf_vertex_handle_vec.end());


  if constexpr (bidirectional) {
    for (auto in_edge_it = centre_vertex_handle->InEdgeBegin();
              !in_edge_it.IsDone();
              in_edge_it++) {
      GraphType decomposed_path;

      VertexHandleType current_v_handle = in_edge_it->src_handle();
      VertexHandleType prev_v_handle = centre_vertex_handle;

      auto vertex_res = decomposed_path.AddVertex(prev_v_handle->id(), prev_v_handle->label());
      assert(vertex_res.first && vertex_res.second);

      vertex_res = decomposed_path.AddVertex(current_v_handle->id(), current_v_handle->label());
      assert(vertex_res.first && vertex_res.second);

      auto edge_res = decomposed_path.AddEdge(current_v_handle->id(), prev_v_handle->id(),
                              in_edge_it->label(), in_edge_it->id());
      assert(edge_res.first && edge_res.second);

      while (leaf_vertex_handle_set.find(current_v_handle)
                == leaf_vertex_handle_set.end()) {
        
        bool found = false;
        for (auto c_in_edge_it = current_v_handle->InEdgeBegin();
                  !c_in_edge_it.IsDone();
                  c_in_edge_it++) {
          if (c_in_edge_it->src_handle() == prev_v_handle) continue;

          prev_v_handle = current_v_handle;
          current_v_handle = c_in_edge_it->src_handle();

          vertex_res = decomposed_path.AddVertex(current_v_handle->id(), current_v_handle->label());
          assert(vertex_res.first && vertex_res.second);

          edge_res = decomposed_path.AddEdge(current_v_handle->id(), prev_v_handle->id(),
                                  c_in_edge_it->label(), c_in_edge_it->id());
          assert(edge_res.first && edge_res.second);

          bool found = true;
          break;
        }

        if (found) continue;

        for (auto c_out_edge_it = current_v_handle->OutEdgeBegin();
                  !c_out_edge_it.IsDone();
                  c_out_edge_it++) {
          if (c_out_edge_it->dst_handle() == prev_v_handle) continue;

          prev_v_handle = current_v_handle;
          current_v_handle = c_out_edge_it->dst_handle();

          vertex_res = decomposed_path.AddVertex(current_v_handle->id(), current_v_handle->label());
          assert(vertex_res.first && vertex_res.second);

          edge_res = decomposed_path.AddEdge(prev_v_handle->id(), current_v_handle->id(),
                                  c_out_edge_it->label(), c_out_edge_it->id());
          assert(edge_res.first && edge_res.second);

          bool found = true;
          break;
        }
      }

      ret_paths.emplace_back(decomposed_path);
    }
  }

  for (auto out_edge_it = centre_vertex_handle->OutEdgeBegin();
            !out_edge_it.IsDone();
            out_edge_it++) {
    GraphType decomposed_path;

    VertexHandleType current_v_handle = out_edge_it->dst_handle();
    VertexHandleType prev_v_handle = centre_vertex_handle;

    auto vertex_res = decomposed_path.AddVertex(prev_v_handle->id(), prev_v_handle->label());
    assert(vertex_res.first && vertex_res.second);

    vertex_res = decomposed_path.AddVertex(current_v_handle->id(), current_v_handle->label());
    assert(vertex_res.first && vertex_res.second);

    auto edge_res = decomposed_path.AddEdge(prev_v_handle->id(), current_v_handle->id(),
                            out_edge_it->label(), out_edge_it->id());
    assert(edge_res.first && edge_res.second);

    while (leaf_vertex_handle_set.find(current_v_handle)
            == leaf_vertex_handle_set.end()) {
      bool found = false;

      if constexpr (bidirectional) {
        for (auto c_in_edge_it = current_v_handle->InEdgeBegin();
                  !c_in_edge_it.IsDone();
                  c_in_edge_it++) {
          if (c_in_edge_it->src_handle() == prev_v_handle) continue;

          prev_v_handle = current_v_handle;
          current_v_handle = c_in_edge_it->src_handle();

          vertex_res = decomposed_path.AddVertex(current_v_handle->id(), current_v_handle->label());
          assert(vertex_res.first && vertex_res.second);

          edge_res = decomposed_path.AddEdge(current_v_handle->id(), prev_v_handle->id(),
                                c_in_edge_it->label(), c_in_edge_it->id());
          assert(edge_res.first && edge_res.second);

          bool found = true;
          break;
        }
      }

      if (found) continue;

      for (auto c_out_edge_it = current_v_handle->OutEdgeBegin();
                !c_out_edge_it.IsDone();
                c_out_edge_it++) {
        if (c_out_edge_it->dst_handle() == prev_v_handle) continue;

          prev_v_handle = current_v_handle;
          current_v_handle = c_out_edge_it->dst_handle();

          vertex_res = decomposed_path.AddVertex(current_v_handle->id(), current_v_handle->label());
          assert(vertex_res.first && vertex_res.second);

          edge_res = decomposed_path.AddEdge(prev_v_handle->id(), current_v_handle->id(),
                                c_out_edge_it->label(), c_out_edge_it->id());
          assert(edge_res.first && edge_res.second);
          
          bool found = true;
          break;
        }
    }

    ret_paths.emplace_back(decomposed_path);
  }

  return ret_paths;
}
}  // namespace GUNDAM
#endif  // _GUNDAM_TOOL_TOPOLOGICAL_STAR_DECOMPOSE_STAR_TO_PATH_H