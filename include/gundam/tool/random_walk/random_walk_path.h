#ifndef _GUNDAM_TOOL_RANDOM_WALK_RANDOM_WALK_PATH_H
#define _GUNDAM_TOOL_RANDOM_WALK_RANDOM_WALK_PATH_H

#include <queue>
#include <set>
#include <type_traits>

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"

#include "gundam/tool/random_walk/random_walk.h"

#ifndef NDEBUG
#include "gundam/tool/topological/path/is_path.h"
#include "gundam/tool/connected.h"
#endif // NDEBUG

namespace GUNDAM{

// for (auto src_vertex_handle : )  {
//   std::vector<GrpahType> path;
//   for (size_t path_idx = ) {
//     path[path_idx] = RandomWalkPath(data_graph, 
//                               src_vertex_handle,
//                                     length);
//   }
//   same_vertex_id_set = {src_vertex_handle->id()};
//   auto star = JoinGraph(same_vertex_id_set,
//                         path);
// }

// return a path with given length (vertex number)
// begins at the given src_vertex_handle
template <typename GraphType>
inline std::remove_cv_t<
       std::remove_reference_t<GraphType>>  // ok to return object with move in C++
                RandomWalkPath(GraphType&  data_graph,
         typename VertexHandle<GraphType>::type src_vertex_handle,
                      size_t length) {
  using ReturnGraphType = std::remove_cv_t<
                   std::remove_reference_t<GraphType>>;

  using VertexHandleType = typename VertexHandle<GraphType>::type;

  assert(length > 0 
      && length < data_graph.CountVertex());
  
  ReturnGraphType return_path;

  std::set<VertexHandleType> visited_vertex_set;

  auto prune_callback = [&visited_vertex_set](
        const VertexHandleType& vertex_handle){
    if (visited_vertex_set.find(vertex_handle) 
     != visited_vertex_set.end()){
      // this vertex has been visited
      return true;
    }
    return false;
  };

  VertexHandleType last_vertex_handle = VertexHandleType();
  assert(!last_vertex_handle);

  auto random_walk_callback= [&visited_vertex_set,
                              &return_path,
                              &length,
                              &last_vertex_handle](
            const VertexHandleType& vertex_handle) {
    auto [add_verterx_it,
          add_verterx_ret] = return_path.AddVertex(vertex_handle->id(),
                                                   vertex_handle->label());
    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      // add attribute to new_vertex
      for (auto attr_it = vertex_handle->AttributeBegin();
               !attr_it.IsDone();
                attr_it++){
        auto [attr_handle, attr_ret] = vertex_handle->AddAttribute(
                                        attr_it->key(),
                                        attr_it->value_type(),
                                        attr_it->value_str());
        assert(attr_handle);
        assert(attr_ret);
      }
    }


    assert(add_verterx_it->id()    == vertex_handle->id());
    assert(add_verterx_it->label() == vertex_handle->label());
    assert(add_verterx_ret);

    auto [ visited_vertex_it,
           visited_vertex_ret ] 
         = visited_vertex_set.emplace(vertex_handle);
    assert(visited_vertex_ret);
    assert(visited_vertex_set.size()
        == return_path.CountVertex());

    if (last_vertex_handle) /*[[likely]]*/ {
      for (auto out_edge_it = vertex_handle->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        if (out_edge_it->dst_handle() != last_vertex_handle) {
          continue;
        }
        auto [add_edge_it,
              add_edge_ret] = return_path.AddEdge(out_edge_it->src_handle()->id(),
                                                  out_edge_it->dst_handle()->id(),
                                                  out_edge_it->label(),
                                                  out_edge_it->id());
 
        if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
          for (auto attr_it = out_edge_it->AttributeBegin();
                   !attr_it.IsDone();
                    attr_it++) {
           auto [attr_handle, attr_ret] = add_edge_it->AddAttribute(
                                            attr_it->key(),
                                            attr_it->value_type(),
                                            attr_it->value_str());
          }
        }
        assert(add_edge_ret);
      }
      for (auto in_edge_it = vertex_handle->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        if (in_edge_it->src_handle() != last_vertex_handle) {
          continue;
        }
        auto [add_edge_it,
              add_edge_ret] = return_path.AddEdge(in_edge_it->src_handle()->id(),
                                                  in_edge_it->dst_handle()->id(),
                                                  in_edge_it->label(),
                                                  in_edge_it->id());
        if constexpr (GraphParameter<GraphType>::edge_has_attribute) {
          for (auto attr_it = in_edge_it->AttributeBegin();
                   !attr_it.IsDone();
                    attr_it++) {
           auto [attr_handle, attr_ret] = add_edge_it->AddAttribute(
                                            attr_it->key(),
                                            attr_it->value_type(),
                                            attr_it->value_str());
          }
        }
        assert(add_edge_ret);
      }
    }

    last_vertex_handle = vertex_handle;

    if (visited_vertex_set.size() == length) {
      // long enough, stop walking
      return false;
    }
    // continue walking
    return true;
  };

  RandomWalk(data_graph, src_vertex_handle,
                      random_walk_callback,
                            prune_callback);

  assert(return_path.CountVertex() == length);
  assert(return_path.CountEdge() >= ( length - 1 ));
  assert(Connected<true>(return_path));
  assert(   IsPath<true>(return_path));
  return return_path;
}

}; // namespace GUNDAM

#endif // _GUNDAM_TOOL_RANDOM_WALK_RANDOM_WALK_PATH_H
