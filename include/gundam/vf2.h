#ifndef _VF2_NEW_H
#define _VF2_NEW_H

#include "graph.h"

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <vector>

namespace GUNDAM {

// enum   match tyep bool is_iso,1/// iso or homo
enum MatchSemantics { kIsomorphism, kHomomorphism };

namespace _vf2 {

enum EdgeState { kIn, kOut };

// Init Candidate Set (with label)
template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int InitCandidateSet(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  for (auto query_vertex_iter = query_graph.VertexCBegin();
       !query_vertex_iter.IsDone(); query_vertex_iter++) {
    QueryVertexPtr query_vertex_ptr = query_vertex_iter;
    query_vertex_ptr->CountInVertex();
    auto query_in_count = query_vertex_ptr->CountInEdge();
    auto query_out_count = query_vertex_ptr->CountOutEdge();

    auto &l = candidate_set[query_vertex_iter];
    for (auto target_vertex_iter =
             target_graph.VertexCBegin(query_vertex_iter->label());
         !target_vertex_iter.IsDone(); target_vertex_iter++) {
      TargetVertexPtr target_vertex_ptr = target_vertex_iter;

      auto target_in_count = target_vertex_ptr->CountInEdge();
      if (target_in_count < query_in_count) continue;
      auto target_out_count = target_vertex_ptr->CountOutEdge();
      if (target_out_count < query_out_count) continue;

      l.emplace_back(target_vertex_ptr);
    }
    if (l.empty()) return false;
  }
  return true;
}

// Init Candidate Set
template <enum MatchSemantics match_semantics, class VertexCompare,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
bool InitCandidateSet(
    const GraphType0<configures0...> &query_graph,
    const GraphType1<configures1...> &target_graph, VertexCompare vertex_comp,
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
             std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  for (auto query_vertex_iter = query_graph.VertexCBegin();
       !query_vertex_iter.IsDone(); query_vertex_iter++) {
    QueryVertexPtr query_vertex_ptr = query_vertex_iter;

    auto query_in_count = query_vertex_ptr->CountInEdge();
    auto query_out_count = query_vertex_ptr->CountOutEdge();

    auto &l = candidate_set[query_vertex_iter];
    for (auto target_vertex_iter = target_graph.VertexCBegin();
         !target_vertex_iter.IsDone(); target_vertex_iter++) {
      TargetVertexPtr target_vertex_ptr = target_vertex_iter;

      if (!vertex_comp(query_vertex_ptr, target_vertex_ptr)) continue;

      auto target_in_count = target_vertex_ptr->CountInEdge();
      if (target_in_count < query_in_count) continue;
      auto target_out_count = target_vertex_ptr->CountOutEdge();
      if (target_out_count < query_out_count) continue;

      l.emplace_back(target_vertex_ptr);
    }
    if (l.empty()) return false;
  }
  return true;
}

// Check target_id is in C(query_vertex_ptr)
template <class QueryVertexPtr, class TargetVertexPtr, class TargetVertexID>
bool CheckIsInCandidateSet(
    QueryVertexPtr query_vertex_ptr,
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const TargetVertexID &target_id) {
  for (const auto &v : candidate_set[query_vertex_ptr]) {
    if (v->id() == target_id) {
      return true;
    }
  }
  return false;
}

template <class QueryVertexPtr, class TargetVertexPtr, class MatchStateMap>
QueryVertexPtr DetermineMatchOrder(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const MatchStateMap &match_state) {
  std::set<QueryVertexPtr> next_query_set;
  for (const auto &match_pair : match_state) {
    const auto &query_vertex_ptr = match_pair.first;

    for (auto edge_iter = query_vertex_ptr->OutEdgeCBegin();
         !edge_iter.IsDone(); edge_iter++) {
      auto query_opp_vertex_ptr = edge_iter->const_dst_ptr();
      if (match_state.count(query_opp_vertex_ptr) == 0) {
        next_query_set.emplace(query_opp_vertex_ptr);
      }
    }

    for (auto edge_iter = query_vertex_ptr->InEdgeCBegin(); !edge_iter.IsDone();
         edge_iter++) {
      auto query_opp_vertex_ptr = edge_iter->const_src_ptr();
      if (match_state.count(query_opp_vertex_ptr) == 0) {
        next_query_set.emplace(query_opp_vertex_ptr);
      }
    }
  }

  if (next_query_set.empty()) {
    for (const auto &candidate_pair : candidate_set) {
      const auto &query_vertex_ptr = candidate_pair.first;
      if (match_state.count(query_vertex_ptr) == 0) {
        next_query_set.emplace(query_vertex_ptr);
      }
    }
  }

  QueryVertexPtr res;
  size_t min = SIZE_MAX;
  for (const auto &query_vertex_ptr : next_query_set) {
    size_t candidate_count =
        candidate_set.find(query_vertex_ptr)->second.size();
    if (candidate_count < min) {
      res = query_vertex_ptr;
      min = candidate_count;
    }
  }
  assert(min > 0 && !res.IsNull());
  return res;
}

template <enum EdgeState edge_state, typename QueryVertexPtr,
          typename TargetVertexPtr>
bool JoinableCheck(
    QueryVertexPtr query_vertex_ptr, TargetVertexPtr target_vertex_ptr,
    const std::map<QueryVertexPtr, TargetVertexPtr> &match_state) {
  for (auto query_edge_iter =
           ((edge_state == EdgeState::kIn) ? query_vertex_ptr->InEdgeCBegin()
                                           : query_vertex_ptr->OutEdgeCBegin());
       !query_edge_iter.IsDone(); query_edge_iter++) {
    QueryVertexPtr query_opp_vertex_ptr =
        (edge_state == EdgeState::kIn) ? query_edge_iter->const_src_ptr()
                                       : query_edge_iter->const_dst_ptr();

    // if (match_state.count(query_opp_vertex_ptr) > 0) continue;
    // TargetVertexPtr query_opp_match_vertex_ptr =
    //    match_state[query_opp_vertex_ptr];

    auto match_iter = match_state.find(query_opp_vertex_ptr);
    if (match_iter == match_state.end()) continue;
    TargetVertexPtr query_opp_match_vertex_ptr = match_iter->second;

    bool find_target_flag = false;
    for (auto target_edge_iter =
             ((edge_state == EdgeState::kIn)
                  ? target_vertex_ptr->InEdgeCBegin(query_edge_iter->label())
                  : target_vertex_ptr->OutEdgeCBegin(query_edge_iter->label()));
         !target_edge_iter.IsDone(); target_edge_iter++) {
      auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
                                       ? target_edge_iter->const_src_ptr()
                                       : target_edge_iter->const_dst_ptr();

      if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
        find_target_flag = true;
        break;
      }
    }
    if (!find_target_flag) return false;
  }
  return true;
}

template <enum EdgeState edge_state, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class EdgeCompare>
bool JoinableCheck(QueryVertexPtr query_vertex_ptr,
                   TargetVertexPtr target_vertex_ptr,
                   const MatchStateMap &match_state, EdgeCompare edge_comp) {
  for (auto query_edge_iter =
           ((edge_state == EdgeState::kIn) ? query_vertex_ptr->InEdgeCBegin()
                                           : query_vertex_ptr->OutEdgeCBegin());
       !query_edge_iter.IsDone(); query_edge_iter++) {
    QueryVertexPtr query_opp_vertex_ptr =
        (edge_state == EdgeState::kIn) ? query_edge_iter->const_src_ptr()
                                       : query_edge_iter->const_dst_ptr();

    // if (match_state.count(query_opp_vertex_ptr) == 0) continue;
    // TargetVertexPtr query_opp_match_vertex_ptr =
    //    match_state[query_opp_vertex_ptr];

    auto match_iter = match_state.find(query_opp_vertex_ptr);
    if (match_iter == match_state.end()) continue;
    TargetVertexPtr query_opp_match_vertex_ptr = match_iter->second;

    bool find_target_flag = false;
    for (auto target_edge_iter = ((edge_state == EdgeState::kIn)
                                      ? target_vertex_ptr->InEdgeCBegin()
                                      : target_vertex_ptr->OutEdgeCBegin());
         !target_edge_iter.IsDone(); target_edge_iter++) {
      auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
                                       ? target_edge_iter->const_src_ptr()
                                       : target_edge_iter->const_dst_ptr();
      if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
        // auto query_edge_ptr =
        //     (edge_state == EdgeState::kOut)
        //         ? query_vertex_ptr->FindConstOutEdge(query_edge_iter->id())
        //         :
        //         query_opp_vertex_ptr->FindConstOutEdge(query_edge_iter->id());

        // auto target_edge_ptr =
        //     (edge_state == EdgeState::kOut)
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

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet>
bool IsJoinable(QueryVertexPtr query_vertex_ptr,
                TargetVertexPtr target_vertex_ptr,
                const MatchStateMap &match_state,
                const TargetVertexSet &target_matched) {
  if (match_semantics == MatchSemantics::kIsomorphism &&
      target_matched.count(target_vertex_ptr) > 0) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kIn>(query_vertex_ptr, target_vertex_ptr,
                                     match_state)) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kOut>(query_vertex_ptr, target_vertex_ptr,
                                      match_state)) {
    return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class EdgeCompare>
bool IsJoinable(QueryVertexPtr query_vertex_ptr,
                TargetVertexPtr target_vertex_ptr,
                const MatchStateMap &match_state,
                const TargetVertexSet &target_matched, EdgeCompare edge_comp) {
  if (match_semantics == MatchSemantics::kIsomorphism &&
      target_matched.count(target_vertex_ptr) > 0) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kIn>(query_vertex_ptr, target_vertex_ptr,
                                     match_state, edge_comp)) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kOut>(query_vertex_ptr, target_vertex_ptr,
                                      match_state, edge_comp)) {
    return false;
  }
  return true;
}

template <class QueryVertexPtr, class TargetVertexPtr, class MatchStateMap,
          class TargetVertexSet>
void UpdateState(QueryVertexPtr query_vertex_ptr,
                 TargetVertexPtr target_vertex_ptr, MatchStateMap &match_state,
                 TargetVertexSet &target_matched) {
  match_state.emplace(query_vertex_ptr, target_vertex_ptr);
  target_matched.emplace(target_vertex_ptr);
}

template <class QueryVertexPtr, class TargetVertexPtr, class MatchStateMap,
          class TargetVertexSet>
void RestoreState(QueryVertexPtr query_vertex_ptr,
                  TargetVertexPtr target_vertex_ptr, MatchStateMap &match_state,
                  TargetVertexSet &target_matched) {
  match_state.erase(query_vertex_ptr);
  target_matched.erase(target_vertex_ptr);
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchCallback>
bool _VF2(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    std::map<QueryVertexPtr, TargetVertexPtr> &match_state,
    std::set<TargetVertexPtr> &target_matched, size_t &result_count,
    MatchCallback user_callback) {
  if (match_state.size() == candidate_set.size()) {
    result_count++;
    return user_callback(match_state);
  }

  QueryVertexPtr next_query_vertex_ptr =
      DetermineMatchOrder(candidate_set, match_state);

  for (const TargetVertexPtr &next_target_vertex_ptr :
       candidate_set.find(next_query_vertex_ptr)->second) {
    if (IsJoinable<match_semantics>(next_query_vertex_ptr,
                                    next_target_vertex_ptr, match_state,
                                    target_matched)) {
      UpdateState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
                  target_matched);

      if (!_VF2<match_semantics>(candidate_set, match_state, target_matched,
                                 result_count, user_callback)) {
        return false;
      }

      RestoreState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
                   target_matched);
    }
  }

  return true;
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class EdgeCompare, class MatchCallback>
bool _VF2(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    std::map<QueryVertexPtr, TargetVertexPtr> &match_state,
    std::set<TargetVertexPtr> &target_matched, EdgeCompare edge_comp,
    size_t &result_count, MatchCallback user_callback) {
  if (match_state.size() == candidate_set.size()) {
    result_count++;
    return user_callback(match_state);
  }

  QueryVertexPtr next_query_vertex_ptr =
      DetermineMatchOrder(candidate_set, match_state);

  for (const TargetVertexPtr &next_target_vertex_ptr :
       candidate_set.find(next_query_vertex_ptr)->second) {
    if (IsJoinable<match_semantics>(next_query_vertex_ptr,
                                    next_target_vertex_ptr, match_state,
                                    target_matched, edge_comp)) {
      UpdateState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
                  target_matched);

      if (!_VF2<match_semantics>(candidate_set, match_state, target_matched,
                                 edge_comp, result_count, user_callback)) {
        return false;
      }

      RestoreState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
                   target_matched);
    }
  }

