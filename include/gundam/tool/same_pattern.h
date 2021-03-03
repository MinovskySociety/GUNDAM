#ifndef _SAME_PATTERN_H
#define _SAME_PATTERN_H

#include "gundam/algorithm/dp_iso_using_match.h"

namespace GUNDAM {

template <typename GraphType0,
          typename GraphType1>
inline bool SamePattern(GraphType0& graph_0,
                        GraphType1& graph_1) {
  // just begin bfs at a random vertex and find whether
  // it can reach all vertexes
  if ((graph_0.CountVertex()
    != graph_1.CountVertex())
    ||(graph_0.CountEdge()
    != graph_1.CountEdge())) {
    return false;
  }

  auto ret = GUNDAM::DpisoUsingMatch(graph_0, graph_1, 1);

  if (ret == 0){
    return false;
  }
  return true;
}

}  // namespace GUNDAM

#endif // _SAME_PATTERN_H