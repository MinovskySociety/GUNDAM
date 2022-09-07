#ifndef _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H

#include "gundam/tool/vertex_degree_filter.h"
#include "gundam/tool/topological/dag/is_acyclic.h"
#include "gundam/tool/operator/merge_vertex.h"

#include "gundam/algorithm/connected_component.h"

#include "gundam/match/match.h"

#include "gundam/util/hash.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <limits>

namespace GUNDAM {

// for general case
template <typename GraphType>
inline void BisimulationGeneralCase(GraphType& graph,
  std::vector<
  std::vector<typename VertexHandle<GraphType>::type>>& equivalent_class_set) {

  using VertexHandleType = typename VertexHandle<GraphType>::type;
  using VertexIDType     = typename VertexID    <GraphType>::type;

  using RankType = int64_t;

  equivalent_class_set.clear();

  auto strong_connected_component_vertex_set
     = StrongConnectedComponent(graph);

  /* ################################################ *
   * ##  calc leaf_idx_set_in_g_scc                ## *
   * ##     for a vertex, if its out edge are all  ## *
   * ##     in the same SCC, then it is an leaf    ## *
   * ##     vertex in the g_scc                    ## *
   * ################################################ */
  std::unordered_map<VertexIDType, size_t> vertex_handle_scc_idx_map;
  std::vector<size_t> leaf_idx_set_in_g_scc;
  leaf_idx_set_in_g_scc.reserve(strong_connected_component_vertex_set.size());
  for (size_t strong_connected_component_vertex_idx = 0;
              strong_connected_component_vertex_idx
            < strong_connected_component_vertex_set.size();
              strong_connected_component_vertex_idx++) {
    const auto& strong_connected_component_vertex 
              = strong_connected_component_vertex_set[
                strong_connected_component_vertex_idx];
    assert(std::is_sorted(strong_connected_component_vertex.begin(),
                          strong_connected_component_vertex.end()));
    // set to false here in order not to get into (1)
    // in the first loop
    bool is_leaf = false;
    for (const auto& vertex_handle : strong_connected_component_vertex) {
      auto [ map_it,
             map_ret ] = vertex_handle_scc_idx_map.emplace(vertex_handle->id(),
                                strong_connected_component_vertex_idx);
      assert(map_ret);
      if (is_leaf) { 
        /* ######### *
         * ## (1) ## *
         * ######### */
        // has already determined this scc is a leaf
        // no longer need to check the out edge
        continue;
      }
      is_leaf = true;
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
      /* ######################################### *
       * ##  don't break here, needs to add to  ## *
       * ##  strong_connected_component_vertex  ## *
       * ######################################### */
      // if (is_leaf) {
      //   // is a leaf
      //   break;
      // }
    }
    if (!is_leaf) {
      continue;
    }
    leaf_idx_set_in_g_scc.emplace_back(strong_connected_component_vertex_idx);
  }
  assert(vertex_handle_scc_idx_map.size() == graph.CountVertex());

  // std::cout << "leaf_idx_set_in_g_scc: " << std::endl;
  // for (const auto& leaf_idx : leaf_idx_set_in_g_scc) {
  //   std::cout << leaf_idx << std::endl;
  // }

  /* ################################ *
   * ##  calc out_degree_in_g_scc  ## *
   * ################################ */
  std::vector<size_t> out_degree_in_g_scc(strong_connected_component_vertex_set.size(), 0);
  for (size_t strong_connected_component_vertex_idx = 0;
              strong_connected_component_vertex_idx
            < strong_connected_component_vertex_set.size();
              strong_connected_component_vertex_idx++) {
    const auto&  strong_connected_component_vertex 
               = strong_connected_component_vertex_set[
                 strong_connected_component_vertex_idx];
    std::vector<size_t> connected_scc_idx;
    for (const auto& vertex_handle : strong_connected_component_vertex) {
      for (auto out_edge_it = vertex_handle->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        assert(vertex_handle_scc_idx_map.find(out_edge_it->dst_handle()->id())
            != vertex_handle_scc_idx_map.end());
        const auto kDstSccIdx = vertex_handle_scc_idx_map[out_edge_it->dst_handle()->id()];
        if (kDstSccIdx == strong_connected_component_vertex_idx) {
          // to this scc
          continue;
        }
        connected_scc_idx.emplace_back(kDstSccIdx);
      }
      std::sort(connected_scc_idx.begin(),
                connected_scc_idx.end());
      connected_scc_idx.erase(std::unique(connected_scc_idx.begin(), 
                                          connected_scc_idx.end()), 
                                          connected_scc_idx.end());
    }
    assert(out_degree_in_g_scc[strong_connected_component_vertex_idx] == 0);
    out_degree_in_g_scc[strong_connected_component_vertex_idx] 
                             = connected_scc_idx.size();
  }

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
                             RankType, v_handle_hash> rank;

  // for the vertex n is a leaf in G
  /* ###################################### *
   * ##  rank (n)=0 if n is a leaf in G  ## *
   * ###################################### */
  for (const auto& leaf_handle_in_g
                 : leaf_handle_set_in_g) {
    // is a leaf in graph
    // should not have been considered before
    assert(rank.find(leaf_handle_in_g) == rank.end());
    rank.emplace(leaf_handle_in_g, 0);
  }

  for (const auto& leaf_idx_in_g_scc
                 : leaf_idx_set_in_g_scc) {
    const auto& strong_connected_component_vertex 
              = strong_connected_component_vertex_set[leaf_idx_in_g_scc];
    for (const auto& vertex_handle : strong_connected_component_vertex) {
      /* ########################################## *
       * ##  rank (n) = −∞ if c(n) is a leaf in  ## *
       * ##  Gscc and n is not a leaf in G       ## *
       * ########################################## */
      auto [ rank_it,
             rank_ret ] = rank.emplace(vertex_handle,
                                       std::numeric_limits<RankType>::min());
      #ifndef NDEBUG 
      if (!rank_ret) {
        // added fail, should have already been considered
        // in leaf_handle_set_in_g
        assert(std::binary_search(leaf_handle_set_in_g.begin(),
                                  leaf_handle_set_in_g.end(),
                                  vertex_handle));
        continue;
      }
      #endif // NDEBUG 
    }
  }

  // std::cout << "## rank for all leaf in G ##" << std::endl;
  // for (const auto& [vertex_handle, 
  //                   vertex_rank] : rank) {
  //   std::cout << "\tvertex_handle->id(): " << vertex_handle->id()
  //             << "\tvertex_rank: "         << vertex_rank << std::endl;
  // }

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

  assert(not_in_wf.size() == queue_of_vertex_in_scc_with_cycle.size());
  while (!queue_of_vertex_in_scc_with_cycle.empty()) {
    auto vertex_handle = queue_of_vertex_in_scc_with_cycle.front();
    assert(not_in_wf.find(vertex_handle->id())
        != not_in_wf.end ());
    queue_of_vertex_in_scc_with_cycle.pop();
    for (auto in_edge_it = vertex_handle->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      auto not_in_wf_it  = not_in_wf.find(in_edge_it->src_handle()->id());
      if ( not_in_wf_it != not_in_wf.end()) {
        // has been considered
        continue;
      }
      not_in_wf.emplace_hint(not_in_wf_it, in_edge_it->src_handle()->id());
      queue_of_vertex_in_scc_with_cycle.emplace(in_edge_it->src_handle());
    }
  }

  std::queue<size_t, 
  std::deque<size_t>> queue_of_scc_idx(
  std::deque<size_t>(leaf_idx_set_in_g_scc.begin(),
                     leaf_idx_set_in_g_scc.end()));

  // {rank (m) : hc(n), c(m)i ∈ Escc, m 6∈ W F(G)}) otherwise
  while (!queue_of_scc_idx.empty()) {
    auto current_scc_idx = queue_of_scc_idx.front();
    queue_of_scc_idx.pop();
    const auto& strong_connected_component_vertex
              = strong_connected_component_vertex_set[current_scc_idx];
    for (const auto& vertex_handle : strong_connected_component_vertex) {
      assert(rank.find(vertex_handle) != rank.end());
      const bool kNotInWf    = not_in_wf.find(vertex_handle->id()) != not_in_wf.end();
      const auto kRank       =      rank.find(vertex_handle)->second;
      const bool kIsInfinite =    (kRank == std::numeric_limits<RankType>::min());
      std::vector<size_t> connected_scc_idx;
      for (auto in_edge_it = vertex_handle->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        auto src_handle = in_edge_it->src_handle();
        assert(vertex_handle_scc_idx_map.find(src_handle->id())
            != vertex_handle_scc_idx_map. end());
        const auto kSrcSccIdx = vertex_handle_scc_idx_map[src_handle->id()];
        if (kSrcSccIdx == current_scc_idx) {
          continue;
        }
        connected_scc_idx.emplace_back(kSrcSccIdx);
        auto [ rank_it,
               rank_ret ] = rank.emplace(src_handle, 
                            std::numeric_limits<RankType>::min());
        // this vertex has not been considered
        if (kIsInfinite) {
          rank_it->second = std::max(rank_it->second,
                            std::numeric_limits<RankType>::min());
          continue;
        }
        if (kNotInWf) {
          rank_it->second = std::max(rank_it->second, kRank);
          continue;
        }
        // in WF
        rank_it->second = std::max(rank_it->second, kRank + 1);
      }
      std::sort(connected_scc_idx.begin(),
                connected_scc_idx.end());
      connected_scc_idx.erase(std::unique(connected_scc_idx.begin(), 
                                          connected_scc_idx.end()), 
                                          connected_scc_idx.end());
      for (const auto& scc_idx : connected_scc_idx) {
        assert(scc_idx != current_scc_idx);
        out_degree_in_g_scc[scc_idx]--;
        assert(out_degree_in_g_scc[scc_idx] >= 0);
        if (out_degree_in_g_scc[scc_idx] == 0) {
          queue_of_scc_idx.emplace(scc_idx);
        }
      }
    }
  }

  // std::cout << "## 2 ##" << std::endl;
  // for (const auto& [vertex_handle, 
  //                   vertex_rank] : rank) {
  //   std::cout << "\tvertex_handle->id(): " << vertex_handle->id()
  //             << "\tvertex_rank: "         << vertex_rank << std::endl;
  // }

  assert(rank.size() == graph.CountVertex());

  // sort the rank
  std::map<RankType,  /* rank */
           std::vector<
           std::vector<VertexHandleType>> /* set of blocks */
          > sorted_rank;
  for (const auto& [vertex_handle, 
                    vertex_rank] : rank) {
    auto [ sorted_rank_it,
           sorted_rank_ret ] = sorted_rank.emplace(vertex_rank, std::vector<
                                                                std::vector<VertexHandleType>>(1));
    sorted_rank_it->second.begin()->emplace_back(vertex_handle);
  }

  // for (const auto& [vertex_rank, 
  //                   vertex_block_set] : sorted_rank) {
  //   std::cout << "vertex_rank: " << vertex_rank << std::endl;
  //   for (const auto& block : vertex_block_set) {
  //     for (const auto& vertex_handle : block) {
  //       std::cout << "\t" << vertex_handle->id();
  //     }
  //     std::cout << std::endl;
  //   }
  // }

  /* ############################################################################# *
   * ## for i = 0,...,ρ do                                                      ## *
   * ## (a) Di := {X ∈ P : X ⊆ Bi}; — determine the blocks currently at rank i  ## *
   * ## (b) for X ∈ Di do                                                       ## *
   * ##       G := collapse(G, X); — collapse nodes at rank i                   ## *
   * ## (c) for n ∈ N ∩ Bi do — refine blocks at higher ranks                   ## *
   * ##       for C ∈ P and C ⊆ Bi+1 ∪ ... ∪ Bρ do                              ## *
   * ##         P := (P \ {C}) ∪ {{m ∈ C : hm, ni ∈ E}, {m ∈ C : hm, ni ∈/ E}}; ## *
   * ##         // divide the C into two part:                                  ## *
   * ##         //    has edge connected to Bi                                  ## *
   * ##         //    has no edge connected to Bi                               ## *
   * ############################################################################# */
  for (auto sorted_rank_it  = sorted_rank.begin();
            sorted_rank_it != sorted_rank.end();
            sorted_rank_it++) {
    const auto& [vertex_rank, block_set] = *sorted_rank_it;
    
    std::unordered_map<VertexIDType, // vertex id
                       size_t        // the idx of block it belongs to
                      > vertex_block_idx_map;
    for (size_t block_idx = 0;
                block_idx < block_set.size();
                block_idx++) {
      /* ######################################## *
       * ##  wenzhi:                           ## *
       * ##    not sure which one is more      ## *
       * ##    efficient here:                 ## *
       * ##       actually merge the graph     ## *
       * ##       or mark the set of vertexes  ## *
       * ##       that are merged              ## *
       * ######################################## */
      // here, we only mark the set of vertexes
      // that to be merged
      for (const auto& vertex_handle : block_set[block_idx]) {
        vertex_block_idx_map.emplace(vertex_handle->id(), 
                                     block_idx);
      }
    }

    auto higher_sorted_rank_it = sorted_rank_it;
    higher_sorted_rank_it++;

    for (;higher_sorted_rank_it != sorted_rank.end();
          higher_sorted_rank_it++) {
      const auto& [higher_vertex_rank, 
                   higher_block_set] = *higher_sorted_rank_it;
      assert(higher_vertex_rank
                  > vertex_rank);
      std::vector<
      std::vector<VertexHandleType>> temp_higher_block_set;
      for (const auto& higher_block : higher_block_set) {
        std::map<std::vector<size_t>,
                 std::vector<VertexHandleType>> connected_to_bi;
        for (const auto& higher_vertex_handle 
                       : higher_block) {
          std::vector<size_t> connected_block_idx_set;
          for (auto out_edge_it = higher_vertex_handle->OutEdgeBegin();
                   !out_edge_it.IsDone();
                    out_edge_it++) {
            auto vertex_block_idx_it 
               = vertex_block_idx_map.find(out_edge_it->dst_handle()->id());
            if ( vertex_block_idx_it
              == vertex_block_idx_map.end()) {
              // does not connected to Bi in this layer
              continue;
            }
            connected_block_idx_set.emplace_back(vertex_block_idx_it->second);
          }
          std::sort(connected_block_idx_set.begin(),
                    connected_block_idx_set.end());
          connected_block_idx_set.erase(std::unique(connected_block_idx_set.begin(), 
                                                    connected_block_idx_set.end()), 
                                                    connected_block_idx_set.end());
          connected_to_bi[connected_block_idx_set].emplace_back(higher_vertex_handle);
        }
        #ifndef NDEBUG
        size_t _temp_higher_block_size = 0;
        for (const auto& [connected_block_idx_set,
                                vertex_handle_set] : connected_to_bi) {
          _temp_higher_block_size += vertex_handle_set.size();
        }
        assert(higher_block.size() == _temp_higher_block_size);
        #endif // NDEBUG

        for (auto& [connected_block_idx_set,
                          vertex_handle_set] : connected_to_bi) {
          temp_higher_block_set.emplace_back(std::move(vertex_handle_set));
        }
      }
      higher_sorted_rank_it->second.swap(temp_higher_block_set);
    }
  }
  for (auto& [vertex_rank, block_set] : sorted_rank) {
    std::move(block_set.begin(), 
              block_set.end(), 
              std::back_inserter(equivalent_class_set));
  }

  for (auto& equivalent_class : equivalent_class_set) {
    std::sort(equivalent_class.begin(),
              equivalent_class.end());
  }
  std::sort(equivalent_class_set.begin(),
            equivalent_class_set.end());

  #ifndef NDEBUG
  size_t _vertex_counter = 0;
  for (const auto& equivalent_class : equivalent_class_set) {
    _vertex_counter += equivalent_class.size();
  }
  assert(_vertex_counter == graph.CountVertex());
  #endif // NDEBUG
  return;
}

}; // GUNDAM

#endif // _GUNDAM_ALGORITHM_SIMULATION_BISIMULATION_H