  return true;
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class MatchStack>
bool InitMatch(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const MatchStateMap &match_state, const TargetVertexSet &target_matched,
    MatchStack &match_stack) {
  auto query_vertex_ptr = DetermineMatchOrder(candidate_set, match_state);
  auto &target_candidate = candidate_set.find(query_vertex_ptr)->second;

  auto target_vertex_iter = target_candidate.begin();
  while (target_vertex_iter != target_candidate.end()) {
    if (IsJoinable<match_semantics>(query_vertex_ptr, *target_vertex_iter,
                                    match_state, target_matched)) {
      match_stack.emplace(std::make_pair(query_vertex_ptr, target_vertex_iter));
      return true;
    }
    target_vertex_iter++;
  }
  return false;
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class EdgeCompare, class MatchStack>
bool InitMatch(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const MatchStateMap &match_state, const TargetVertexSet &target_matched,
    EdgeCompare edge_comp, MatchStack &match_stack) {
  auto query_vertex_ptr = DetermineMatchOrder(candidate_set, match_state);
  auto &target_candidate = candidate_set.find(query_vertex_ptr)->second;

  auto target_vertex_iter = target_candidate.begin();
  while (target_vertex_iter != target_candidate.end()) {
    if (IsJoinable<match_semantics>(query_vertex_ptr, *target_vertex_iter,
                                    match_state, target_matched, edge_comp)) {
      match_stack.emplace(std::make_pair(query_vertex_ptr, target_vertex_iter));
      return true;
    }
    target_vertex_iter++;
  }
  return false;
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class MatchStack>
bool NextMatch(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const MatchStateMap &match_state, const TargetVertexSet &target_matched,
    MatchStack &match_stack) {
  auto query_vertex_ptr = match_stack.top().first;
  auto &target_vertex_iter = match_stack.top().second;
  auto &target_candidate = candidate_set.find(query_vertex_ptr)->second;

  target_vertex_iter++;
  while (target_vertex_iter != target_candidate.end()) {
    if (IsJoinable<match_semantics>(query_vertex_ptr, *target_vertex_iter,
                                    match_state, target_matched)) {
      return true;
    }
    target_vertex_iter++;
  }
  match_stack.pop();
  return false;
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class EdgeCompare, class MatchStack>
bool NextMatch(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const MatchStateMap &match_state, const TargetVertexSet &target_matched,
    EdgeCompare edge_comp, MatchStack &match_stack) {
  auto query_vertex_ptr = match_stack.top().first;
  auto &target_vertex_iter = match_stack.top().second;
  auto &target_candidate = candidate_set.find(query_vertex_ptr)->second;

  target_vertex_iter++;
  while (target_vertex_iter != target_candidate.end()) {
    if (IsJoinable<match_semantics>(query_vertex_ptr, *target_vertex_iter,
                                    match_state, target_matched, edge_comp)) {
      return true;
    }
    target_vertex_iter++;
  }
  match_stack.pop();
  return false;
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class MatchCallback>
int VF2_Recursive(const GraphType0<configures0...> &query_graph,
                  const GraphType1<configures1...> &target_graph,
                  MatchCallback user_callback) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph,
                                         candidate_set)) {
    return 0;
  }

  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  size_t result_count = 0;
  _VF2<match_semantics>(candidate_set, match_state, target_matched,
                        result_count, user_callback);

  return static_cast<int>(result_count);
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class VertexCompare, class EdgeCompare, class MatchCallback>
int VF2_Recursive(const GraphType0<configures0...> &query_graph,
                  const GraphType1<configures1...> &target_graph,
                  VertexCompare vertex_comp, EdgeCompare edge_comp,
                  MatchCallback user_callback) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph, vertex_comp,
                                         candidate_set)) {
    return 0;
  }

  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  size_t result_count = 0;
  _VF2<match_semantics>(candidate_set, match_state, target_matched, edge_comp,
                        result_count, user_callback);

