#ifdef _GUNDAM_TOOL_RANDOM_WALK_RANDOM_WALK_PATH_H
#define _GUNDAM_TOOL_RANDOM_WALK_RANDOM_WALK_PATH_H

#include <queue>
#include <set>
#include <type_traits>

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

#include "gundam/algorithm/bfs.h"

namespace GUNDAM{

for (auto src_vertex_handle : )  {
  std::vector<GrpahType> path;
  for (size_t path_idx = ) {
    path[path_idx] = RandomWalkPath(data_graph, 
                              src_vertex_handle,
                                    length);
  }
  same_vertex_id_set = {src_vertex_handle->id()};
  auto star = JoinGraph(same_vertex_id_set,
                        path);
}

// return a path with given length (vertex number)
// begins at the given src_vertex_handle
template <typename GraphType>
inline std::remove_cv_t<
       std::remove_reference_t<GraphType>>  // ok to return object with move in C++
                RandomWalkPath(GraphType&  graph,
         typename VertexHandle<GraphType>::type src_vertex_handle,
                      size_t length) {
  using ReturnGraphType = std::remove_cv_t<
                   std::remove_reference_t<GraphType>>;

  using VertexHandleType = typename VertexHandle<GraphType>::type;

  assert(length > 0);
  
  ReturnGraphType return_path;

  std::set<VertexHandleType> visited_vertex_set;

  auto prune_callback = [&visited_vertex_set](
         VertexHandleType vertex_handle){
    if (visited_vertex_set.find(vertex_handle) 
     != visited_vertex_set.end()){
      // this vertex has been visited
      return true;
    }
    return false;
  };

  auto random_walk_callback= [&user_callback,
                              &visited_vertex,
                              &kSizeLimit](
              VertexHandleType vertex_handle) {
    assert(visited_vertex.size() < kSizeLimit);
    visited_vertex.emplace(vertex_handle);
    bool user_callback_ret = user_callback(vertex_handle);
    return (visited_vertex.size() < kSizeLimit)
         && user_callback_ret;
  };

  assert(return_path.CountVertex() == length);
  return;
}

}; // namespace GUNDAM

#endif // _GUNDAM_TOOL_RANDOM_WALK_RANDOM_WALK_PATH_H