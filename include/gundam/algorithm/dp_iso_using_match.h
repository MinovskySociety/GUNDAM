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
inline size_t DPISO_UsingMatch(
        QueryGraph&  query_graph, 
       TargetGraph& target_graph,
  Match<QueryGraph,
       TargetGraph>& match_state,
  std::map<QueryGraphVertexPtr, 
           std::vector<TargetGraphVertexPtr>>& candidate_set,
  PruneCallback prune_callback,
  MatchCallback match_callback, 
   double time_limit = -1.0) {

  using MatchMap = std::map<QueryGraphVertexPtr, 
                           TargetGraphVertexPtr>;

  using MatchContainer = std::vector<MatchMap>;

  MatchMap match_state;
  std::set<TargetGraphVertexPtr> target_matched;
  for (auto vertex_it = query_graph.VertexBegin(); 
           !vertex_it.IsDone();
            vertex_it++) {
    const QueryGraphVertexPtr src_ptr = vertex_it;
    if (!partical_match.HasMap(src_ptr)) {
      continue;
    }
    const auto dst_ptr = partical_match.MapTo(src_ptr);
    match_state.emplace(src_ptr, dst_ptr);
    target_matched.insert(dst_ptr);
  }


  MatchContainer match_result;

  if (query_graph.CountEdge() < _dp_iso::large_query_edge) {
    size_t result_count = 0;
    _dp_iso::_DPISO<match_semantics, 
                    QueryGraph, 
                   TargetGraph>(
        candidate_set, match_state, target_matched, result_count, 
        match_callback,
        prune_callback, 
        clock(), time_limit);
    return result_count;
  }

  using FailSetContainer = std::vector<QueryGraphVertexPtr>;
  using  ParentContainer = std::map<QueryGraphVertexPtr, 
                                    std::vector<QueryGraphVertexPtr>>;
  FailSetContainer fail_set;
   ParentContainer parent;
  for (const auto &map : match_state) {
    _dp_iso::UpdateParent(match_state, map.first, parent);
  }

  size_t result_count = 0;
  _dp_iso::_DPISO<match_semantics, 
                  QueryGraph, 
                 TargetGraph>(
      candidate_set, match_state, target_matched, parent, fail_set,
      result_count, 
      match_callback, 
      prune_callback,
      clock(), time_limit);
  return result_count;
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename    QueryGraph,
          typename   TargetGraph,
          typename    QueryGraphVertexPtr,
          typename   TargetGraphVertexPtr,
          typename PruneCallback, 
          typename MatchCallback>
inline size_t DPISO_UsingMatch(
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
