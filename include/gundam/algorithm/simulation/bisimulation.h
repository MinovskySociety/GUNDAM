#ifndef _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H

#include "gundam/tool/vertex_degree_filter.h"
#include "gundam/tool/topological/dag/is_acyclic.h"
#include "gundam/tool/operator/merge_vertex.h"

#include "gundam/match/match.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <limits>

namespace GUNDAM {

// for general case
template <typename GraphType>
inline void BisimulationGeneralCase(GraphType& graph,
     std::map<typename VertexHandle<GraphType>::type,
  std::vector<typename VertexHandle<GraphType>::type>>& match_set) {

  using VertexHandleType = typename VertexHandle<GraphType>::type;
  using VertexIDType     = typename VertexID    <GraphType>::type;

  using RankType = int64_t;

  auto strong_connected_component_vertex_set
     = StrongConnectedComponent(graph);

  assert(IsAcyclic(MergeVertex(graph, strong_connected_component_vertex_set)));

  /* ################################################ *
   * ##  calc leaf_handle_set_in_g_scc             ## *
   * ##     for a vertex, if its out edge are all  ## *
   * ##     in the same SCC, then it is an leaf    ## *
   * ##     vertex in the g_scc                    ## *
   * ################################################ */
  std::vector<VertexHandleType> leaf_handle_set_in_g_scc;
  for (const auto& strong_connected_component_vertex
                 : strong_connected_component_vertex_set) {
    assert(std::is_sorted(strong_connected_component_vertex.begin(),
                          strong_connected_component_vertex.end()));
    bool is_leaf = true;
    for (const auto& vertex_handle : strong_connected_component_vertex) {
      for (auto out_edge_it = vertex_handle->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        if (std::binary_search(strong_connected_component_vertex.begin(),
                               strong_connected_component_vertex.end(),
                               out_edge_it->dst_handle())) {
          // within this SCC
          continue;
        }
        // outside this SCC
        is_leaf = false;
        break;
      }
      if (!is_leaf) {
        // is not leaf, does not add to leaf_handle_set_in_g_scc
        continue;
      }
      leaf_handle_set_in_g_scc.emplace_back(vertex_handle);
    }
  }
  std::sort(leaf_handle_set_in_g_scc.begin(),
            leaf_handle_set_in_g_scc.end());

  /* ################################# *
   * ##  calc leaf_handle_set_in_g  ## *
   * ################################# */
  auto leaf_handle_set_in_g = VertexDegreeFilter<FilterType::kEqualTo,
                                              EdgeDirection::kOut>(graph, 0);
  std::sort(leaf_handle_set_in_g.begin(),
            leaf_handle_set_in_g.end());

  /* ################# *
   * ##  calc rank  ## *
   * ################# */
  std::unordered_map<VertexHandleType,
                             RankType> rank;

  // for the vertex n is a leaf in G
  for (const auto& leaf_handle_in_g
                 : leaf_handle_set_in_g) {
    // is a leaf in graph
    // should not have been considered before
    assert(rank.find(leaf_handle_in_g) == rank.end());
    rank.emplace(leaf_handle_in_g, 0);
  }

  std::vector<VertexHandleType> leaf_handle_set_in_g_scc_not_in_g;

  std::set_difference(leaf_handle_set_in_g_scc.begin(),
                      leaf_handle_set_in_g_scc.end(),
                      leaf_handle_set_in_g.begin(),
                      leaf_handle_set_in_g.end(),
   std::back_inserter(leaf_handle_set_in_g_scc_not_in_g));

  assert(std::is_sorted(leaf_handle_set_in_g_scc_not_in_g.begin(),
                        leaf_handle_set_in_g_scc_not_in_g.end()));

  // for the vertex n is a leaf in G_scc but not a leaf in G
  for (const auto& leaf_handle_in_g_scc_not_in_g
                 : leaf_handle_set_in_g_scc_not_in_g) {
    //     is not a leaf in graph
    // and is not a leaf in graph_scc
    assert(rank.find(leaf_handle_in_g_scc_not_in_g) == rank.end());
    rank.emplace(leaf_handle_in_g_scc_not_in_g, 
                 std::numeric_limits<RankType>::min());
    assert(std::numeric_limits<RankType>::min() < 0);
  }

  /* ################################################ *
   * ##  calc the WF                               ## *
   * ##     WF(G) = {n ∈ N : G(n) is acyclic}.     ## *
   * ##     G(n) is the reachable subgraph from n  ## *
   * ################################################ */
  std::vector<VertexHandleType> vertexes_in_scc_with_cycle;
  std::unordered_set<VertexIDType> not_in_wf;
  for (auto& strong_connected_component_vertex
           : strong_connected_component_vertex_set) {
    assert(!strong_connected_component_vertex.empty());
    if (strong_connected_component_vertex.size() == 1) {
      continue;
    }
    for (const auto& vertex_handle : strong_connected_component_vertex) {
      not_in_wf.emplace(vertex_handle->id());
    }
    // such a subgraph contains loop, iterate over
    // input edges from 
    std::move(strong_connected_component_vertex.begin(), 
              strong_connected_component_vertex.end(), 
              std::back_inserter(vertexes_in_scc_with_cycle));
  }

  std::queue<VertexHandleType, 
  std::deque<VertexHandleType>> queue_of_vertex_in_scc_with_cycle(
  std::deque<VertexHandleType>(vertexes_in_scc_with_cycle.begin(),
                               vertexes_in_scc_with_cycle.end()));

  assert(not_in_wf_it.size() == queue_of_vertex_in_scc_with_cycle.size());
  while (!queue_of_vertex_in_scc_with_cycle.empty()) {
    auto vertex_handle = queue_of_vertex_in_scc_with_cycle.front();
    assert(not_in_wf.find(vertex_handle->id())
        != not_in_wf.end ());
    queue_of_vertex_in_scc_with_cycle.pop();
    for (auto in_edge_it = vertex_handle.InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      auto not_in_wf_it  = not_in_wf.find(in_edge_it->src_handle()->id());
      if ( not_in_wf_it != not_in_wf.end()) {
        // has been considered
        continue;
      }
      not_in_wf.emplace_hint(not_in_wf_it, in_edge_it->src_handle()->id());
      queue_of_vertex_in_scc_with_cycle.emplace_back(in_edge_it->src_handle());
    }
  }

  std::queue<VertexHandleType, 
  std::deque<VertexHandleType>> queue_of_vertex(
  std::deque<VertexHandleType>(leaf_handle_set_in_g.begin(),
                               leaf_handle_set_in_g.end()));

  while (!queue_of_vertex.empty()) {

  }
  return;
}

}; // GUNDAM

#endif // _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H
