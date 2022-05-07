#ifndef _GUNDAM_TOOL_DIAMETER_H
#define _GUNDAM_TOOL_DIAMETER_H

#include <limits>

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

namespace GUNDAM {

// #################
// #  optimize me  #
// #################
//
// using floyd algorithm, only works for small graph
template <bool bidirectional = false,
          typename GraphType>
inline size_t Diameter(GraphType& graph) {
  using VertexIDType = typename GraphType::VertexType::IDType;
  std::map<VertexIDType, size_t> vertex_id_dict;

  // construct the map from vertex id to continued index
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    auto [ vertex_id_it,
           vertex_id_ret ]
         = vertex_id_dict.emplace(vertex_it->id(),
                                  vertex_id_dict.size());
    assert(vertex_id_ret);
  }
  const size_t kVertexNum = vertex_id_dict.size();

  using DistanceType = size_t;
  std::vector<std::vector<DistanceType>> distance;
  distance.resize(kVertexNum);
  for (auto& singe_vertex_distance
                        : distance) {
    singe_vertex_distance.resize(kVertexNum, std::numeric_limits<DistanceType>::max());
  }

  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    auto src_vertex_id_it 
           = vertex_id_dict.find(vertex_it->id());
    assert(src_vertex_id_it 
            != vertex_id_dict.end());
    size_t src_vertex_idx 
         = src_vertex_id_it->second;
    distance[src_vertex_idx]
            [src_vertex_idx] = 0;
    for (auto edge_it = vertex_it->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      auto dst_vertex_id_it 
             = vertex_id_dict.find(edge_it->dst_handle()->id());
      assert(dst_vertex_id_it 
              != vertex_id_dict.end());
      size_t dst_vertex_idx 
           = dst_vertex_id_it->second;
      distance[src_vertex_idx]
              [dst_vertex_idx] = 1;
      if constexpr (bidirectional) {
        distance[dst_vertex_idx]
                [src_vertex_idx] = 1;
      }
    }
  }

  for (size_t k = 0; k < kVertexNum; k++) {
    for (size_t i = 0; i < kVertexNum; i++) {
      if (distance[i][k] == std::numeric_limits<DistanceType>::max()) {
        continue;
      }
      for (size_t j = 0; j < kVertexNum; j++) {
        if (distance[k][j] == std::numeric_limits<DistanceType>::max()) {
          continue;
        }
        distance[i][j] = distance[i][j] < (distance[i][k] + distance[k][j])?
                         distance[i][j] : (distance[i][k] + distance[k][j]);
      }
    }
  }

  size_t diameter = 0;

  for (size_t i = 0; i < kVertexNum; i++) {
    for (size_t j = 0; j < kVertexNum; j++) {
      if (distance[i][j] == std::numeric_limits<DistanceType>::max()){
        continue;
      }
      diameter = diameter > distance[i][j] ?
                 diameter : distance[i][j];
    }
  }
  return diameter;
}

}; // GUNDAM

#endif // _GUNDAM_TOOL_DIAMETER_H