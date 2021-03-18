#ifndef _GUNDAM_ALGORITHM_BFS_H
#define _GUNDAM_ALGORITHM_BFS_H

#include <queue>
#include <set>
#include <type_traits>

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

namespace GUNDAM {

// legal prune_callback forms:
//    prune_callback(next_vertex_handle)
//    prune_callback(next_vertex_handle, current_bfs_idx)
//    prune_callback(next_vertex_handle, current_bfs_idx, current_distance)
//    prune_callback(next_vertex_handle, next_edge_handle)
//    prune_callback(next_vertex_handle, next_edge_handle, current_bfs_idx)
//    prune_callback(next_vertex_handle, next_edge_handle, current_bfs_idx, current_distance)
//
// legal callback forms:
//    user_callback(current_vertex_handle)
//    user_callback(current_vertex_handle, current_bfs_idx)
//    user_callback(current_vertex_handle, current_bfs_idx, current_distance)
template <bool bidirectional = false,
          typename         GraphType,
          typename  UserCallBackType,
          typename PruneCallBackType,
          typename std::enable_if_t< 
                   // prune_callback(next_vertex_handle)
                   std::is_convertible_v<
                            PruneCallBackType, 
                            std::function<bool(typename VertexHandle<GraphType>::type)> >
                || // prune_callback(next_vertex_handle, current_bfs_idx)
                   std::is_convertible_v<
                            PruneCallBackType, 
                            std::function<bool(typename VertexHandle<GraphType>::type,
                                               typename  GraphType::VertexCounterType)> >
                || // prune_callback(next_vertex_handle, current_bfs_idx, current_distance)
                   std::is_convertible_v<
                            PruneCallBackType, 
                            std::function<bool(typename VertexHandle<GraphType>::type,
                                               typename  GraphType::VertexCounterType,
                                               typename  GraphType::VertexCounterType)> >
                || // prune_callback(next_vertex_handle, next_edge_handle)
                   std::is_convertible_v<
                            PruneCallBackType, 
                            std::function<bool(typename VertexHandle<GraphType>::type, 
                                               typename   EdgeHandle<GraphType>::type)> >
                || // prune_callback(next_vertex_handle, next_edge_handle, current_bfs_idx)
                   std::is_convertible_v<
                            PruneCallBackType, 
                            std::function<bool(typename VertexHandle<GraphType>::type, 
                                               typename   EdgeHandle<GraphType>::type,
                                               typename  GraphType::VertexCounterType)> >
                || // prune_callback(next_vertex_handle, next_edge_handle, current_bfs_idx, current_distance)
                   std::is_convertible_v<
                            PruneCallBackType, 
                            std::function<bool(typename VertexHandle<GraphType>::type, 
                                               typename   EdgeHandle<GraphType>::type,
                                               typename  GraphType::VertexCounterType,
                                               typename  GraphType::VertexCounterType)> >, bool> = false>
inline size_t Bfs(GraphType&  graph,
  const std::set<typename VertexHandle<GraphType>::type>& src_vertex_handle_set,
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
    || // user_callback(vertex_handle, bfs_idx)
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType, 
                                   VertexCounterType)> >
    || // user_callback(vertex_handle, bfs_idx, distance)
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType, 
                                   VertexCounterType, 
                                   VertexCounterType)> >, 
      "illegal callback type, only allows one of user_callback(vertex_handle), user_callback(vertex_handle, bfs_idx) and user_callback(vertex_handle, bfs_idx, distance)");

  static_assert(
       // prune_callback(next_vertex_handle)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType)> >
    || // prune_callback(next_vertex_handle, current_bfs_idx)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType,
                                  VertexCounterType)> >
    || // prune_callback(next_vertex_handle, current_bfs_idx, current_distance)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType,
                                  VertexCounterType,
                                  VertexCounterType)> >
    || // prune_callback(next_vertex_handle, next_edge_handle)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType, 
                                     EdgeHandleType)> >
    || // prune_callback(next_vertex_handle, next_edge_handle, current_bfs_idx)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType, 
                                     EdgeHandleType,
                                  VertexCounterType)> >
    || // prune_callback(next_vertex_handle, next_edge_handle, current_bfs_idx, current_distance)
      std::is_convertible_v<
                PruneCallBackType, 
                std::function<bool(VertexHandleType, 
                                     EdgeHandleType,
                                  VertexCounterType,
                                  VertexCounterType)> >,
      "illegal callback type, only allows one of prune_callback(vertex_handle) and prune_callback(vertex_handle, edge_handle)");

  VertexCounterType bfs_idx = 0;
  std::queue<std::pair<VertexHandleType, 
                       VertexCounterType>> vertex_handle_queue;
  std:: set <VertexHandleType> visited;
  for (const auto& src_vertex_handle 
                 : src_vertex_handle_set){
    vertex_handle_queue.emplace(src_vertex_handle, 0);
    visited.emplace(src_vertex_handle);
  }
  while (!vertex_handle_queue.empty()){
    auto [current_vertex_handle, 
          current_distance] = vertex_handle_queue.front();
    vertex_handle_queue.pop();
    bfs_idx++;
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
                          bfs_idx);
    }
    if constexpr (
      std::is_convertible_v<
                UserCallBackType, 
                std::function<bool(VertexHandleType,
                                   VertexCounterType,
                                   VertexCounterType)> >){
      ret = user_callback(current_vertex_handle,
                          bfs_idx,
                          current_distance);
    }
    if (!ret){
      // meets stopping condition, stop the matching process
      return bfs_idx;
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
        std::is_convertible_v<
                  PruneCallBackType, 
                  std::function<bool(VertexHandleType)> >){
        prune_ret = prune_callback(edge_it->dst_ptr());
      }
      if constexpr (
        std::is_convertible_v<
                  PruneCallBackType, 
                  std::function<bool(VertexHandleType,
                                    VertexCounterType)> >){
        prune_ret = prune_callback(edge_it->dst_ptr(), bfs_idx);
      }
      if constexpr (
        std::is_convertible_v<
                  PruneCallBackType, 
                  std::function<bool(VertexHandleType,
                                    VertexCounterType,
                                    VertexCounterType)> >){
        prune_ret = prune_callback(edge_it->dst_ptr(), bfs_idx, current_distance);
      }

      // if constexpr (
      //   std::is_convertible_v<
      //             PruneCallBackType, 
      //             std::function<bool(VertexHandleType, 
      //                                 EdgeHandleType)> >){
      //   prune_ret = prune_callback(edge_it->dst_ptr(),
      //                              edge_it);
      // }
      // if constexpr (
      //   std::is_convertible_v<
      //             PruneCallBackType, 
      //             std::function<bool(VertexHandleType, 
      //                                   EdgeHandleType,
      //                               VertexCounterType)> >){
      //   prune_ret = prune_callback(edge_it->dst_ptr(),
      //                              edge_it, bfs_idx);
      // }
      // if constexpr (
      //   std::is_convertible_v<
      //             PruneCallBackType, 
      //             std::function<bool(VertexHandleType, 
      //                                   EdgeHandleType,
      //                               VertexCounterType,
      //                               VertexCounterType)> >){
      //   prune_ret = prune_callback(edge_it->dst_ptr(),
      //                              edge_it, bfs_idx, current_distance);
      // }
      if (prune_ret){
        // this vertex is pruned, does not be considered
        continue;
      }
      visited.emplace(edge_it->dst_ptr());
      vertex_handle_queue.emplace(edge_it->dst_ptr(), current_distance + 1);
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
                                      VertexCounterType)> >){
          prune_ret = prune_callback(edge_it->src_ptr(), bfs_idx);
        }
        if constexpr (
          std::is_convertible_v<
                    PruneCallBackType, 
                    std::function<bool(VertexHandleType,
                                      VertexCounterType,
                                      VertexCounterType)> >){
          prune_ret = prune_callback(edge_it->src_ptr(), bfs_idx, current_distance);
        }

        // if constexpr (
        //   std::is_convertible_v<
        //             PruneCallBackType, 
        //             std::function<bool(VertexHandleType, 
        //                                 EdgeHandleType)> >){
        //   prune_ret = prune_callback(edge_it->src_ptr(),
        //                              edge_it);
        // }
        // if constexpr (
        //   std::is_convertible_v<
        //             PruneCallBackType, 
        //             std::function<bool(VertexHandleType, 
        //                                  EdgeHandleType,
        //                               VertexCounterType)> >){
        //   prune_ret = prune_callback(edge_it->src_ptr(),
        //                              edge_it, bfs_idx);
        // }
        // if constexpr (
        //   std::is_convertible_v<
        //             PruneCallBackType, 
        //             std::function<bool(VertexHandleType, 
        //                                  EdgeHandleType,
        //                               VertexCounterType,
        //                               VertexCounterType)> >){
        //   prune_ret = prune_callback(edge_it->src_ptr(),
        //                              edge_it, bfs_idx, current_distance);
        // }

        if (prune_ret){
          // this vertex is pruned, does not be considered
          continue;
        }
        visited.emplace(edge_it->src_ptr());
        vertex_handle_queue.emplace(edge_it->src_ptr(), current_distance + 1);
      }
    }
  }
  return bfs_idx;
}

