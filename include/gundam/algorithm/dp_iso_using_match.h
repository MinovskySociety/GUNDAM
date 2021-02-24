#ifndef _DPISO_USING_MATCH_H
#define _DPISO_USING_MATCH_H
#include <cassert>
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <type_traits>
#include <vector>

#include "gundam/algorithm/dpiso.h"
#include "gundam/match/match.h"

namespace GUNDAM {

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename    QueryGraph,
          typename   TargetGraph,
          typename    QueryGraphVertexPtr,
          typename   TargetGraphVertexPtr,
          typename PruneCallback, 
          typename MatchCallback>
inline int DPISO_UsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  Match<QueryGraph,
       TargetGraph>& match_state,
  std::map<QueryGraphVertexPtr, 
            std::vector<TargetGraphVertexPtr>>& candidate_set,
  PruneCallback prune_callback,
  MatchCallback match_callback, 
   double query_limit_time = -1.0) {
  
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename    QueryGraph,
          typename   TargetGraph,
          typename    QueryGraphVertexPtr,
          typename   TargetGraphVertexPtr,
          typename PruneCallback, 
          typename MatchCallback>
inline int DPISO_UsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  std::map<QueryGraphVertexPtr, 
            std::vector<TargetGraphVertexPtr>>& candidate_set,
  PruneCallback prune_callback
  MatchCallback match_callback, 
   double time_limit = -1.0) {

  Match<QueryGraph, TargetGraph> match_state;

  return DPISO_UsingMatch(query_graph, 
                         target_graph,
                          match_state,
                        candidate_set,
                       prune_callback,
                       match_callback, 
                           time_limit);
}


}  // namespace GUNDAM
#endif //_DPISO_USING_MATCH_H
