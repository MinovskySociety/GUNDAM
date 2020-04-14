#ifndef _VF2LABELEQUAL_H
#define _VF2LABELEQUAL_H
#include <cstdint>
#include <ctime>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <queue>
#include <set>
#include <stack>
#include <type_traits>
#include <vector>
//#include "graph.h"
#include "vf2.h"
namespace GUNDAM {
namespace _vf2_label_equal {
enum EdgeState { kIn, kOut };

template <class GraphType, class Fn, class VertexPtr1>
inline bool ForEachVertexIf(const GraphType &graph, Fn f,
                            const VertexPtr1 &vertex_a_ptr) {
  for (auto vertex_iter = graph.VertexCBegin(vertex_a_ptr->label());
       !vertex_iter.IsDone(); vertex_iter++) {
    typename GraphType::VertexConstPtr vertex_ptr = vertex_iter;
    if (!f(vertex_ptr)) return false;
  }
  return true;
}
template <enum EdgeState edge_state, class VertexPtr, class Fn, class EdgePtr1>
inline bool ForEachEdgeIf(const VertexPtr &vertex_ptr, Fn f,
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
// Init Candidate Set
template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class CandidateSetContainer>
inline bool InitCandidateSet(const QueryGraph &query_graph,
                             const TargetGraph &target_graph,
                             CandidateSetContainer &candidate_set) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  time_t begin = clock();

  for (auto query_vertex_iter = query_graph.VertexCBegin();
       !query_vertex_iter.IsDone(); query_vertex_iter++) {
    QueryVertexPtr query_vertex_ptr = query_vertex_iter;
    auto &l = candidate_set[query_vertex_iter];
    for (auto target_vertex_iter =
             target_graph.VertexCBegin(query_vertex_ptr->label());
         !target_vertex_iter.IsDone(); target_vertex_iter++) {
      TargetVertexPtr target_vertex_ptr = target_vertex_iter;
      int flag = 0;
      if (query_vertex_ptr->CountOutVertex() >
          target_vertex_ptr->CountOutVertex())
        continue;
      if (query_vertex_ptr->CountInVertex() >
          target_vertex_ptr->CountInVertex())
        continue;
      for (auto edge_label_it = query_vertex_ptr->OutEdgeLabelBegin();
           !edge_label_it.IsDone(); edge_label_it++) {
        auto query_out_count = query_vertex_ptr->CountOutEdge(
            *edge_label_it /*edge_label_it->label()*/);
        auto target_out_count = target_vertex_ptr->CountOutEdge(
            *edge_label_it /*edge_label_it->label()*/);
        if (query_out_count > target_out_count) {
          flag = 1;
          break;
        }
        query_out_count = query_vertex_ptr->CountOutVertex(
            *edge_label_it /*edge_label_it->label()*/);
        target_out_count = target_vertex_ptr->CountOutVertex(
            *edge_label_it /*edge_label_it->label()*/);
        if (query_out_count > target_out_count) {
          flag = 1;
          break;
        }
      }
      for (auto edge_label_it = query_vertex_ptr->InEdgeLabelBegin();
           !edge_label_it.IsDone(); edge_label_it++) {
        auto query_in_count = query_vertex_ptr->CountInEdge(
            *edge_label_it /*edge_label_it->label()*/);
        auto target_in_count = target_vertex_ptr->CountInEdge(
            *edge_label_it /*edge_label_it->label()*/);
        if (query_in_count > target_in_count) {
          flag = 1;
          break;
        }
        query_in_count = query_vertex_ptr->CountInVertex(
            *edge_label_it /*edge_label_it->label()*/);
        target_in_count = target_vertex_ptr->CountInVertex(
            *edge_label_it /*edge_label_it->label()*/);
        if (query_in_count > target_in_count) {
          flag = 1;
          break;
        }
      }
      if (!flag) {
        l.emplace_back(target_vertex_ptr);
      }
    }
    if (l.empty()) return false;

    /*
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
                    query_vertex_ptr);

    if (l.empty()) return false;
    */
  }
  time_t end = clock();
  // std::cout << "init time = " << (1.0 * end - begin) / CLOCKS_PER_SEC
  //          << std::endl;
  for (auto &it : candidate_set) {
    sort(it.second.begin(), it.second.end());
  }
  return true;
}
// Check target_id is in C(query_vertex_ptr)
template <class QueryVertexPtr, class TargetVertexID,
          class CandidateSetContainer>
inline bool CheckIsInCandidateSet(QueryVertexPtr query_vertex_ptr,
                                  const CandidateSetContainer &candidate_set,
                                  const TargetVertexID &target_id) {
  for (const auto &v : candidate_set.find(query_vertex_ptr)->second) {
    if (v->id() == target_id) {
      return true;
    }
  }
  return false;
}

//
template <class QueryVertexPtr, class TargetVertexPtr,
          class CandidateSetContainer>
inline QueryVertexPtr DetermineMatchOrder(
    const CandidateSetContainer &candidate_set,
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
  assert(min >= 0 && !res.IsNull());
  return res;
}

template <enum EdgeState edge_state, class QueryGraph, class TargetGraph,
          class QueryVertexPtr, class TargetVertexPtr, class MatchStateMap>
inline bool JoinableCheck(const QueryVertexPtr &query_vertex_ptr,
                          const TargetVertexPtr &target_vertex_ptr,
                          const MatchStateMap &match_state) {
  using QueryEdgePtr = typename QueryGraph::EdgeConstPtr;
  using TargetEdgePtr = typename TargetGraph::EdgeConstPtr;
  using QueryEdgeMapPair = std::pair<typename QueryGraph::EdgeType::LabelType,
                                     typename QueryGraph::VertexConstPtr>;
  using TargetEdgeMapPair = std::pair<typename TargetGraph::EdgeType::LabelType,
                                      typename TargetGraph::VertexConstPtr>;
  std::set<typename TargetGraph::EdgeType::IDType> used_edge;
  for (auto query_edge_iter =
           ((edge_state == EdgeState::kIn) ? query_vertex_ptr->InEdgeCBegin()
                                           : query_vertex_ptr->OutEdgeCBegin());
       !query_edge_iter.IsDone(); query_edge_iter++) {
    QueryEdgePtr query_edge_ptr = query_edge_iter;
    QueryVertexPtr query_opp_vertex_ptr = (edge_state == EdgeState::kIn)
                                              ? query_edge_ptr->const_src_ptr()
                                              : query_edge_ptr->const_dst_ptr();
    auto match_iter = match_state.find(query_opp_vertex_ptr);
    if (match_iter == match_state.end()) continue;

    TargetVertexPtr query_opp_match_vertex_ptr = match_iter->second;

    bool find_target_flag = false;
    for (auto target_edge_iter =
             ((edge_state == EdgeState::kIn)
                  ? target_vertex_ptr->InEdgeCBegin(query_edge_ptr->label(),
                                                    query_opp_match_vertex_ptr)
                  : target_vertex_ptr->OutEdgeCBegin(
                        query_edge_ptr->label(), query_opp_match_vertex_ptr));
         !target_edge_iter.IsDone(); target_edge_iter++) {
      if (used_edge.count(target_edge_iter->id())) continue;
      /*
      TargetEdgePtr target_edge_ptr = target_edge_iter;
      TargetVertexPtr target_opp_vertex_ptr =
          (edge_state == EdgeState::kIn) ? target_edge_iter->const_src_ptr()
                                         : target_edge_iter->const_dst_ptr();
      if (target_opp_vertex_ptr != query_opp_match_vertex_ptr) continue;
      */
      find_target_flag = true;
      used_edge.insert(target_edge_iter->id());
      break;
    }
    if (!find_target_flag) return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class QueryVertexPtr, class TargetVertexPtr,
          class MatchStateMap, class TargetVertexSet>
inline bool IsJoinable(QueryVertexPtr query_vertex_ptr,
                       TargetVertexPtr target_vertex_ptr,
                       const MatchStateMap &match_state,
                       const TargetVertexSet &target_matched) {
  if (match_semantics == MatchSemantics::kIsomorphism &&
      target_matched.count(target_vertex_ptr) > 0) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kIn, QueryGraph, TargetGraph>(
          query_vertex_ptr, target_vertex_ptr, match_state)) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kOut, QueryGraph, TargetGraph>(
          query_vertex_ptr, target_vertex_ptr, match_state)) {
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
template <enum EdgeState edge_state, class QueryGraph, class TargetGraph,
          class QueryVertexPtr, class TargetVertexPtr, class MatchStateMap,
          class TargetVertexSet, class CandidateSetContainer>
inline void UpdateCandidateSetOneDirection(
    QueryVertexPtr query_vertex_ptr, TargetVertexPtr target_vertex_ptr,
    CandidateSetContainer &candidate_set, const MatchStateMap &match_state,
    const TargetVertexSet &target_matched) {
  std::map<typename QueryGraph::VertexType::LabelType,
           std::set<TargetVertexPtr>>
      temp_adj_vertex;
  for (auto label_it = ((edge_state == EdgeState::kIn)
                            ? query_vertex_ptr->InEdgeLabelBegin()
                            : query_vertex_ptr->OutEdgeLabelBegin());
       !label_it.IsDone(); label_it++) {
    for (auto it = ((edge_state == EdgeState::kIn)
                        ? target_vertex_ptr->InVertexCBegin(
                              *label_it /*label_it->label()*/)
                        : target_vertex_ptr->OutVertexCBegin(
                              *label_it /*label_it->label()*/));
         !it.IsDone(); it++) {
      // std::cout << "target 111" << std::endl;
      TargetVertexPtr temp_target_ptr = it;
      // if (target_matched.count(temp_target_ptr)) continue;
      temp_adj_vertex[temp_target_ptr->label()].insert(temp_target_ptr);
    }
    for (auto vertex_it = ((edge_state == EdgeState::kIn)
                               ? query_vertex_ptr->InVertexCBegin(
                                     *label_it /*label_it->label()*/)
                               : query_vertex_ptr->OutVertexCBegin(
                                     *label_it /*label_it->label()*/));
         !vertex_it.IsDone(); vertex_it++) {
      QueryVertexPtr temp_vertex_ptr = vertex_it;
      std::vector<TargetVertexPtr> res_candidate;
      if (match_state.count(temp_vertex_ptr)) continue;
      // std::cout << "label = " << vertex_it->label() << std::endl;
      // std::cout << "temp_adj_vertex:\n";
      // for (auto &it : temp_adj_vertex[vertex_it->label()]) {
      //   std::cout << it->id() << " ";
      // }
      // std::cout << std::endl;

      // std::cout << "candidate set:\n";
      // for (auto &it : candidate_set[temp_vertex_ptr]) {
      //  std::cout << it->id() << " ";
      //}
      // std::cout << std::endl;
      std::set_intersection(temp_adj_vertex[vertex_it->label()].begin(),
                            temp_adj_vertex[vertex_it->label()].end(),
                            candidate_set[temp_vertex_ptr].begin(),
                            candidate_set[temp_vertex_ptr].end(),
                            inserter(res_candidate, res_candidate.begin()));
      candidate_set[temp_vertex_ptr] = res_candidate;
    }
  }
}
template <class QueryGraph, class TargetGraph, class QueryVertexPtr,
          class TargetVertexPtr, class MatchStateMap, class TargetVertexSet,
          class CandidateSetContainer>
inline void UpdateCandidateSet(QueryVertexPtr query_vertex_ptr,
                               TargetVertexPtr target_vertex_ptr,
                               CandidateSetContainer &candidate_set,
                               const MatchStateMap &match_state,
                               const TargetVertexSet &target_matched) {
  time_t begin = clock();
  UpdateCandidateSetOneDirection<EdgeState::kIn, QueryGraph, TargetGraph>(
      query_vertex_ptr, target_vertex_ptr, candidate_set, match_state,
      target_matched);
  UpdateCandidateSetOneDirection<EdgeState::kOut, QueryGraph, TargetGraph>(
      query_vertex_ptr, target_vertex_ptr, candidate_set, match_state,
      target_matched);
  time_t end = clock();
  // std::cout << "update candidate time = "
  //           << (1.0 * end - begin) / CLOCKS_PER_SEC << std::endl;
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
template <class QueryVertexPtr, class TargetVertexPtr, class ResultContainer>
inline bool MatchCallbackSaveSupp(std::map<QueryVertexPtr, TargetVertexPtr> &m,
                                  QueryVertexPtr *query_vertex_ptr,
                                  ResultContainer *r) {
  if (r) {
    r->insert(m[*query_vertex_ptr]);
  }
  return true;
}
template <class QueryVertexPtr, class TargetVertexPtr, class SuppContainer>
inline bool PruneSuppCallback(
    const std::map<QueryVertexPtr, TargetVertexPtr> &m,
    QueryVertexPtr *query_vertex_ptr, SuppContainer *r) {
  if (r) {
    auto it = m.find(*query_vertex_ptr);
    if (it == m.end()) {
      return false;
    }
    TargetVertexPtr target_vertex_ptr = it->second;
    if (r->count(target_vertex_ptr)) {
      return true;
    }
  }
  return false;
}
template <class QueryVertexPtr, class TargetVertexPtr>
inline bool PruneCallbackEmpty(
    const std::map<QueryVertexPtr, TargetVertexPtr> &m) {
  return false;
}
int func_call = 0;
template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class CandidateSetContainer, class QueryVertexPtr,
          class TargetVertexPtr, class MatchCallback, class PruneCallback>
bool _VF2(const CandidateSetContainer &candidate_set,
          std::map<QueryVertexPtr, TargetVertexPtr> &match_state,
          std::set<TargetVertexPtr> &target_matched, size_t &result_count,
          MatchCallback user_callback, PruneCallback prune_callback) {
  // for (auto &it : match_state) {
  //  std::cout << it.first->id() << " " << it.second->id() << std::endl;
  //}
  // std::cout << std::endl;
  func_call++;
  if (prune_callback(match_state)) {
    return true;
  }
  if (match_state.size() == candidate_set.size()) {
    result_count++;
    return user_callback(match_state);
  }
  QueryVertexPtr next_query_vertex_ptr =
      DetermineMatchOrder(candidate_set, match_state);
  // std::cout << "next size = "
  //          << candidate_set.find(next_query_vertex_ptr)->second.size()
  //          << std::endl;
  for (const TargetVertexPtr &next_target_vertex_ptr :
       candidate_set.find(next_query_vertex_ptr)->second) {
    if (prune_callback(match_state)) {
      return true;
    }
    if (IsJoinable<match_semantics, QueryGraph, TargetGraph>(
            next_query_vertex_ptr, next_target_vertex_ptr, match_state,
            target_matched)) {
      std::map<QueryVertexPtr, std::vector<TargetVertexPtr>>
          temp_candidate_set = candidate_set;
      UpdateState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
                  target_matched);

      UpdateCandidateSet<QueryGraph, TargetGraph>(
          next_query_vertex_ptr, next_target_vertex_ptr, temp_candidate_set,
          match_state, target_matched);

      if (!_VF2<match_semantics, QueryGraph, TargetGraph>(
              temp_candidate_set, match_state, target_matched, result_count,
              user_callback, prune_callback)) {
        return false;
      }
      RestoreState(next_query_vertex_ptr, next_target_vertex_ptr, match_state,
                   target_matched);
    }
  }

  return true;
}
template <class QueryVertexPtr, class TargetVertexPtr>
bool UpdateCandidateCallbackEmpty(
    std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> &candidate_set) {
  return true;
}
template <class QueryVertexPtr, class TargetVertexPtr,
          class CandidateSetContainer>
bool UpdateCandidateCallback(
    CandidateSetContainer &candidate_set, QueryVertexPtr *query_vertex_ptr,
    const std::vector<TargetVertexPtr> &possible_candidate) {
  if (possible_candidate.empty()) {
    return true;
  }
  std::vector<TargetVertexPtr> res_candidate;
  auto &query_vertex_candidate = candidate_set[*query_vertex_ptr];
  std::set_intersection(query_vertex_candidate.begin(),
                        query_vertex_candidate.end(),
                        possible_candidate.begin(), possible_candidate.end(),
                        inserter(res_candidate, res_candidate.begin()));
  if (res_candidate.empty()) return false;
  query_vertex_candidate = res_candidate;
  return true;
}
template <class QueryGraph, class InDegreeContainer>
inline void BFS(const QueryGraph &query_graph, InDegreeContainer &in_degree) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using QueryEdgeIDType = typename QueryGraph::EdgeType::IDType;
  std::set<QueryVertexPtr> used_vertex;
  std::set<QueryEdgeIDType> used_edge;
  // bfs get indegree (build dag but not really get new graph)
  for (auto it = query_graph.VertexCBegin(); !it.IsDone(); it++) {
    QueryVertexPtr query_vertex_ptr = it;
    if (used_vertex.count(query_vertex_ptr)) continue;
    std::queue<QueryVertexPtr> bfs_queue;
    in_degree[query_vertex_ptr] = 0;
    used_vertex.insert(query_vertex_ptr);
    bfs_queue.push(query_vertex_ptr);
    while (!bfs_queue.empty()) {
      QueryVertexPtr now_vertex_ptr = bfs_queue.front();
      bfs_queue.pop();
      for (auto edge_it = now_vertex_ptr->OutEdgeCBegin(); !edge_it.IsDone();
           edge_it++) {
        if (used_edge.count(edge_it->id())) continue;
        QueryVertexPtr next_vertex_ptr = edge_it->const_dst_ptr();
        used_edge.insert(edge_it->id());
        in_degree[next_vertex_ptr]++;
        if (!used_vertex.count(next_vertex_ptr)) {
          bfs_queue.push(next_vertex_ptr);
          used_vertex.insert(next_vertex_ptr);
        }
      }
      for (auto edge_it = now_vertex_ptr->InEdgeCBegin(); !edge_it.IsDone();
           edge_it++) {
        if (used_edge.count(edge_it->id())) continue;
        QueryVertexPtr next_vertex_ptr = edge_it->const_src_ptr();
        used_edge.insert(edge_it->id());
        in_degree[next_vertex_ptr]++;
        if (!used_vertex.count(next_vertex_ptr)) {
          bfs_queue.push(next_vertex_ptr);
          used_vertex.insert(next_vertex_ptr);
        }
      }
    }
  }
}
template <class QueryGraph, class InDegreeContainer, class TopuSeqContainer>
inline void TopuSort(const QueryGraph &query_graph,
                     InDegreeContainer &in_degree, TopuSeqContainer &topu_seq) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using QueryEdgeIDType = typename QueryGraph::EdgeType::IDType;
  std::queue<QueryVertexPtr> topu_sort_queue;
  std::set<QueryEdgeIDType> used_edge;
  // topu sort
  for (auto it = query_graph.VertexCBegin(); !it.IsDone(); it++) {
    QueryVertexPtr query_vertex_ptr = it;
    if (in_degree[query_vertex_ptr] == 0) {
      topu_sort_queue.push(query_vertex_ptr);
    }
  }
  while (!topu_sort_queue.empty()) {
    QueryVertexPtr query_vertex_ptr = topu_sort_queue.front();
    topu_sort_queue.pop();
    topu_seq.emplace_back(query_vertex_ptr);
    for (auto edge_it = query_vertex_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      if (used_edge.count(edge_it->id())) continue;
      QueryVertexPtr next_vertex_ptr = edge_it->const_dst_ptr();
      in_degree[next_vertex_ptr]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_ptr] == 0) {
        topu_sort_queue.push(next_vertex_ptr);
      }
    }
    for (auto edge_it = query_vertex_ptr->InEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      if (used_edge.count(edge_it->id())) continue;
      QueryVertexPtr next_vertex_ptr = edge_it->const_src_ptr();
      in_degree[next_vertex_ptr]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_ptr] == 0) {
        topu_sort_queue.push(next_vertex_ptr);
      }
    }
  }
}
template <class QueryGraph, class TopuSeqContainer>
inline void GetTopuSeq(const QueryGraph &query_graph,
                       TopuSeqContainer &topu_seq) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using QueryEdgeIDType = typename QueryGraph::EdgeType::IDType;
  std::map<QueryVertexPtr, int> in_degree;
  BFS(query_graph, in_degree);
  assert(in_degree.size() == query_graph.CountVertex());
  TopuSort(query_graph, in_degree, topu_seq);
}
template <class QueryVertexPtr, class TargetVertexPtr, class EdgeCountContainer>
inline bool EdgeCheck(const QueryVertexPtr &query_vertex_ptr,
                      const QueryVertexPtr &temp_query_vertex_ptr,
                      const TargetVertexPtr &target_vertex_ptr,
                      const TargetVertexPtr &temp_target_vertex_ptr,
                      const EdgeCountContainer &out_edge_count,
                      const EdgeCountContainer &in_edge_count) {
  auto it = out_edge_count.find(temp_query_vertex_ptr);
  if (it != out_edge_count.end()) {
    for (const auto &edge_count : it->second) {
      if (target_vertex_ptr->CountOutEdge(
              edge_count.first, temp_target_vertex_ptr) < edge_count.second)
        return false;
    }
  }
  it = in_edge_count.find(temp_query_vertex_ptr);
  if (it != in_edge_count.end()) {
    for (const auto &edge_count : it->second) {
      if (target_vertex_ptr->CountInEdge(
              edge_count.first, temp_target_vertex_ptr) < edge_count.second)
        return false;
    }
  }
  return true;
}
template <bool is_out_direction, class QueryVertexPtr, class TargetVertexPtr,
          class CandidateSetContainer, class EdgeCountContainer>
