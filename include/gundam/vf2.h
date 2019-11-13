#ifndef _VF2_H
#define _VF2_H

#include "graph.h"

#include <algorithm>
#include <iostream>
#include <set>

namespace GUNDAM {

/// enum   match tyep bool is_iso,1/// iso or homo
enum class MatchSemantics { kIsomorphism, kHomomorphism };

namespace __VF2 {

enum class EdgeState : bool { kIn_, kOut_ };
// Cal Degree
template <enum EdgeState edge_state, typename VertexPtr>
int Degree(VertexPtr vertex_ptr) {
  int vertex_degree = 0;
  for (auto it =
           ((edge_state == EdgeState::kIn_) ?
           vertex_ptr->InEdgeCBegin() : vertex_ptr->OutEdgeCBegin());
       !it.IsDone(); it++) {
    ++vertex_degree;
  }
  return vertex_degree;
}
// Init Candidate Set
template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int InitCandidateSet(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;
  for (auto query_it = query_graph.VertexCBegin(); !query_it.IsDone();
       query_it++) {
    PatternVertexPtr query_ptr = query_it;
    for (auto target_it = target_graph.VertexCBegin(query_it->label()); 
        !target_it.IsDone(); target_it++) {
      DataGraphVertexPtr target_ptr = target_it;
      int query_in_count = 0, query_out_count = 0;
      int target_in_count = 0, target_out_count = 0;
      query_in_count = Degree<EdgeState::kIn_>(query_ptr);
      query_out_count = Degree<EdgeState::kOut_>(query_ptr);
      target_in_count = Degree<EdgeState::kIn_>(target_ptr);
      target_out_count = Degree<EdgeState::kOut_>(target_ptr);
      if ((target_in_count < query_in_count) ||
          (target_out_count < query_out_count)) {
        continue;
      }
      candidate_set[query_ptr].push_back(target_ptr);
    }
  }
  return 1;
}

// Check target_id is in C(query_vertex_ptr)
template <typename QueryPtr, typename TargetPtr>
int CheckIsInCandidateSet(
    QueryPtr query_vertex_ptr,
    std::map<QueryPtr, std::vector<TargetPtr>> &candidate_set, int target_id) {
  for (auto it : candidate_set[query_vertex_ptr]) {
    if (it->id() == target_id) {
      return 1;
    }
  }
  return 0;
}

template <enum EdgeState edge_state, typename PatternVertexPtr,
          typename DataGraphVertexPtr>
int JoinableCheck(PatternVertexPtr query_vertex_ptr,
                  DataGraphVertexPtr target_vertex_ptr,
                  std::map<PatternVertexPtr, DataGraphVertexPtr> &match_state) {
  for (auto it = ((edge_state == EdgeState::kIn_)
                      ? query_vertex_ptr->InEdgeCBegin()
                      : query_vertex_ptr->OutEdgeCBegin());
       !it.IsDone(); it++) {
    PatternVertexPtr temp_vertex_ptr = (edge_state == EdgeState::kIn_)
                                           ? it->const_src_ptr()
                                           : it->const_dst_ptr();
    if (!match_state.count(temp_vertex_ptr)) continue;
    DataGraphVertexPtr temp_target_vertex_ptr = match_state[temp_vertex_ptr];
    int find_target_flag = 0;
	// test by wenzhi:
   // std::cout << it->label().to_string() << std::endl;
    auto wenzhi_label = it->label();
    auto wenzhi_it = target_vertex_ptr->InEdgeCBegin();
    for (auto bit = ((edge_state == EdgeState::kIn_)
                         ? target_vertex_ptr-> InEdgeCBegin(it->label())
                         : target_vertex_ptr->OutEdgeCBegin(it->label()));
         !bit.IsDone(); bit++) {
      auto temp_id = (edge_state == EdgeState::kIn_)
                         ? bit->const_src_ptr()->id()
                         : bit->const_dst_ptr()->id();
      if (temp_id == temp_target_vertex_ptr->id()) {
        find_target_flag = 1;
        break;
      }
    }
    if (find_target_flag == 0) return 0;
  }
  return 1;
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int IsJoinable(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    typename GraphType0<configures0...>::VertexConstPtr next_query_vertex,
    typename GraphType1<configures1...>::VertexConstPtr next_target_vertex,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             typename GraphType1<configures1...>::VertexConstPtr> &match_state,
    std::set<typename GraphType1<configures1...>::VertexConstPtr>
        &target_graph_used_node) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;
  if (match_semantics == GUNDAM::MatchSemantics::kIsomorphism)
    if (target_graph_used_node.count(next_target_vertex))
      return 0;
  if (!JoinableCheck<EdgeState::kIn_>(
          next_query_vertex, next_target_vertex, match_state)) {
    return 0;
  }
  if (!JoinableCheck<EdgeState::kOut_>(
          next_query_vertex, next_target_vertex, match_state)) {
    return 0;
  }
  return 1;
}

template <template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int DetermineMatchOrder(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             typename GraphType1<configures1...>::VertexConstPtr> &match_state,
    std::vector<
        std::pair<int, typename GraphType0<configures0...>::VertexConstPtr>>
        &query_vertex_sequence) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;
  std::set<PatternVertexPtr> next_query_set;
  for (auto &it : match_state) {
    PatternVertexPtr query_u_ptr = it.first;
    for (auto bit = query_u_ptr->OutEdgeCBegin(); !bit.IsDone(); bit++) {
      if (match_state.count(bit->const_dst_ptr())) continue;
      next_query_set.insert(bit->const_dst_ptr());
    }
    for (auto bit = query_u_ptr->InEdgeCBegin(); !bit.IsDone(); bit++) {
      if (match_state.count(bit->const_src_ptr())) continue;
      next_query_set.insert(bit->const_src_ptr());
    }
  }
  if (next_query_set.size()==0){
    for (auto bit = query_graph.VertexCBegin(); !bit.IsDone(); bit++){
      PatternVertexPtr query_it = bit;
      next_query_set.insert(query_it);
    }
  }
  for (auto it : next_query_set) {
    query_vertex_sequence.push_back(
        std::make_pair((int)candidate_set[it].size(), it));
  }
  sort(query_vertex_sequence.begin(), query_vertex_sequence.end());
  return 1;
}

template <typename QueryPtr, typename TargetPtr>
int UpdateState(QueryPtr query_vertex_ptr, TargetPtr target_vertex_ptr,
                std::map<QueryPtr, TargetPtr> &match_state,
                std::set<TargetPtr> &target_graph_used_node) {
  match_state.insert(std::make_pair(query_vertex_ptr, target_vertex_ptr));
  target_graph_used_node.insert(target_vertex_ptr);
  return 1;
}
template <typename QueryPtr, typename TargetPtr>
int RestoreState(QueryPtr query_vertex_ptr, TargetPtr target_vertex_ptr,
                 std::map<QueryPtr, TargetPtr> &match_state,
                 std::set<TargetPtr> &target_graph_used_node) {
  match_state.erase(match_state.find(query_vertex_ptr));
  target_graph_used_node.erase(target_vertex_ptr);
  return 1;
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int VF2(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             typename GraphType1<configures1...>::VertexConstPtr> &match_state,
    std::set<typename GraphType1<configures1...>::VertexConstPtr>
        &target_graph_used_node,
    std::vector<std::map<typename GraphType0<configures0...>::VertexConstPtr,
                         typename GraphType1<configures1...>::VertexConstPtr>>
        &match_result,
    int top_k = -1) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexLabelType = typename PatternType::VertexType::LabelType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;
  int find_match_tag = 0;
  size_t query_size = query_graph.CountVertex();
  if (match_state.size() == query_size) {
    match_result.push_back(match_state);
    return 1;
  }
  std::vector<std::pair<int, PatternVertexPtr>> query_vertex_sequence;
  DetermineMatchOrder(query_graph, target_graph, candidate_set, match_state,
                      query_vertex_sequence);
  PatternVertexPtr next_query_vertex = query_vertex_sequence[0].second;
  for (auto next_target_vertex : candidate_set[next_query_vertex]) {
    if (IsJoinable<match_semantics>(query_graph, target_graph,
                                    next_query_vertex, next_target_vertex,
                                    match_state, target_graph_used_node)) {
      UpdateState(next_query_vertex, next_target_vertex, match_state,
                  target_graph_used_node);
      VF2<match_semantics>(query_graph, target_graph, candidate_set,
                           match_state, target_graph_used_node, match_result,
                           top_k);
      if (top_k >= 0 && match_result.size() >= top_k) return 1;
      RestoreState(next_query_vertex, next_target_vertex, match_state,
                   target_graph_used_node);
    }
  }
  if (!match_result.empty())
    return 1;
  else
    return 0;
}
};  // namespace __VF2

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int VF2(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    typename GraphType0<configures0...>::VertexType::IDType query_id,
    typename GraphType1<configures1...>::VertexType::IDType target_id,
    std::vector<std::map<typename GraphType0<configures0...>::VertexConstPtr,
                         typename GraphType1<configures1...>::VertexConstPtr>>
        &match_result,
    int top_k = 2147483647) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;
  std::map<typename GraphType0<configures0...>::VertexConstPtr,
           std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
      candidate_set;
  __VF2::InitCandidateSet<match_semantics>(query_graph, target_graph, candidate_set);
  PatternVertexPtr query_ptr = query_graph.FindConstVertex(query_id);
  DataGraphVertexPtr target_ptr = target_graph.FindConstVertex(target_id);
  if (!__VF2::CheckIsInCandidateSet(query_ptr, candidate_set, target_id)) {
    return 0;
  }
  std::map<PatternVertexPtr, DataGraphVertexPtr> match_state;
  std::set<DataGraphVertexPtr> target_used_node;
  __VF2::UpdateState(query_ptr, target_ptr, match_state, target_used_node);
  if (__VF2::VF2<match_semantics>(query_graph, target_graph, candidate_set,
                                       match_state, target_used_node,
                                       match_result, top_k)) {
    return static_cast<int>(match_result.size());
  }
  return static_cast<int>(match_result.size());
}


template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int VF2(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::vector<std::map<typename GraphType0<configures0...>::VertexConstPtr,
                         typename GraphType1<configures1...>::VertexConstPtr>>
        &match_result,
    int top_k = -1) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;
  std::map<typename GraphType0<configures0...>::VertexConstPtr,
           std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
      candidate_set;
  __VF2::InitCandidateSet<match_semantics>(query_graph, target_graph, candidate_set);

  std::map<PatternVertexPtr, DataGraphVertexPtr> match_state;
  std::set<DataGraphVertexPtr> target_used_node;
  if (__VF2::VF2<match_semantics>(query_graph, target_graph, candidate_set,
                                       match_state, target_used_node,
                                       match_result, top_k)) {
    return static_cast<int>(match_result.size());
  }
  return static_cast<int>(match_result.size());
}
}  // namespace GUNDAM

#endif