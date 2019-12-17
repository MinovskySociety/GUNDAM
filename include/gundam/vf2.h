#ifndef _VF2_H
#define _VF2_H

#include <cstdint>
#include <functional>
#include <list>
#include <map>
#include <set>
#include <stack>
#include <type_traits>
#include <vector>

#include "gundam/graph.h"

namespace GUNDAM {

// enum   match tyep bool is_iso,1/// iso or homo
enum MatchSemantics { kIsomorphism, kHomomorphism };

namespace _vf2 {

enum EdgeState { kIn, kOut };

template <typename Ptr1, typename Ptr2>
class LabelEqual {
 public:
  constexpr bool operator()(const Ptr1 &a, const Ptr2 &b) const {
    return a->label() == b->label();
  }
};

template <class GraphType, class Fn, class VertexPtr1>
inline bool ForEachVertexIf(
    const GraphType &graph, Fn f,
    LabelEqual<VertexPtr1,
               typename GraphType::VertexConstPtr> /* vertex_comp */,
    const VertexPtr1 &vertex_a_ptr) {
  for (auto vertex_iter = graph.VertexCBegin(vertex_a_ptr->label());
       !vertex_iter.IsDone(); vertex_iter++) {
    typename GraphType::VertexConstPtr vertex_ptr = vertex_iter;
    if (!f(vertex_ptr)) return false;
  }
  return true;
}

template <class GraphType, class Fn, class VertexCompare, class VertexPtr1>
inline bool ForEachVertexIf(const GraphType &graph, Fn f,
                            VertexCompare vertex_comp,
                            const VertexPtr1 &vertex_a_ptr) {
  for (auto vertex_iter = graph.VertexCBegin(); !vertex_iter.IsDone();
       vertex_iter++) {
    typename GraphType::VertexConstPtr vertex_ptr = vertex_iter;
    if (vertex_comp(vertex_a_ptr, vertex_ptr)) {
      if (!f(vertex_ptr)) return false;
    }
  }
  return true;
}

