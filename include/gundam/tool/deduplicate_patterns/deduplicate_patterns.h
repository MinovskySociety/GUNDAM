#ifndef _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DEDUPLICATE_PATTERNS_H
#define _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DEDUPLICATE_PATTERNS_H

#include "gundam/tool/same_pattern.h"
#include "gundam/tool/deduplicate_patterns/dfs_code.h"
#include <vector>
#include <map>
#include <gundam/component/disjoint_set.h>
#include <set>

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

  std::map<DfsCode<GraphPatternType>, std::set<size_t>> dfs_code_map;

  for (size_t i = 0; i < graph_patterns.size(); i++) {
    auto& pattern = graph_patterns[i];
    std::vector<DfsCode<GraphPatternType>> dfs_codes = GetDFSCode(pattern);
    for (size_t j = 0; j < dfs_codes.size(); j++) {
      dfs_code_map[dfs_codes[j]].insert(i);
    }
  }

  DisjointSet<size_t> disjoint_set(graph_patterns.size()); 

  for (const auto& [dfs_code, pattern_id_set] : dfs_code_map) {
    for (const auto& pattern_id : pattern_id_set) {
      disjoint_set.Merge(*pattern_id_set.begin(), pattern_id);
    }
  }

  dfs_code_map.clear();

  std::map<size_t, std::vector<size_t>> merged_pattern_id_map;
  for (size_t i = 0; i < graph_patterns.size(); i++) {
    merged_pattern_id_map[disjoint_set.Find(i)].emplace_back(i);
  }

  std::vector<GraphPatternType> ret_graph_patterns;
  for (auto &p : merged_pattern_id_map) {
    size_t id = p.first;
    ret_graph_patterns.push_back(graph_patterns[id]);
  }
  graph_patterns.swap(ret_graph_patterns);

  return;
}

}; // namespace GUNDAM

#endif // _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DEDUPLICATE_PATTERNS_H