  return static_cast<int>(result_count);
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class MatchCallback>
int VF2_NonRecursive(const GraphType0<configures0...> &query_graph,
                     const GraphType1<configures1...> &target_graph,
                     MatchCallback user_callback) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  const size_t &query_vertex_count = query_graph.CountVertex();

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph,
                                         candidate_set)) {
    return 0;
  }

  assert(candidate_set.size() == query_vertex_count);

  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  std::stack<std::pair<QueryVertexPtr,
                       typename std::vector<TargetVertexPtr>::const_iterator>>
      match_stack;

  int result_count = 0;

  do {
    bool match_flag;

    if (match_state.size() == query_vertex_count) {
      result_count++;
      if (!user_callback(match_state)) break;
      match_flag = false;
    } else if (match_stack.size() == match_state.size()) {
      match_flag = InitMatch<match_semantics>(candidate_set, match_state,
                                              target_matched, match_stack);
    } else {
      assert(match_stack.size() == match_state.size() + 1);
      match_flag = NextMatch<match_semantics>(candidate_set, match_state,
                                              target_matched, match_stack);
    }

    if (match_flag) {
      assert(!match_stack.empty());
      UpdateState(match_stack.top().first, *match_stack.top().second,
                  match_state, target_matched);
    } else {
      if (!match_stack.empty()) {
        RestoreState(match_stack.top().first, *match_stack.top().second,
                     match_state, target_matched);
      }
    }

  } while (!match_stack.empty());

  return result_count;
}

