#ifndef _GUNDAM_TOOL_SAME_PATTERN_H
#define _GUNDAM_TOOL_SAME_PATTERN_H

#include "gundam/algorithm/match_using_match.h"

namespace GUNDAM {

template <bool vertex_id_needs_to_be_same = false,
          typename GraphType0,
          typename GraphType1>
inline bool SamePattern(GraphType0& graph_0,
                        GraphType1& graph_1
                        #ifndef NDEBUG
                        , bool double_check = true
                        #endif
                        ) {

  if constexpr (vertex_id_needs_to_be_same) {
    using MatchType = Match<GraphType0,
                            GraphType1>;
    bool same_pattern = false;
    auto same_pattern_callback
     = [&same_pattern](const MatchType& match){
      bool same_vertex_id_map = true;
      for (auto map_it = match.MapBegin();
              !map_it.IsDone();
                map_it++) {
        if (map_it->src_handle()->id()
        == map_it->dst_handle()->id()) {
          continue;
        }
        same_vertex_id_map = false;
        break;
      }
      if (same_vertex_id_map) {
        same_pattern = true;
        // stop matching
        return false;
      }
      // continue matching
      return true;
    };
    return same_pattern;
  }

  // just begin bfs at a random vertex and find whether
  // it can reach all vertexes
  if ((graph_0.CountVertex()
    != graph_1.CountVertex())
    ||(graph_0.CountEdge()
    != graph_1.CountEdge())) {
    return false;
  }

  auto ret = GUNDAM::MatchUsingMatch<MatchSemantics::kIsomorphism,
                                     MatchAlgorithm::kVf2,
                                     MergeNecConfig::kNotMerge>(graph_0, graph_1, 1);

  if (ret == 0){
    #ifndef NDEBUG
    if (double_check) {
      assert(!SamePattern(graph_1, graph_0, false));
    }
    #endif // NDEBUG
    return false;
  }
  #ifndef NDEBUG
  if (double_check) {
    assert(SamePattern(graph_1, graph_0, false));
  }
  #endif // NDEBUG
  return true;
}

}  // namespace GUNDAM

#endif // _GUNDAM_TOOL_SAME_PATTERN_H