template <bool bidirectional = false,
          typename        GraphType,
          typename UserCallBackType>
inline size_t Bfs(GraphType&  graph,
  const std::set<typename VertexHandle<GraphType>::type>& src_vertex_handle_set,
           UserCallBackType  user_callback,
           int distance_limit = -1) {
  using VertexCounterType = typename  GraphType::VertexCounterType;
  using  VertexHandleType = typename VertexHandle<GraphType>::type;
  auto prune_distance_callback = [&distance_limit](
         VertexHandleType vertex_handle,
        VertexCounterType current_idx,
        VertexCounterType current_distance){
    if (distance_limit == -1){
      // distance limit is not set, prune nothing
      return false;
    }
    assert(current_distance <= distance_limit);
    if (current_distance == distance_limit) {
      // reach the distance limit, prune this vertex
      return true;
    }
    // does not reach the limit, do not prune
    return false;
  };
  return Bfs<bidirectional>(graph,
                src_vertex_handle_set, 
                user_callback, 
      prune_distance_callback); 
}

template<bool bidirectional = false,
         typename GraphType,
         typename UserCallBackType>
inline size_t Bfs(GraphType& graph,
        const typename VertexHandle<GraphType>::type& src_vertex_handle,
           UserCallBackType user_callback,
           int distance_limit = -1) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::set<VertexHandleType> src_vertex_handle_set 
                          = {src_vertex_handle};
  return Bfs<bidirectional>(graph, src_vertex_handle_set, user_callback, distance_limit);
}

template<bool bidirectional = false,
         typename GraphType>
inline size_t Bfs(GraphType& graph,
        const typename VertexHandle<GraphType>::type& src_vertex_handle,
           int distance_limit = -1) {

  using VertexHandleType = typename VertexHandle<GraphType>::type;
  auto do_nothing_callback = [](VertexHandleType vertex_handle, 
                                typename GraphType::VertexCounterType current_distance){
    // do nothing, continue matching
    return true;
  };
  return Bfs<bidirectional>(graph, src_vertex_handle, do_nothing_callback, distance_limit);
}

}  // namespace GUNDAM

#endif // _BFS_H
