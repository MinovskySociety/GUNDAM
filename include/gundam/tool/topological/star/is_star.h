#ifndef _GUNDAM_TOOL_TOPOLOGICAL_STAR_IS_STAR_H
#define _GUNDAM_TOOL_TOPOLOGICAL_STAR_IS_STAR_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/tool/connected.h"
#include "gundam/tool/topological/path/is_path.h"

namespace GUNDAM {

template<bool bidirectional = false,
         typename GraphType>
std::pair<std::vector<typename VertexHandle<GraphType>::type>, // set of end points, return an empty vector if this not a star
                                                               // okey to return STL container after c11
                      typename VertexHandle<GraphType>::type>  // the central vertex, if this pattern is a star with more than two paths, else return nullptr
              StarEndPoints(GraphType& graph) {
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::vector<VertexHandleType> end_points;
  end_points.clear();

  //static_assert(bidirectional, "have not implemented bidirectional=false yet!");

  if (!Connected<true>(graph)) {
    return std::pair(std::vector<VertexHandleType>(), 
                                 VertexHandleType ());
  }

  // check whether the pattern is a path
  auto [src_handle, dst_handle] = PathEndPoints<bidirectional>(graph);
  if (src_handle) {
    return std::pair(std::vector<VertexHandleType>{src_handle, dst_handle}, 
                                 VertexHandleType ());
  }

  // if (graph.CountVertex() == 1) {
  //   // the pattern is an isolated vertex, which is considered as the centre of a star
  //   return std::pair(end_points, graph.VertexBegin());
  // }

  // try to find centre of the star

  std::vector<VertexHandleType> centre_candidates;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {

    size_t in_out_degree = vertex_it->CountInEdge() + vertex_it->CountOutEdge();
    size_t out_degree = vertex_it->CountOutEdge();
    if constexpr (bidirectional) {
      if (in_out_degree > 2) {
        centre_candidates.emplace_back(vertex_it);
      }
    } else {
      if (in_out_degree - out_degree > 1 ) {
        //not a star, if any vertex with an in_degree > 1
        return std::pair(std::vector<VertexHandleType>(), 
                              VertexHandleType ());
      }
      if (out_degree > 1) {
        if (in_out_degree != out_degree) {
          // this is not a star, when a centre vertex has any input edge
          return std::pair(std::vector<VertexHandleType>(), 
                                VertexHandleType ());
        }
        centre_candidates.emplace_back(vertex_it);
      }
    }
  }

  // if the pattern has more than 1 centre
  // then it is not a star
  if (centre_candidates.size() != 1) {
    return std::pair(std::vector<VertexHandleType>(), 
                                 VertexHandleType ());
  }
  assert(centre_candidates.size() == 1);

  VertexHandleType centre = centre_candidates[0];

  if (bidirectional) {
    // check the incoming paths of the centre
    // for undirected graph only
    for (auto in_edge_it = centre->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      VertexHandleType current_v = in_edge_it->src_handle();
      VertexHandleType prev_v = centre;

      while (true) {
        // endpoint of a path
        if (current_v->CountInEdge() + current_v->CountOutEdge() == 1){
          end_points.emplace_back(current_v);
          break;
        }

        bool found = false;

        for (auto c_in_edge_it = current_v->InEdgeBegin();
                  !c_in_edge_it.IsDone();
                  c_in_edge_it++) {
          if (c_in_edge_it->src_handle() == centre && centre != prev_v) {
            return std::pair(std::vector<VertexHandleType>(), 
                                         VertexHandleType ());
          }
          if (c_in_edge_it->src_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_in_edge_it->src_handle();

          found = true;
          break;
        }

        if (found) continue;

        for (auto c_out_edge_it = current_v->OutEdgeBegin();
                 !c_out_edge_it.IsDone();
                  c_out_edge_it++) {
          if (c_out_edge_it->dst_handle() == centre && centre != prev_v) {
            return std::pair(std::vector<VertexHandleType>(), 
                                         VertexHandleType ());
          }
          if (c_out_edge_it->dst_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_out_edge_it->dst_handle();

          found = true;
          break;          
        }
      }
    }
  }

  // check outgoing paths of the centre
  for (auto out_edge_it = centre->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    VertexHandleType current_v = out_edge_it->dst_handle();
    VertexHandleType prev_v = centre;
    while (true) {
      // endpoint of a path

      if (current_v->CountInEdge() + current_v->CountOutEdge() == 1) {
        end_points.emplace_back(current_v);
        break;
      }

      bool found = false;

      if (bidirectional) {
      for (auto c_in_edge_it = current_v->InEdgeBegin();
                !c_in_edge_it.IsDone();
                c_in_edge_it++) {
          if (c_in_edge_it->src_handle() == centre && centre != prev_v) {
            return std::pair(std::vector<VertexHandleType>(), 
                                         VertexHandleType ());
          }
          if (c_in_edge_it->src_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_in_edge_it->src_handle();
          found = true;
          break;
        }
      }
      if (found) continue;

      for (auto c_out_edge_it = current_v->OutEdgeBegin();
                !c_out_edge_it.IsDone();
                c_out_edge_it++) {
        if (c_out_edge_it->dst_handle() == centre && centre != prev_v) {
          return std::pair(std::vector<VertexHandleType>(), 
                                       VertexHandleType ());
        }
        if (c_out_edge_it->dst_handle() == prev_v) continue;

        prev_v = current_v;
        current_v = c_out_edge_it->dst_handle();

        found = true;
        break;          
      }
    }
  }

  return std::pair(end_points, centre);
}

template<bool bidirectional = false,
         typename GraphType> 
inline bool IsStar(GraphType& graph) {
  const auto [end_points,
              central_vertex_handle] = StarEndPoints<bidirectional>(graph);
  // if (central_vertex) {
  //   return true;
  // } else {
  //   return false;
  // }
  if (!end_points.empty()) {
    assert(end_points.size() >= 2);
    assert(end_points.size() == 2 || central_vertex_handle);
    return true;
  }
  assert(!central_vertex_handle);
  return false;
}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_STAR_IS_STAR_H