template <enum MatchSemantics match_semantics,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class VertexCompare, class EdgeCompare, class MatchCallback>
int VF2_NonRecursive(const GraphType0<configures0...> &query_graph,
                     const GraphType1<configures1...> &target_graph,
                     VertexCompare vertex_comp, EdgeCompare edge_comp,
                     MatchCallback user_callback) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  const size_t &query_vertex_count = query_graph.CountVertex();

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph, vertex_comp,
                                         candidate_set)) {
    return 0;
  }

  assert(candidate_set.size() == query_vertex_count);

  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  std::stack<std::pair<QueryVertexPtr,
                       typename std::vector<TargetVertexPtr>::const_iterator>>
      match_stack;

  int result_count = 0;

  do {
    bool match_flag;

    if (match_state.size() == query_vertex_count) {
      result_count++;
      if (!user_callback(match_state)) break;
      match_flag = false;
    } else if (match_stack.size() == match_state.size()) {
      match_flag = InitMatch<match_semantics>(
          candidate_set, match_state, target_matched, edge_comp, match_stack);
    } else {
      assert(match_stack.size() == match_state.size() + 1);
      match_flag = NextMatch<match_semantics>(
          candidate_set, match_state, target_matched, edge_comp, match_stack);
    }

    if (match_flag) {
      assert(!match_stack.empty());
      UpdateState(match_stack.top().first, *match_stack.top().second,
                  match_state, target_matched);
    } else {
      if (!match_stack.empty()) {
        RestoreState(match_stack.top().first, *match_stack.top().second,
                     match_state, target_matched);
      }
    }

  } while (!match_stack.empty());

  return result_count;
}

