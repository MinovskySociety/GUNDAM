#ifndef _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DEDUPLICATE_PATTERNS_H
#define _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DEDUPLICATE_PATTERNS_H

#include "gundam/tool/same_pattern.h"

namespace GUNDAM {

template<typename GraphPatternType>
inline void DeduplicatePatterns(std::vector<GraphPatternType>& graph_patterns) {

  for (auto graph_pattern_it  = graph_patterns.begin();
            graph_pattern_it != graph_patterns.end();){
    bool duplicate = false;
    for (auto deduplicated_graph_pattern_it  = graph_patterns.begin();
              deduplicated_graph_pattern_it != graph_pattern_it;
              deduplicated_graph_pattern_it++) {
      if (SamePattern(*graph_pattern_it,
              *deduplicated_graph_pattern_it)){
        duplicate = true;
        break;
      }
    }
    if (duplicate){
      graph_pattern_it = graph_patterns.erase(graph_pattern_it);
      continue;
    }
    graph_pattern_it++;
  }
  return;
}

template<typename GraphPatternType>
inline void DeduplicatePatternsWithDfsCode(std::vector<GraphPatternType>& graph_patterns) {
  /* #################################################### *
   * ##  wenzhi: use DFS code to deduplicate patterns  ## *
   * #################################################### */
  return;
}

}; // namespace GUNDAM

#endif // _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DEDUPLICATE_PATTERNS_H