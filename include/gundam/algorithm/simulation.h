#ifndef _GUNDAM_ALGORITHM_SIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_H

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/algorithm/dp_iso_using_match.h"
#include "gundam/algorithm/match_semantics.h"

#include "gundam/tool/connected_component.h"
#include "gundam/tool/diameter.h"

#include "gundam/match/match.h"

namespace GUNDAM {

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kDualSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
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
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
     std::map<typename VertexHandle< QueryGraph>::type,
  std::vector<typename VertexHandle<TargetGraph>::type>>& match_set) {

  static_assert(match_semantics == MatchSemantics::kStrongSimulation);

  using  QueryVerteHandleType = typename VertexHandle< QueryGraph>::type;
  using TargetVerteHandleType = typename VertexHandle<TargetGraph>::type;

  using CandidateSet =  std::map< QueryVerteHandleType,
                     std::vector<TargetVerteHandleType>>;

  const size_t dQ = Diameter<true>(query_graph);

  std::vector<QueryGraph> maximum_perfect_subgraphs;

  for (auto vertex_it = target_graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    // to obtain the ball
    TargetGraph ball = KHop<true>(target_graph, vertex_it, dQ);
    CandidateSet match_set_in_ball;
    MatchUsingMatch<MatchSemantics::kDualSimulation>(
                    query_graph, ball,
                    match_set_in_ball);

    // ExtractMaxPG(query_graph, ball, match_set_in_ball);
    std::vector<TargetVerteHandleType> vertex_in_ball;
    bool contained_in_sw = false;
    for (const auto& [query_handle,
                      candidate_set] : match_set_in_ball) {
      std::copy(vertex_in_ball.begin(), 
                vertex_in_ball.end(), 
                std::back_inserter(candidate_set));
      if (std::find(candidate_set.begin(),
                    candidate_set.end(), vertex_it)) {
        contained_in_sw = true;
      }
    }
    if (!contained_in_sw) {
      continue;
    }
    std::sort( vertex_in_ball.begin(), 
               vertex_in_ball.end() );
    vertex_in_ball.erase(
      std::unique( vertex_in_ball.begin(),
                   vertex_in_ball.end() ),
                   vertex_in_ball.end() );
    assert(vertex_in_ball.size() <= ball.CountVertex());
    // construct the matching graph Gm
    QueryGraph Gm = PreserveVertexSet(ball, vertex_in_ball);
    // find connected component containing w
    QueryGraph Gs = ConnectedComponent<true>(Gm, vertex_it);
    maximum_perfect_subgraphs.emplace_back(std::move(Gs));
  }
  return match_count;
};

};

#endif // _GUNDAM_ALGORITHM_SIMULATION_H