#ifndef _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H

#include "gundam/tool/vertex_degree_filter.h"

#include "gundam/match/match.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace GUNDAM {

// for general case
template <typename GraphType>
inline void BisimulationGeneralCase(GraphType& graph,
     std::map<typename VertexHandle<GraphType>::type,
  std::vector<typename VertexHandle<GraphType>::type>>& match_set) {

  auto strong_connected_component_vertex_set
     = StrongConnectedComponent(graph);

  // Gscc
  auto graph_scc = MergeVertex(graph,
    strong_connected_component_vertex_set);

  auto leaf_in_g     = VertexDegreeFilter<FilterType::kEqualTo,
                                       EdgeDirection::kOut>(graph,     0);
  auto leaf_in_g_scc = VertexDegreeFilter<FilterType::kEqualTo,
                                       EdgeDirection::kOut>(graph_scc, 0);

  // calculate the rank of each vertex
  return;
}

}; // GUNDAM

#endif // _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H
