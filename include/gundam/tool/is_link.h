#ifndef _GUNDAM_IS_LINK_H
#define _GUNDAM_IS_LINK_H

#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {

//  #######################
//  ##  can be optmized  ##
//  #######################
// 
// if the input graph is a link, return the 
// the two end vertexes ([src_handle, dst_handle] if bidirectional == false)
// could be the same if the input graph is just an isolated
//
// if the input graph is not a link, then return two nullptr
//
template<bool bidirectional = false,
         typename GraphType>
std::pair<typename VertexHandle<GraphType>::type,
          typename VertexHandle<GraphType>::type> LinkEndPoints(GraphType& graph) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  if (graph.CountVertex() == 1
   && graph.CountEdge() == 0) {
    // only has one isolated vertex, can be viewed as 
    // link
    return std::pair(graph.VertexBegin(),
                     graph.VertexBegin());
  }
  if (graph.CountVertex()
   != graph.CountEdge() + 1) {
    // is not a link, return two nullptr
    return std::pair(VertexHandleType(),
                     VertexHandleType());
  }
  // could also contain loop now
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    // first count the input degree for this vertex
    size_t in_edge_num = vertex_it->CountInEdge();
    if constexpr (!bidirectional) {
      // only consider one direction
      if (in_edge_num > 1) {
        // under this situation, no vertex could have more than
        // one input edge, is not a link
        return std::pair(VertexHandleType(),
                         VertexHandleType());
      }
      if (in_edge_num == 1) {
        // is not a end point
        continue;
      }
      assert(in_edge_num == 0);
      // does not have input edges, is an end vertex
      VertexHandleType current_handle = vertex_it,
                           src_handle = vertex_it;
      // both current_handle and src_handle points
      // to one end of the link
      assert(current_handle);
      assert(    src_handle);
      size_t visited_vertex_num = 1;
      while (true) {
        assert(visited_vertex_num <= graph.CountVertex());
        size_t out_edge_num = current_handle->CountOutEdge();
        if (out_edge_num != 1) {
          if (out_edge_num == 0) {
            // reach end, 
            // to find whether has visited all vertex
            if (visited_vertex_num != graph.CountVertex()) {
              // has not visited all vertexes
              assert(visited_vertex_num < graph.CountVertex());
              // return two nullptr which represents the
              // input grpah is not a link
              return std::pair(VertexHandleType(),
                               VertexHandleType());
            }
            // has visited all vertex
            // the current_handle is the other end 
            assert(current_handle);
            assert(    src_handle);
            return std::pair(src_handle,
                         current_handle);
          }
          // illegal output edge number
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
        // only have one out edge points to the next vertex 
        // move to the next vertex pointed by the only out edge
        current_handle = current_handle->OutEdgeBegin()->dst_handle();
        assert(current_handle);
        visited_vertex_num++;
        // verify the input edge number of the next vertex
        size_t in_edge_num = current_handle->CountInEdge();
        assert(in_edge_num > 0);
        if (in_edge_num > 1) {
          // has more than one in edge, is not link
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
      }
      continue;
    }
    // considering both input edges and output edges
    size_t connected_edge_num 
                = in_edge_num + vertex_it->CountOutEdge();
    if (connected_edge_num > 2
     || connected_edge_num == 0) {
      // illegal, edge vertex can only have at most 2
      // connected edges and cannot be isolated vertex
      return std::pair(VertexHandleType(),
                       VertexHandleType());
    }
    if (connected_edge_num == 2) {
      // is not end point
      continue;
    }
    assert(connected_edge_num == 1);
    // find an end point
    VertexHandleType previous_vertex_handle = VertexHandleType(),
                      current_vertex_handle = vertex_it,
                          src_vertex_handle = vertex_it;
    // should be initialized as nullptr
    assert(!previous_vertex_handle);
    assert(  current_vertex_handle);
    assert(      src_vertex_handle);
    size_t visited_vertex_num = 1;
    // is an end point
    while (true) {
      VertexHandleType unvisited_vertex = VertexHandleType();
      // should be initialized as nullptr
      assert(!unvisited_vertex);
      size_t connected_edge_num = 0;
      for (auto out_edge_it = current_vertex_handle->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        connected_edge_num++;
        if (connected_edge_num > 2){
          // illegal connected edge number
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
        if (out_edge_it->dst_handle()
          == previous_vertex_handle) {
          // this edge is where it comes from
          continue;
        }
        assert(out_edge_it->dst_handle()
             != previous_vertex_handle);
        if (unvisited_vertex) {
          // is not null, has already met one unvisted vertex
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
        unvisited_vertex = out_edge_it->dst_handle();
      }
      for (auto in_edge_it = current_vertex_handle->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        connected_edge_num++;
        if (connected_edge_num > 2){
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
        if (in_edge_it->src_handle()
         == previous_vertex_handle) {
          // this edge is where it comes from
          continue;
        }
        assert(in_edge_it->src_handle()
            != previous_vertex_handle);
        if (unvisited_vertex) {
          // is not null
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
        unvisited_vertex = in_edge_it->src_handle();
      }
      assert(connected_edge_num >  0
          == connected_edge_num <= 2);
      if (connected_edge_num == 2) {
        if (!unvisited_vertex){
          // unvisited_vertex is null
          // has two connected edges but point
          // to the same vertex
          return std::pair(VertexHandleType(),
                           VertexHandleType());
        }
        // move to the next vertex
        previous_vertex_handle = current_vertex_handle;
         current_vertex_handle = unvisited_vertex;
         visited_vertex_num++;
        continue;
      }
      assert(connected_edge_num == 1);
      // is an end point
      if (unvisited_vertex) {
        // unvisited_vertex is not null
        // is the beginning end point
        assert(current_vertex_handle 
                == src_vertex_handle);
        assert(visited_vertex_num == 1);
        // move to the next vertex
        previous_vertex_handle = current_vertex_handle;
         current_vertex_handle = unvisited_vertex;
         visited_vertex_num++;
        continue;
      }
      // unvisited_vertex is null 
      // meet the other end point
      assert(current_vertex_handle 
              != src_vertex_handle);
      if (visited_vertex_num == graph.CountVertex()) {
        // has visited all vertexes, is a link
        return std::pair(src_vertex_handle,
                     current_vertex_handle);
      }
      assert(visited_vertex_num < graph.CountVertex());
      // does not visited all vertexes, is not a link
      return std::pair(VertexHandleType(),
                       VertexHandleType());
    }
  }
  // fail to find the end point, is not a link
  return std::pair(VertexHandleType(),
                   VertexHandleType());
}

template<bool bidirectional = false,
         typename GraphType> 
inline bool IsLink(GraphType& graph) {
  const auto [end_handle_0,
              end_handle_1] = LinkEndPoints<bidirectional>(graph);
  if (end_handle_0) {
    assert(end_handle_1);
    return true;
  }
  assert(!end_handle_1);
  return false;
}

};

#endif // _GUNDAM_IS_LINK_H