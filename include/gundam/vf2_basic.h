#ifndef _VF2_BASIC_H
#define _VF2_BASIC_H

#include "gundam/graph.h"
#include "gundam/vf2.h"

#include <map>
#include <set>

namespace GUNDAM {

namespace __VF2Basic {

enum class EdgeState { kIn_, kOut_ };

// Cal Degree
template <enum EdgeState edge_state, typename VertexPtr>
int Degree(VertexPtr vertex_ptr) {
  int vertex_degree = 0;
  for (auto it =
           ((edge_state == EdgeState::kIn_) ? vertex_ptr->InEdgeCBegin()
                                            : vertex_ptr->OutEdgeCBegin());
       !it.IsDone(); it++) {
    vertex_degree++;
  }
  return vertex_degree;
}

// Init Candidate Set
template <enum MatchSemantics match_semantics, class VertexCompare,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
bool InitCandidateSet(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set,
    VertexCompare vertex_comp) {
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
    int query_in_count = Degree<EdgeState::kIn_>(query_ptr);
    int query_out_count = Degree<EdgeState::kOut_>(query_ptr);
    auto &q_t = candidate_set[query_it];
    for (auto target_it = target_graph.VertexCBegin(); !target_it.IsDone();
         target_it++) {
      DataGraphVertexPtr target_ptr = target_it;

      if (!vertex_comp(query_ptr, target_ptr)) continue;

      // std::cout << query_it->id() << " " << target_it->id() << std::endl;

      int target_in_count = Degree<EdgeState::kIn_>(target_ptr);
      if (target_in_count < query_in_count) continue;
      int target_out_count = Degree<EdgeState::kOut_>(target_ptr);
      if (target_out_count < query_out_count) continue;

      q_t.emplace_back(std::move(target_ptr));
    }
    if (q_t.empty()) return false;
  }
  return true;
}
/* // Check target_id is in C(query_vertex_ptr)
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
} */

template <enum EdgeState edge_state, typename PatternVertexPtr,
          typename DataGraphVertexPtr, class EdgeCompare,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
bool JoinableCheck(const GraphType0<configures0...> &query_graph,
                   const GraphType1<configures1...> &target_graph,
                   PatternVertexPtr query_vertex_ptr,
                   DataGraphVertexPtr target_vertex_ptr,
                   std::map<PatternVertexPtr, DataGraphVertexPtr> &match_state,
                   EdgeCompare edge_comp) {
  for (auto query_edge_iter = ((edge_state == EdgeState::kIn_)
                                   ? query_vertex_ptr->InEdgeCBegin()
                                   : query_vertex_ptr->OutEdgeCBegin());
       !query_edge_iter.IsDone(); query_edge_iter++) {
    PatternVertexPtr query_opp_vertex_ptr =
        (edge_state == EdgeState::kIn_) ? query_edge_iter->const_src_ptr()
                                        : query_edge_iter->const_dst_ptr();

    // if (match_state.count(query_opp_vertex_ptr) == 0) continue;
    // DataGraphVertexPtr query_opp_match_vertex_ptr =
    //    match_state[query_opp_vertex_ptr];

    auto match_iter = match_state.find(query_opp_vertex_ptr);
    if (match_iter == match_state.end()) continue;
    DataGraphVertexPtr query_opp_match_vertex_ptr = match_iter->second;

    bool find_target_flag = false;
    for (auto target_edge_iter = ((edge_state == EdgeState::kIn_)
                                      ? target_vertex_ptr->InEdgeCBegin()
                                      : target_vertex_ptr->OutEdgeCBegin());
         !target_edge_iter.IsDone(); target_edge_iter++) {
      auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn_)
                                       ? target_edge_iter->const_src_ptr()
                                       : target_edge_iter->const_dst_ptr();
      if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
        // auto query_edge_ptr =
        //     (edge_state == EdgeState::kOut_)
        //         ? query_vertex_ptr->FindConstOutEdge(query_edge_iter->id())
        //         :
        //         query_opp_vertex_ptr->FindConstOutEdge(query_edge_iter->id());

        // auto target_edge_ptr =
        //     (edge_state == EdgeState::kOut_)
        //         ? target_vertex_ptr->FindConstOutEdge(target_edge_iter->id())
        //         : query_opp_match_vertex_ptr->FindConstOutEdge(
        //               target_edge_iter->id());

        if (edge_comp(query_edge_iter, target_edge_iter)) {
          find_target_flag = true;
          break;
        }
      }
    }
    if (!find_target_flag) return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class EdgeCompare>
bool IsJoinable(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    typename GraphType0<configures0...>::VertexConstPtr query_vertex_ptr,
    typename GraphType1<configures1...>::VertexConstPtr target_vertex_ptr,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             typename GraphType1<configures1...>::VertexConstPtr> &match_state,
    std::set<typename GraphType1<configures1...>::VertexConstPtr>
        &target_graph_used_node,
    EdgeCompare edge_comp) {
  if (match_semantics == GUNDAM::MatchSemantics::kIsomorphism &&
      target_graph_used_node.count(target_vertex_ptr) > 0) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kIn_>(query_graph, target_graph,
                                      query_vertex_ptr, target_vertex_ptr,
                                      match_state, edge_comp)) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kOut_>(query_graph, target_graph,
                                       query_vertex_ptr, target_vertex_ptr,
                                       match_state, edge_comp)) {
    return false;
  }
  return true;
}

