#ifndef _GUNDAM_ALGORITHM_DPISO_H
#define _GUNDAM_ALGORITHM_DPISO_H
#include <cassert>
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

#include "match_helper.h"
#include "vf2.h"

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

namespace GUNDAM {
  
enum MatchSemantics { kIsomorphism, kHomomorphism };

namespace _dp_iso {
enum EdgeState { kIn, kOut };
// if query.CountEdge()>=large_query_edge, using fail set
// small query not using fail set
constexpr size_t large_query_edge = 6;
constexpr size_t adj_vertex_limit = 200000;

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle>
inline bool DegreeFiltering(
              QueryVertexHandle&  query_vertex_hanlde,
             TargetVertexHandle& target_vertex_handle) {
  if (query_vertex_hanlde->CountInEdge() 
   > target_vertex_handle->CountInEdge())
    return false;
  if (query_vertex_hanlde->CountOutEdge() 
   > target_vertex_handle->CountOutEdge())
    return false;
  if (query_vertex_hanlde->CountInVertex() 
   > target_vertex_handle->CountInVertex())
    return false;
  if (query_vertex_hanlde->CountOutVertex() 
   > target_vertex_handle->CountOutVertex())
    return false;
  return true;
}

template <enum EdgeState   edge_state, 
          typename  QueryVertexHandle,
          typename TargetVertexHandle>
inline bool NeighborLabelFrequencyFilteringSingleDirection(
     QueryVertexHandle&  query_vertex_hanlde,
    TargetVertexHandle& target_vertex_handle) {
  for (auto edge_label_it =
           (edge_state == EdgeState::kIn ? query_vertex_hanlde-> InEdgeBegin()
                                         : query_vertex_hanlde->OutEdgeBegin());
           !edge_label_it.IsDone(); 
            edge_label_it++) {
    auto query_count =
        (edge_state == EdgeState::kIn
             ? query_vertex_hanlde-> CountInVertex(edge_label_it->label())
             : query_vertex_hanlde->CountOutVertex(edge_label_it->label()));
    auto target_count =
        (edge_state == EdgeState::kIn
             ? target_vertex_handle-> CountInVertex(edge_label_it->label())
             : target_vertex_handle->CountOutVertex(edge_label_it->label()));
    if (query_count > target_count) {
      return false;
    }
    query_count =
        (edge_state == EdgeState::kIn
             ? query_vertex_hanlde-> CountInEdge(edge_label_it->label())
             : query_vertex_hanlde->CountOutEdge(edge_label_it->label()));
    target_count =
        (edge_state == EdgeState::kIn
             ? target_vertex_handle->CountInEdge(edge_label_it->label())
             : target_vertex_handle->CountOutEdge(edge_label_it->label()));
    if (query_count > target_count) {
      return false;
    }
  }
  return true;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle>
inline bool NeighborLabelFrequencyFiltering(
     QueryVertexHandle&  query_vertex_hanlde,
    TargetVertexHandle& target_vertex_handle) {
  if (!NeighborLabelFrequencyFilteringSingleDirection<EdgeState::kIn>(
          query_vertex_hanlde, 
         target_vertex_handle)) {
    return false;
  }
  if (!NeighborLabelFrequencyFilteringSingleDirection<EdgeState::kOut>(
          query_vertex_hanlde, 
         target_vertex_handle)) {
    return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph, 
          typename CandidateSetContainer>
inline bool InitCandidateSet(QueryGraph&  query_graph,
                            TargetGraph& target_graph,
                  CandidateSetContainer& candidate_set) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  for (auto query_vertex_iter = query_graph.VertexBegin();
           !query_vertex_iter.IsDone(); 
            query_vertex_iter++) {
    QueryVertexHandle query_vertex_hanlde = query_vertex_iter;
    auto &l = candidate_set[query_vertex_iter];
    for (auto target_vertex_iter = target_graph.VertexBegin(query_vertex_hanlde->label());
             !target_vertex_iter.IsDone(); 
              target_vertex_iter++) {
      TargetVertexHandle target_vertex_handle = target_vertex_iter;
      if (!DegreeFiltering(query_vertex_hanlde, target_vertex_handle)) {
        continue;
      }
      if (!NeighborLabelFrequencyFiltering(query_vertex_hanlde,
                                           target_vertex_handle)) {
        continue;
      }
      l.emplace_back(target_vertex_handle);
    }
    if (l.empty()) return false;
  }
  for (auto &it : candidate_set) {
    sort(it.second.begin(), it.second.end());
  }
  return true;
}

template <enum EdgeState   edge_state, 
          typename  QueryVertexHandle,
          typename TargetVertexHandle,
          typename CandidateSetContainer>
inline void GetAdjNotMatchedVertexSingleDirection(
    QueryVertexHandle &query_vertex_hanlde,
    CandidateSetContainer &candidate_set,
     std::map<QueryVertexHandle, 
             TargetVertexHandle> &match_state,
     std::set<QueryVertexHandle> &next_query_set) {
  for (auto edge_iter =
           (edge_state == EdgeState::kIn ? query_vertex_hanlde->InEdgeBegin()
                                         : query_vertex_hanlde->OutEdgeBegin());
           !edge_iter.IsDone(); 
            edge_iter++) {
    QueryVertexHandle query_opp_vertex_handle =
        edge_state == EdgeState::kIn ? edge_iter->src_ptr()
                                     : edge_iter->dst_ptr();
    if (match_state.count(query_opp_vertex_handle) == 0 &&
        candidate_set.count(query_opp_vertex_handle)) {
      next_query_set.insert(query_opp_vertex_handle);
    }
  }
  return;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle,
          typename CandidateSetContainer>
inline void GetAdjNotMatchedVertex(
    QueryVertexHandle &query_vertex_hanlde,
    CandidateSetContainer &candidate_set,
    std::map<QueryVertexHandle, TargetVertexHandle> &match_state,
    std::set<QueryVertexHandle> &next_query_set) {
  GetAdjNotMatchedVertexSingleDirection<EdgeState::kOut>(
      query_vertex_hanlde, candidate_set, match_state, next_query_set);
  GetAdjNotMatchedVertexSingleDirection<EdgeState::kIn>(
      query_vertex_hanlde, candidate_set, match_state, next_query_set);
}
template <typename  QueryVertexHandle, 
          typename TargetVertexHandle,
          typename CandidateSetContainer>
inline QueryVertexHandle NextMatchVertex(
    CandidateSetContainer &candidate_set,
    std::map<QueryVertexHandle, 
            TargetVertexHandle> &match_state) {
  std::set<QueryVertexHandle> next_query_set;
  for (auto &match_pair : match_state) {
    QueryVertexHandle vertex_handle = match_pair.first;
    GetAdjNotMatchedVertex(vertex_handle, candidate_set, match_state,
                           next_query_set);
  }
  if (next_query_set.empty()) {
    for (auto &candidate_pair : candidate_set) {
      auto &query_vertex_hanlde = candidate_pair.first;
      if (match_state.count(query_vertex_hanlde) == 0) {
        next_query_set.emplace(query_vertex_hanlde);
      }
    }
  }

  QueryVertexHandle res;
  size_t min = SIZE_MAX;
  for (auto &query_vertex_hanlde : next_query_set) {
    size_t candidate_count =
        candidate_set.find(query_vertex_hanlde)->second.size();
    if (candidate_count < min) {
      res = query_vertex_hanlde;
      min = candidate_count;
    }
  }
  // assert(min >= 0 && !res.IsNull());
  return res;
}

template <enum EdgeState edge_state, 
          typename QueryGraph, 
          typename TargetGraph,
          typename MatchStateMap>
inline bool JoinableCheck(
      typename VertexHandle< QueryGraph>::type& query_vertex_hanlde,
      typename VertexHandle<TargetGraph>::type& target_vertex_handle,
      MatchStateMap &match_state) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using  QueryEdgeHandle = typename EdgeHandle< QueryGraph>::type;
  using TargetEdgeHandle = typename EdgeHandle<TargetGraph>::type;
  using QueryEdgeMapPair = std::pair<typename QueryGraph::EdgeType::LabelType,
                                     typename VertexHandle< QueryGraph>::type>;
  using TargetEdgeMapPair = std::pair<typename TargetGraph::EdgeType::LabelType,
                                      typename VertexHandle<TargetGraph>::type>;
  std::set<typename TargetGraph::EdgeType::IDType> used_edge;
  for (auto query_edge_iter =
           ((edge_state == EdgeState::kIn) ? query_vertex_hanlde->InEdgeBegin()
                                           : query_vertex_hanlde->OutEdgeBegin());
       !query_edge_iter.IsDone(); query_edge_iter++) {
    QueryEdgeHandle query_edge_handle = query_edge_iter;
    QueryVertexHandle query_opp_vertex_handle = (edge_state == EdgeState::kIn)
                                              ? query_edge_handle->src_ptr()
                                              : query_edge_handle->dst_ptr();
    auto match_iter = match_state.find(query_opp_vertex_handle);
    if (match_iter == match_state.end()) continue;

    TargetVertexHandle query_opp_match_vertex_handle = match_iter->second;

    bool find_target_flag = false;
    for (auto target_edge_iter =
             ((edge_state == EdgeState::kIn)
                  ? target_vertex_handle->InEdgeBegin(query_edge_handle->label(),
                                                    query_opp_match_vertex_handle)
                  : target_vertex_handle->OutEdgeBegin(
                        query_edge_handle->label(), query_opp_match_vertex_handle));
         !target_edge_iter.IsDone(); target_edge_iter++) {
      if (used_edge.count(target_edge_iter->id())) continue;
      find_target_flag = true;
      used_edge.insert(target_edge_iter->id());
      break;
    }
    if (!find_target_flag) return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph, 
          typename MatchStateMap, 
          typename TargetVertexSet>
inline bool IsJoinable(
      typename VertexHandle< QueryGraph>::type  query_vertex_hanlde,
      typename VertexHandle<TargetGraph>::type target_vertex_handle,
      MatchStateMap &match_state,
      TargetVertexSet &target_matched) {
  if (match_semantics == MatchSemantics::kIsomorphism &&
      target_matched.count(target_vertex_handle) > 0) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kIn, QueryGraph, TargetGraph>(
          query_vertex_hanlde, 
         target_vertex_handle, 
          match_state)) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kOut, QueryGraph, TargetGraph>(
          query_vertex_hanlde, 
         target_vertex_handle, 
          match_state)) {
    return false;
  }
  return true;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle,
          typename MatchStateMap,
          typename TargetVertexSet>
inline void UpdateState(QueryVertexHandle query_vertex_hanlde,
                       TargetVertexHandle target_vertex_handle,
                        MatchStateMap &match_state,
                       TargetVertexSet &target_matched) {
  match_state.insert(std::make_pair(query_vertex_hanlde, 
                                   target_vertex_handle));
  target_matched.insert(target_vertex_handle);
  return;
}

template <enum EdgeState edge_state, 
          typename QueryGraph, 
          typename TargetGraph, 
          typename MatchStateMap,
          typename TargetVertexSet, 
          typename CandidateSetContainer>
inline void UpdateCandidateSetOneDirection(
    typename VertexHandle< QueryGraph>::type  query_vertex_hanlde, 
    typename VertexHandle<TargetGraph>::type target_vertex_handle,
    CandidateSetContainer &candidate_set, 
    MatchStateMap &match_state,
    TargetVertexSet &target_matched) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  std::map<typename TargetGraph::VertexType::LabelType,
           std::vector<TargetVertexHandle>>
      temp_adj_vertex;
  std::set<typename QueryGraph::EdgeType::LabelType> used_edge_label;

  for (auto label_it =
           ((edge_state == EdgeState::kIn) ? query_vertex_hanlde->InEdgeBegin()
                                           : query_vertex_hanlde->OutEdgeBegin());
       !label_it.IsDone(); label_it++) {
    QueryVertexHandle temp_handle = (edge_state == EdgeState::kIn)
                                  ? label_it->src_ptr()
                                  : label_it->dst_ptr();
    if (match_state.count(temp_handle)) continue;
    if (!candidate_set.count(temp_handle)) continue;
    //枚举label
    if (used_edge_label.count(label_it->label())) continue;
    used_edge_label.insert(label_it->label());
    temp_adj_vertex.clear();
    size_t adj_count =
        (edge_state == EdgeState::kIn)
            ? target_vertex_handle->CountInVertex(label_it->label())
            : target_vertex_handle->CountOutVertex(label_it->label());
    if (adj_count > adj_vertex_limit) {
      continue;
    }
    for (auto it =
             ((edge_state == EdgeState::kIn)
                  ? target_vertex_handle->InVertexBegin(label_it->label())
                  : target_vertex_handle->OutVertexBegin(label_it->label()));
         !it.IsDone(); it++) {
      TargetVertexHandle temp_target_handle = it;
      temp_adj_vertex[temp_target_handle->label()].push_back(temp_target_handle);
    }
    for (auto &it : temp_adj_vertex) {
      std::sort(it.second.begin(), it.second.end());
      auto erase_it = std::unique(it.second.begin(), it.second.end());
      it.second.erase(erase_it, it.second.end());
    }
    std::set<QueryVertexHandle> used_vertex;
    for (auto vertex_it = ((edge_state == EdgeState::kIn)
                               ? query_vertex_hanlde->InEdgeBegin()
                               : query_vertex_hanlde->OutEdgeBegin());
         !vertex_it.IsDone(); vertex_it++) {
      //枚举Vertex
      if (vertex_it->label() != label_it->label()) continue;

      QueryVertexHandle temp_vertex_handle = (edge_state == EdgeState::kIn)
                                           ? vertex_it->src_ptr()
                                           : vertex_it->dst_ptr();
      if (used_vertex.count(temp_vertex_handle)) continue;
      if (!candidate_set.count(temp_vertex_handle)) continue;
      used_vertex.insert(temp_vertex_handle);
      std::vector<TargetVertexHandle> res_candidate;
      if (match_state.count(temp_vertex_handle)) continue;
      std::set_intersection(temp_adj_vertex[temp_vertex_handle->label()].begin(),
                            temp_adj_vertex[temp_vertex_handle->label()].end(),
                            candidate_set[temp_vertex_handle].begin(),
                            candidate_set[temp_vertex_handle].end(),
                            inserter(res_candidate, res_candidate.begin()));
      candidate_set[temp_vertex_handle] = res_candidate;
    }
  }
  return;
}

template <typename QueryGraph, 
          typename TargetGraph,
          typename MatchStateMap, 
          typename TargetVertexSet,
          typename CandidateSetContainer>
inline void UpdateCandidateSet(
      typename VertexHandle< QueryGraph>::type  query_vertex_hanlde,
      typename VertexHandle<TargetGraph>::type target_vertex_handle,
      CandidateSetContainer &candidate_set,
      MatchStateMap &match_state,
      TargetVertexSet &target_matched) {
  // test liantong data:
  UpdateCandidateSetOneDirection<EdgeState::kIn, QueryGraph, TargetGraph>(
      query_vertex_hanlde, 
     target_vertex_handle, 
      candidate_set, match_state, target_matched);
  UpdateCandidateSetOneDirection<EdgeState::kOut, QueryGraph, TargetGraph>(
      query_vertex_hanlde, 
     target_vertex_handle, 
      candidate_set, match_state, target_matched);
  return;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle, 
          typename MatchStateMap,
          typename TargetVertexSet>
inline void RestoreState(QueryVertexHandle  query_vertex_hanlde,
                        TargetVertexHandle target_vertex_handle,
                         MatchStateMap &match_state,
                         TargetVertexSet &target_matched) {
    match_state .erase( query_vertex_hanlde);
  target_matched.erase(target_vertex_handle);
  return;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle, 
          typename ResultContainer>
inline bool MatchCallbackSaveSupp(
           std::map<QueryVertexHandle, 
                   TargetVertexHandle> &m,
                    QueryVertexHandle *query_vertex_hanlde,
                    ResultContainer *r) {
  if (r) {
    r->insert(m[*query_vertex_hanlde]);
  }
  return true;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle, 
          typename SuppContainer>
inline bool PruneSuppCallback(
    std::map<QueryVertexHandle, 
                  TargetVertexHandle> &m,
                   QueryVertexHandle *query_vertex_hanlde, 
                       SuppContainer *r) {
  if (r) {
    auto it = m.find(*query_vertex_hanlde);
    if (it == m.end()) {
      return false;
    }
    TargetVertexHandle target_vertex_handle = it->second;
    if (r->count(target_vertex_handle)) {
      return true;
    }
  }
  return false;
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle>
inline bool PruneCallbackEmpty(
    std::map<QueryVertexHandle, 
                  TargetVertexHandle> &m) {
  return false;
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph, 
          typename CandidateSetContainer,
          typename MatchCallback, 
          typename PruneCallback>
bool _DPISO(CandidateSetContainer &candidate_set,
            std::map<typename VertexHandle< QueryGraph>::type, 
                     typename VertexHandle<TargetGraph>::type> &match_state,
            std::set<typename VertexHandle<TargetGraph>::type> &target_matched, 
            size_t &result_count,
            MatchCallback user_callback, 
            PruneCallback prune_callback,
            time_t begin_time, 
            double query_limit_time = 1200) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  if (query_limit_time > 0 &&
      ((clock() - begin_time) / CLOCKS_PER_SEC) > query_limit_time) {
    return false;
  }
  // for (auto &it : match_state) {
  //  std::cout << it.first->id() << " " << it.second->id() << std::endl;
  //}
  // std::cout << std::endl;
  if (prune_callback(match_state)) {
    return true;
  }
  if (match_state.size() == candidate_set.size()) {
    result_count++;
    return user_callback(match_state);
  }
  QueryVertexHandle next_query_vertex_handle =
      NextMatchVertex(candidate_set, match_state);
  for (TargetVertexHandle &next_target_vertex_handle :
       candidate_set.find(next_query_vertex_handle)->second) {
    if (prune_callback(match_state)) {
      return true;
    }
    if (IsJoinable<match_semantics, QueryGraph, TargetGraph>(
            next_query_vertex_handle, next_target_vertex_handle, match_state,
            target_matched)) {
      std::map<QueryVertexHandle, std::vector<TargetVertexHandle>>
          temp_candidate_set = candidate_set;
      UpdateState(next_query_vertex_handle, next_target_vertex_handle, match_state,
                  target_matched);
      auto t_begin = clock();
      UpdateCandidateSet<QueryGraph, TargetGraph>(
          next_query_vertex_handle, next_target_vertex_handle, temp_candidate_set,
          match_state, target_matched);
      auto t_end = clock();
      // std::cout << "update1 time = " << (1.0 * t_end - t_begin) /
      // CLOCKS_PER_SEC
      //          << std::endl;
      if (!_DPISO<match_semantics, QueryGraph, TargetGraph>(
              temp_candidate_set, match_state, target_matched, result_count,
              user_callback, prune_callback, begin_time, query_limit_time)) {
        return false;
      }
      RestoreState(next_query_vertex_handle, next_target_vertex_handle, match_state,
                   target_matched);
    }
  }
  return true;
}

template <enum EdgeState edge_state, 
          class QueryVertexHandle,
          class TargetVertexHandle>
void UpdateParentSingleDirection(
    std::map<QueryVertexHandle, 
                  TargetVertexHandle> &match_state,
    QueryVertexHandle update_query_vertex_handle,
    std::map<QueryVertexHandle, 
             std::vector<QueryVertexHandle>> &parent) {
  for (auto edge_it = (edge_state == EdgeState::kIn
                           ? update_query_vertex_handle->InEdgeBegin()
                           : update_query_vertex_handle->OutEdgeBegin());
       !edge_it.IsDone(); edge_it++) {
    QueryVertexHandle update_query_adj_handle = edge_state == EdgeState::kIn
                                              ? edge_it->src_ptr()
                                              : edge_it->dst_ptr();
    if (match_state.find(update_query_adj_handle) == match_state.end()) continue;
    auto &query_parent_set = parent.find(update_query_vertex_handle)->second;
    auto find_it = parent.find(update_query_adj_handle);
    if (find_it != parent.end()) {
      for (auto &it : find_it->second) {
        query_parent_set.push_back(it);
      }
    }
  }
  return;
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle>
void UpdateParent(
    std::map<QueryVertexHandle,
                  TargetVertexHandle> &match_state,
    QueryVertexHandle update_query_vertex_handle,
    std::map<QueryVertexHandle, 
             std::vector<QueryVertexHandle>> &parent) {
  parent[update_query_vertex_handle].push_back(update_query_vertex_handle);
  UpdateParentSingleDirection<EdgeState::kIn>(match_state,
                                              update_query_vertex_handle, parent);
  UpdateParentSingleDirection<EdgeState::kOut>(match_state,
                                               update_query_vertex_handle, parent);
  auto &l = parent.find(update_query_vertex_handle)->second;
  std::sort(l.begin(), l.end());
  auto erase_it = std::unique(l.begin(), l.end());
  l.erase(erase_it, l.end());
  return;
}

// using Fail Set
template <enum MatchSemantics match_semantics, 
          class QueryGraph,
          typename TargetGraph, 
          class CandidateSetContainer, class MatchCallback, class PruneCallback>
bool _DPISO(CandidateSetContainer &candidate_set,
            std::map<typename VertexHandle< QueryGraph>::type, 
                     typename VertexHandle<TargetGraph>::type> &match_state,
            std::set<typename VertexHandle<TargetGraph>::type> &target_matched,
            std::map<typename VertexHandle< QueryGraph>::type, 
                     std::vector<typename VertexHandle< QueryGraph>::type>> &parent,
            std::vector<typename VertexHandle< QueryGraph>::type> &fail_set, 
            size_t &result_count,
            MatchCallback user_callback, 
            PruneCallback prune_callback,
            time_t begin_time, 
            double query_limit_time = 1200) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  if (query_limit_time > 0 &&
      ((clock() - begin_time) / CLOCKS_PER_SEC) > query_limit_time) {
    return false;
  }
  if (match_state.size() == candidate_set.size()) {
    // find match ,so fail set is empty
    fail_set.clear();
    result_count++;
    return user_callback(match_state);
  }
  if (prune_callback(match_state)) {
    return true;
  }
  QueryVertexHandle next_query_vertex_handle =
      NextMatchVertex(candidate_set, match_state);
  // cal this vertex's  parent
  UpdateParent(match_state, next_query_vertex_handle, parent);
  std::vector<QueryVertexHandle> this_state_fail_set;
  bool find_fail_set_flag = false;
  if (candidate_set.find(next_query_vertex_handle)->second.size() == 0) {
    // C(u) is empty ,so fail set = anc(u)
    this_state_fail_set = parent.find(next_query_vertex_handle)->second;
  }
  for (TargetVertexHandle &next_target_vertex_handle :
       candidate_set.find(next_query_vertex_handle)->second) {
    if (prune_callback(match_state)) {
      return true;
    }
    if (find_fail_set_flag && !this_state_fail_set.empty() &&
        !std::binary_search(this_state_fail_set.begin(),
                            this_state_fail_set.end(), next_query_vertex_handle)) {
      // find fail set , u is not in fail set and fail set is not empty!
      // so not expand
      std::swap(fail_set, this_state_fail_set);
      return true;
    }
    if (!find_fail_set_flag &&
        match_semantics == MatchSemantics::kIsomorphism &&
        target_matched.count(next_target_vertex_handle) > 0) {
      // find u' satisfy that match_state[u']=next_target_vertex_handle
      // next_state_fail_set = anc[u] union anc[u']
      std::vector<QueryVertexHandle> next_state_fail_set;
      for (auto &it : match_state) {
        if (it.second == next_target_vertex_handle) {
          std::vector<QueryVertexHandle> temp_next_state_fail_set;
          std::set_union(std::begin(parent.find(next_query_vertex_handle)->second),
                         std::end(parent.find(next_query_vertex_handle)->second),
                         std::begin(parent.find(it.first)->second),
                         std::end(parent.find(it.first)->second),
                         inserter(temp_next_state_fail_set,
                                  temp_next_state_fail_set.begin()));
          std::swap(next_state_fail_set, temp_next_state_fail_set);
          break;
        }
      }
      // update this_state_fail_set
      // anc[u] must contain u ,so union fail set
      std::vector<QueryVertexHandle> temp_this_state_fail_set;
      std::set_union(
          std::begin(next_state_fail_set), std::end(next_state_fail_set),
          std::begin(this_state_fail_set), std::end(this_state_fail_set),
          inserter(temp_this_state_fail_set, temp_this_state_fail_set.begin()));
      std::swap(this_state_fail_set, temp_this_state_fail_set);
    }
    if (IsJoinable<match_semantics, QueryGraph, TargetGraph>(
            next_query_vertex_handle, next_target_vertex_handle, match_state,
            target_matched)) {
      std::map<QueryVertexHandle, std::vector<TargetVertexHandle>>
          temp_candidate_set = candidate_set;
      UpdateState(next_query_vertex_handle, next_target_vertex_handle, match_state,
                  target_matched);
      UpdateCandidateSet<QueryGraph, TargetGraph>(
          next_query_vertex_handle, next_target_vertex_handle, temp_candidate_set,
          match_state, target_matched);
      std::vector<QueryVertexHandle> next_state_fail_set;
      if (!_DPISO<match_semantics, QueryGraph, TargetGraph>(
              temp_candidate_set, match_state, target_matched, parent,
              next_state_fail_set, result_count, user_callback, prune_callback,
              begin_time, query_limit_time)) {
        return false;
      }

      RestoreState(next_query_vertex_handle, next_target_vertex_handle, match_state,
                   target_matched);

      if (next_state_fail_set.empty()) {
        // if ont child node's fail set is empty
        // so this state's fail set is empty
        find_fail_set_flag = true;
        this_state_fail_set.clear();
      } else if (!std::binary_search(next_state_fail_set.begin(),
                                     next_state_fail_set.end(),
                                     next_query_vertex_handle)) {
        // if one child node's fail set not contain next_query_vertex_handle
        // so this state's fail set is next_state_fail_set
        find_fail_set_flag = true;
        this_state_fail_set = next_state_fail_set;
      } else if (!find_fail_set_flag) {
        std::vector<QueryVertexHandle> temp_this_state_fail_set;
        std::set_union(
            std::begin(next_state_fail_set), std::end(next_state_fail_set),
            std::begin(this_state_fail_set), std::end(this_state_fail_set),
            inserter(temp_this_state_fail_set,
                     temp_this_state_fail_set.begin()));
        std::swap(temp_this_state_fail_set, this_state_fail_set);
      }
    }
  }
  std::swap(fail_set, this_state_fail_set);
  return true;
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle>
bool UpdateCandidateCallbackEmpty(
    std::map<QueryVertexHandle, 
             std::vector<TargetVertexHandle>> &candidate_set) {
  return true;
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle,
          class CandidateSetContainer>
bool UpdateCandidateCallback(
    CandidateSetContainer &candidate_set, 
        QueryVertexHandle *query_vertex_hanlde,
    std::vector<TargetVertexHandle> &possible_candidate) {
  if (possible_candidate.empty()) {
    return true;
  }
  std::vector<TargetVertexHandle> res_candidate;
  auto &query_vertex_candidate = candidate_set[*query_vertex_hanlde];
  std::set_intersection(query_vertex_candidate.begin(),
                        query_vertex_candidate.end(),
                        possible_candidate.begin(), possible_candidate.end(),
                        inserter(res_candidate, res_candidate.begin()));
  if (res_candidate.empty()) return false;
  query_vertex_candidate = res_candidate;
  return true;
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle>
inline bool MatchCallbackLimit(
    std::map<QueryVertexHandle, 
                  TargetVertexHandle> &, int *max_result) {
  if (max_result && *max_result > 0) {
    (*max_result)--;
    if (*max_result == 0) return false;
  }
  return true;
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle, 
          class ResultContainer>
inline bool MatchCallbackSaveResult1(
    std::map<QueryVertexHandle, 
                  TargetVertexHandle> &m, int *max_result,
    ResultContainer *r) {
  if (r) {
    r->emplace_back(m);
  }

  return MatchCallbackLimit(m, max_result);
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle, 
          class ResultContainer>
inline bool MatchCallbackSaveResult2(
    std::map<QueryVertexHandle,
                  TargetVertexHandle> &m, int *max_result,
    ResultContainer *r) {
  if (r) {
    using ResultType = typename ResultContainer::value_type;
    ResultType tmp;
    for (auto &p : m) {
      tmp.emplace_back(p);
    }
    r->emplace_back(std::move(tmp));
  }

  return MatchCallbackLimit(m, max_result);
}

template <class A, class B>
inline void CopyMap(std::map<A, B> &c, std::map<A, B> &m) {
  c = m;
  return;
};

template <class A, class B>
inline void CopyMap(std::list<std::pair<A, B>> &c, std::map<A, B> &m) {
  for (auto &p : m) {
    c.emplace_back(p);
  }
  return;
};

template <class A, class B>
inline void CopyMap(std::vector<std::pair<A, B>> &c, std::map<A, B> &m) {
  for (auto &p : m) {
    c.emplace_back(p);
  }
  return;
};

template <class A, class B>
inline void CopyMap(std::map<A, B> &c, const std::map<A, B> &m) {
  c = m;
  return;
};

template <class A, class B>
inline void CopyMap(std::list<std::pair<A, B>> &c, const std::map<A, B> &m) {
  for (auto &p : m) {
    c.emplace_back(p);
  }
  return;
};

template <class A, class B>
inline void CopyMap(std::vector<std::pair<A, B>> &c, const std::map<A, B> &m) {
  for (auto &p : m) {
    c.emplace_back(p);
  }
  return;
};

template <class  QueryVertexHandle, 
          class TargetVertexHandle, 
          class ResultContainer>
inline bool MatchCallbackSaveResult(
    std::map<QueryVertexHandle, 
                  TargetVertexHandle> &m, int *max_result,
    ResultContainer *r) {
  if (r) {
    using ResultType = typename ResultContainer::value_type;

    ResultType tmp;
    CopyMap(tmp, m);
    r->emplace_back(std::move(tmp));
  }

  return MatchCallbackLimit(m, max_result);
}

namespace _DAGDP {

template <class QueryGraph, 
          class InDegreeContainer>
inline void BFS(QueryGraph &query_graph, 
               InDegreeContainer &in_degree) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType = typename QueryGraph::EdgeType::IDType;
  std::set<QueryVertexHandle> used_vertex;
  std::set<QueryEdgeIDType> used_edge;
  // bfs get indegree (build dag but not really get new graph)
  for (auto it = query_graph.VertexBegin(); !it.IsDone(); it++) {
    QueryVertexHandle query_vertex_hanlde = it;
    if (used_vertex.count(query_vertex_hanlde)) continue;
    std::queue<QueryVertexHandle> bfs_queue;
    in_degree[query_vertex_hanlde] = 0;
    used_vertex.insert(query_vertex_hanlde);
    bfs_queue.push(query_vertex_hanlde);
    while (!bfs_queue.empty()) {
      QueryVertexHandle now_vertex_handle = bfs_queue.front();
      bfs_queue.pop();
      for (auto edge_it = now_vertex_handle->OutEdgeBegin(); !edge_it.IsDone();
           edge_it++) {
        if (used_edge.count(edge_it->id())) continue;
        QueryVertexHandle next_vertex_handle = edge_it->dst_ptr();
        used_edge.insert(edge_it->id());
        in_degree[next_vertex_handle]++;
        if (!used_vertex.count(next_vertex_handle)) {
          bfs_queue.push(next_vertex_handle);
          used_vertex.insert(next_vertex_handle);
        }
      }
      for (auto edge_it = now_vertex_handle->InEdgeBegin(); !edge_it.IsDone();
           edge_it++) {
        if (used_edge.count(edge_it->id())) continue;
        QueryVertexHandle next_vertex_handle = edge_it->src_ptr();
        used_edge.insert(edge_it->id());
        in_degree[next_vertex_handle]++;
        if (!used_vertex.count(next_vertex_handle)) {
          bfs_queue.push(next_vertex_handle);
          used_vertex.insert(next_vertex_handle);
        }
      }
    }
  }
  return;
}

template <class QueryGraph, 
          class InDegreeContainer, 
          class TopuSeqContainer>
inline void TopuSort(QueryGraph &query_graph,
                     InDegreeContainer &in_degree, 
                      TopuSeqContainer &topu_seq) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType = typename QueryGraph::EdgeType::IDType;
  std::queue<QueryVertexHandle> topu_sort_queue;
  std::set<QueryEdgeIDType> used_edge;
  // topu sort
  for (auto it = query_graph.VertexBegin(); !it.IsDone(); it++) {
    QueryVertexHandle query_vertex_hanlde = it;
    if (in_degree[query_vertex_hanlde] == 0) {
      topu_sort_queue.push(query_vertex_hanlde);
    }
  }
  while (!topu_sort_queue.empty()) {
    QueryVertexHandle query_vertex_hanlde = topu_sort_queue.front();
    topu_sort_queue.pop();
    topu_seq.emplace_back(query_vertex_hanlde);
    for (auto edge_it = query_vertex_hanlde->OutEdgeBegin(); !edge_it.IsDone();
         edge_it++) {
      if (used_edge.count(edge_it->id())) continue;
      QueryVertexHandle next_vertex_handle = edge_it->dst_ptr();
      in_degree[next_vertex_handle]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_handle] == 0) {
        topu_sort_queue.push(next_vertex_handle);
      }
    }
    for (auto edge_it = query_vertex_hanlde->InEdgeBegin(); !edge_it.IsDone();
         edge_it++) {
      if (used_edge.count(edge_it->id())) continue;
      QueryVertexHandle next_vertex_handle = edge_it->src_ptr();
      in_degree[next_vertex_handle]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_handle] == 0) {
        topu_sort_queue.push(next_vertex_handle);
      }
    }
  }
  return;
}

template <class QueryGraph, 
          class TopuSeqContainer>
inline void GetTopuSeq(QueryGraph &query_graph,
                       TopuSeqContainer &topu_seq) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType = typename QueryGraph::EdgeType::IDType;
  std::map<QueryVertexHandle, int> in_degree;
  BFS(query_graph, in_degree);
  assert(in_degree.size() == query_graph.CountVertex());
  TopuSort(query_graph, in_degree, topu_seq);
}

template <class  QueryVertexHandle, 
          class TargetVertexHandle, 
          class EdgeCountContainer>
inline bool EdgeCheck(QueryVertexHandle &query_vertex_hanlde,
                      QueryVertexHandle &temp_query_vertex_handle,
                      TargetVertexHandle &target_vertex_handle,
                      TargetVertexHandle &temp_target_vertex_handle,
                      EdgeCountContainer &out_edge_count,
                      EdgeCountContainer &in_edge_count) {
  auto it = out_edge_count.find(temp_query_vertex_handle);
  if (it != out_edge_count.end()) {
    for (auto &edge_count : it->second) {
      if (target_vertex_handle->CountOutEdge(
              edge_count.first, temp_target_vertex_handle) < edge_count.second)
        return false;
    }
  }
  it = in_edge_count.find(temp_query_vertex_handle);
  if (it != in_edge_count.end()) {
    for (auto &edge_count : it->second) {
      if (target_vertex_handle->CountInEdge(
              edge_count.first, temp_target_vertex_handle) < edge_count.second)
        return false;
    }
  }
  return true;
}
template <bool is_out_direction, 
          class  QueryVertexHandle,
          class TargetVertexHandle,
          class CandidateSetContainer, 
          class EdgeCountContainer>
inline bool CheckIsLegal(QueryVertexHandle query_vertex_hanlde,
                        TargetVertexHandle target_vertex_handle,
                         std::set<QueryVertexHandle> &used_vertex,
                         EdgeCountContainer &out_edge_count,
                         EdgeCountContainer &in_edge_count,
                         CandidateSetContainer &candidate_set) {
  for (auto edge_it = is_out_direction ? query_vertex_hanlde->OutEdgeBegin()
                                       : query_vertex_hanlde->InEdgeBegin();
       !edge_it.IsDone(); edge_it++) {
    QueryVertexHandle next_vertex_handle =
        is_out_direction ? edge_it->dst_ptr() : edge_it->src_ptr();
    if (!used_vertex.count(next_vertex_handle)) continue;
    auto &l1 = candidate_set.find(next_vertex_handle)->second;
    bool find_flag = false;
    for (auto target_edge_it =
             is_out_direction
                 ? target_vertex_handle->OutVertexBegin(edge_it->label())
                 : target_vertex_handle->InVertexBegin(edge_it->label());
         !target_edge_it.IsDone(); target_edge_it++) {
      TargetVertexHandle temp_target_vertex_handle = target_edge_it;
      if (!EdgeCheck(query_vertex_hanlde, next_vertex_handle, target_vertex_handle,
                     temp_target_vertex_handle, out_edge_count, in_edge_count))
        continue;
      auto it1 = std::lower_bound(l1.begin(), l1.end(), temp_target_vertex_handle);
      if (it1 != l1.end() && (*it1) == temp_target_vertex_handle) {
        find_flag = true;
        break;
      }
    }
    if (!find_flag) return false;
  }
  return true;
}

}  // namespace _DAGDP
template <typename QueryGraph, 
          typename TargetGraph, 
          class CandidateSetContainer>
inline bool DAGDP(
     QueryGraph&  query_graph, 
    TargetGraph& target_graph,
    std::vector<typename VertexHandle<QueryGraph>::type> &topu_seq,
    CandidateSetContainer &candidate_set) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  using QueryEdgeLabelType = typename QueryGraph::EdgeType::LabelType;
  std::set<QueryVertexHandle> used_vertex;
  CandidateSetContainer temp_candidate_set;
  for (auto &vertex_handle : topu_seq) {
    used_vertex.insert(vertex_handle);
    auto &l = candidate_set.find(vertex_handle)->second;
    auto &l1 = temp_candidate_set[vertex_handle];
    std::map<QueryVertexHandle, 
             std::map<QueryEdgeLabelType, int>> in_edge_count,
        out_edge_count;
    for (auto edge_it = vertex_handle->OutEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      QueryVertexHandle dst_handle = edge_it->dst_ptr();
      if (!used_vertex.count(dst_handle)) {
        continue;
      }
      out_edge_count[dst_handle][edge_it->label()]++;
    }
    for (auto edge_it = vertex_handle->InEdgeBegin(); !edge_it.IsDone();
         edge_it++) {
      QueryVertexHandle dst_handle = edge_it->src_ptr();
      if (!used_vertex.count(dst_handle)) {
        continue;
      }
      in_edge_count[dst_handle][edge_it->label()]++;
    }
    for (auto &candidate_it : l) {
      bool candidate_flag = true;
      candidate_flag = _DAGDP::CheckIsLegal<true>(vertex_handle, candidate_it,
                                                  used_vertex, out_edge_count,
                                                  in_edge_count, candidate_set);
      if (!candidate_flag) {
        continue;
      }
      candidate_flag = _DAGDP::CheckIsLegal<false>(
          vertex_handle, candidate_it, used_vertex, out_edge_count, in_edge_count,
          candidate_set);
      if (candidate_flag) {
        l1.emplace_back(candidate_it);
      }
    }
    l = l1;
    if (l.empty()) return false;
  }
  return true;
}

template <typename QueryGraph, 
          typename TargetGraph, class CandidateSetContainer>
inline bool RefineCandidateSet(QueryGraph &query_graph,
                               TargetGraph &target_graph,
                               CandidateSetContainer &candidate_set) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  std::vector<QueryVertexHandle> topu_seq;
  _DAGDP::GetTopuSeq(query_graph, topu_seq);
  if (!DAGDP(query_graph, target_graph, topu_seq, candidate_set)) return false;
  constexpr int loop_num = 2;
  for (int i = 1; i <= loop_num; i++) {
    std::reverse(topu_seq.begin(), topu_seq.end());
    if (!DAGDP(query_graph, target_graph, topu_seq, candidate_set))
      return false;
  };
  return true;
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph,
          class MatchCallback, 
          class PruneCallback,
          class UpdateCandidateCallback>
inline int DPISO_Recursive(QueryGraph &query_graph,
                          TargetGraph &target_graph,
                          MatchCallback user_callback,
                          PruneCallback prune_callback,
                          UpdateCandidateCallback update_candidate_callback,
                          double query_limit_time = 1200) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  std::map<QueryVertexHandle, 
           std::vector<TargetVertexHandle>> candidate_set;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph,
                                         candidate_set)) {
    return 0;
  }
  if (!RefineCandidateSet(query_graph, target_graph, candidate_set)) {
    return 0;
  }
  if (!update_candidate_callback(candidate_set)) {
    return 0;
  }
  std::map<QueryVertexHandle,
          TargetVertexHandle> match_state;
  std::set<TargetVertexHandle> target_matched;
  size_t result_count = 0;
  if (query_graph.CountEdge() < large_query_edge) {
    _DPISO<match_semantics, QueryGraph, TargetGraph>(
        candidate_set, match_state, target_matched, result_count, user_callback,
        prune_callback, clock(), query_limit_time);
  } else {
    std::map<QueryVertexHandle, std::vector<QueryVertexHandle>> parent;
    std::vector<QueryVertexHandle> fail_set;
    _DPISO<match_semantics, QueryGraph, TargetGraph>(
        candidate_set, match_state, target_matched, parent, fail_set,
        result_count, user_callback, prune_callback, clock(), query_limit_time);
  }
  return static_cast<int>(result_count);
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph, 
          class MatchCallback, 
          class PruneCallback>
inline int DPISO_Recursive(
     QueryGraph& query_graph, 
    TargetGraph& target_graph,
    std::map<typename VertexHandle< QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>
            >& candidate_set,
    std::map<typename VertexHandle< QueryGraph>::type,
             typename VertexHandle<TargetGraph>::type>& match_state,
    MatchCallback user_callback, 
    PruneCallback prune_callback,
    double query_limit_time = 1200.0) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  std::set<TargetVertexHandle> target_matched;
  for (auto &match : match_state) {
    target_matched.insert(match.second);
  }
  for (auto &match : match_state) {
    UpdateCandidateSet<QueryGraph, TargetGraph>(
        match.first, match.second, candidate_set, match_state, target_matched);
  }
  size_t result_count = 0;
  if (query_graph.CountEdge() >= large_query_edge) {
    std::map<QueryVertexHandle, std::vector<QueryVertexHandle>> parent;
    for (auto &match : match_state) {
      _dp_iso::UpdateParent(match_state, match.first, parent);
    }
    std::vector<QueryVertexHandle> fail_set;
    _dp_iso::_DPISO<match_semantics, QueryGraph, TargetGraph>(
        candidate_set, match_state, target_matched, parent, fail_set,
        result_count, user_callback, prune_callback, clock(), query_limit_time);
  } else {
    _dp_iso::_DPISO<match_semantics, QueryGraph, TargetGraph>(
        candidate_set, match_state, target_matched, result_count, user_callback,
        prune_callback, clock(), query_limit_time);
  }
  return static_cast<int>(result_count);
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph, 
          class MatchCallback, 
          class PruneCallback>
inline int DPISO_Recursive(
     QueryGraph &query_graph, 
    TargetGraph &target_graph,
    std::map<typename VertexHandle< QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    MatchCallback user_callback, 
    PruneCallback prune_callback,
    double query_limit_time = 1200) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  std::map<typename  QueryGraph::VertexConstPtr,
           typename VertexHandle<TargetGraph>::type> match_state;
  return DPISO_Recursive<match_semantics, QueryGraph, TargetGraph>(
      query_graph, target_graph, candidate_set, match_state, user_callback,
      prune_callback, query_limit_time);
}

template <enum MatchSemantics match_semantics, 
          typename QueryGraph,
          typename TargetGraph, 
          class MatchCallback, 
          class PruneCallback,
          class UpdateCandidateCallback>
inline int DPISO_Recursive(
     QueryGraph &query_graph, 
    TargetGraph &target_graph,
    std::map<typename VertexHandle< QueryGraph>::type,
             typename VertexHandle<TargetGraph>::type> &match_state,
    MatchCallback user_callback, 
    PruneCallback prune_callback,
    UpdateCandidateCallback update_candidate_callback,
    double query_limit_time = 1200) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  std::map<QueryVertexHandle,
           std::vector<TargetVertexHandle>> candidate_set;
  // std::cout << "begin" << std::endl;
  if (!InitCandidateSet<match_semantics>(query_graph, target_graph,
                                         candidate_set)) {
    return 0;
  }
  for (auto &match : match_state) {
    candidate_set[match.first].clear();
    candidate_set[match.first].push_back(match.second);
  }
  if (!RefineCandidateSet(query_graph, target_graph, candidate_set)) {
    return 0;
  }
  if (!update_candidate_callback(candidate_set)) {
    return 0;
  }
  return DPISO_Recursive<match_semantics>(
      query_graph, target_graph, candidate_set, match_state, user_callback,
      prune_callback, query_limit_time);
}

template <enum MatchSemantics match_semantics, 
          typename  QueryGraph,
          typename TargetGraph, 
          class MatchCallback>
inline int DPISO_Recursive(QueryGraph &query_graph,
                          TargetGraph &target_graph,
                          MatchCallback user_callback,
                          double query_limit_time = 1200) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  return DPISO_Recursive<match_semantics>(
      query_graph, target_graph, user_callback,
      std::bind(PruneCallbackEmpty<QueryVertexHandle, 
                                  TargetVertexHandle>,
                std::placeholders::_1),
      std::bind(UpdateCandidateCallbackEmpty<QueryVertexHandle, 
                                            TargetVertexHandle>,
                std::placeholders::_1),
      query_limit_time);
}
}  // namespace _dp_iso

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchCallback>
inline int DPISO(QueryGraph &query_graph, 
                TargetGraph &target_graph,
                 MatchCallback user_callback, double query_limit_time = 1200) {
  return _dp_iso::DPISO_Recursive<match_semantics>(
      query_graph, target_graph, user_callback, query_limit_time);
}
template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          class QueryGraph, class TargetGraph>
inline int DPISO(QueryGraph &query_graph, 
                TargetGraph &target_graph,
                 int max_result, double query_limit_time = 1200) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  return DPISO<match_semantics>(
      query_graph, target_graph,
      std::bind(_dp_iso::MatchCallbackLimit<QueryVertexHandle, 
                                           TargetVertexHandle>,
                std::placeholders::_1, &max_result),
      query_limit_time);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class ResultContainer>
