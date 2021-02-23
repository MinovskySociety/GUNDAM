#ifndef _BFS_H
#define _BFS_H
#include <cassert>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <type_traits>
#include <vector>

namespace GUNDAM {

template <typename        GraphType,
          typename    VertexPtrType,
          typename UserCallBackType>
inline size_t Bfs(GraphType&  graph,
     std::set<VertexPtrType>& src_vertex_ptr_set,
           UserCallBackType&  user_callback) {
  size_t counter = 0;
  std::queue<VertexPtrType> vertex_ptr_queue;
  std:: set <VertexPtrType> visited;
  for (const auto& src_vertex_ptr : src_vertex_ptr_set){
    vertex_ptr_queue.emplace(src_vertex_ptr);
    visited.emplace(src_vertex_ptr);
  }
  while (!vertex_ptr_queue.empty()){
    auto current_vertex_ptr = vertex_ptr_queue.front();
    vertex_ptr_queue.pop();
    counter++;
    user_callback(current_vertex_ptr);
    for (auto edge_it = current_vertex_ptr->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      if (visited.find(edge_it->dst_ptr()) != visited.end()){
        // already visited
        continue;
      }
      visited.emplace(edge_it->dst_ptr());
      vertex_ptr_queue.emplace(edge_it->dst_ptr());
    }
  }
  return counter;
}

template <typename        GraphType,
          typename    VertexPtrType,
          typename UserCallBackType>
inline size_t Bfs(GraphType& graph,
              VertexPtrType& src_vertex_ptr,
           UserCallBackType& user_callback) {
  std::set<VertexPtrType> src_vertex_ptr_set = {src_vertex_ptr};
  return Bfs(graph, src_vertex_ptr_set, user_callback);
}

}  // namespace GUNDAM
#endif // _BFS_H
