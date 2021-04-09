#ifndef _GUNDAM_TOOL_MAXIMAL_COMMON_SUBGRAPH
#define _GUNDAM_TOOL_MAXIMAL_COMMON_SUBGRAPH

#include "gundam/tool/unique_patterns.h"

namespace GUNDAM{

template <typename GraphPatternType>
std::vector<GraphPatternType> 
  MaximalCommonSubgraph(GraphPatternType& q0,
                        GraphPatternType& q1){
  if (SamePattern(q0, q1)){
    return std::vector<GraphPatternType>(q0);
  }
  using VertexLabelType = typename GraphPatternType
                                       ::VertexType
                                        ::LabelType;
  using   EdgeLabelType = typename GraphPatternType
                                         ::EdgeType
                                        ::LabelType;
  std::set<VertexLabelType> q0_vertex_label_set;
  std::set<std::tuple<VertexLabelType,
                        EdgeLabelType,
                      VertexLabelType>> q0_edge_type_set;
  for (auto vertex_it = q0.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    q0_vertex_label_set.emplace(vertex_it->label());
    for (auto edge_it = vertex_it->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      q0_edge_type_set.emplace(edge_it->src_handle()->label(),
                               edge_it->label(),
                               edge_it->dst_handle()->label());
    }
  }
  std::set<VertexLabelType> q1_vertex_label_set;
  std::set<std::tuple<VertexLabelType,
                        EdgeLabelType,
                      VertexLabelType>> q1_edge_type_set;
  for (auto vertex_it = q1.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    q1_vertex_label_set.emplace(vertex_it->label());
    for (auto edge_it = vertex_it->OutEdgeBegin();
             !edge_it.IsDone();
              edge_it++) {
      q1_edge_type_set.emplace(edge_it->src_handle()->label(),
                               edge_it->label(),
                               edge_it->dst_handle()->label());
    }
  }

  std::set<VertexLabelType> q0_intersect_q1_vertex_label_set;
 
  std::set_intersection(q0_vertex_label_set.begin(), q0_vertex_label_set.end(),
                        q1_vertex_label_set.begin(), q1_vertex_label_set.end(),
                        std::back_inserter(q0_intersect_q1_vertex_label_set));

  std::set<std::tuple<VertexLabelType,
                        EdgeLabelType,
                      VertexLabelType>> q0_intersect_q1_edge_type_set;
 
  std::set_intersection(q0_edge_type_set.begin(), q0_edge_type_set.end(),
                        q1_edge_type_set.begin(), q1_edge_type_set.end(),
                        std::back_inserter(q0_intersect_q1_edge_type_set));
  
  if (q0_intersect_q1_vertex_label_set.empty()
   || q0_intersect_q1_edge_type_set.empty()){
    return std::vector<GraphPatternType>();
  }

  std::vector<GraphPatternType> current_graph_patterns;

  for (const auto& vertex_label : q0_intersect_q1_vertex_label_set) {
    GraphPatternType pattern;
    pattern.AddVertex(0, vertex_label);
    current_graph_patterns.emplace_back(pattern);
  }
  assert(q0_intersect_q1_vertex_label_set.size()
                == current_graph_patterns.size());

  while (true) {
    std::vector<GraphPatternType> expanded_graph_patterns;

    expanded_graph_patterns = ExpandPattern(current_graph_patterns,
                                  q0_intersect_q1_vertex_label_set,
                                  q0_intersect_q1_edge_type_set);

    UniquePatterns(expanded_graph_patterns);

    for (auto expanded_graph_pattern_it
            = expanded_graph_patterns.begin();
              expanded_graph_pattern_it 
           != expanded_graph_patterns.end();) {
      auto match_in_q0 = DpisoUsingMatch(*expanded_graph_pattern_it, q0, 1);
      assert(match_in_q0 == 0
          || match_in_q0 == 1);
      if (match_in_q0 == 0) {
        // cannot be matched into q0
        expanded_graph_pattern_it = expanded_graph_patterns
                             .erase(expanded_graph_pattern_it);
        continue;
      }

      auto match_in_q1 = DpisoUsingMatch(*expanded_graph_pattern_it, q1, 1);
      assert(match_in_q1 == 0
          || match_in_q1 == 1);
      if (match_in_q1 == 0) {
        // cannot be matched into q1
        expanded_graph_pattern_it = expanded_graph_patterns
                             .erase(expanded_graph_pattern_it);
        continue;
      }

      expanded_graph_pattern_it++;
    }
    if (!expanded_graph_patterns.empty()){
      // the current pattern can be further expanded is not maximal
      current_graph_patterns.swap(expanded_graph_patterns);
      continue;
    }
    // the pattern(s) in current_graph_patterns is maximal
    // select one
    return current_graph_patterns;
  }
  return std::vector<GraphPatternType>();
}

};

#endif // _GUNDAM_TOOL_MAXIMAL_COMMON_SUBGRAPH