inline int DPISO(QueryGraph &query_graph, 
                TargetGraph &target_graph,
                 int max_result, ResultContainer &match_result,
                 double query_limit_time = 1200) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  match_result.clear();
  return DPISO<match_semantics>(
      query_graph, target_graph,
      std::bind(
          _dp_iso::MatchCallbackSaveResult<QueryVertexHandle,
                                          TargetVertexHandle,
                                           ResultContainer>,
          std::placeholders::_1, &max_result, &match_result),
      query_limit_time);
}
template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          class TargetGraph>
inline int DPISO(
     QueryGraph &query_graph, 
    TargetGraph &target_graph,
    typename VertexHandle< QueryGraph>::type cal_supp_vertex_handle,
    std::vector<typename VertexHandle<TargetGraph>::type> &possible_supp,
    std::vector<typename VertexHandle<TargetGraph>::type> &supp,
    double single_query_limit_time = 100) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  std::map<QueryVertexHandle, std::vector<TargetVertexHandle>> candidate_set;
  if (!_dp_iso::InitCandidateSet<match_semantics>(query_graph, target_graph,
                                                  candidate_set)) {
    return 0;
  }
  if (!_dp_iso::UpdateCandidateCallback<QueryVertexHandle, TargetVertexHandle>(
          candidate_set, &cal_supp_vertex_handle, possible_supp)) {
    return 0;
  }
  if (!_dp_iso::RefineCandidateSet(query_graph, target_graph, candidate_set)) {
    return 0;
  }

  for (auto &target_handle :
       candidate_set.find(cal_supp_vertex_handle)->second) {
    time_t begin = clock();
    int max_result = 1;
    size_t result_count = 0;
    std::map<QueryVertexHandle, TargetVertexHandle> match_state;
    std::map<QueryVertexHandle, std::vector<TargetVertexHandle>> temp_candidate_set =
        candidate_set;
    auto prune_callback =
        std::bind(_dp_iso::PruneCallbackEmpty<QueryVertexHandle, TargetVertexHandle>,
                  std::placeholders::_1);
    auto user_callback =
        std::bind(_dp_iso::MatchCallbackLimit<QueryVertexHandle, TargetVertexHandle>,
                  std::placeholders::_1, &max_result);
    std::set<TargetVertexHandle> target_matched;
    _dp_iso::UpdateState(cal_supp_vertex_handle, target_handle, match_state,
                         target_matched);
    auto t_begin = clock();
    _dp_iso::UpdateCandidateSet<QueryGraph, TargetGraph>(
        cal_supp_vertex_handle, target_handle, temp_candidate_set, match_state,
        target_matched);
    auto t_end = clock();
    if (query_graph.CountEdge() >= _dp_iso::large_query_edge) {
      std::map<QueryVertexHandle, std::vector<QueryVertexHandle>> parent;
      _dp_iso::UpdateParent(match_state, cal_supp_vertex_handle, parent);
      std::vector<QueryVertexHandle> fail_set;

      _dp_iso::_DPISO<match_semantics, QueryGraph, TargetGraph>(
          temp_candidate_set, match_state, target_matched, parent, fail_set,
          result_count, user_callback, prune_callback, clock(),
          single_query_limit_time);
    } else {
      _dp_iso::_DPISO<match_semantics, QueryGraph, TargetGraph>(
          temp_candidate_set, match_state, target_matched, result_count,
          user_callback, prune_callback, clock(), single_query_limit_time);
    }

    if (max_result == 0) {
      supp.emplace_back(target_handle);
    }
    time_t end = clock();
  }
  return static_cast<int>(supp.size());
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchCallback,
          class PruneCallBack, class UpdateInitCandidateCallback>