inline bool CheckIsLegal(const QueryVertexPtr query_vertex_ptr,
                         const TargetVertexPtr target_vertex_ptr,
                         const std::set<QueryVertexPtr> &used_vertex,
                         const EdgeCountContainer &out_edge_count,
                         const EdgeCountContainer &in_edge_count,
                         CandidateSetContainer &candidate_set) {
  for (auto edge_it = is_out_direction ? query_vertex_ptr->OutEdgeCBegin()
                                       : query_vertex_ptr->InEdgeCBegin();
       !edge_it.IsDone(); edge_it++) {
    QueryVertexPtr next_vertex_ptr =
        is_out_direction ? edge_it->const_dst_ptr() : edge_it->const_src_ptr();
    if (!used_vertex.count(next_vertex_ptr)) continue;
    auto &l1 = candidate_set.find(next_vertex_ptr)->second;
    bool find_flag = false;
    for (auto target_edge_it =
             is_out_direction
                 ? target_vertex_ptr->OutVertexCBegin(edge_it->label())
                 : target_vertex_ptr->InVertexCBegin(edge_it->label());
         !target_edge_it.IsDone(); target_edge_it++) {
      TargetVertexPtr temp_target_vertex_ptr = target_edge_it;
      if (!EdgeCheck(query_vertex_ptr, next_vertex_ptr, target_vertex_ptr,
                     temp_target_vertex_ptr, out_edge_count, in_edge_count))
        continue;
      auto it1 = std::lower_bound(l1.begin(), l1.end(), temp_target_vertex_ptr);
      if (it1 != l1.end() && (*it1) == temp_target_vertex_ptr) {
        find_flag = true;
        break;
      }
    }
    if (!find_flag) return false;
  }
  return true;
}
template <class QueryGraph, class TargetGraph, class CandidateSetContainer>
inline bool DAGDP(
    const QueryGraph &query_graph, const TargetGraph &target_graph,
    const std::vector<typename QueryGraph::VertexConstPtr> &topu_seq,
    CandidateSetContainer &candidate_set) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  using QueryEdgeLabelType = typename QueryGraph::EdgeType::LabelType;
  std::set<QueryVertexPtr> used_vertex;
  CandidateSetContainer temp_candidate_set;
  for (const auto &vertex_ptr : topu_seq) {
    used_vertex.insert(vertex_ptr);
    auto &l = candidate_set.find(vertex_ptr)->second;
    auto &l1 = temp_candidate_set[vertex_ptr];
    std::map<QueryVertexPtr, std::map<QueryEdgeLabelType, int>> in_edge_count,
        out_edge_count;
    for (auto edge_it = vertex_ptr->OutEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      QueryVertexPtr dst_ptr = edge_it->const_dst_ptr();
      if (!used_vertex.count(dst_ptr)) {
        continue;
      }
      out_edge_count[dst_ptr][edge_it->label()]++;
    }
    for (auto edge_it = vertex_ptr->InEdgeCBegin(); !edge_it.IsDone();
         edge_it++) {
      QueryVertexPtr dst_ptr = edge_it->const_src_ptr();
      if (!used_vertex.count(dst_ptr)) {
        continue;
      }
      in_edge_count[dst_ptr][edge_it->label()]++;
    }
    for (const auto &candidate_it : l) {
      bool candidate_flag = true;
      candidate_flag =
          CheckIsLegal<true>(vertex_ptr, candidate_it, used_vertex,
                             out_edge_count, in_edge_count, candidate_set);
      if (!candidate_flag) {
        continue;
      }
      candidate_flag =
          CheckIsLegal<false>(vertex_ptr, candidate_it, used_vertex,
                              out_edge_count, in_edge_count, candidate_set);
      if (candidate_flag) {
        l1.emplace_back(candidate_it);
      }
    }
    l = l1;
    if (l.empty()) return false;
  }
  return true;
}
template <class QueryGraph, class TargetGraph, class CandidateSetContainer>
inline bool RefineCandidateSet(const QueryGraph &query_graph,
                               const TargetGraph &target_graph,
                               CandidateSetContainer &candidate_set) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  std::vector<QueryVertexPtr> topu_seq;
  GetTopuSeq(query_graph, topu_seq);
  if (!DAGDP(query_graph, target_graph, topu_seq, candidate_set)) return false;
  std::reverse(topu_seq.begin(), topu_seq.end());
  if (!DAGDP(query_graph, target_graph, topu_seq, candidate_set)) return false;
  return true;
}

