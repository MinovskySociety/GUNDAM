#ifndef _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H
#define _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H

#include "gundam/tool/is_star.h"
#include "gundam/tool/is_link.h"
#include "gundam/tool/connected_component.h"

namespace GUNDAM {
/* ######################################################################## *
 * ##  wenzhi:                                                           ## *
 * ##    move dfs code from                                              ## *
 * ##    grape-gundam/src/apps/gar_discover/gar_discover/gar_dfs_code.h  ## *
 * ######################################################################## */

// modify from 
//    template <typename GraphPatternType>
//    class GARDFSCode;
template <class GraphType>
class DfsCode;

// modify from 
//    template <class GraphPatternType, 
//              class DFSCodeContainer>
//    inline void GetGARDFSCode(GraphPatternType& graph_pattern,
//           std::vector<typename GUNDAM::VertexHandle<GraphPatternType>::type>& src_vertex_handle_set,
//                               DFSCodeContainer& dfs_code_container);
template <class GraphPatternType, 
          class DFSCodeContainer>
inline void GetDFSCode(GraphPatternType& graph_pattern,
 const std::vector<
 typename VertexHandle<GraphPatternType>::type>& src_vertex_handle_set,
                               DFSCodeContainer& dfs_code_container);

// generate all possible DFS code here, call the above method
template <class GraphPatternType, 
          class DFSCodeContainer>
inline void GetDFSCode(const GraphPatternType& graph_pattern,
                             DFSCodeContainer& dfs_code_container) {
  std::vector<GraphPatternType> decomposed_patterns 
                = ConnectedComponent(graph_pattern);
  for (const auto& pattern : decomposed_patterns) {
    if (IsLink<true>(pattern)) {
      // do some thing for link
      continue;
    }
    if (IsStar<true>(pattern)) {
      // do some thing for star
      continue;
    }
    // do some thing for general pattern
  }
  if (decomposed_patterns.size() == 1) {
    return;
  }
  // do something for dis-connected pattern
  return;
}

};

#endif // _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H