template <class QueryVertexPtr, class TargetVertexPtr>
bool MatchCallbackLimit(const std::map<QueryVertexPtr, TargetVertexPtr> &,
                        int *max_result) {
  if (max_result && *max_result > 0) {
    (*max_result)--;
    if (*max_result == 0) return false;
  }
  return true;
}

template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
bool MatchCallbackSaveResult1(
    const std::map<QueryVertexPtr, TargetVertexPtr> &m, int *max_result,
    ResultContainer *r) {
  if (r) {
    r->emplace_back(m);
  }

  return MatchCallbackLimit(m, max_result);
}

template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
bool MatchCallbackSaveResult2(
    const std::map<QueryVertexPtr, TargetVertexPtr> &m, int *max_result,
    ResultContainer *r) {
  if (r) {
    using ResultType = typename ResultContainer::value_type;
    ResultType tmp;
    for (const auto &p : m) {
      tmp.emplace_back(p);
    }
    r->emplace_back(std::move(tmp));
  }

  return MatchCallbackLimit(m, max_result);
}

template <class A, class B>
void CopyMap(std::map<A, B> &c, const std::map<A, B> &m) {
  c = m;
};

template <class A, class B>
void CopyMap(std::list<std::pair<A, B>> &c, const std::map<A, B> &m) {
  for (const auto &p : m) {
    c.emplace_back(p);
  }
};