template <template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
std::vector<
    std::pair<size_t, typename GraphType0<configures0...>::VertexConstPtr>>
DetermineMatchOrder(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             typename GraphType1<configures1...>::VertexConstPtr>
        &match_state) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;

  std::set<PatternVertexPtr> next_query_set;
  for (const auto &match_pair : match_state) {
    const PatternVertexPtr &query_u_ptr = match_pair.first;
    for (auto edge_iter = query_u_ptr->OutEdgeCBegin(); !edge_iter.IsDone();
         edge_iter++) {
      if (match_state.count(edge_iter->const_dst_ptr()) > 0) continue;
      next_query_set.insert(edge_iter->const_dst_ptr());
    }
    for (auto edge_iter = query_u_ptr->InEdgeCBegin(); !edge_iter.IsDone();
         edge_iter++) {
      if (match_state.count(edge_iter->const_src_ptr()) > 0) continue;
      next_query_set.insert(edge_iter->const_src_ptr());
    }
  }
  if (next_query_set.empty()) {
    for (auto vertex_iter = query_graph.VertexCBegin(); !vertex_iter.IsDone();
         vertex_iter++) {
      next_query_set.insert(vertex_iter);
    }
  }

  std::vector<std::pair<size_t, PatternVertexPtr>> query_vertex_sequence;
  for (const auto &vertex_iter : next_query_set) {
    query_vertex_sequence.emplace_back(candidate_set[vertex_iter].size(),
                                       vertex_iter);
  }

  sort(query_vertex_sequence.begin(), query_vertex_sequence.end());

  return std::move(query_vertex_sequence);
}

template <typename QueryPtr, typename TargetPtr>
void UpdateState(QueryPtr query_vertex_ptr, TargetPtr target_vertex_ptr,
                 std::map<QueryPtr, TargetPtr> &match_state,
                 std::set<TargetPtr> &target_graph_used_node) {
  match_state.emplace(query_vertex_ptr, target_vertex_ptr);
  target_graph_used_node.emplace(target_vertex_ptr);
}

template <typename QueryPtr, typename TargetPtr>
void RestoreState(QueryPtr query_vertex_ptr, TargetPtr target_vertex_ptr,
                  std::map<QueryPtr, TargetPtr> &match_state,
                  std::set<TargetPtr> &target_graph_used_node) {
  match_state.erase(query_vertex_ptr);
  target_graph_used_node.erase(target_vertex_ptr);
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class EdgeCompare>
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
    EdgeCompare edge_comp, int max_result = -1) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexLabelType = typename PatternType::VertexType::LabelType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;

  size_t query_size = query_graph.CountVertex();
  if (match_state.size() == query_size) {
    match_result.push_back(match_state);
    return static_cast<int>(match_result.size());
  }

  auto query_vertex_sequence = DetermineMatchOrder(query_graph, target_graph,
                                                   candidate_set, match_state);

  int find_match_tag = 0;
  const PatternVertexPtr &next_query_vertex = query_vertex_sequence[0].second;
  for (const auto &next_target_vertex : candidate_set[next_query_vertex]) {
    if (IsJoinable<match_semantics>(
            query_graph, target_graph, next_query_vertex, next_target_vertex,
            match_state, target_graph_used_node, edge_comp)) {
      UpdateState(next_query_vertex, next_target_vertex, match_state,
                  target_graph_used_node);

      VF2<match_semantics>(query_graph, target_graph, candidate_set,
                           match_state, target_graph_used_node, match_result,
                           edge_comp, max_result);

      if (max_result >= 0 &&
          static_cast<int>(match_result.size()) >= max_result)
        return static_cast<int>(match_result.size());

      RestoreState(next_query_vertex, next_target_vertex, match_state,
                   target_graph_used_node);
    }
  }

  return static_cast<int>(match_result.size());
}
};  // namespace __VF2Basic

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class VertexCompare, class EdgeCompare>
int VF2(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::vector<std::map<typename GraphType0<configures0...>::VertexConstPtr,
                         typename GraphType1<configures1...>::VertexConstPtr>>
        &match_result,
    VertexCompare vertex_comp, EdgeCompare edge_comp, int max_result = -1) {
  using PatternType = GraphType0<configures0...>;
  using DataGraphType = GraphType1<configures1...>;
  using PatternIDType = typename PatternType::VertexType::IDType;
  using DataGraphIDType = typename DataGraphType::VertexType::IDType;
  using PatternVertexType = typename PatternType::VertexType;
  using PatternVertexPtr = typename PatternType::VertexConstPtr;
  using DataGraphVertexType = typename DataGraphType::VertexType;
  using DataGraphVertexPtr = typename DataGraphType::VertexConstPtr;

  match_result.clear();

  std::map<PatternVertexPtr, std::vector<DataGraphVertexPtr>> candidate_set;
  if (!__VF2Basic::InitCandidateSet<match_semantics>(
          query_graph, target_graph, candidate_set, vertex_comp)) {
    return 0;
  }

  std::map<PatternVertexPtr, DataGraphVertexPtr> match_state;
  std::set<DataGraphVertexPtr> target_used_node;
  return __VF2Basic::VF2<match_semantics>(
      query_graph, target_graph, candidate_set, match_state, target_used_node,
      match_result, edge_comp, max_result);
}
};  // namespace GUNDAM

#endif