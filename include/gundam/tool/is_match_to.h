#ifndef _GUNDAM_IS_MATCH_TO_H
#define _GUNDAM_IS_MATCH_TO_H

#include "gundam/match/match.h"
#include "gundam/algorithm/match_using_match.h"

namespace GUNDAM {

// return true if it is a match from graph_0 to graph_1
template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename GraphType0,
          typename GraphType1>
inline bool IsMatchTo(GraphType0& graph_0,
                      GraphType1& graph_1,                       
          const Match<GraphType0,
                      GraphType1>& match_graph_0_to_graph_1) {
                        
  if (graph_0.CountVertex() != match_graph_0_to_graph_1.size()) {
    return false;
  }

  auto ret = GUNDAM::MatchUsingMatch<match_semantics,
                                     MatchAlgorithm::kDagDp,
                                     MergeNecConfig::kNotMerge>(graph_0, graph_1, match_graph_0_to_graph_1);
  assert(ret == 0 || ret == 1);
  if (ret == 0){
    return false;
  }
  return true;
}

// return true if it is a match from graph_0 to graph_1
template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename GraphType0,
          typename GraphType1>
inline bool IsPartialMatchTo(GraphType0& graph_0,
                             GraphType1& graph_1,                       
                 const Match<GraphType0,
                             GraphType1>& match_graph_0_to_graph_1) {
                        
  if (graph_0.CountVertex() < match_graph_0_to_graph_1.size()) {
    return false;
  }

  return _dp_iso_using_match::CheckMatchIsLegal<match_semantics>(match_graph_0_to_graph_1);
}

}  // namespace GUNDAM

#endif // _GUNDAM_IS_MATCH_TO_H