template <class A, class B>
void CopyMap(std::vector<std::pair<A, B>> &c, const std::map<A, B> &m) {
  for (const auto &p : m) {
    c.emplace_back(p);
  }
};

template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
bool MatchCallbackSaveResult(const std::map<QueryVertexPtr, TargetVertexPtr> &m,
                             int *max_result, ResultContainer *r) {
  if (r) {
    using ResultType = typename ResultContainer::value_type;

    ResultType tmp;
    CopyMap(tmp, m);
    r->emplace_back(std::move(tmp));
  }

  return MatchCallbackLimit(m, max_result);
}

};  // namespace _vf2

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class MatchCallback>
int VF2(const GraphType0<configures0...> &query_graph,
        const GraphType1<configures1...> &target_graph,
        MatchCallback user_callback) {
  return _vf2::VF2_Recursive<match_semantics>(query_graph, target_graph,
                                                 user_callback);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class VertexCompare, class EdgeCompare, class MatchCallback>
int VF2(const GraphType0<configures0...> &query_graph,
        const GraphType1<configures1...> &target_graph,
        VertexCompare vertex_comp, EdgeCompare edge_comp,
        MatchCallback user_callback) {
  return _vf2::VF2_Recursive<match_semantics>(
      query_graph, target_graph, vertex_comp, edge_comp, user_callback);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1>
int VF2(const GraphType0<configures0...> &query_graph,
        const GraphType1<configures1...> &target_graph, int max_result) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  return VF2<match_semantics>(
      query_graph, target_graph,
      std::bind(_vf2::MatchCallbackLimit<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1, &max_result));
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          template <typename...> class GraphType0, typename... configures0,
          template <typename...> class GraphType1, typename... configures1,
          class VertexCompare, class EdgeCompare>
int VF2(const GraphType0<configures0...> &query_graph,
        const GraphType1<configures1...> &target_graph,
        VertexCompare vertex_comp, EdgeCompare edge_comp, int max_result) {
  using QueryGraph = GraphType0<configures0...>;
  using TargetGraph = GraphType1<configures1...>;
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  return VF2<match_semantics>(
      query_graph, target_graph, vertex_comp, edge_comp,
      std::bind(_vf2::MatchCallbackLimit<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1, &max_result));
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class ResultContainer>
int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
        int max_result, ResultContainer &match_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  match_result.clear();

  return VF2<match_semantics>(
      query_graph, target_graph,
      std::bind(_vf2::MatchCallbackSaveResult<QueryVertexPtr, TargetVertexPtr,
                                              ResultContainer>,
                std::placeholders::_1, &max_result, &match_result));
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class VertexCompare,
          class EdgeCompare, class ResultContainer>
int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
        VertexCompare vertex_comp, EdgeCompare edge_comp, int max_result,
        ResultContainer &match_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  match_result.clear();

  return VF2<match_semantics>(
      query_graph, target_graph, vertex_comp, edge_comp,
      std::bind(_vf2::MatchCallbackSaveResult<QueryVertexPtr, TargetVertexPtr,
                                              ResultContainer>,
                std::placeholders::_1, &max_result, &match_result));
}

}  // namespace GUNDAM

#endif