inline int DPISO(QueryGraph &query_graph, 
                TargetGraph &target_graph,
                 MatchCallback match_callback, 
                 PruneCallBack prune_callback,
                 UpdateInitCandidateCallback update_initcandidate_callback,
                 double query_limit_time = 1200) {
  return _dp_iso::DPISO_Recursive<match_semantics>(
      query_graph, target_graph, match_callback, prune_callback,
      update_initcandidate_callback, query_limit_time);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchCallback,
          class PruneCallback>
inline int DPISO(
     QueryGraph &query_graph, 
    TargetGraph &target_graph,
    std::map<typename VertexHandle< QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    std::map<typename VertexHandle< QueryGraph>::type,
             typename VertexHandle<TargetGraph>::type> &match_state,
    MatchCallback user_callback, PruneCallback prune_callback,
    double query_limit_time = 1200) {
  return _dp_iso::DPISO_Recursive<match_semantics>(
      query_graph, target_graph, candidate_set, match_state, user_callback,
      prune_callback, query_limit_time);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchCallback,
          class PruneCallback>
inline int DPISO(
    QueryGraph &query_graph, 
   TargetGraph &target_graph,
    std::map<typename VertexHandle< QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    MatchCallback user_callback, 
    PruneCallback prune_callback,
    double query_limit_time = 1200) {
  return _dp_iso::DPISO_Recursive<match_semantics>(
      query_limit_time, target_graph, candidate_set, user_callback,
      prune_callback, query_limit_time);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchCallback,
          class PruneCallback, 
          class UpdateCandidateCallback>
inline int DPISO(QueryGraph &query_graph, 
                TargetGraph &target_graph,
                 std::map<typename VertexHandle< QueryGraph>::type,
                          typename VertexHandle<TargetGraph>::type> &match_state,
                 MatchCallback user_callback, 
                 PruneCallback prune_callback,
                 UpdateCandidateCallback update_candidate_callback,
                 double query_limit_time = 1200) {
  return _dp_iso::DPISO_Recursive<match_semantics>(
      query_graph, target_graph, match_state, user_callback, prune_callback,
      update_candidate_callback, query_limit_time);
}

template <class CandidateSetContainer, class Pivot>
inline bool SuppUpdateCallBack(CandidateSetContainer &candidate_set,
                               Pivot &supp_list) {
  for (auto it = candidate_set.begin(); it != candidate_set.end();) {
    if (std::find(std::begin(supp_list), std::end(supp_list), it->first) ==
        std::end(supp_list)) {
      it = candidate_set.erase(it);
    } else {
      it++;
    }
  }
  return true;
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchCallback,
          class SuppContainer>
inline int DPISO(QueryGraph &query_graph, 
                TargetGraph &target_graph,
                 SuppContainer &supp_list, 
                 MatchCallback user_callback,
                 double single_query_time = 100) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  using MatchMap = std::map<QueryVertexHandle, 
                           TargetVertexHandle>;
  using MatchResult = std::vector<MatchMap>;
  using CandidateSetContainerType =
      std::map<QueryVertexHandle, 
               std::vector<TargetVertexHandle>>;

  auto update_callback =
      std::bind(SuppUpdateCallBack<CandidateSetContainerType, SuppContainer>,
                std::placeholders::_1, std::ref(supp_list));

  MatchResult supp_match;
  int max_result = -1;
  auto match_callback =
      std::bind(_dp_iso::MatchCallbackSaveResult<QueryVertexHandle,
                                                 TargetVertexHandle, MatchResult>,
                std::placeholders::_1, &max_result, &supp_match);
  auto prune_callback =
      std::bind(_dp_iso::PruneCallbackEmpty<QueryVertexHandle, TargetVertexHandle>,
                std::placeholders::_1);
  DPISO<match_semantics>(query_graph, target_graph, match_callback,
                         prune_callback, update_callback, double(1200));
  MatchResult match_result;
  CandidateSetContainerType candidate_set;
  if (!_dp_iso::InitCandidateSet<match_semantics>(query_graph, target_graph,
                                                  candidate_set)) {
    return 0;
  }
  if (!_dp_iso::RefineCandidateSet(query_graph, target_graph, candidate_set)) {
    return 0;
  }
  for (auto &single_match : supp_match) {
    bool flag = 0;
    for (auto &it : single_match) {
      if (!std::binary_search(std::begin(candidate_set[it.first]),
                              std::end(candidate_set[it.first]), it.second)) {
        flag = 1;
        break;
      }
    }
    if (flag) continue;
    int max_result = 1;
    auto match_callback = std::bind(
        _dp_iso::MatchCallbackSaveResult<QueryVertexHandle, TargetVertexHandle,
                                         MatchResult>,
        std::placeholders::_1, &max_result, &match_result);
    auto temp_candidate_set = candidate_set;
    DPISO<match_semantics>(query_graph, target_graph, temp_candidate_set,
                           single_match, match_callback, prune_callback,
                           single_query_time);
  }
  for (auto &it : match_result) {
    user_callback(it);
  }
  return static_cast<int>(match_result.size());
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class MatchSet>
inline int DPISO_UsingMatchSet(QueryGraph &query_graph,
                              TargetGraph &target_graph,
                               MatchSet &match_set) {
  using Match = typename MatchSet::MatchType;
  auto user_callback = [&match_set](auto &match_state) {
    Match match;
    for (auto &single_match : match_state) {
      match.AddMap(single_match.first, single_match.second);
    }
    match_set.AddMatch(match);
    return true;
  };
  return DPISO<match_semantics>(query_graph, target_graph, user_callback,
                                (double)-1);
}

template <enum MatchSemantics match_semantics = MatchSemantics::kIsomorphism,
          typename QueryGraph, 
          typename TargetGraph, 
          class Match, 
          class MatchSet>
inline int DPISO_UsingPatricalMatchAndMatchSet(QueryGraph &query_graph,
                                              TargetGraph &target_graph,
                                              Match &partical_match,
                                               MatchSet &match_set) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  using MatchMap = std::map<QueryVertexHandle, 
                           TargetVertexHandle>;
  using MatchContainer = std::vector<MatchMap>;
  using CandidateSetContainer = std::map<QueryVertexHandle, 
                            std::vector<TargetVertexHandle>>;
  CandidateSetContainer candidate_set;
  if (!_dp_iso::InitCandidateSet<match_semantics>(query_graph, target_graph,
                                                  candidate_set)) {
    return 0;
  }
  if (!_dp_iso::RefineCandidateSet(query_graph, target_graph, candidate_set)) {
    return 0;
  }
  MatchMap match_state;
  MatchContainer match_result;
  std::set<TargetVertexHandle> target_matched;
  for (auto vertex_it = query_graph.VertexBegin(); !vertex_it.IsDone();
       vertex_it++) {
    QueryVertexHandle vertex_handle = vertex_it;
    if (partical_match.HasMap(vertex_handle)) {
      TargetVertexHandle match_vertex_handle =
          partical_match.MapTo(vertex_handle);
      match_state.insert(std::make_pair(vertex_handle, match_vertex_handle));
      target_matched.insert(match_vertex_handle);
    }
  }
  int max_result = -1;
  size_t result_count = 0;
  auto user_callback = std::bind(
      _dp_iso::MatchCallbackSaveResult<QueryVertexHandle,
                                      TargetVertexHandle, MatchContainer>,
      std::placeholders::_1, &max_result, &match_result);
  if (query_graph.CountEdge() < _dp_iso::large_query_edge) {
    _dp_iso::_DPISO<match_semantics, QueryGraph, TargetGraph>(
        candidate_set, match_state, target_matched, result_count, user_callback,
        [](auto &match_state) { return false; }, clock(), -1);
  } else {
    using FailSetContainer = std::vector<QueryVertexHandle>;
    using ParentContainer =
        std::map<QueryVertexHandle, std::vector<QueryVertexHandle>>;
    ParentContainer parent;
    FailSetContainer fail_set;
    for (auto &single_match : match_state) {
      _dp_iso::UpdateParent(match_state, single_match.first, parent);
    }
    _dp_iso::_DPISO<match_semantics, QueryGraph, TargetGraph>(
        candidate_set, match_state, target_matched, parent, fail_set,
        result_count, user_callback, [](auto &match_state) { return false; },
        clock(), -1.0);
  }

  for (auto &single_match : match_result) {
    Match match;
    for (auto &match_pair : single_match) {
      match.AddMap(match_pair.first, match_pair.second);
    }
    match_set.AddMatch(match);
  }
  return result_count;
}
}  // namespace GUNDAM
#endif
