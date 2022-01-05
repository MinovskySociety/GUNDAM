#ifndef _GUNDAM_TOOL_DISTANCE_H
#define _GUNDAM_TOOL_DISTANCE_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_id.h"

#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/edge_id.h"

namespace GUNDAM {

// if connected, then return std::pair(distance, true)
// if not connected, then return std::pair(0, false)
//
// #################
// #  optimize me  #
// #################
//
// using floyd algorithm, only works for small graph
template <bool bidirectional = false,
          typename GraphType>
inline std::pair<size_t, bool> 
                 Distance(const GraphType& graph,
              typename VertexID<GraphType>::type src_vertex_id,
              typename VertexID<GraphType>::type dst_vertex_id) {
  using VertexIDType = typename GraphType::VertexType::IDType;
  std::map<VertexIDType, size_t> vertex_id_dict;

  assert(graph.FindVertex(src_vertex_id)
      && graph.FindVertex(dst_vertex_id));

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
     auto  src_vertex_id_it  = vertex_id_dict.find(vertex_it->id());
    assert(src_vertex_id_it != vertex_id_dict.end());
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
      distance[src_vertex_idx][dst_vertex_idx] = 1;
      if constexpr (bidirectional) {
        distance[dst_vertex_idx][src_vertex_idx] = 1;
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

  assert(vertex_id_dict.find(src_vertex_id) != vertex_id_dict.end());
  assert(vertex_id_dict.find(dst_vertex_id) != vertex_id_dict.end());

  if constexpr (bidirectional) {
    assert(distance[vertex_id_dict[src_vertex_id]][vertex_id_dict[dst_vertex_id]]
        == distance[vertex_id_dict[dst_vertex_id]][vertex_id_dict[src_vertex_id]]);
  }

  if (distance[vertex_id_dict[src_vertex_id]]
              [vertex_id_dict[dst_vertex_id]] 
      == std::numeric_limits<DistanceType>::max()) {
    // not connected
    return std::pair(0, false);
  }
  // connected
  return std::pair(distance[vertex_id_dict[src_vertex_id]]
                           [vertex_id_dict[dst_vertex_id]], true);
}

}; // GUNDAM

#endif // _GUNDAM_TOOL_DISTANCE_H