template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class MatchCallback, class PruneCallback,
          class UpdateCandidateCallback>
inline int VF2_Recursive(const QueryGraph &query_graph,
                         const TargetGraph &target_graph,
                         MatchCallback user_callback,
                         PruneCallback prune_callback,
                         UpdateCandidateCallback update_candidate_callback) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph,
                                         candidate_set)) {
    return 0;
  }
  if (!update_candidate_callback(candidate_set)) {
    return 0;
  }
  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  size_t result_count = 0;
  _VF2<match_semantics, QueryGraph, TargetGraph>(candidate_set, match_state,
                                                 target_matched, result_count,
                                                 user_callback, prune_callback);

  return static_cast<int>(result_count);
}
template <enum MatchSemantics match_semantics, class QueryGraph,
          class TargetGraph, class MatchCallback>
inline int VF2_Recursive(const QueryGraph &query_graph,
                         const TargetGraph &target_graph,
                         MatchCallback user_callback) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  return VF2_Recursive<match_semantics>(
      query_graph, target_graph, user_callback,
      std::bind(PruneCallbackEmpty<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1),
      std::bind(UpdateCandidateCallbackEmpty<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1));
}
}  // namespace _vf2_label_equal

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class MatchCallback>
inline int VF2_Label_Equal(const QueryGraph &query_graph,
                           const TargetGraph &target_graph,
                           MatchCallback user_callback) {
  return _vf2_label_equal::VF2_Recursive<match_semantics>(
      query_graph, target_graph, user_callback);
}
template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph>
inline int VF2_Label_Equal(const QueryGraph &query_graph,
                           const TargetGraph &target_graph, int max_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  return VF2_Label_Equal<match_semantics>(
      query_graph, target_graph,
      std::bind(_vf2::MatchCallbackLimit<QueryVertexPtr, TargetVertexPtr>,
                std::placeholders::_1, &max_result));
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class ResultContainer>
inline int VF2_Label_Equal(const QueryGraph &query_graph,
                           const TargetGraph &target_graph, int max_result,
                           ResultContainer &match_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;

  match_result.clear();

  return VF2_Label_Equal<match_semantics>(
      query_graph, target_graph,
      std::bind(_vf2::MatchCallbackSaveResult<QueryVertexPtr, TargetVertexPtr,
                                              ResultContainer>,
                std::placeholders::_1, &max_result, &match_result));
}
template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph>
inline int VF2_Label_Equal(
    const QueryGraph &query_graph, const TargetGraph &target_graph,
    typename QueryGraph::VertexConstPtr cal_supp_vertex_ptr,
    const std::vector<typename TargetGraph::VertexConstPtr> &possible_supp,
    std::vector<typename TargetGraph::VertexConstPtr> &supp) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  time_t t_begin = clock();
  if (!_vf2_label_equal::InitCandidateSet<match_semantics>(
          query_graph, target_graph, candidate_set))
    return 0;
  time_t t_end = clock();
  // std::cout << "init time = " << (1.0 * t_end - t_begin) / CLOCKS_PER_SEC
  //          << std::endl;
  if (!_vf2_label_equal::UpdateCandidateCallback<QueryVertexPtr,
                                                 TargetVertexPtr>(
          candidate_set, &cal_supp_vertex_ptr, possible_supp))
    return 0;
  time_t refine_begin = clock();
  size_t tot_size0 = 0, tot_size1 = 0;
  for (auto &it : candidate_set) tot_size0 += it.second.size();
  if (!_vf2_label_equal::RefineCandidateSet(query_graph, target_graph,
                                            candidate_set)) {
    return 0;
  }
  // for (const auto &it : candidate_set) {
  //  std::cout << it.first->id() << " " << it.second.size() << std::endl;
  //}
  time_t refine_end = clock();
  // std::cout << "refine time = "
  //          << (1.0 * refine_end - refine_begin) / CLOCKS_PER_SEC <<
  //          std::endl;
  for (auto &it : candidate_set) tot_size1 += it.second.size();
  // std::cout << "before size = " << tot_size0 << std::endl;
  // std::cout << "after size = " << tot_size1 << std::endl;
  // return 0;
  int total_call = 0;
  for (const auto &target_ptr :
       candidate_set.find(cal_supp_vertex_ptr)->second) {
    // if (target_ptr->id() == 1) break;
    _vf2_label_equal::func_call = 0;
    time_t begin = clock();
    int max_result = 1;
    size_t result_count = 0;
    std::map<QueryVertexPtr, TargetVertexPtr> match_state;
    std::set<TargetVertexPtr> target_matched;
    std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> temp_candidate_set =
        candidate_set;
    auto prune_callback = std::bind(
        _vf2_label_equal::PruneCallbackEmpty<QueryVertexPtr, TargetVertexPtr>,
        std::placeholders::_1);
    auto user_callback =
        std::bind(_vf2::MatchCallbackLimit<QueryVertexPtr, TargetVertexPtr>,
                  std::placeholders::_1, &max_result);
    _vf2_label_equal::UpdateState(cal_supp_vertex_ptr, target_ptr, match_state,
                                  target_matched);

    _vf2_label_equal::UpdateCandidateSet<QueryGraph, TargetGraph>(
        cal_supp_vertex_ptr, target_ptr, temp_candidate_set, match_state,
        target_matched);
    _vf2_label_equal::_VF2<match_semantics, QueryGraph, TargetGraph>(
        temp_candidate_set, match_state, target_matched, result_count,
        user_callback, prune_callback);
    // std::cout << "target ptr =" << target_ptr->id() << std::endl;
    // std::cout << "result_count = " << result_count << std::endl;
    if (result_count >= 1) {
      supp.emplace_back(target_ptr);
    }
    time_t end = clock();
    // std::cout << "id = " << target_ptr->id() << " "
    //          << "time = " << (1.0 * end - begin) / CLOCKS_PER_SEC <<
    //          std::endl;
    // std::cout << "call = " << _vf2_label_equal::func_call << std::endl;
    total_call += _vf2_label_equal::func_call;
  }
  // std::cout << "total call = " << total_call << std::endl;
  return static_cast<int>(supp.size());
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class MatchCallback,
          class PruneCallBack, class UpdateInitCandidateCallback>
inline int VF2_Label_Equal(
    const QueryGraph &query_graph, const TargetGraph &target_graph,
    MatchCallback match_callback, PruneCallBack prune_callback,
    UpdateInitCandidateCallback update_initcandidate_callback) {
  return _vf2_label_equal::VF2_Recursive<match_semantics>(
      query_graph, target_graph, match_callback, prune_callback,
      update_initcandidate_callback);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph, class ResultContainer>
inline int VF2_Label_Equal(
    const QueryGraph &query_graph, const TargetGraph &target_graph,
    const typename QueryGraph::VertexType::IDType query_id,
    const typename TargetGraph::VertexType::IDType target_id, int max_result,
    ResultContainer &match_result) {
  using QueryVertexPtr = typename QueryGraph::VertexConstPtr;
  using TargetVertexPtr = typename TargetGraph::VertexConstPtr;
  match_result.clear();
  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> candidate_set;
  if (!_vf2_label_equal::InitCandidateSet<match_semantics>(
          query_graph, target_graph, candidate_set))
    return 0;
  if (!_vf2_label_equal::RefineCandidateSet(query_graph, target_graph,
                                            candidate_set)) {
    return 0;
  }
  QueryVertexPtr query_ptr = query_graph.FindConstVertex(query_id);
  TargetVertexPtr target_ptr = target_graph.FindConstVertex(target_id);
  if (!_vf2_label_equal::CheckIsInCandidateSet(query_ptr, candidate_set,
                                               target_id)) {
    return 0;
  }
  std::map<QueryVertexPtr, TargetVertexPtr> match_state;
  std::set<TargetVertexPtr> target_matched;
  std::map<QueryVertexPtr, std::vector<TargetVertexPtr>> temp_candidate_set =
      candidate_set;
  _vf2_label_equal::UpdateState(query_ptr, target_ptr, match_state,
                                target_matched);
  _vf2_label_equal::UpdateCandidateSet<QueryGraph, TargetGraph>(
      query_ptr, target_ptr, temp_candidate_set, match_state, target_matched);
  size_t result_count = 0;
  auto prune_callback = std::bind(
      _vf2_label_equal::PruneCallbackEmpty<QueryVertexPtr, TargetVertexPtr>,
      std::placeholders::_1);
  return _vf2_label_equal::_VF2<match_semantics, QueryGraph, TargetGraph>(
      temp_candidate_set, match_state, target_matched, result_count,
      std::bind(_vf2::MatchCallbackSaveResult<QueryVertexPtr, TargetVertexPtr,
                                              ResultContainer>,
                std::placeholders::_1, &max_result, &match_result),
      prune_callback);
}
}  // namespace GUNDAM
#endif