 template <enum EdgeState edge_state, class VertexPtr, class Fn, class
 EdgePtr1> inline bool ForEachEdgeIf(
    const VertexPtr &vertex_ptr, Fn f,
    LabelEqual<EdgePtr1,
               typename VertexPtr::GraphType::EdgeConstPtr> /* edge_comp */,
    const EdgePtr1 &edge_a_ptr) {
  for (auto edge_iter = (edge_state == EdgeState::kIn)
                            ? vertex_ptr->InEdgeCBegin(edge_a_ptr->label())
                            : vertex_ptr->OutEdgeCBegin(edge_a_ptr->label());
       !edge_iter.IsDone(); edge_iter++) {
      typename VertexPtr::GraphType::EdgeConstPtr edge_ptr = edge_iter;
    if (!f(edge_ptr)) return false;
  }
  return true;
}

template <enum EdgeState edge_state, class VertexPtr, class Fn,
          class EdgeCompare, class EdgePtr1>
inline bool ForEachEdgeIf(const VertexPtr &vertex_ptr, Fn f,
                          EdgeCompare edge_comp, const EdgePtr1 &edge_a_ptr) {
  for (auto edge_iter = (edge_state == EdgeState::kIn)
                            ? vertex_ptr->InEdgeCBegin()
                            : vertex_ptr->OutEdgeCBegin();
       !edge_iter.IsDone(); edge_iter++) {
    typename VertexPtr::GraphType::EdgeConstPtr edge_ptr = edge_iter;
    if (edge_comp(edge_a_ptr, edge_ptr)) {
      if (!f(edge_ptr)) return false;
    }
  }
  return true;
}

// Init Candidate Set
template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class VertexCompare>
inline bool InitCandidateSet(
    const QueryGraph &query_graph, const TargetGraph &target_graph,
    VertexCompare vertex_comp,
    std::map<typename QueryGraph::VertexConstPtr,
             std::vector<typename TargetGraph::VertexConstPtr>>
        &candidate_set) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  for (auto query_vertex_iter = query_graph.VertexCBegin();
       !query_vertex_iter.IsDone(); query_vertex_iter++) {
    QueryVertexPtr query_vertex_ptr = query_vertex_iter;

    auto query_in_count = query_vertex_ptr->CountInEdge();
    auto query_out_count = query_vertex_ptr->CountOutEdge();

    auto &l = candidate_set[query_vertex_iter];

    ForEachVertexIf(target_graph,
                    [&query_in_count, &query_out_count,
                     &l](const TargetVertexPtr &target_vertex_ptr) -> bool {
                      auto target_in_count = target_vertex_ptr->CountInEdge();
                      if (target_in_count >= query_in_count) {
                        auto target_out_count =
                            target_vertex_ptr->CountOutEdge();
                        if (target_out_count >= query_out_count) {
                          l.emplace_back(target_vertex_ptr);
                        }
                      }
                      return true;
                    },
                    vertex_comp, query_vertex_ptr);

    if (l.empty()) return false;
  }
  return true;
}

// Check target_id is in C(query_vertex_ptr)
template <class QueryVertexPtr, class TargetVertexPtr, class TargetVertexID>
inline bool CheckIsInCandidateSet(
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

//
template <class QueryVertexPtr, class TargetVertexPtr>
inline QueryVertexPtr DetermineMatchOrder(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const std::map<QueryVertexPtr, TargetVertexPtr> &match_state) {
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

// template <enum EdgeState edge_state, typename QueryVertexPtr,
//          typename TargetVertexPtr>
// inline bool JoinableCheck(
//    QueryVertexPtr query_vertex_ptr, TargetVertexPtr target_vertex_ptr,
//    const std::map<QueryVertexPtr, TargetVertexPtr> &match_state) {
//
//  using QueryGraph = typename QueryVertexPtr::GraphType;
//  using TargetGraph = typename TargetVertexPtr::GraphType;
//  using QueryEdgePtr = typename QueryGraph::EdgeConstPtr;
//  using TargetEdgePtr = typename TargetGraph::EdgeConstPtr;
//
//  for (auto query_edge_iter =
//           ((edge_state == EdgeState::kIn) ? query_vertex_ptr->InEdgeCBegin()
//                                           :
//                                           query_vertex_ptr->OutEdgeCBegin());
//       !query_edge_iter.IsDone(); query_edge_iter++) {
//    QueryVertexPtr query_opp_vertex_ptr =
//        (edge_state == EdgeState::kIn) ? query_edge_iter->const_src_ptr()
//                                       : query_edge_iter->const_dst_ptr();
//
//    // if (match_state.count(query_opp_vertex_ptr) > 0) continue;
//    // TargetVertexPtr query_opp_match_vertex_ptr =
//    //    match_state[query_opp_vertex_ptr];
//
//    auto match_iter = match_state.find(query_opp_vertex_ptr);
//    if (match_iter == match_state.end()) continue;
//    TargetVertexPtr query_opp_match_vertex_ptr = match_iter->second;
//
//    bool find_target_flag = false;
//    ForEachEdgeIf<edge_state>(
//        target_vertex_ptr,
//        [&query_opp_match_vertex_ptr,
//         &find_target_flag](const TargetEdgePtr &edge_ptr) -> bool {
//          auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
//                                           ? edge_ptr->const_src_ptr()
//                                           : edge_ptr->const_dst_ptr();
//          if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id())
//          {
//            find_target_flag = true;
//            return false;
//          }
//          return true;
//        },
//        LabelEqual<QueryEdgePtr, TargetEdgePtr>(), query_edge_iter);
//
//    //for (auto target_edge_iter =
//    //         ((edge_state == EdgeState::kIn)
//    //              ?
//    target_vertex_ptr->InEdgeCBegin(query_edge_iter->label())
//    //              :
//    target_vertex_ptr->OutEdgeCBegin(query_edge_iter->label()));
//    //     !target_edge_iter.IsDone(); target_edge_iter++) {
//    //  auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
//    //                                   ? target_edge_iter->const_src_ptr()
//    //                                   : target_edge_iter->const_dst_ptr();
//
//    //  if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
//    //    find_target_flag = true;
//    //    break;
//    //  }
//    //}
//    if (!find_target_flag) return false;
//  }
//  return true;
//}

template <enum EdgeState edge_state, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class EdgeCompare>
inline bool JoinableCheck(const QueryVertexPtr &query_vertex_ptr,
                          const TargetVertexPtr &target_vertex_ptr,
                          const MatchStateMap &match_state,
                          EdgeCompare edge_comp) {
  using QueryGraph = typename QueryVertexPtr::GraphType;
  using TargetGraph = typename TargetVertexPtr::GraphType;
  using QueryEdgePtr = typename QueryGraph::EdgeConstPtr;
  using TargetEdgePtr = typename TargetGraph::EdgeConstPtr;

  for (auto query_edge_iter =
           ((edge_state == EdgeState::kIn) ? query_vertex_ptr->InEdgeCBegin()
                                           : query_vertex_ptr->OutEdgeCBegin());
       !query_edge_iter.IsDone(); query_edge_iter++) {
    QueryEdgePtr query_edge_ptr = query_edge_iter;
    QueryVertexPtr query_opp_vertex_ptr = (edge_state == EdgeState::kIn)
                                              ? query_edge_ptr->const_src_ptr()
                                              : query_edge_ptr->const_dst_ptr();

    // if (match_state.count(query_opp_vertex_ptr) == 0) continue;
    // TargetVertexPtr query_opp_match_vertex_ptr =
    //    match_state[query_opp_vertex_ptr];

    auto match_iter = match_state.find(query_opp_vertex_ptr);
    if (match_iter == match_state.end()) continue;

    TargetVertexPtr query_opp_match_vertex_ptr = match_iter->second;

    bool find_target_flag = false;
    ForEachEdgeIf<edge_state>(
        target_vertex_ptr,
        [&query_opp_match_vertex_ptr,
         &find_target_flag](const TargetEdgePtr &target_edge_ptr) -> bool {
          auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
                                           ? target_edge_ptr->const_src_ptr()
                                           : target_edge_ptr->const_dst_ptr();
          if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
            find_target_flag = true;
            return false;
          }
          return true;
        },
        edge_comp, query_edge_ptr);

    // for (auto target_edge_iter = ((edge_state == EdgeState::kIn)
    //                                  ? target_vertex_ptr->InEdgeCBegin()
    //                                  : target_vertex_ptr->OutEdgeCBegin());
    //     !target_edge_iter.IsDone(); target_edge_iter++) {
    //  auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
    //                                   ? target_edge_iter->const_src_ptr()
    //                                   : target_edge_iter->const_dst_ptr();
    //  if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
    //    // auto query_edge_ptr =
    //    //     (edge_state == EdgeState::kOut)
    //    //         ? query_vertex_ptr->FindConstOutEdge(query_edge_iter->id())
    //    //         :
    //    // query_opp_vertex_ptr->FindConstOutEdge(query_edge_iter->id());

    //    // auto target_edge_ptr =
    //    //     (edge_state == EdgeState::kOut)
    //    //         ?
    //    target_vertex_ptr->FindConstOutEdge(target_edge_iter->id())
    //    //         : query_opp_match_vertex_ptr->FindConstOutEdge(
    //    //               target_edge_iter->id());

    //    if (edge_comp(query_edge_iter, target_edge_iter)) {
    //      find_target_flag = true;
    //      break;
    //    }
    //  }
    //}
    if (!find_target_flag) return false;
  }
  return true;
}

// template <class Key1, class Key2, class Value>
// using Map2 = typename std::map<Key1, std::map<Key2, Value>>;
//
// template <enum EdgeState edge_state, class QueryVertexPtr,
//          class TargetVertexPtr, class MatchStateMap, class EdgeCompare>
// inline bool JoinableCheck2(const QueryVertexPtr &query_vertex_ptr,
//                          const TargetVertexPtr &target_vertex_ptr,
//                          const MatchStateMap &match_state,
//                          EdgeCompare edge_comp) {
//  using QueryGraph = typename QueryVertexPtr::GraphType;
//  using TargetGraph = typename TargetVertexPtr::GraphType;
//  using QueryEdgePtr = typename QueryGraph::EdgeConstPtr;
//  using TargetEdgePtr = typename TargetGraph::EdgeConstPtr;
//  using QueryEdgeLabelType = typename QueryGraph::EdgeType::LabelType;
//  using TargetEdgeLabelType = typename QueryGraph::TargetType::LabelType;
//
//  Map2<QueryEdgeLabelType, QueryVertexPtr, size_t> query_edge_count;
//
//  for (auto query_edge_iter =
//           ((edge_state == EdgeState::kIn) ? query_vertex_ptr->InEdgeCBegin()
//                                           :
//                                           query_vertex_ptr->OutEdgeCBegin());
//       !query_edge_iter.IsDone(); query_edge_iter++) {
//    QueryEdgePtr query_edge_ptr = query_edge_iter;
//    QueryVertexPtr query_opp_vertex_ptr = (edge_state == EdgeState::kIn)
//                                              ?
//                                              query_edge_ptr->const_src_ptr()
//                                              :
//                                              query_edge_ptr->const_dst_ptr();
//
//
//    auto match_iter = match_state.find(query_opp_vertex_ptr);
//    if (match_iter == match_state.end()) continue;
//
//    TargetVertexPtr query_opp_match_vertex_ptr = match_iter->second;
//
//    bool find_target_flag = false;
//    ForEachEdgeIf<edge_state>(
//        target_vertex_ptr,
//        [&query_opp_match_vertex_ptr,
//         &find_target_flag](const TargetEdgePtr &target_edge_ptr) -> bool {
//          auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
//                                           ? target_edge_ptr->const_src_ptr()
//                                           : target_edge_ptr->const_dst_ptr();
//          if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id())
//          {
//            find_target_flag = true;
//            return false;
//          }
//          return true;
//        },
//        edge_comp, query_edge_ptr);
//
//    // for (auto target_edge_iter = ((edge_state == EdgeState::kIn)
//    //                                  ? target_vertex_ptr->InEdgeCBegin()
//    //                                  : target_vertex_ptr->OutEdgeCBegin());
//    //     !target_edge_iter.IsDone(); target_edge_iter++) {
//    //  auto target_opp_vertex_ptr = (edge_state == EdgeState::kIn)
//    //                                   ? target_edge_iter->const_src_ptr()
//    //                                   : target_edge_iter->const_dst_ptr();
//    //  if (target_opp_vertex_ptr->id() == query_opp_match_vertex_ptr->id()) {
//    //    // auto query_edge_ptr =
//    //    //     (edge_state == EdgeState::kOut)
//    //    //         ?
//    query_vertex_ptr->FindConstOutEdge(query_edge_iter->id())
//    //    //         :
//    //    // query_opp_vertex_ptr->FindConstOutEdge(query_edge_iter->id());
//
//    //    // auto target_edge_ptr =
//    //    //     (edge_state == EdgeState::kOut)
//    //    //         ?
//    //    target_vertex_ptr->FindConstOutEdge(target_edge_iter->id())
//    //    //         : query_opp_match_vertex_ptr->FindConstOutEdge(
//    //    //               target_edge_iter->id());
//
//    //    if (edge_comp(query_edge_iter, target_edge_iter)) {
//    //      find_target_flag = true;
//    //      break;
//    //    }
//    //  }
//    //}
//    if (!find_target_flag) return false;
//  }
//  return true;
//}

// template <enum MatchSemantics match_semantics, class QueryVertexPtr,
//          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet>
// inline bool IsJoinable(QueryVertexPtr query_vertex_ptr,
//                       TargetVertexPtr target_vertex_ptr,
//                       const MatchStateMap &match_state,
//                       const TargetVertexSet &target_matched) {
//  if (match_semantics == MatchSemantics::kIsomorphism &&
//      target_matched.count(target_vertex_ptr) > 0) {
//    return false;
//  }
//  if (!JoinableCheck<EdgeState::kIn>(query_vertex_ptr, target_vertex_ptr,
//                                     match_state)) {
//    return false;
//  }
//  if (!JoinableCheck<EdgeState::kOut>(query_vertex_ptr, target_vertex_ptr,
//                                      match_state)) {
//    return false;
//  }
//  return true;
//}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class EdgeCompare>
inline bool IsJoinable(QueryVertexPtr query_vertex_ptr,
                       TargetVertexPtr target_vertex_ptr,
                       const MatchStateMap &match_state,
                       const TargetVertexSet &target_matched,
                       EdgeCompare edge_comp) {
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
inline void UpdateState(QueryVertexPtr query_vertex_ptr,
                        TargetVertexPtr target_vertex_ptr,
                        MatchStateMap &match_state,
                        TargetVertexSet &target_matched) {
  match_state.emplace(query_vertex_ptr, target_vertex_ptr);
  target_matched.emplace(target_vertex_ptr);
}

template <class QueryVertexPtr, class TargetVertexPtr, class MatchStateMap,
          class TargetVertexSet>
inline void RestoreState(QueryVertexPtr query_vertex_ptr,
                         TargetVertexPtr target_vertex_ptr,
                         MatchStateMap &match_state,
                         TargetVertexSet &target_matched) {
  match_state.erase(query_vertex_ptr);
  target_matched.erase(target_vertex_ptr);
}

// template <enum MatchSemantics match_semantics, class QueryVertexPtr,
//          class TargetVertexPtr, class MatchCallback>
// bool _VF2(
//    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>>
//    &candidate_set, std::map<QueryVertexPtr, TargetVertexPtr> &match_state,
//    std::set<TargetVertexPtr> &target_matched, size_t &result_count,
//    MatchCallback user_callback) {
//  if (match_state.size() == candidate_set.size()) {
//    result_count++;
//    return user_callback(match_state);
//  }
//
//  QueryVertexPtr next_query_vertex_ptr =
//      DetermineMatchOrder(candidate_set, match_state);
//
//  for (const TargetVertexPtr &next_target_vertex_ptr :
//       candidate_set.find(next_query_vertex_ptr)->second) {
//    if (IsJoinable<match_semantics>(next_query_vertex_ptr,
//                                    next_target_vertex_ptr, match_state,
//                                    target_matched)) {
//      UpdateState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
//                  target_matched);
//
//      if (!_VF2<match_semantics>(candidate_set, match_state, target_matched,
//                                 result_count, user_callback)) {
//        return false;
//      }
//
//      RestoreState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
//                   target_matched);
//    }
//  }
//
//  return true;
//}

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

// template <enum MatchSemantics match_semantics, class QueryVertexPtr,
//          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
//          class MatchStack>
// inline bool InitMatch(
//    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>>
//    &candidate_set, const MatchStateMap &match_state, const TargetVertexSet
//    &target_matched, MatchStack &match_stack) {
//  auto query_vertex_ptr = DetermineMatchOrder(candidate_set, match_state);
//  auto &target_candidate = candidate_set.find(query_vertex_ptr)->second;
//
//  auto target_vertex_iter = target_candidate.begin();
//  while (target_vertex_iter != target_candidate.end()) {
//    if (IsJoinable<match_semantics>(query_vertex_ptr, *target_vertex_iter,
//                                    match_state, target_matched)) {
//      match_stack.emplace(std::make_pair(query_vertex_ptr,
//      target_vertex_iter)); return true;
//    }
//    target_vertex_iter++;
//  }
//  return false;
//}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class EdgeCompare, class MatchStack>
inline bool InitMatch(
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

// template <enum MatchSemantics match_semantics, class QueryVertexPtr,
//          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
//          class MatchStack>
// inline bool NextMatch(
//    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>>
//    &candidate_set, const MatchStateMap &match_state, const TargetVertexSet
//    &target_matched, MatchStack &match_stack) {
//  auto query_vertex_ptr = match_stack.top().first;
//  auto &target_vertex_iter = match_stack.top().second;
//  auto &target_candidate = candidate_set.find(query_vertex_ptr)->second;
//
//  target_vertex_iter++;
//  while (target_vertex_iter != target_candidate.end()) {
//    if (IsJoinable<match_semantics>(query_vertex_ptr, *target_vertex_iter,
//                                    match_state, target_matched)) {
//      return true;
//    }
//    target_vertex_iter++;
//  }
//  match_stack.pop();
//  return false;
//}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class EdgeCompare, class MatchStack>
inline bool NextMatch(
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

template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class VertexCompare, class EdgeCompare,
          class MatchCallback>
inline int VF2_Recursive(const QueryGraph &query_graph,
                         const TargetGraph &target_graph,
                         VertexCompare vertex_comp, EdgeCompare edge_comp,
                         MatchCallback user_callback) {
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

// template <enum MatchSemantics match_semantics, class QueryGraph,
//          class TargetGraph, class MatchCallback>
// inline int VF2_Recursive(const QueryGraph &query_graph,
//                         const TargetGraph &target_graph,
//                         MatchCallback user_callback) {
//  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
//  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
//  using QueryEdgePtr = typename QueryGraph::EdgeConstPtr;
//  using TargetEdgePtr = typename TargetGraph::EdgeConstPtr;
//
//  return VF2_Recursive<match_semantics>(
//      query_graph, target_graph, LabelEqual<QueryVertexPtr,
//      TargetVertexPtr>(), LabelEqual<QueryEdgePtr, TargetEdgePtr>(),
//      user_callback);
//}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class EdgeCompare, class MatchCallback>
int VF2_NonRecursive(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    std::map<QueryVertexPtr, TargetVertexPtr> &match_state,
    std::set<TargetVertexPtr> &target_matched,
    std::stack<std::pair<QueryVertexPtr,
                         typename std::vector<TargetVertexPtr>::const_iterator>>
        &match_stack,
    EdgeCompare edge_comp, MatchCallback user_callback) {
  const size_t &query_vertex_count = candidate_set.size();

  int result_count = 0;

  bool match_flag = true;

  do {
    if (match_state.size() == query_vertex_count) {
      result_count++;
      if (!user_callback(match_state)) break;
      match_flag = false;
    } else if (match_flag) {
      match_flag = InitMatch<match_semantics>(
          candidate_set, match_state, target_matched, edge_comp, match_stack);
    } else {
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

template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class VertexCompare, class EdgeCompare,
          class MatchCallback>
inline int VF2_NonRecursive(const QueryGraph &query_graph,
                            const TargetGraph &target_graph,
                            VertexCompare vertex_comp, EdgeCompare edge_comp,
                            MatchCallback user_callback) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph, vertex_comp,
                                         candidate_set)) {
    return 0;
  }

  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  std::stack<std::pair<QueryVertexPtr,
                       typename std::vector<TargetVertexPtr>::const_iterator>>
      match_stack;

  return VF2_NonRecursive<match_semantics>(candidate_set, match_state,
                                           target_matched, match_stack,
                                           edge_comp, user_callback);
}

template <enum MatchSemantics match_semantics, class QueryVertexPtr,
          class TargetVertexPtr, class MatchSequence,
          class = typename std::enable_if<std::is_convertible<
              std::pair<QueryVertexPtr, TargetVertexPtr>,
              typename MatchSequence::value_type>::value>::type,
          class EdgeCompare, class MatchCallback>
int VF2_NonRecursive(
    const std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set,
    const MatchSequence &match_part, size_t anchor_count, EdgeCompare edge_comp,
    MatchCallback user_callback) {
  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  std::stack<std::pair<QueryVertexPtr,
                       typename std::vector<TargetVertexPtr>::const_iterator>>
      match_stack;

  for (auto match_iter = match_part.begin(); match_iter != match_part.end();
       match_iter++) {
    const QueryVertexPtr &query_vertex_ptr = match_iter->first;
    const TargetVertexPtr &target_vertex_ptr = match_iter->second;

    match_state.emplace(query_vertex_ptr, target_vertex_ptr);
    target_matched.emplace(target_vertex_ptr);

    if (anchor_count == 0) {
      auto target_vertex_iter =
          candidate_set.find(query_vertex_ptr)->second.cbegin();
      while (*target_vertex_iter != target_vertex_ptr) {
        target_vertex_iter++;
      }
      match_stack.emplace(query_vertex_ptr, target_vertex_iter);
    } else {
      anchor_count--;
    }
  }

  return VF2_NonRecursive<match_semantics>(candidate_set, match_state,
                                           target_matched, match_stack,
                                           edge_comp, user_callback);
}

template <
    enum MatchSemantics match_semantics, class QueryGraph, class TargetGraph,
    class MatchSequence,
    class = typename std::enable_if<
        std::is_convertible<std::pair<typename QueryGraph::VertexConstPtr,
                                      typename TargetGraph::VertexConstPtr>,
                            typename MatchSequence::value_type>::value>::type,
    class VertexCompare,
    class =
        typename std::enable_if<std::is_function<VertexCompare>::value>::type,
    class EdgeCompare,
    class = typename std::enable_if<std::is_function<EdgeCompare>::value>::type,
    class MatchCallback,
    class =
        typename std::enable_if<std::is_function<MatchCallback>::value>::type>

inline int VF2_NonRecursive(const QueryGraph &query_graph,
                            const TargetGraph &target_graph,
                            const MatchSequence &match_part,
                            size_t anchor_count, VertexCompare vertex_comp,
                            EdgeCompare edge_comp,
                            MatchCallback user_callback) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph, vertex_comp,
                                         candidate_set)) {
    return 0;
  }

  return VF2_NonRecursive<match_semantics>(
      candidate_set, match_part, anchor_count, edge_comp, user_callback);
}

//
// template <
//    enum MatchSemantics match_semantics, class QueryGraph, class TargetGraph,
//    class VertexCompare,
//    class =
//        typename std::enable_if<std::is_function<VertexCompare>::value>::type,
//    class EdgeCompare,
//    class = typename
//    std::enable_if<std::is_function<EdgeCompare>::value>::type, class
//    MatchCallback, class =
//        typename std::enable_if<std::is_function<MatchCallback>::value>::type>
// int VF2_NonRecursive(const QueryGraph &query_graph,
//                     const TargetGraph &target_graph, VertexCompare
//                     vertex_comp, EdgeCompare edge_comp, MatchCallback
//                     user_callback) {
//  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
//  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
//
//  const size_t &query_vertex_count = query_graph.CountVertex();
//
//  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
//  if (!InitCandidateSet<match_semantics>(query_graph, target_graph,
//  vertex_comp,
//                                         candidate_set)) {
//    return 0;
//  }
//
//  assert(candidate_set.size() == query_vertex_count);
//
//  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
//  std::set<TargetVertexPtr> target_matched;
//  std::stack<std::pair<QueryVertexPtr,
//                       typename std::vector<TargetVertexPtr>::const_iterator>>
//      match_stack;
//
//  int result_count = 0;
//
//  do {
//    bool match_flag;
//
//    if (match_state.size() == query_vertex_count) {
//      result_count++;
//      if (!user_callback(match_state)) break;
//      match_flag = false;
//    } else if (match_stack.size() == match_state.size()) {
//      match_flag = InitMatch<match_semantics>(
//          candidate_set, match_state, target_matched, edge_comp, match_stack);
//    } else {
//      assert(match_stack.size() == match_state.size() + 1);
//      match_flag = NextMatch<match_semantics>(
//          candidate_set, match_state, target_matched, edge_comp, match_stack);
//    }
//
//    if (match_flag) {
//      assert(!match_stack.empty());
//      UpdateState(match_stack.top().first, *match_stack.top().second,
//                  match_state, target_matched);
//    } else {
//      if (!match_stack.empty()) {
//        RestoreState(match_stack.top().first, *match_stack.top().second,
//                     match_state, target_matched);
//      }
//    }
//
//  } while (!match_stack.empty());
//
//  return result_count;
//}

template <class QueryVertexPtr, class TargetVertexPtr>
inline bool MatchCallbackLimit(
    const std::map<QueryVertexPtr, TargetVertexPtr> &, int *max_result) {
  if (max_result && *max_result > 0) {
    (*max_result)--;
    if (*max_result == 0) return false;
  }
  return true;
}

template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
inline bool MatchCallbackSaveResult1(
    const std::map<QueryVertexPtr, TargetVertexPtr> &m, int *max_result,
    ResultContainer *r) {
  if (r) {
    r->emplace_back(m);
  }

  return MatchCallbackLimit(m, max_result);
}

template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
inline bool MatchCallbackSaveResult2(
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
inline void CopyMap(std::map<A, B> &c, const std::map<A, B> &m) {
  c = m;
};

template <class A, class B>
inline void CopyMap(std::list<std::pair<A, B>> &c, const std::map<A, B> &m) {
  for (const auto &p : m) {
    c.emplace_back(p);
  }
};

template <class A, class B>
inline void CopyMap(std::vector<std::pair<A, B>> &c, const std::map<A, B> &m) {
  for (const auto &p : m) {
    c.emplace_back(p);
  }
};

template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
inline bool MatchCallbackSaveResult(
    const std::map<QueryVertexPtr, TargetVertexPtr> &m, int *max_result,
    ResultContainer *r) {
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
          class QueryGraph, class TargetGraph, class VertexCompare,
          class EdgeCompare, class MatchCallback>
inline int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
               VertexCompare vertex_comp, EdgeCompare edge_comp,
               MatchCallback user_callback) {
  return _vf2::VF2_NonRecursive<match_semantics>(
      query_graph, target_graph, vertex_comp, edge_comp, user_callback);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class MatchCallback>
inline int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
               MatchCallback user_callback) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  using QueryEdgePtr = typename QueryGraph::EdgeConstPtr;
  using TargetEdgePtr = typename TargetGraph::EdgeConstPtr;

  return VF2<match_semantics>(
      query_graph, target_graph,
      _vf2::LabelEqual<QueryVertexPtr, TargetVertexPtr>(),
      _vf2::LabelEqual<QueryEdgePtr, TargetEdgePtr>(), user_callback);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class VertexCompare,
          class EdgeCompare>
inline int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
               VertexCompare vertex_comp, EdgeCompare edge_comp,
               int max_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  return VF2<match_semantics>(
      query_graph, target_graph, vertex_comp, edge_comp,
      std::bind(_vf2::MatchCallbackLimit<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1, &max_result));
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph>
inline int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
               int max_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  return VF2<match_semantics>(
      query_graph, target_graph,
      std::bind(_vf2::MatchCallbackLimit<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1, &max_result));
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class VertexCompare,
          class EdgeCompare, class ResultContainer>
inline int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
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

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class ResultContainer>
inline int VF2(const QueryGraph &query_graph, const TargetGraph &target_graph,
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

}  // namespace GUNDAM

#endif