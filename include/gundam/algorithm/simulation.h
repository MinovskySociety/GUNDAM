#ifndef _GUNDAM_ALGORITHM_SIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_H

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/algorithm/dp_iso_using_match.h"
#include "gundam/algorithm/match_semantics.h"

#include "gundam/tool/connected_component.h"
#include "gundam/tool/diameter.h"
#include "gundam/tool/operator/preserve.h"

#include "gundam/match/match.h"

namespace GUNDAM {

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kDualSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t Simulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
     std::map<typename VertexHandle< QueryGraph>::type,
  std::vector<typename VertexHandle<TargetGraph>::type>>& match_set) {
  assert(match_set.empty());
  match_set.clear();

  static_assert(match_semantics == MatchSemantics::kDualSimulation);

  if (!_dp_iso_using_match::InitCandidateSet<match_semantics>(query_graph,
                                                             target_graph,
                                                              match_set)) {
    return 0;
  }

  if (!_dp_iso_using_match::RefineCandidateSet(query_graph, 
                                              target_graph, 
                                               match_set)) {
    return 0;
  }

  size_t match_count = 1;
  for (const auto& [query_vertex_handle,
                    query_vertex_candidate_set] : match_set) {
    match_count *= query_vertex_candidate_set.size();
  }

  return match_count;
};


template <enum MatchSemantics match_semantics 
             = MatchSemantics::kStrongSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline std::vector<TargetGraph> StrongSimulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph) {

  static_assert(match_semantics == MatchSemantics::kStrongSimulation);

  using  QueryVerteHandleType = typename VertexHandle< QueryGraph>::type;
  using TargetVerteHandleType = typename VertexHandle<TargetGraph>::type;
  using  TargetEdgeHandleType = typename   EdgeHandle<TargetGraph>::type;

  using CandidateSet =  std::map< QueryVerteHandleType,
                     std::vector<TargetVerteHandleType>>;

  const size_t dQ = Diameter<true>(query_graph);

  std::vector<TargetGraph> maximum_perfect_subgraphs;

  for (auto vertex_it = target_graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    TargetVerteHandleType w_vertex_handle = vertex_it;
    // to obtain the ball
    TargetGraph ball = KHop<true>(target_graph, w_vertex_handle, 2 * dQ);
    CandidateSet match_set_in_ball;
    if (Simulation<MatchSemantics::kDualSimulation>(
                    query_graph, ball,
                    match_set_in_ball) == 0) {
      continue;
    }

    TargetVerteHandleType w_vertex_handle_in_ball
        = ball.FindVertex(w_vertex_handle->id());
    assert(w_vertex_handle_in_ball);

    // ExtractMaxPG(query_graph, ball, match_set_in_ball);
    std::vector<TargetVerteHandleType> vertex_handle_to_preserve;
    bool contained_in_sw = false;
    for (auto& [query_handle,
                candidate_set] : match_set_in_ball) {
      std::sort(candidate_set.begin(),
                candidate_set.end());
      std::copy(candidate_set.begin(), 
                candidate_set.end(), 
                std::back_inserter(vertex_handle_to_preserve));
      if (std::binary_search(candidate_set.begin(),
                             candidate_set.end(), 
                             w_vertex_handle_in_ball)) {
        contained_in_sw = true;
      }
    }
    if (!contained_in_sw) {
      continue;
    }
    std::sort( vertex_handle_to_preserve.begin(), 
               vertex_handle_to_preserve.end() );
    vertex_handle_to_preserve.erase(
      std::unique( vertex_handle_to_preserve.begin(),
                   vertex_handle_to_preserve.end() ),
                   vertex_handle_to_preserve.end() );
    assert(vertex_handle_to_preserve.size() <= ball.CountVertex());
    // to select the edge, map each edge in query_graph
    std::vector<TargetEdgeHandleType> edge_handle_to_preserve;
    for (auto vertex_it = query_graph.VertexBegin();
             !vertex_it.IsDone();
              vertex_it++) {
      for (auto out_edge_it = vertex_it->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        // to find the set of edges that can be matched by
        // this edge can
        assert(match_set_in_ball.find(vertex_it)
            != match_set_in_ball. end());
        const auto& candidate_set_u0 = match_set_in_ball.find(vertex_it)->second;
        assert(match_set_in_ball.find(out_edge_it->dst_handle())
            != match_set_in_ball. end());
        const auto& candidate_set_u1 = match_set_in_ball.find(out_edge_it->dst_handle())->second;
        for (const auto& dst_handle_u0 : candidate_set_u0) {
          for (const auto& dst_handle_u1 : candidate_set_u1) {
            // to find whether there is an edge from dst_handle_u0 
            // to dst_handle_u1
            for (auto out_edge_it = dst_handle_u0->OutEdgeBegin();
                     !out_edge_it.IsDone();
                      out_edge_it++) {
              if (out_edge_it->dst_handle() != dst_handle_u1) {
                continue;
              }
              edge_handle_to_preserve.emplace_back(out_edge_it);
              // in case there are more than one edge between these two 
              // vertexes
              // break;
            }
          }
        }
      }
    }

    std::sort( edge_handle_to_preserve.begin(),
               edge_handle_to_preserve.end() );
    edge_handle_to_preserve.erase(
      std::unique( edge_handle_to_preserve.begin(),
                   edge_handle_to_preserve.end() ),
                   edge_handle_to_preserve.end() );

    TargetGraph Gm = Preserve(ball, vertex_handle_to_preserve,
                                      edge_handle_to_preserve);
    assert(Gm.FindVertex(w_vertex_handle->id()));
    // find connected component containing w
    maximum_perfect_subgraphs.emplace_back(ConnectedComponent<true>(Gm, Gm.FindVertex(w_vertex_handle->id())));
  }
  return std::move(maximum_perfect_subgraphs);
};

};

#endif // _GUNDAM_ALGORITHM_SIMULATION_H