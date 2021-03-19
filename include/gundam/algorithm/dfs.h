#ifndef _GUNDAM_ALGORITHM_DFS_H
#define _GUNDAM_ALGORITHM_DFS_H

#include <stack>
#include <set>
#include <type_traits>

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

namespace GUNDAM {

// legal prune_callback forms:
//    prune_callback(vertex_handle)
//    prune_callback(vertex_handle, edge_handle)
//    prune_callback(vertex_handle, edge_handle, dfs_idx)
//
// legal user_callback forms:
//    user_callback(vertex_handle)
//    user_callback(vertex_handle, dfs_idx)
template <bool bidirectional = false,
          typename         GraphType,
          typename  UserCallBackType,
          typename PruneCallBackType>
inline size_t Dfs(GraphType& graph,
              typename VertexHandle<GraphType>::type& src_vertex_handle,
           UserCallBackType  user_callback,
          PruneCallBackType prune_callback) {
  using VertexCounterType = typename  GraphType::VertexCounterType;
  using  VertexHandleType = typename VertexHandle<GraphType>::type;
  using    EdgeHandleType = typename   EdgeHandle<GraphType>::type;
  static_assert(
       // user_callback(vertex_handle)
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType)> >
    || // user_callback(vertex_handle, dfs_idx)
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType, 
                                   VertexCounterType)> >,
      "illegal callback type, only allows one of user_callback(vertex_handle) and user_callback(vertex_handle, dfs_idx)");

  static_assert(
       // prune_callback(vertex_handle)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType)> >
    || // prune_callback(vertex_handle, edge_handle)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType, 
                                     EdgeHandleType)> >
    || // prune_callback(vertex_handle, edge_handle, dfs_idx)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType, 
                                     EdgeHandleType,
                                  VertexCounterType)> >,
      "illegal callback type, only allows one of prune_callback(vertex_handle), prune_callback(vertex_handle, edge_handle) and prune_callback(vertex_handle, edge_handle, dfs_idx)");

  VertexCounterType dfs_idx = 0;
  std::stack<VertexHandleType> vertex_handle_stack;
  std:: set <VertexHandleType> visited;
  vertex_handle_stack.emplace(src_vertex_handle);
  visited.emplace(src_vertex_handle);
  while (!vertex_handle_stack.empty()) {
    auto current_vertex_handle = vertex_handle_stack.top();
    vertex_handle_stack.pop();
    bool ret = false;
    if constexpr (
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType)> >){
      ret = user_callback(current_vertex_handle);
    }
    if constexpr (
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType,
                                  VertexCounterType)> >){
      ret = user_callback(current_vertex_handle,
                          dfs_idx);
    }
    dfs_idx++;
    if (!ret){
      // meets stopping condition, stop the matching process
      return dfs_idx;
    }
    for (auto edge_it = current_vertex_handle->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      if (visited.find(edge_it->dst_ptr()) != visited.end()){
        // already visited
        continue;
      }
      bool prune_ret = false;
      if constexpr (
        // prune_callback(vertex_handle)
        std::is_convertible_v<
                  PruneCallBackType, 
                  std::function<bool(VertexHandleType)> >){
        prune_ret = prune_callback(edge_it->dst_ptr());
      }
      if constexpr (
        // prune_callback(vertex_handle, edge_handle)
        std::is_convertible_v<
                  PruneCallBackType, 
                  std::function<bool(VertexHandleType, 
                                       EdgeHandleType)> >){
        prune_ret = prune_callback(edge_it->dst_ptr(),
                                   edge_it);
      }
      if constexpr (
        // prune_callback(vertex_handle, edge_handle, dfs_idx)
        std::is_convertible_v<
                  PruneCallBackType, 
                  std::function<bool(VertexHandleType, 
                                       EdgeHandleType,
                                    VertexCounterType)> >){
        prune_ret = prune_callback(edge_it->dst_ptr(),
                                   edge_it, dfs_idx);
      }
      if (prune_ret){
        // this vertex is pruned, does not be considered
        continue;
      }
      visited.emplace(edge_it->dst_ptr());
      vertex_handle_stack.emplace(edge_it->dst_ptr());
    }
    if constexpr (bidirectional){
      for (auto edge_it = current_vertex_handle->InEdgeBegin();
               !edge_it.IsDone();
                edge_it++) {
        if (visited.find(edge_it->src_ptr()) != visited.end()){
          // already visited
          continue;
        }
        bool prune_ret = false;
        if constexpr (
          std::is_convertible_v<
                    PruneCallBackType, 
                    std::function<bool(VertexHandleType)> >){
          prune_ret = prune_callback(edge_it->src_ptr());
        }
        if constexpr (
          std::is_convertible_v<
                    PruneCallBackType, 
                    std::function<bool(VertexHandleType, 
                                        EdgeHandleType)> >){
          prune_ret = prune_callback(edge_it->src_ptr(),
                                     edge_it);
        }
        if (prune_ret){
          // this vertex is pruned, does not be considered
          continue;
        }
        visited.emplace(edge_it->src_ptr());
        vertex_handle_stack.emplace(edge_it->src_ptr());
      }
    }
  }
  return dfs_idx;
}

template <bool bidirectional = false,
          typename        GraphType,
          typename UserCallBackType>
inline size_t Dfs(GraphType& graph,
              typename VertexHandle<GraphType>::type& src_vertex_handle,
           UserCallBackType  user_callback) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  auto prune_nothing_callback = [](VertexHandleType vertex_handle){
    // prune nothing, continue matching
    return false;
  };
  return Dfs<bidirectional>(graph,
                src_vertex_handle, 
                    user_callback, 
           prune_nothing_callback); 
}

template<bool bidirectional = false,
         typename GraphType>
inline size_t Dfs(GraphType& graph,
              typename VertexHandle<GraphType>::type& src_vertex_handle) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  auto do_nothing_callback = [](const VertexHandleType& vertex_handle, 
                                const size_t&           dfs_idx){
    // do nothing, continue matching
    return true;
  };
  return Dfs<bidirectional>(graph, src_vertex_handle, do_nothing_callback);
}

}  // namespace GUNDAM

#endif // _DFS_H
