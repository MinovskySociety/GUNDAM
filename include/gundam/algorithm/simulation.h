#ifndef _GUNDAM_ALGORITHM_SIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_H

#include "gundam/algorithm/dp_iso_using_match.h"

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

};

#endif // _GUNDAM_ALGORITHM_SIMULATION_H