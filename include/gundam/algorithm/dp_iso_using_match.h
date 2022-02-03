#ifndef _GUNDAM_ALGORITHM_DPISO_USING_MATCH_H
#define _GUNDAM_ALGORITHM_DPISO_USING_MATCH_H
#include <omp.h>

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
#include <limits>

#include "assert.h"
#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/edge_label.h"
#include "gundam/type_getter/vertex_handle.h"
#include "match_helper.h"
#include "match_semantics.h"

#include "gundam/type_getter/graph_parameter_getter.h"

#include "gundam/match/match.h"
#include "gundam/algorithm/bfs.h"

namespace GUNDAM {

namespace _dp_iso_using_match {
enum EdgeState { kIn, kOut };
// if query.CountEdge()>=large_query_edge, using fail set
// small query not using fail set
constexpr size_t large_query_edge = 4;
constexpr size_t adj_vertex_limit = 200000;

template <typename GraphType>
size_t CountInVertex(const typename VertexHandle<GraphType>::type &vertex_handle,
                     const typename    EdgeLabel<GraphType>::type &edge_label) {
  if constexpr (GraphParameter<GraphType>::vertex_level_edge_label_index) {
    return vertex_handle->CountInVertex(edge_label);
  } else {
    std::set<typename VertexHandle<GraphType>::type> vertex_set;
    for (auto edge_it = vertex_handle->InEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (edge_it->label() != edge_label) {
        continue;
      }
      vertex_set.emplace(edge_it->src_handle());
    }
    return vertex_set.size();
  }
  return 0;
}

template <typename GraphType>
size_t CountOutVertex(const typename VertexHandle<GraphType>::type &vertex_handle,
                      const typename    EdgeLabel<GraphType>::type &edge_label) {
  if constexpr (GraphParameter<GraphType>::vertex_level_edge_label_index) {
    return vertex_handle->CountOutVertex(edge_label);
  } else {
    std::set<typename VertexHandle<GraphType>::type> vertex_set;
    for (auto edge_it = vertex_handle->OutEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (edge_it->label() != edge_label) {
        continue;
      }
      vertex_set.emplace(edge_it->dst_handle());
    }
    return vertex_set.size();
  }
  return 0;
}

template <typename GraphType>
size_t CountInEdge(const typename VertexHandle<GraphType>::type &vertex_handle,
                   const typename    EdgeLabel<GraphType>::type &edge_label) {
  if constexpr (GraphParameter<GraphType>::vertex_level_edge_label_index) {
    return vertex_handle->CountInEdge(edge_label);
  } else {
    size_t counter = 0;
    for (auto edge_it = vertex_handle->InEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (edge_it->label() == edge_label) {
        counter++;
      }
    }
    return counter;
  }
  return 0;
}

template <typename GraphType>
size_t CountOutEdge(const typename VertexHandle<GraphType>::type &vertex_handle,
                    const typename    EdgeLabel<GraphType>::type &edge_label) {
  if constexpr (GraphParameter<GraphType>::vertex_level_edge_label_index) {
    return vertex_handle->CountOutEdge(edge_label);
  } else {
    size_t counter = 0;
    for (auto edge_it = vertex_handle->OutEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (edge_it->label() == edge_label) {
        counter++;
      }
    }
    return counter;
  }
  return 0;
}

template <typename GraphType>
size_t CountInEdge(const typename VertexHandle<GraphType>::type &vertex_handle,
                   const typename    EdgeLabel<GraphType>::type &edge_label,
                   const typename VertexHandle<GraphType>::type &src_vertex_handle) {
  if constexpr (GraphParameter<GraphType>::vertex_level_edge_label_index) {
    return vertex_handle->CountInEdge(edge_label, src_vertex_handle);
  } else {
    size_t counter = 0;
    for (auto edge_it = vertex_handle->InEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (edge_it->label() == edge_label &&
          edge_it->src_handle() == src_vertex_handle) {
        counter++;
      }
    }
    return counter;
  }
  return 0;
}

template <typename GraphType>
size_t CountOutEdge(const typename VertexHandle<GraphType>::type &vertex_handle,
                    const typename    EdgeLabel<GraphType>::type &edge_label,
                    const typename VertexHandle<GraphType>::type &dst_vertex_handle) {
  if constexpr (GraphParameter<GraphType>::vertex_level_edge_label_index) {
    return vertex_handle->CountOutEdge(edge_label, dst_vertex_handle);
  } else {
    size_t counter = 0;
    for (auto edge_it = vertex_handle->OutEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (edge_it->label() == edge_label 
       && edge_it->dst_handle() == dst_vertex_handle) {
        counter++;
      }
    }
    return counter;
  }
  return 0;
}

template <bool is_out_direction,
          typename GraphType>
inline auto EdgeBegin(const typename VertexHandle<GraphType>::type& vertex_handle) {
  if constexpr (is_out_direction) {
    return vertex_handle->OutEdgeBegin();
  }
  else {
    return vertex_handle->InEdgeBegin();
  }
}

template <typename  QueryVertexHandle, 
          typename TargetVertexHandle>
inline bool DegreeFiltering(const  QueryVertexHandle  &query_vertex_handle,
                            const TargetVertexHandle &target_vertex_handle) {
  if (query_vertex_handle->CountInEdge() 
   > target_vertex_handle->CountInEdge())
    return false;
  if (query_vertex_handle->CountOutEdge() 
   > target_vertex_handle->CountOutEdge())
    return false;
  if (query_vertex_handle->CountInVertex() 
   > target_vertex_handle->CountInVertex())
    return false;
  if (query_vertex_handle->CountOutVertex() 
   > target_vertex_handle->CountOutVertex())
    return false;
  return true;
}

template <enum EdgeState edge_state, 
          typename  QueryGraph,
          typename TargetGraph>
inline bool NeighborLabelFrequencyFilteringSingleDirection(
    const typename VertexHandle< QueryGraph>::type  &query_vertex_handle,
    const typename VertexHandle<TargetGraph>::type &target_vertex_handle) {
  std::set<typename EdgeLabel<QueryGraph>::type> used_edge_label;
  for (auto edge_label_it = EdgeBegin<edge_state == EdgeState::kOut, QueryGraph>(
                                      query_vertex_handle);
           !edge_label_it.IsDone(); 
            edge_label_it++) {
    auto[used_edge_label_it,
         used_edge_label_ret] 
       = used_edge_label.emplace(edge_label_it->label());
    if (!used_edge_label_ret) {
      // add fail, this edge label has already been added
      // in used_edge_label
      continue;
    }
    auto  query_count = (edge_state == EdgeState::kIn
                            ? _dp_iso_using_match:: CountInVertex<QueryGraph>(
                                  query_vertex_handle, edge_label_it->label())
                            : _dp_iso_using_match::CountOutVertex<QueryGraph>(
                                  query_vertex_handle, edge_label_it->label()));
    auto target_count = (edge_state == EdgeState::kIn
                            ? _dp_iso_using_match:: CountInVertex<TargetGraph>(
                                 target_vertex_handle, edge_label_it->label())
                            : _dp_iso_using_match::CountOutVertex<TargetGraph>(
                                 target_vertex_handle, edge_label_it->label()));
    assert(query_count >= 0 && target_count >= 0);
    if (query_count > target_count) {
      return false;
    }
     query_count = (edge_state == EdgeState::kIn
                       ? _dp_iso_using_match:: CountInEdge< QueryGraph>(
                             query_vertex_handle, edge_label_it->label())
                       : _dp_iso_using_match::CountOutEdge< QueryGraph>(
                             query_vertex_handle, edge_label_it->label()));
    target_count = (edge_state == EdgeState::kIn
                       ? _dp_iso_using_match:: CountInEdge<TargetGraph>(
                            target_vertex_handle, edge_label_it->label())
                       : _dp_iso_using_match::CountOutEdge<TargetGraph>(
                            target_vertex_handle, edge_label_it->label()));
    assert(query_count >= 0 && target_count >= 0);
    if (query_count > target_count) {
      // can be filted
      return false;
    }
  }
  return true;
}

template <typename  QueryGraph, 
          typename TargetGraph>
inline bool NeighborLabelFrequencyFiltering(
    const typename VertexHandle< QueryGraph>::type  &query_vertex_handle,
    const typename VertexHandle<TargetGraph>::type &target_vertex_handle) {
  if (!NeighborLabelFrequencyFilteringSingleDirection<EdgeState:: kIn,
                                                      QueryGraph, TargetGraph>(
           query_vertex_handle, 
          target_vertex_handle)) {
    return false;
  }
  if (!NeighborLabelFrequencyFilteringSingleDirection<EdgeState::kOut,
                                                      QueryGraph, TargetGraph>(
           query_vertex_handle, 
          target_vertex_handle)) {
    return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics, 
          typename  QueryGraph,
          typename TargetGraph>
inline bool InitCandidateSet(
     QueryGraph  &query_graph, 
    TargetGraph &target_graph,
    std::map<typename VertexHandle<QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  using CandidateContainer 
               = std::vector<typename VertexHandle<TargetGraph>::type>;
  candidate_set.clear();
  for (auto query_vertex_iter = query_graph.VertexBegin();
           !query_vertex_iter.IsDone();
            query_vertex_iter++) {
    QueryVertexHandle query_vertex_handle 
                    = query_vertex_iter;
    CandidateContainer query_vertex_candidate;
    if constexpr (GraphParameter<TargetGraph>::graph_level_vertex_label_index) {
      // TargetGraph has graph.VertexBegin(vertex_label) method
      for (auto target_vertex_iter = target_graph.VertexBegin(query_vertex_handle->label());
               !target_vertex_iter.IsDone(); 
                target_vertex_iter++) {
        TargetVertexHandle target_vertex_handle = target_vertex_iter;
        if (!DegreeFiltering(query_vertex_handle, target_vertex_handle)) {
          continue;
        }
        if (!NeighborLabelFrequencyFiltering<QueryGraph, TargetGraph>(
                query_vertex_handle, target_vertex_handle)) {
          continue;
        }
        query_vertex_candidate.emplace_back(target_vertex_handle);
      }
    } else {
      // TargetGraph does not have graph.VertexBegin(vertex_label) method
      for (auto target_vertex_iter = target_graph.VertexBegin();
               !target_vertex_iter.IsDone(); 
                target_vertex_iter++) {
        if (target_vertex_iter->label() != query_vertex_handle->label()) {
          continue;
        }
        TargetVertexHandle target_vertex_handle = target_vertex_iter;
        if (!DegreeFiltering(query_vertex_handle, 
                            target_vertex_handle)) {
          continue;
        }
        if (!NeighborLabelFrequencyFiltering<QueryGraph, TargetGraph>(
                query_vertex_handle, 
               target_vertex_handle)) {
          continue;
        }
        query_vertex_candidate.emplace_back(target_vertex_handle);
      }
    }
    if (query_vertex_candidate.empty()) {
      return false;
    }
    assert(candidate_set.find(query_vertex_handle) 
        == candidate_set.end());
    candidate_set.emplace(query_vertex_handle,
                std::move(query_vertex_candidate));
  }
  for (auto &[query_vertex_handle,
              query_vertex_candidate] : candidate_set) {
    std::stable_sort(query_vertex_candidate.begin(),
                     query_vertex_candidate.end());
  }
  return true;
}

template <bool use_dag_order,
          enum EdgeState edge_state, 
          typename  QueryGraph, 
          typename TargetGraph>
inline void GetAdjNotMatchedVertexOneDirection(
                const typename VertexHandle< QueryGraph>::type &query_vertex_handle,
    const std:: map  <typename VertexHandle< QueryGraph>::type, 
          std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    const Match<QueryGraph, TargetGraph> &match_state,
          std:: set  <typename VertexHandle<QueryGraph>::type> &next_query_set,
    const std:: map  <typename VertexHandle<QueryGraph>::type,
          std::vector<typename VertexHandle<QueryGraph>::type>>& parent) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  for (auto edge_iter = (edge_state == EdgeState::kIn ? 
                          query_vertex_handle-> InEdgeBegin()
                        : query_vertex_handle->OutEdgeBegin());
           !edge_iter.IsDone(); 
            edge_iter++) {
    QueryVertexHandle query_opp_vertex_handle = edge_state == EdgeState::kIn
                                                    ? edge_iter->src_handle()
                                                    : edge_iter->dst_handle();
    if (!match_state.HasMap(query_opp_vertex_handle)
      && candidate_set.find(query_opp_vertex_handle)
      != candidate_set.end()) {
      if constexpr (use_dag_order) {
        if (!parent.empty()) {
          // "a query vertex u should be processed after all its parents in qD have
          //  been processed, which is ensured by a DAG ordering"
          auto parent_it = parent.find(query_opp_vertex_handle);
          assert(parent_it != parent.end());
          bool all_parent_matched = true;
          for (const auto& parent_vertex_handle 
                         : parent_it->second) {
            if (parent_vertex_handle == query_opp_vertex_handle) {
              continue;
            }
            if (match_state.HasMap(parent_vertex_handle)) {
              // this parent has been matched
              continue;
            }
            all_parent_matched = false;
            break;
          }
          if (!all_parent_matched) {
            continue;
          }
        }
      }
      next_query_set.emplace(query_opp_vertex_handle);
    }
  }
  return;
}

template <bool use_dag_order,
          typename  QueryGraph, 
          typename TargetGraph>
inline void GetAdjNotMatchedVertex(
    const typename VertexHandle<QueryGraph>::type& query_vertex_handle,
    const std:: map  <typename VertexHandle< QueryGraph>::type, 
          std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    const Match<QueryGraph, TargetGraph> &match_state,
          std:: set  <typename VertexHandle<QueryGraph>::type> &next_query_set,
    const std:: map  <typename VertexHandle<QueryGraph>::type,
          std::vector<typename VertexHandle<QueryGraph>::type>>& parent) {
  GetAdjNotMatchedVertexOneDirection<use_dag_order, EdgeState::kOut>(
      query_vertex_handle, candidate_set, match_state, next_query_set, parent);
  GetAdjNotMatchedVertexOneDirection<use_dag_order, EdgeState::kIn>(
      query_vertex_handle, candidate_set, match_state, next_query_set, parent);
}

// to find a un-matched vertex to match
template <bool use_dag_order,
          typename  QueryGraph, 
          typename TargetGraph>
inline typename VertexHandle<QueryGraph>::type NextMatchVertex(
    const std:: map  <typename VertexHandle< QueryGraph>::type, 
          std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    const Match<QueryGraph, TargetGraph> &match_state,
    const std:: map  <typename VertexHandle<QueryGraph>::type,
          std::vector<typename VertexHandle<QueryGraph>::type>>& parent) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  std::set<QueryVertexHandle> next_query_set;
  // first to find whether there are un-matched vertexes
  // adjacent to the match vertexes
  for (auto map_it = match_state.MapBegin();
           !map_it.IsDone();
            map_it++) {
    GetAdjNotMatchedVertex<use_dag_order>(map_it->src_handle(), 
                                          candidate_set, 
                                          match_state,
                                          next_query_set,
                                          parent);
  }
  if (next_query_set.empty()) {
    // all vertexes connected to the matched vertexes are 
    // matched, find from the rest candidated set
    // the pattern should be not connected
    for (const auto &[query_vertex_handle, 
                      query_vertex_candidate] : candidate_set) {
      if (!match_state.HasMap(query_vertex_handle)) {
        next_query_set.emplace(query_vertex_handle);
      }
    }
  }

  if (next_query_set.empty()) {
    // all vertexes have already been matched
    QueryVertexHandle res = QueryVertexHandle();
    assert(!res);
    return res;
  }

  // there still vertexes have not been matched
  // find the one with the minimal candidate set to 
  // match next
  QueryVertexHandle res = QueryVertexHandle();
  assert(!res);
  size_t min = std::numeric_limits<size_t>::max();
  for (const auto &query_vertex_handle : next_query_set) {
    assert(candidate_set.find(query_vertex_handle) 
        != candidate_set.end());
    size_t candidate_count 
         = candidate_set.find(query_vertex_handle)->second.size();
    if (candidate_count < min) {
      res = query_vertex_handle;
      min = candidate_count;
    }
  }
  assert(min >= 0 && min < std::numeric_limits<size_t>::max());
  assert(res);
  return res;
}

// add a new query-target vertexes pair into the match_state
// enumerate all its adjacent edges to check whether this
// vertexes pair can be added legally
template <enum EdgeState edge_state, 
          typename  QueryGraph, 
          typename TargetGraph>
inline bool JoinableCheck(
      typename VertexHandle< QueryGraph>::type  query_vertex_handle,
      typename VertexHandle<TargetGraph>::type target_vertex_handle,
    const Match<QueryGraph, TargetGraph> &match_state) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using  QueryEdgeHandle = typename EdgeHandle< QueryGraph>::type;
  using TargetEdgeHandle = typename EdgeHandle<TargetGraph>::type;

  std::set<TargetEdgeHandle> used_edge;
  for (auto query_edge_iter = ((edge_state == EdgeState::kIn)
                                   ? query_vertex_handle-> InEdgeBegin()
                                   : query_vertex_handle->OutEdgeBegin());
           !query_edge_iter.IsDone(); 
            query_edge_iter++) {
    QueryEdgeHandle query_edge_handle(query_edge_iter);
    QueryVertexHandle query_opp_vertex_handle =
        (edge_state == EdgeState::kIn) ? query_edge_handle->src_handle()
                                       : query_edge_handle->dst_handle();

    TargetVertexHandle query_opp_match_vertex_handle 
         = match_state.MapTo(query_opp_vertex_handle);
    if (!query_opp_match_vertex_handle) {
      // the query_opp_vertex_handle is not mapped in match_state
      continue;
    }

    // to find whether this is such a kind of edge in the target graph
    bool found_edge_in_target_graph = false;
    if constexpr (GraphParameter<TargetGraph>::vertex_level_edge_label_index) {
      // vertex has vertex_handle->In/Out EdgeBegin(edge_label, vertex_handle)
      // method
      for (auto target_edge_iter = ((edge_state == EdgeState::kIn)
                                        ? target_vertex_handle->InEdgeBegin(
                                              query_edge_handle->label(),
                                              query_opp_match_vertex_handle)
                                        : target_vertex_handle->OutEdgeBegin(
                                              query_edge_handle->label(),
                                              query_opp_match_vertex_handle));
               !target_edge_iter.IsDone(); 
                target_edge_iter++) {
        TargetEdgeHandle target_edge_handle = target_edge_iter;
        if (used_edge.find(target_edge_handle)
         != used_edge.end()) {
          // this edge has been used
          continue;
        }
        found_edge_in_target_graph = true;
        auto [ used_edge_it,
               used_edge_ret ] = used_edge.emplace(target_edge_handle);
        assert(used_edge_ret);
        break;
      }
    } else {
      // vertex does not have vertex_handle->In/Out EdgeBegin(edge_label,
      // vertex_handle) method
      for (auto target_edge_iter = (edge_state == EdgeState::kIn)
                                       ? target_vertex_handle-> InEdgeBegin()
                                       : target_vertex_handle->OutEdgeBegin();
               !target_edge_iter.IsDone(); 
                target_edge_iter++) {
                  
        if (target_edge_iter->label() != query_edge_handle->label()) {
          continue;
        }

        auto opp_vertex_handle = (edge_state == EdgeState::kIn)
                         ? target_edge_iter->src_handle()
                         : target_edge_iter->dst_handle();

        if (query_opp_match_vertex_handle 
                     != opp_vertex_handle) {
          continue;
        }
        if (used_edge.find(target_edge_iter)
         != used_edge.end()) {
          continue;
        }
        found_edge_in_target_graph = true;
        auto [ used_edge_it,
               used_edge_ret ] = used_edge.emplace(target_edge_iter);
        assert(used_edge_ret);
        break;
      }
    }
    if (!found_edge_in_target_graph) {
      return false;
    }
  }
  return true;
}

template <enum MatchSemantics match_semantics, 
          typename  QueryGraph,
          typename TargetGraph>
inline bool IsJoinable(
      typename VertexHandle< QueryGraph>::type  query_vertex_handle,
      typename VertexHandle<TargetGraph>::type target_vertex_handle,
    const Match<QueryGraph, TargetGraph> &match_state,
    std::set<typename VertexHandle<TargetGraph>::type> &target_matched) {
  if (match_semantics == MatchSemantics::kIsomorphism 
   && target_matched.find(target_vertex_handle) 
   != target_matched.end()) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kIn>(
          query_vertex_handle, target_vertex_handle, match_state)) {
    return false;
  }
  if (!JoinableCheck<EdgeState::kOut>(
          query_vertex_handle, target_vertex_handle, match_state)) {
    return false;
  }
  return true;
}

template <typename  QueryGraph, 
          typename TargetGraph>
inline void UpdateState(
    typename VertexHandle< QueryGraph>::type  query_vertex_handle,
    typename VertexHandle<TargetGraph>::type target_vertex_handle,
        Match<QueryGraph, TargetGraph> &match_state,
    std::set<typename VertexHandle<TargetGraph>::type> &target_matched) {
  assert(!match_state.HasMap(query_vertex_handle));
  auto ret = match_state.AddMap(query_vertex_handle, 
                               target_vertex_handle);
  // should added successfully
  assert(ret);
  target_matched.emplace(target_vertex_handle);
  return;
}

// the query-target vertex pair has been added into the match_state
// use the just matched target_vertex_handle to prune the candidate_set 
// of the un-matched vertexes adjacent to the query_vertex_handle
template <enum EdgeState edge_state, 
          typename  QueryGraph,
          typename TargetGraph>
inline void UpdateCandidateSetOneDirection(
    typename VertexHandle< QueryGraph>::type  query_vertex_handle,
    typename VertexHandle<TargetGraph>::type target_vertex_handle,
    std::map<typename VertexHandle<QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
    const Match<QueryGraph, TargetGraph> &match_state) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using CandidateContainerType =
      std::vector<typename VertexHandle<TargetGraph>::type>;

  std::set<typename EdgeLabel<QueryGraph>::type> used_edge_label;

  for (auto edge_label_it = ((edge_state == EdgeState::kIn)
                            ? query_vertex_handle-> InEdgeBegin()
                            : query_vertex_handle->OutEdgeBegin());
           !edge_label_it.IsDone(); 
            edge_label_it++) {
    QueryVertexHandle temp_vertex_handle = (edge_state == EdgeState::kIn)
                                          ? edge_label_it->src_handle()
                                          : edge_label_it->dst_handle();
    if (match_state.HasMap(temp_vertex_handle)) {
      // this vertex has already been mapped
      // it would be unnecessary to update its candidate set
      // since the candidate set is only useful for un-match
      // vertexes
      continue;
    }
    if (candidate_set.count(temp_vertex_handle) == 0) {
      // has no candidate set for this vertex (?)
      continue;
    }
    //枚举label
    if (used_edge_label.find(edge_label_it->label())
     != used_edge_label.end()) {
      // has already visited this link of edge label
      continue;
    }
    auto [ used_edge_label_it,
           used_edge_label_ret ] 
         = used_edge_label.emplace(edge_label_it->label());
    assert(used_edge_label_ret); // should have been added successfully

    // there is an input/output edge with label "edge_label_it->label()"
    // points from/to a vertex that have not been added in the match_state
    // use this edge label for filting

    size_t adj_count = (edge_state == EdgeState::kIn)
                           ? _dp_iso_using_match:: CountInVertex<TargetGraph>(
                                 target_vertex_handle, edge_label_it->label())
                           : _dp_iso_using_match::CountOutVertex<TargetGraph>(
                                 target_vertex_handle, edge_label_it->label());
    if (adj_count > adj_vertex_limit) {
      // the target_vertex_handle is used as a filter,
      // if it has too many adjacent vertex
      // then it would be unlikely for it to filt
      // out much vertex in candidate set
      continue;
    }

    std::map<typename TargetGraph::VertexType::LabelType,
          std::vector<TargetVertexHandle>> temp_adj_vertex;

    if constexpr (GraphParameter<TargetGraph>::vertex_level_edge_label_index) {
      // vertex has vertex_handle->In/Out VertexBegin(edge_label) method
      for (auto vertex_it = ((edge_state == EdgeState::kIn)
                    ? target_vertex_handle-> InVertexBegin(edge_label_it->label())
                    : target_vertex_handle->OutVertexBegin(edge_label_it->label()));
               !vertex_it.IsDone(); vertex_it++) {
        TargetVertexHandle temp_target_handle = vertex_it;
        auto temp_target_handle_it 
           = temp_adj_vertex.find(temp_target_handle->label());
        if (temp_target_handle_it != temp_adj_vertex.end()) {
          // there has already have this vertex label
          temp_target_handle_it->second.emplace_back(temp_target_handle);
          continue;
        }
        temp_adj_vertex.emplace_hint(temp_target_handle_it,
                                     temp_target_handle->label(),
              CandidateContainerType{temp_target_handle});
      }
    } else {
      // vertex does not have vertex_handle->In/Out VertexBegin(edge_label) method
      for (auto edge_it = ((edge_state == EdgeState::kIn)
                               ? target_vertex_handle-> InEdgeBegin()
                               : target_vertex_handle->OutEdgeBegin());
               !edge_it.IsDone(); edge_it++) {
        if (edge_it->label() != edge_label_it->label()) {
          continue;
        }
        TargetVertexHandle temp_target_handle =
            (edge_state == EdgeState::kIn) ? edge_it->src_handle()   //  in edge
                                           : edge_it->dst_handle();  // out edge
        // there might be two edges with same label point to the same vertex
        // but would be uniqued later
        auto temp_target_handle_it 
           = temp_adj_vertex.find(temp_target_handle->label());
        if (temp_target_handle_it != temp_adj_vertex.end()) {
          // there has already have this vertex label
          temp_target_handle_it->second.emplace_back(temp_target_handle);
          continue;
        }
        temp_adj_vertex.emplace_hint(temp_target_handle_it,
                                     temp_target_handle->label(),
              CandidateContainerType{temp_target_handle});
      }
    }
    for (auto &[vertex_label, 
                vertex_label_candidate] : temp_adj_vertex) {
      std::sort(vertex_label_candidate.begin(), 
                vertex_label_candidate.end());
      auto erase_it = std::unique(vertex_label_candidate.begin(),
                                  vertex_label_candidate.end());
      vertex_label_candidate.erase(erase_it, vertex_label_candidate.end());
      assert(!vertex_label_candidate.empty());
    }
    std::set<QueryVertexHandle> used_vertex;
    // to find all unmacted vertexes adjacent to the query_vertex_handle
    // update their candidate set
    for (auto edge_it = ((edge_state == EdgeState::kIn)
                        ? query_vertex_handle-> InEdgeBegin()
                        : query_vertex_handle->OutEdgeBegin());
             !edge_it.IsDone(); 
              edge_it++) {
      //枚举Vertex connected by the denoted edge label
      if (edge_it->label() != edge_label_it->label()) {
        continue;
      }
      QueryVertexHandle temp_vertex_handle = (edge_state == EdgeState::kIn)
                                            ? edge_it->src_handle()
                                            : edge_it->dst_handle();
      if (match_state.HasMap(temp_vertex_handle)) {
        // this vertex has been matched, its candidate set 
        // does not need to be updated
        continue;
      }
      if (used_vertex.find(temp_vertex_handle)
       != used_vertex.end()) {
        // this vertex has already been used
        continue;
      }
      auto [ used_vertex_it,
             used_vertex_ret ] 
           = used_vertex.emplace(temp_vertex_handle);
      assert(used_vertex_ret);
      if (candidate_set.find(temp_vertex_handle) 
       == candidate_set.end()) {
        // (?)
        continue;
      }
      if (temp_adj_vertex.find(temp_vertex_handle->label()) 
       == temp_adj_vertex.end()) {
        // intersection result is empty
        candidate_set.find(temp_vertex_handle)->second.clear();
        continue;
      }
      std::vector<TargetVertexHandle> res_candidate;
      std::set_intersection(
          temp_adj_vertex.find(temp_vertex_handle->label())->second.begin(),
          temp_adj_vertex.find(temp_vertex_handle->label())->second.end(),
            candidate_set.find(temp_vertex_handle)->second.begin(),
            candidate_set.find(temp_vertex_handle)->second.end(),
                 std::inserter(res_candidate, 
                               res_candidate.begin()));
      std::swap(candidate_set.find(temp_vertex_handle)->second,
                res_candidate);
    }
  }
  return;
}

template <typename  QueryGraph, 
          typename TargetGraph>
inline void UpdateCandidateSet(
    typename VertexHandle< QueryGraph>::type  query_vertex_handle,
    typename VertexHandle<TargetGraph>::type target_vertex_handle,
    std::map<typename VertexHandle<QueryGraph>::type,
             std::vector<typename VertexHandle<TargetGraph>::type>>
        &candidate_set,
    const Match<QueryGraph, TargetGraph> &match_state) {

  UpdateCandidateSetOneDirection<EdgeState:: kIn>(
       query_vertex_handle, 
      target_vertex_handle, candidate_set, match_state);
  UpdateCandidateSetOneDirection<EdgeState::kOut>(
       query_vertex_handle, 
      target_vertex_handle, candidate_set, match_state);
  return;
}

// build a dag based on the given src vertexes
// only calc the parents of each vertex on the 
// only one of its parent has been matched 
template <typename QueryGraph,
          typename TargetVertexHandle>
inline void BuildDag(             QueryGraph& query_graph,
   std::set<typename VertexHandle<QueryGraph>::type>& src_vertex_set,
   std::map<typename VertexHandle<QueryGraph>::type,
   std::vector<TargetVertexHandle>> candidate_set,
   std::map<typename VertexHandle<QueryGraph>::type, 
std::vector<typename VertexHandle<QueryGraph>::type>> &parent) {
  assert(!src_vertex_set.empty());
  assert(parent.empty());
  
  using QueryVertexHandleType 
  = typename VertexHandle<QueryGraph>::type;

  std::map<typename QueryGraph::VertexCounterType,
           std::vector<QueryVertexHandleType>> grouped_query_vertex_set;

  auto build_dag_callback 
    = [&grouped_query_vertex_set](
             const QueryVertexHandleType& query_vertex_handle,
          typename QueryGraph::VertexCounterType bfx_idx,
          typename QueryGraph::VertexCounterType distance) {

    grouped_query_vertex_set[distance].emplace_back(query_vertex_handle);
    return true;

    // // // query_vertex_handle should not have been visited
    // // assert(visited_vertex_set.find(query_vertex_handle)
    // //     != visited_vertex_set.end());

    // auto [ parent_it,
    //        parent_ret ] 
    //      = parent.emplace(query_vertex_handle, std::vector<QueryVertexHandleType>());
    // // should be added successfully
    // assert(parent_ret);
    // parent_it->second.emplace_back(query_vertex_handle);
    
    // for (auto out_edge_it = query_vertex_handle->OutEdgeBegin();
    //          !out_edge_it.IsDone();
    //           out_edge_it++) {
    //   typename EdgeHandle<QueryGraph>::type edge_handle = out_edge_it;
    //   QueryVertexHandleType opp_vertex_handle = out_edge_it->dst_handle();
    //   if (visited_vertex_set.find(opp_vertex_handle)
    //    == visited_vertex_set.end()) {
    //     // this vertex has not been added into the dag
    //     // would not be added as the parent of 
    //     // query_vertex_handle
    //     continue;
    //   }
    //   parent_it->second.emplace_back(opp_vertex_handle);
    // }
    
    // for (auto in_edge_it = query_vertex_handle->InEdgeBegin();
    //          !in_edge_it.IsDone();
    //           in_edge_it++) {
    //   auto opp_vertex_handle = in_edge_it->src_handle();
    //   if (visited_vertex_set.find(opp_vertex_handle)
    //    == visited_vertex_set.end()) {
    //     // this vertex has not been added into the dag
    //     // would not be added as the parent of 
    //     // query_vertex_handle
    //     continue;
    //   }
    //   parent_it->second.emplace_back(opp_vertex_handle);
    // }

    // auto [ visited_vertex_it,
    //        visited_vertex_ret ] 
    //      = visited_vertex_set.emplace(query_vertex_handle);
    // // may be already added before
    // // assert(visited_vertex_ret);
    // return true;
  };

  Bfs<true>(query_graph, src_vertex_set, build_dag_callback);

  // // by wenzhi
  // std::string query_graph_str;
  // query_graph_str << query_graph;
  // std::cout << "query graph str" << std::endl
  //           <<  query_graph_str  << std::endl;

  // for (auto layer_it  = grouped_query_vertex_set.begin();
  //           layer_it != grouped_query_vertex_set.end();
  //           layer_it++){
  //   const auto& distance = layer_it->first;
  //   auto& vertex_set     = layer_it->second;
  //   std::cout << "distance: " << distance << std::endl;
  //   for (const auto& vertex : vertex_set) {
  //     std::cout << "\t" << vertex->id();
  //   }
  //   std::cout << std::endl;
  // }

  auto comp = [&candidate_set](const QueryVertexHandleType& a, 
                               const QueryVertexHandleType& b) -> bool {
    assert(candidate_set.find(a) != candidate_set.end());
    assert(candidate_set.find(b) != candidate_set.end());
    if (candidate_set[a].size() < candidate_set[b].size()) {
      return true;
    }
    if (candidate_set[a].size() > candidate_set[b].size()) {
      return false;
    }
    return a < b;
  };

  typename std::map<typename QueryGraph::VertexCounterType,
                 std::vector<QueryVertexHandleType>>::const_iterator
    previous_layer_it = grouped_query_vertex_set.end();

  for (auto layer_it  = grouped_query_vertex_set.begin();
            layer_it != grouped_query_vertex_set.end();
            layer_it++) {
    const auto& distance = layer_it->first;
    std::sort(layer_it->second.begin(),
              layer_it->second.end(), comp);
    const auto& vertex_set = layer_it->second;
    for (const auto& vertex_handle : vertex_set) {
      // the set of parent for vertex_handle should not have
      // been added
      // std::cout << "vertex_handle->id(): " << vertex_handle->id()
      //           << " candidate size: " << candidate_set[vertex_handle].size() << std::endl;
      assert(parent.find(vertex_handle) == parent.end());
      auto [ parent_it, parent_ret ] 
           = parent.emplace(vertex_handle, std::vector<QueryVertexHandleType>{vertex_handle});
      // should have been added successfully
      assert(parent_ret);
      auto&  parent_vertex_set = parent_it->second;
      assert(parent_vertex_set.size() == 1
         && *parent_vertex_set.begin() == vertex_handle);
      assert(parent.find(vertex_handle) != parent.end());
    }
    // add edges from previous layer to this layer
    if (distance > 0) {
      // add edge from previous layer only if this
      // vertex is not the root(s) of the DAG
      assert(previous_layer_it != grouped_query_vertex_set.end());
      const auto& previous_layer_vertex_set
                = previous_layer_it->second;
      for (const auto& vertex_handle : vertex_set) {
        // std::cout << "wenzhi: distance: " 
        //           <<          distance << std::endl;
        // std::cout << "wenzhi: vertex_handle->id(): " 
        //           <<          vertex_handle->id() << std::endl;
        // iterate over the vertexes that are connected to
        // vertex_handle to find whether it is in the previous
        // layer
        auto parent_it = parent.find(vertex_handle);
        assert(parent_it != parent.end()); // should found that vertex
        auto& parent_vertex_set = parent_it->second;
        assert(parent_vertex_set.size() == 1
           && *parent_vertex_set.begin() == vertex_handle);

        for (auto out_edge_it = vertex_handle->OutEdgeBegin();
                 !out_edge_it.IsDone();
                  out_edge_it++) {
          auto opp_vertex_handle = out_edge_it->dst_handle();
          // to find whether it is contained in the previous layer
          assert(std::is_sorted(previous_layer_vertex_set.begin(),
                                previous_layer_vertex_set.end(), comp));
          if (!std::binary_search(previous_layer_vertex_set.begin(),
                                  previous_layer_vertex_set.end(),
                                  opp_vertex_handle, comp)) {
            // the opp vertex is not contained in the previous layer 
            continue;
          }
          // the opp vertex is contained in the previous layer,
          // add an edge from opp vertex to vertex_handle in the DAG
          auto opp_vertex_handle_parent_it = parent.find(opp_vertex_handle);
          // should have parent for this vertex
          assert(opp_vertex_handle_parent_it != parent.end());
          auto& opp_vertex_handle_parent 
              = opp_vertex_handle_parent_it->second;
          
          int mid = parent_vertex_set.size(); // Store the end of first sorted range

          //First copy the second sorted range into the destination vector
          std::copy(opp_vertex_handle_parent.begin(), 
                    opp_vertex_handle_parent.end(), 
                    std::back_inserter(parent_vertex_set));

          //Then perform the in place merge on the two sub-sorted ranges.
          std::inplace_merge(parent_vertex_set.begin(), 
                             parent_vertex_set.begin() + mid, 
                             parent_vertex_set.end());

          //Remove duplicate elements from the sorted vector
          parent_vertex_set.erase(std::unique(parent_vertex_set.begin(), 
                                              parent_vertex_set.end()), 
                                              parent_vertex_set.end());
        }

        for (auto in_edge_it = vertex_handle->InEdgeBegin();
                 !in_edge_it.IsDone();
                  in_edge_it++) {
          auto opp_vertex_handle = in_edge_it->src_handle();
          // to find whether it is contained in the previous layer
          assert(std::is_sorted(previous_layer_vertex_set.begin(),
                                previous_layer_vertex_set.end(), comp));
          if (!std::binary_search(previous_layer_vertex_set.begin(),
                                  previous_layer_vertex_set.end(),
                                  opp_vertex_handle, comp)) {
            // the opp vertex is not contained in the previous layer 
            continue;
          }
          // the opp vertex is contained in the previous layer,
          // add an edge from opp vertex to vertex_handle in the DAG
          auto opp_vertex_handle_parent_it = parent.find(opp_vertex_handle);
          // should have parent for this vertex
          assert(opp_vertex_handle_parent_it != parent.end());
          auto& opp_vertex_handle_parent 
              = opp_vertex_handle_parent_it->second;
          
          int mid = parent_vertex_set.size(); // Store the end of first sorted range

          //First copy the second sorted range into the destination vector
          std::copy(opp_vertex_handle_parent.begin(), 
                    opp_vertex_handle_parent.end(), 
                   std::back_inserter(parent_vertex_set));

          //Then perform the in place merge on the two sub-sorted ranges.
          std::inplace_merge(parent_vertex_set.begin(), 
                             parent_vertex_set.begin() + mid, 
                             parent_vertex_set.end());

          //Remove duplicate elements from the sorted vector
          parent_vertex_set.erase(std::unique(parent_vertex_set.begin(), 
                                              parent_vertex_set.end()), 
                                              parent_vertex_set.end());
        }
      }
    }
    previous_layer_it = layer_it;
    // #############################################
    // ##  build the edge between the same laeyr  ##
    // ##  from vertex with large candidate set   ## 
    // ##  to vertex with samller candidate set   ##
    // #############################################

    std::vector<QueryVertexHandleType> added_vertex_in_this_layer;
    for (const auto& vertex_handle : vertex_set) {
      added_vertex_in_this_layer.emplace_back(vertex_handle);
      // iterate over the vertexes that are connected to
      // vertex_handle to find whether it is in the previous
      // layer
       auto  parent_it  = parent.find(vertex_handle);
      assert(parent_it != parent.end()); // should found that vertex
       auto&  parent_vertex_set = parent_it->second;
      assert(!parent_vertex_set.empty());

      for (auto out_edge_it = vertex_handle->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        auto opp_vertex_handle = out_edge_it->dst_handle();
        // to find whether it is contained in the previous layer
        assert(std::is_sorted(added_vertex_in_this_layer.begin(),
                              added_vertex_in_this_layer.end(), comp));
        if (!std::binary_search(added_vertex_in_this_layer.begin(),
                                added_vertex_in_this_layer.end(),
                                opp_vertex_handle, comp)){
          // the opp vertex is not contained in the previous layer 
          continue;
        }
        // the opp vertex is contained in the previous layer,
        // add an edge from opp vertex to vertex_handle in the DAG
        auto opp_vertex_handle_parent_it = parent.find(opp_vertex_handle);
        // std::cout << "opp_vertex_handle->id(): "
        //           <<  opp_vertex_handle->id() << std::endl;
        // should have parent for this vertex
        assert(opp_vertex_handle_parent_it != parent.end());
        auto& opp_vertex_handle_parent 
            = opp_vertex_handle_parent_it->second;
        
        int mid = parent_vertex_set.size(); // Store the end of first sorted range

        //First copy the second sorted range into the destination vector
        std::copy(opp_vertex_handle_parent.begin(), 
                  opp_vertex_handle_parent.end(), 
                  std::back_inserter(parent_vertex_set));

        //Then perform the in place merge on the two sub-sorted ranges.
        std::inplace_merge(parent_vertex_set.begin(), 
                           parent_vertex_set.begin() + mid, 
                           parent_vertex_set.end());

        //Remove duplicate elements from the sorted vector
        parent_vertex_set.erase(std::unique(parent_vertex_set.begin(), 
                                            parent_vertex_set.end()), 
                                            parent_vertex_set.end());
      }

      for (auto in_edge_it = vertex_handle->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        auto opp_vertex_handle = in_edge_it->src_handle();
        // to find whether it is contained in the previous layer
        assert(std::is_sorted(added_vertex_in_this_layer.begin(),
                              added_vertex_in_this_layer.end(), comp));
        if (!std::binary_search(added_vertex_in_this_layer.begin(),
                                added_vertex_in_this_layer.end(),
                                  opp_vertex_handle, comp)) {
          // the opp vertex is not contained in the previous layer 
          continue;
        }
        // the opp vertex is contained in the previous layer,
        // add an edge from opp vertex to vertex_handle in the DAG
         auto  opp_vertex_handle_parent_it  = parent.find(opp_vertex_handle);
        // std::cout << "opp_vertex_handle->id(): "
        //           <<  opp_vertex_handle->id() << std::endl;
        // should have parent for this vertex
        assert(opp_vertex_handle_parent_it != parent.end());
        auto& opp_vertex_handle_parent 
            = opp_vertex_handle_parent_it->second;
        
        int mid = parent_vertex_set.size(); // Store the end of first sorted range

        //First copy the second sorted range into the destination vector
        std::copy(opp_vertex_handle_parent.begin(), 
                  opp_vertex_handle_parent.end(), 
                  std::back_inserter(parent_vertex_set));

        //Then perform the in place merge on the two sub-sorted ranges.
        std::inplace_merge(parent_vertex_set.begin(), 
                           parent_vertex_set.begin() + mid, 
                           parent_vertex_set.end());

        //Remove duplicate elements from the sorted vector
        parent_vertex_set.erase(std::unique(parent_vertex_set.begin(), 
                                            parent_vertex_set.end()), 
                                            parent_vertex_set.end());
      }
    }
  }

  for (auto& [query_vertex_handle,
              query_vertex_parent] : parent) {
    assert( query_vertex_handle);
    assert(!query_vertex_parent.empty());
    std::sort(query_vertex_parent.begin(), 
              query_vertex_parent.end());
    // could contain duplicated vertex in parent if there
    // are more than one edge between two vertexes
    auto erase_it = std::unique(query_vertex_parent.begin(), 
                                query_vertex_parent.end());
    query_vertex_parent.erase(erase_it, query_vertex_parent.end());
  }

  #ifndef NDEBUG

  // std::cout << "###  src vertex id  ###" << std::endl;
  // for (const auto& src_vertex_it : src_vertex_set) {
  //   std::cout << "\t" << src_vertex_it->id();
  // }
  // std::cout << std::endl;

  // std::cout << "###  parent vertex id  ###" << std::endl;
  // for (const auto& [query_vertex_handle,
  //                   query_vertex_parent] : parent) {
  //   std::cout << "\t" << query_vertex_handle->id();
  // }
  // std::cout << std::endl;
  
  for (auto vertex_it = query_graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandleType vertex_handle = vertex_it;
    // std::cout << "query vertex id: " << vertex_handle->id() << std::endl;
    auto parent_it = parent.find(vertex_handle);
    // the parent set should contain each vertex in query graph
    assert(parent_it != parent.end());
    // the parent set should contain itself
    assert(std::binary_search(parent_it->second.begin(),
                              parent_it->second.end(),
                              vertex_handle));
  }
  #endif // NEDBUG
  return;
}

template <typename  QueryGraph, 
          typename TargetGraph>
inline void RestoreState(
    typename VertexHandle< QueryGraph>::type  query_vertex_handle,
    typename VertexHandle<TargetGraph>::type target_vertex_handle,
    Match<QueryGraph, TargetGraph> &match_state,
    std::set<typename VertexHandle<TargetGraph>::type> &target_matched) {
  assert(match_state.HasMap(query_vertex_handle));
  assert(target_matched.find(target_vertex_handle)
      != target_matched.end());
  match_state.EraseMap( query_vertex_handle);
  target_matched.erase(target_vertex_handle);
  return;
}

template <enum EdgeState edge_state,
          typename  QueryGraphType, 
          typename TargetGraphType>
void UpdateParentOneDirection(
    const Match<QueryGraphType, 
               TargetGraphType> &match_state,
    typename VertexHandle<QueryGraphType>::type update_query_vertex_handle,
    std::map<typename VertexHandle<QueryGraphType>::type, 
 std::vector<typename VertexHandle<QueryGraphType>::type>> &parent) {
  using QueryVertexHandle = typename VertexHandle<QueryGraphType>::type;
  for (auto edge_it = (edge_state == EdgeState::kIn
                      ? update_query_vertex_handle-> InEdgeBegin()
                      : update_query_vertex_handle->OutEdgeBegin());
           !edge_it.IsDone(); edge_it++) {
    QueryVertexHandle update_query_adj_handle = edge_state == EdgeState::kIn
                                                    ? edge_it->src_handle()
                                                    : edge_it->dst_handle();
    if (!match_state.HasMap(update_query_adj_handle)) {
      // update_query_adj_handle is not matched yet
      continue;
    }
    // update_query_adj_handle has been matched
    assert(parent.find(update_query_vertex_handle) != parent.end());
    auto &query_parent_set = parent.find(update_query_vertex_handle)->second;
    auto find_it = parent.find(update_query_adj_handle);
    if (find_it == parent.end()) {
      continue;
    } 
    for (const auto &it : find_it->second) {
      query_parent_set.emplace_back(it);
    }
  }
  return;
}

// to find all parents of the update_query_vertex_handle
// e.g. all vertexes connected to update_query_vertex_handle
// that have already been matched in match_state
template <typename  QueryGraphType, 
          typename TargetGraphType>
void UpdateParent(
    const Match<QueryGraphType, 
               TargetGraphType> &match_state,
    typename VertexHandle<QueryGraphType>::type update_query_vertex_handle,
    std::map<typename VertexHandle<QueryGraphType>::type, 
 std::vector<typename VertexHandle<QueryGraphType>::type>> &parent) {
  using QueryVertexHandleType = typename VertexHandle<QueryGraphType>::type;
  assert(parent.find(update_query_vertex_handle)
      == parent.end());
  auto [ parent_it,
         parent_ret ] = parent.emplace(update_query_vertex_handle,
    std::vector<QueryVertexHandleType>{update_query_vertex_handle});
  assert(parent_ret);
  UpdateParentOneDirection<EdgeState::kIn >(match_state,
                                            update_query_vertex_handle, parent);
  UpdateParentOneDirection<EdgeState::kOut>(match_state,
                                            update_query_vertex_handle, parent);
  auto &l = parent.find(update_query_vertex_handle)->second;
  std::sort(l.begin(), l.end());
  auto erase_it = std::unique(l.begin(), l.end());
  l.erase(erase_it, l.end());
  return;
}

template <enum MatchSemantics match_semantics, 
          bool use_fail_set,
          bool use_dag_order,
          typename  QueryGraph,
          typename TargetGraph>
bool _DPISOUsingMatch(
      const std::map<typename VertexHandle<QueryGraph>::type,
         std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
            Match<QueryGraph, TargetGraph> &match_state,
            std::set<typename VertexHandle<TargetGraph>::type> &target_matched,
            std::map<typename VertexHandle<QueryGraph>::type,
                    std::vector<typename VertexHandle<QueryGraph>::type>> &parent,
            std::vector<typename VertexHandle<QueryGraph>::type> &current_state_fail_set,
            const std::function<bool(const Match<QueryGraph, TargetGraph>&)>&  user_callback, 
            const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
            time_t begin_time, 
            double query_limit_time) {

  assert(current_state_fail_set.empty());

  // std::cout << "<<  match_state  >>" << std::endl;
  // for (auto map_it = match_state.MapBegin();
  //          !map_it.IsDone();
  //           map_it++)  {
  //   std::cout << "\t<" << map_it->src_handle()->id()
  //             << ",\t" << map_it->dst_handle()->id()
  //             << ">"   << std::endl;
  // }

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  if (query_limit_time > 0 
   && query_limit_time < (std::time(NULL) - begin_time)) {
    // has reached time limit
    return false;
  }

  if (prune_callback(match_state)) {
    return true;
  }

  if (match_state.size() == candidate_set.size()) {
    return user_callback(match_state);
  }

  assert(match_state.size() < candidate_set.size());
  // find the next vertex for querying
  QueryVertexHandle next_query_vertex_handle
   = NextMatchVertex<use_dag_order>(candidate_set, match_state, parent);

  // bool wenzhi_debug_to_pause = false;
  // if (match_state.HasMap(0)
  //  && match_state.HasMap(3)
  //  && match_state.HasMap(5)
  //  && match_state.MapTo(0)->id() == 75502 
  //  && match_state.MapTo(3)->id() == 55974 
  //  && match_state.MapTo(5)->id() == 81527) {
  //   std::cout << "next_query_vertex_handle->id(): "
  //             <<  next_query_vertex_handle->id() << std::endl;
  //   std::cout << "candidates: " << std::endl;
  //   for (const auto& candidate : candidate_set.find(next_query_vertex_handle)->second) {
  //     std::cout << "\t" << candidate->id() << std::endl;
  //   }
  //   wenzhi_debug_to_pause = true;
  //   getchar();
  // }
	// 1186
	// 7612
	// 8826
	// 9820
	// 11554
	// 13246
	// 16533
	// 32537
	// 33391
	// 38795
	// 49735
	// 53364
	// 66531
	// 74936
	// 75502

  // should have found next query vertex since the matching 
  // is not ended
  assert(next_query_vertex_handle);
  assert(!match_state.HasMap(next_query_vertex_handle));
  assert( candidate_set.find(next_query_vertex_handle) 
       != candidate_set.end());
  const auto& candidate_set_for_next_query_vertex 
            = candidate_set.find(next_query_vertex_handle)->second;

  if constexpr (use_fail_set) {
    // a new vertex in the query graph is selected for querying
    // maintain the parent set of the new added query vertex
    if constexpr (!use_dag_order) {
      UpdateParent(match_state, next_query_vertex_handle, parent);
    }
    if (candidate_set_for_next_query_vertex.empty()) {
      // #######################################################
      // # is called emptyset-class corresponding to the paper #
      // #######################################################
      //
      // there is no legal candidate for the selected
      // query vertex, set the fail set as all the parent of 
      // the selected query vertex
      current_state_fail_set = parent.find(next_query_vertex_handle)->second;
      if constexpr (!use_dag_order) {
        // restore parent
        auto ret = parent.erase(next_query_vertex_handle);
        assert(ret > 0);
      }
      // continue recursive matching
      return true;
    }
  }

  bool has_one_child_with_empty_fail_set = false;

  for (const auto &next_target_vertex_handle 
       : candidate_set_for_next_query_vertex) {
    // if (wenzhi_debug_to_pause) {
    //   std::cout << "next_query_vertex_handle->id(): "
    //             <<  next_query_vertex_handle->id() << std::endl;
    //   std::cout << "next_target_vertex_handle->id(): "
    //             <<  next_target_vertex_handle->id() << std::endl;
    //   std::cout << "current_state_fail_set: " << std::endl;
    //   for (const auto& handle : current_state_fail_set) {
    //     std::cout << "\t" << handle->id() << std::endl;
    //   }
    //   getchar();
    // }
    if constexpr (use_fail_set) {
      if (!current_state_fail_set.empty()
       && !std::binary_search(current_state_fail_set.begin(),
                              current_state_fail_set.end(),
                              next_query_vertex_handle)) {
        // has found fail set , u is not in fail set and 
        // fail set is not empty!
        // does not need further expand
        if constexpr (!use_dag_order) {
          // restore parent
          auto ret = parent.erase(next_query_vertex_handle);
          assert(ret > 0);
        }
        // if (wenzhi_debug_to_pause) {
        //   std::cout << "next_target_vertex_handle contained in current_state_fail_set: " << std::endl;
        //   for (const auto& handle : current_state_fail_set) {
        //     std::cout << "\t" << handle->id() << std::endl;
        //   }
        //   getchar();
        // }
        return true;
      }
      if (match_semantics == MatchSemantics::kIsomorphism 
       && !has_one_child_with_empty_fail_set
       && target_matched.find(next_target_vertex_handle) 
       != target_matched.end()) {
        // #######################################################
        // # is called conflict-class corresponding to the paper #
        // #######################################################
        //
        // find u' satisfy that match_state[u']=next_target_vertex_handle
        // next_state_fail_set = anc[u] union anc[u']
        std::vector<QueryVertexHandle> next_state_fail_set;
        for (auto map_it = match_state.MapBegin();
                 !map_it.IsDone(); 
                  map_it++) {
          if (map_it->dst_handle() != next_target_vertex_handle){
            // is not the query vertex that match to the same
            // vertex in target graph
            continue;
          }
          // is the vertex that match to the same vertex in
          // the target graph
          // wenzhi: assert fail here!
          assert(parent.find(next_query_vertex_handle)
              != parent.end());
          assert(parent.find(map_it->src_handle())
              != parent.end());
          std::set_union(parent.find(next_query_vertex_handle)->second.begin(),
                         parent.find(next_query_vertex_handle)->second.end(),
                         parent.find(map_it->src_handle())->second.begin(),
                         parent.find(map_it->src_handle())->second.end(),
                         std::inserter(next_state_fail_set,
                                       next_state_fail_set.begin()));
          // there won't be another vertex that match to the
          // same vertex
          break;
        }
        // update current_state_fail_set
        // anc[u] must contain u ,so union fail set
        std::vector<QueryVertexHandle> temp_fail_set;
        std::set_union(next_state_fail_set.begin(), 
                       next_state_fail_set.end(),
                    current_state_fail_set.begin(),
                    current_state_fail_set.end(),
               std::inserter(temp_fail_set, 
                             temp_fail_set.begin()));
        std::swap(current_state_fail_set, temp_fail_set);
        continue;
      }
    }
    if (!IsJoinable<match_semantics>(
              next_query_vertex_handle, 
             next_target_vertex_handle, match_state,
                  target_matched)) {
      // this query-target vertex pair cannot be added to 
      // the current match state, enumerate the next target vertex
      // if (wenzhi_debug_to_pause) {
      //   std::cout << "is not IsJoinable!" << std::endl;
      //   getchar();
      // }
      continue;
    }
    // this query-target vertex pair can be added to the current
    // match state, continue recursive searching
    std::map<QueryVertexHandle, 
              std::vector<TargetVertexHandle>> 
            temp_candidate_set{candidate_set};
    // add the query-target vertex pair into the match state, 
    // add the target vertex into the match_state and target_matched
    UpdateState(next_query_vertex_handle, 
               next_target_vertex_handle,
                match_state, target_matched);
    // update the candidate set based on the new enumerated query-target vertex pair
    UpdateCandidateSet(
                next_query_vertex_handle, 
               next_target_vertex_handle,
               temp_candidate_set, 
                match_state);
    std::vector<typename VertexHandle<QueryGraph>::type> next_state_fail_set;
    if (!_DPISOUsingMatch<match_semantics, use_fail_set, use_dag_order>(
            temp_candidate_set, match_state, 
            target_matched, 
            parent, next_state_fail_set,
             user_callback,
            prune_callback, begin_time, query_limit_time)) {
      if constexpr (use_fail_set && !use_dag_order) {
        // restore parent
        auto ret = parent.erase(next_query_vertex_handle);
        assert(ret > 0);
      }
      return false;
    }
    RestoreState(next_query_vertex_handle, 
                next_target_vertex_handle,
                  match_state, target_matched);
    if constexpr (!use_fail_set) {
      // does not use fail set
      continue;
    }
    // if (wenzhi_debug_to_pause) {
    //   std::cout << "next_query_vertex_handle->id(): "
    //             <<  next_query_vertex_handle->id() << std::endl;
    //   std::cout << "next_target_vertex_handle->id(): "
    //             <<  next_target_vertex_handle->id() << std::endl;
    //   std::cout << "next_state_fail_set: " << std::endl;
    //   for (const auto& vertex_handle : next_state_fail_set) {
    //     std::cout << "\t" << vertex_handle->id() << std::endl;
    //   }
    //   std::cout << "has_one_child_with_empty_fail_set: "
    //             <<  has_one_child_with_empty_fail_set
    //             << std::endl;
    //   getchar();
    // }
    if (has_one_child_with_empty_fail_set) {
      assert(current_state_fail_set.empty());
      continue;
    }
    // use fail set, needs to maintain it
    if (next_state_fail_set.empty()) {
      // if ont child node's fail set is empty
      // so this state's fail set is empty
      current_state_fail_set.clear();
      has_one_child_with_empty_fail_set = true;
      continue;
    }
    if (!std::binary_search(next_state_fail_set.begin(),
                            next_state_fail_set.end(),
                            next_query_vertex_handle)) {
      // if one child node's fail set not contain next_query_vertex_handle
      // so this state's fail set is next_state_fail_set
      current_state_fail_set = next_state_fail_set;
      continue;
    }
    std::vector<QueryVertexHandle> temp_this_state_fail_set;
    std::set_union(next_state_fail_set.begin(), 
                   next_state_fail_set.end(),
                current_state_fail_set.begin(), 
                current_state_fail_set.end(),
                   std::inserter(temp_this_state_fail_set,
                                 temp_this_state_fail_set.begin()));
    std::swap(temp_this_state_fail_set, 
                current_state_fail_set);
  }
  if constexpr (use_fail_set && !use_dag_order) {
    // restore parent
    auto ret = parent.erase(next_query_vertex_handle);
    assert(ret > 0);
  }
  return true;
}

namespace _DAGDP {

template <class QueryGraph, class InDegreeContainer>
inline void BFS(QueryGraph &query_graph, 
         InDegreeContainer &in_degree,
  const std::set<typename VertexHandle<QueryGraph>::type>& src_vertex_set) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType = typename EdgeID<QueryGraph>::type;
  std::set<QueryVertexHandle> used_vertex,
                          temp_src_vertex_set
                             = src_vertex_set;
  std::set<QueryEdgeIDType> used_edge;
  while (used_vertex.size() < query_graph.CountVertex()) {
    if (temp_src_vertex_set.empty()) {
      // need to find not used vertex as src
      for (auto it = query_graph.VertexBegin(); !it.IsDone(); it++) {
        if (used_vertex.find(it) != used_vertex.end()) {
          // this vertex has been used
          continue;
        }
        // this vertex has not been used
        auto [ temp_src_vertex_set_it,
               temp_src_vertex_set_ret ]
             = temp_src_vertex_set.emplace(it);
        assert(temp_src_vertex_set_ret);
        break;
      }
      assert(!temp_src_vertex_set.empty());
    }
    std::queue<QueryVertexHandle> bfs_queue;
    for (const auto& src_vertex : temp_src_vertex_set) {
      auto [ used_vertex_it,
             used_vertex_ret ]
           = used_vertex.emplace(src_vertex);
      assert(used_vertex_ret);
      bfs_queue.emplace(src_vertex);
      auto [ in_degree_it,
             in_degree_ret ]
           = in_degree.emplace(src_vertex, 0);
      assert(in_degree_ret);
    }
    while (!bfs_queue.empty()) {
      QueryVertexHandle now_vertex_handle = bfs_queue.front();
      bfs_queue.pop();
      for (auto edge_it = now_vertex_handle->OutEdgeBegin(); 
               !edge_it.IsDone();
                edge_it++) {
        if (used_edge.count(edge_it->id())) 
          continue;
        QueryVertexHandle next_vertex_handle = edge_it->dst_handle();
        used_edge.insert(edge_it->id());
        in_degree[next_vertex_handle]++;
        if (!used_vertex.count(next_vertex_handle)) {
          bfs_queue.push(next_vertex_handle);
          used_vertex.insert(next_vertex_handle);
        }
      }
      for (auto edge_it = now_vertex_handle->InEdgeBegin(); 
               !edge_it.IsDone();
                edge_it++) {
        if (used_edge.count(edge_it->id())) 
          continue;
        QueryVertexHandle next_vertex_handle = edge_it->src_handle();
        used_edge.insert(edge_it->id());
        in_degree[next_vertex_handle]++;
        if (!used_vertex.count(next_vertex_handle)) {
          bfs_queue.push(next_vertex_handle);
          used_vertex.insert(next_vertex_handle);
        }
      }
    }
    temp_src_vertex_set.clear();
  }
  return;
}

template <class QueryGraph, class InDegreeContainer, class TopoSeqContainer>
inline void TopoSort(QueryGraph &query_graph, InDegreeContainer &in_degree,
                     TopoSeqContainer &topo_seq) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType = typename EdgeID<QueryGraph>::type;
  std::queue<QueryVertexHandle> topo_sort_queue;
  std::set<QueryEdgeIDType> used_edge;
  // topo sort
  for (auto it = query_graph.VertexBegin(); !it.IsDone(); it++) {
    QueryVertexHandle query_vertex_handle = it;
    if (in_degree[query_vertex_handle] == 0) {
      topo_sort_queue.push(query_vertex_handle);
    }
  }
  while (!topo_sort_queue.empty()) {
    QueryVertexHandle query_vertex_handle = topo_sort_queue.front();
    topo_sort_queue.pop();
    topo_seq.emplace_back(query_vertex_handle);
    for (auto edge_it = query_vertex_handle->OutEdgeBegin(); !edge_it.IsDone();
         edge_it++) {
      if (used_edge.count(edge_it->id())) continue;
      QueryVertexHandle next_vertex_handle = edge_it->dst_handle();
      in_degree[next_vertex_handle]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_handle] == 0) {
        topo_sort_queue.push(next_vertex_handle);
      }
    }
    for (auto edge_it = query_vertex_handle->InEdgeBegin(); !edge_it.IsDone();
         edge_it++) {
      if (used_edge.count(edge_it->id())) continue;
      QueryVertexHandle next_vertex_handle = edge_it->src_handle();
      in_degree[next_vertex_handle]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_handle] == 0) {
        topo_sort_queue.push(next_vertex_handle);
      }
    }
  }
  return;
}

template <class QueryGraph, 
          class TopoSeqContainer>
inline void GetTopoSeq(QueryGraph &query_graph, 
                 TopoSeqContainer &topo_seq,
  const std::set<typename VertexHandle<QueryGraph>::type>& src_vertex_set) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType   = typename       EdgeID<QueryGraph>::type;
  std::map<QueryVertexHandle, int> in_degree;
  BFS(query_graph, in_degree, src_vertex_set);
  assert(in_degree.size() == query_graph.CountVertex());
  TopoSort(query_graph, in_degree, topo_seq);
  return;
}

template <class  QueryGraph, 
          class TargetGraph,
          class TopoSeqContainer>
inline void GetTopoSeq(QueryGraph& query_graph, 
                 TopoSeqContainer& topo_seq, 
           const Match<QueryGraph,
                      TargetGraph>& partial_match) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using QueryEdgeIDType = typename EdgeID<QueryGraph>::type;
  std::map<QueryVertexHandle, int> in_degree;
  std::set<QueryVertexHandle> src_vertex_set;
  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone();
            map_it++) {
    src_vertex_set.emplace(map_it->src_handle());
  }
  BFS(query_graph, in_degree, src_vertex_set);
  assert(in_degree.size() == query_graph.CountVertex());
  TopoSort(query_graph, in_degree, topo_seq);
}

// query_vertex_handle has been matched to target_vertex_handle,
// adj_used_vertex_handle is a vertex that adjacent to 
// query_vertex_handle that have been used,
// to check whether temp_target_vertex_handle can be a candidate
// of adj_used_vertex_handle
template <typename  QueryGraph, 
          typename TargetGraph>
inline bool EdgeCheck(
    typename VertexHandle< QueryGraph>::type       &query_vertex_handle,
    typename VertexHandle< QueryGraph>::type    &adj_used_vertex_handle,
    typename VertexHandle<TargetGraph>::type      &target_vertex_handle,
    typename VertexHandle<TargetGraph>::type &temp_target_vertex_handle,
    // statistic of query_vertex_handle
    const std::map<typename VertexHandle<QueryGraph>::type, 
          std::map<typename    EdgeLabel<QueryGraph>::type, int>> &out_edge_count, 
    // statistic of query_vertex_handle
    const std::map<typename VertexHandle<QueryGraph>::type, 
          std::map<typename    EdgeLabel<QueryGraph>::type, int>>  &in_edge_count) {
  // adj_used_vertex_handle is adjacent to query_vertex_handle
  assert((out_edge_count.find(adj_used_vertex_handle) != out_edge_count.end())
      || ( in_edge_count.find(adj_used_vertex_handle) !=  in_edge_count.end()));
  auto it = out_edge_count.find(adj_used_vertex_handle);
  if (it != out_edge_count.end()) {
    for (const auto& [edge_label, edge_count] : it->second) {
      if (_dp_iso_using_match::CountOutEdge<TargetGraph>(
              target_vertex_handle,   edge_label,
         temp_target_vertex_handle) < edge_count) {
        // the vertex in query graph has more output edge toward
        // adj_used_vertex_handle with the specifeid label than 
        // the from target_vertex_handle to temp_target_vertex_handle
        return false;
      }
    }
  }
  it = in_edge_count.find(adj_used_vertex_handle);
  if (it != in_edge_count.end()) {
    for (const auto& [edge_label, edge_count] : it->second) {
      if (_dp_iso_using_match::CountInEdge<TargetGraph>(
              target_vertex_handle,   edge_label,
         temp_target_vertex_handle) < edge_count)
        // the vertex in query graph has more input edge from
        // adj_used_vertex_handle with the specifeid label than 
        // the to target_vertex_handle from temp_target_vertex_handle
        return false;
    }
  }
  return true;
}

// to find whether the target_vertex_handle can be a legal match for 
// query_vertex_handle
// check the candidate set of both the vertexes adjacent
// to query_vertex_handle and the vertexes adjacent to 
// target_vertex_handle
template <bool is_out_direction,
          typename  QueryGraph, 
          typename TargetGraph,
          typename CandidateSetContainer>
inline bool CheckIsLegal(
    typename VertexHandle< QueryGraph>::type  query_vertex_handle,
    typename VertexHandle<TargetGraph>::type target_vertex_handle,
    std::set<typename VertexHandle<QueryGraph>::type> &used_vertex,
    // statistic of query_vertex_handle
    std::map<typename VertexHandle<QueryGraph>::type, 
    std::map<typename    EdgeLabel<QueryGraph>::type, int>> &out_edge_count,
    // statistic of query_vertex_handle
    std::map<typename VertexHandle<QueryGraph>::type, 
    std::map<typename    EdgeLabel<QueryGraph>::type, int>>  &in_edge_count, 
    const CandidateSetContainer &candidate_set) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  for (auto edge_it = EdgeBegin<is_out_direction, QueryGraph>(
                                query_vertex_handle);
           !edge_it.IsDone(); 
            edge_it++) {
    QueryVertexHandle adj_used_vertex_handle
       = is_out_direction ? edge_it->dst_handle() 
                          : edge_it->src_handle();
    if (used_vertex.find(adj_used_vertex_handle)
     == used_vertex.end()) {
      // this vertex is not used, does not need to be checked
      continue;
    }
    assert(candidate_set.find(adj_used_vertex_handle)
        != candidate_set.end());
    auto& candidate_set_for_adj_used_vertex
        = candidate_set.find(adj_used_vertex_handle)->second;
    bool found_legal_vertex_adj_to_target_vertex_handle = false;
    if constexpr (GraphParameter<TargetGraph>::vertex_level_edge_label_index) {
      // vertex has vertex_handle->In/Out VertexBegin(edge_label)
      // method
      for (auto target_vertex_it = is_out_direction
              ? target_vertex_handle->OutVertexBegin(edge_it->label())
              : target_vertex_handle-> InVertexBegin(edge_it->label());
               !target_vertex_it.IsDone(); 
                target_vertex_it++) {
        TargetVertexHandle temp_target_vertex_handle = target_vertex_it;
        if (!EdgeCheck<QueryGraph, TargetGraph>(
                query_vertex_handle,    adj_used_vertex_handle,
               target_vertex_handle, temp_target_vertex_handle, 
               out_edge_count,
                in_edge_count)) {
          continue;
        }
        // temp_target_vertex_handle can be a candidate for adj_used_vertex_handle
        // find whether it is in candidate_set_for_adj_used_vertex
        auto it1 = std::lower_bound(candidate_set_for_adj_used_vertex.begin(), 
                                    candidate_set_for_adj_used_vertex.end(), 
                                    temp_target_vertex_handle);
        if ( it1 != candidate_set_for_adj_used_vertex.end() 
        && (*it1) == temp_target_vertex_handle) {
          found_legal_vertex_adj_to_target_vertex_handle = true;
          break;
        }
      }
    } else {
      // vertex does not have vertex_handle->In/Out VertexBegin(edge_label)
      // method
      for (auto target_edge_it = EdgeBegin<is_out_direction, TargetGraph>(
                                           target_vertex_handle);
               !target_edge_it.IsDone(); target_edge_it++) {
        if (target_edge_it->label() != edge_it->label()) {
          continue;
        }
        TargetVertexHandle temp_target_vertex_handle 
           = is_out_direction ? target_edge_it->dst_handle()  // is out edge
                              : target_edge_it->src_handle(); // is  in edge
        if (!EdgeCheck<QueryGraph, TargetGraph>(
                query_vertex_handle, adj_used_vertex_handle, target_vertex_handle,
                temp_target_vertex_handle, out_edge_count, in_edge_count)) {
          continue;
        }
        // temp_target_vertex_handle can be a candidate for adj_used_vertex_handle
        // find whether it is in candidate_set_for_adj_used_vertex
        auto it1 = std::lower_bound(candidate_set_for_adj_used_vertex.begin(), 
                                    candidate_set_for_adj_used_vertex.end(), 
                                    temp_target_vertex_handle);
        if ( it1 != candidate_set_for_adj_used_vertex.end() 
        && (*it1) == temp_target_vertex_handle) {
          found_legal_vertex_adj_to_target_vertex_handle = true;
          break;
        }
      }
    }
    if (!found_legal_vertex_adj_to_target_vertex_handle) {
      return false;
    }
  }
  return true;
}

}  // namespace _DAGDP
template <typename  QueryGraph,
          typename TargetGraph,
          typename TargetVertexContainerType>
inline bool DAGDP(
     QueryGraph  &query_graph, 
    TargetGraph &target_graph,
    std::vector<typename VertexHandle<QueryGraph>::type> &topo_seq,
       std::map<typename VertexHandle<QueryGraph>::type, 
                   TargetVertexContainerType> &candidate_set) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  using QueryEdgeLabelType = typename EdgeLabel<QueryGraph>::type;
  std::set<QueryVertexHandle> used_vertex;
  for (auto &vertex_handle : topo_seq) {
    used_vertex.emplace(vertex_handle);
    assert(candidate_set.find(vertex_handle)
        != candidate_set.end());
    auto &candidate_set_of_vertex_handle 
        = candidate_set.find(vertex_handle)->second;
    // statistic of vertex_handle
    std::map<QueryVertexHandle, 
             std::map<QueryEdgeLabelType, int>> in_edge_count, 
                                               out_edge_count;
    // count the number of the in/out edges with
    // each kind of edge label
    for (auto out_edge_it = vertex_handle->OutEdgeBegin(); 
             !out_edge_it.IsDone();
              out_edge_it++) {
      QueryVertexHandle dst_handle = out_edge_it->dst_handle();
      if (used_vertex.find(dst_handle) 
       == used_vertex.end()) {
        // this vertex is not matched
        continue;
      }
      out_edge_count[dst_handle][out_edge_it->label()]++;
    }
    for (auto in_edge_it = vertex_handle->InEdgeBegin(); 
             !in_edge_it.IsDone();
              in_edge_it++) {
      QueryVertexHandle src_handle = in_edge_it->src_handle();
      if (used_vertex.find(src_handle) 
       == used_vertex.end()) {
        // this vertex is not matched
        continue;
      }
      in_edge_count[src_handle][in_edge_it->label()]++;
    }
    TargetVertexContainerType
          temp_candidate_set_of_vertex_handle;
    temp_candidate_set_of_vertex_handle.reserve(
         candidate_set_of_vertex_handle.size());
    for (const auto &candidate_dst_handle
                   : candidate_set_of_vertex_handle) {
      // verify the candidate set of vertex_handle, to find whether each of them
      // can be a legal match for vertex_handle
      if (!_DAGDP::CheckIsLegal< true, QueryGraph, TargetGraph>(
          vertex_handle, candidate_dst_handle, used_vertex, out_edge_count,
          in_edge_count, candidate_set)) {
        // candidate_dst_handle cannot be a legal match for vertex_handle
        continue;
      }
      if (!_DAGDP::CheckIsLegal<false, QueryGraph, TargetGraph>(
          vertex_handle, candidate_dst_handle, used_vertex, out_edge_count,
          in_edge_count, candidate_set)) {
        // candidate_dst_handle cannot be a legal match for vertex_handle
        continue;
      }
      // candidate_dst_handle is connected to the vertex,
      // can be preserved in the candidate set of the connected size
      temp_candidate_set_of_vertex_handle.emplace_back(candidate_dst_handle);
    }
    candidate_set_of_vertex_handle.swap(temp_candidate_set_of_vertex_handle);
    if (candidate_set_of_vertex_handle.empty()) {
      return false;
    }
  }
  return true;
}

template <typename  QueryGraph, 
          typename TargetGraph,
          class CandidateSetContainer>
inline bool RefineCandidateSet(QueryGraph  &query_graph,
                              TargetGraph &target_graph,
                               CandidateSetContainer &candidate_set,
                            typename VertexHandle<QueryGraph>::type root_vertex = typename VertexHandle<QueryGraph>::type()) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  std::vector<QueryVertexHandle> topo_seq;
  std::set<QueryVertexHandle> root_vertex_set;
  if (root_vertex) {
    // is not null
    root_vertex_set.emplace(root_vertex);
    assert(root_vertex_set.size() == 1);
  }
  _DAGDP::GetTopoSeq(query_graph, topo_seq, root_vertex_set);
  if (!DAGDP(query_graph, target_graph, topo_seq, candidate_set)) 
    return false;
  constexpr int loop_num = 2;
  for (int i = 1; i <= loop_num; i++) {
    std::reverse(topo_seq.begin(), topo_seq.end());
    if (!DAGDP(query_graph, target_graph, topo_seq, candidate_set))
      return false;
  }
  return true;
}

template <typename  QueryGraph, 
          typename TargetGraph,
          class CandidateSetContainer>
inline bool RefineCandidateSet(QueryGraph  &query_graph,
                              TargetGraph &target_graph,
                               CandidateSetContainer &candidate_set,
                   const Match<QueryGraph,
                              TargetGraph>& partial_match) {
  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone(); map_it++) {
    auto candidate_set_it 
       = candidate_set.find(map_it->src_handle());
    assert(candidate_set_it
        != candidate_set.end());
    candidate_set_it->second.clear();
    candidate_set_it->second.emplace_back(map_it->dst_handle());
  }
  std::vector<QueryVertexHandle> topo_seq;
  _DAGDP::GetTopoSeq(query_graph, topo_seq, partial_match);
  if (!DAGDP(query_graph, 
            target_graph, topo_seq, candidate_set)) 
    return false;
  constexpr int loop_num = 2;
  for (int i = 1; i <= loop_num; i++) {
    std::reverse(topo_seq.begin(), topo_seq.end());
    if (!DAGDP(query_graph, target_graph, topo_seq, candidate_set))
      return false;
  }
  return true;
}

template <enum MatchSemantics match_semantics, 
          typename  QueryGraph,
          typename TargetGraph>
bool CheckMatchIsLegal(
   const Match<QueryGraph, 
              TargetGraph> &match_state) {
  using TargetVertexHandle 
   = typename VertexHandle<TargetGraph>::type;
  std::set<TargetVertexHandle> target_matched;
  for (auto map_it = match_state.MapBegin();
           !map_it.IsDone();
            map_it++) {
    auto [ target_matched_it,
           target_matched_ret] 
         = target_matched.emplace(map_it->dst_handle());
    // assert(target_matched_ret);
  }
  if (match_semantics == MatchSemantics::kIsomorphism 
   && target_matched.size() != match_state.size()) {
    // (more than) two vertexes in query graph are mapped 
    // into a same vertex in the target graph
    assert(target_matched.size() < match_state.size());
    return false;
  }
  for (auto map_it = match_state.MapBegin();
           !map_it.IsDone();
            map_it++) {
    if (!_dp_iso_using_match::JoinableCheck<EdgeState::kIn>(
            map_it->src_handle(), 
            map_it->dst_handle(), match_state)) {
      return false;
    }
    if (!_dp_iso_using_match::JoinableCheck<EdgeState::kOut>(
            map_it->src_handle(), 
            map_it->dst_handle(), match_state)) {
      return false;
    }
  }
  return true;
}

template <enum MatchSemantics match_semantics, 
          bool use_dag_order = true,
          typename  QueryGraph,
          typename TargetGraph>
inline int DPISOUsingMatch_Recursive(
     QueryGraph  &query_graph, 
    TargetGraph &target_graph,
    const std::map<typename VertexHandle<QueryGraph>::type,
       std::vector<typename VertexHandle<TargetGraph>::type>> &candidate_set,
  const Match<QueryGraph, TargetGraph>& partial_match,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>&  user_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
    double query_limit_time = -1.0) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using MatchType = Match<QueryGraph, TargetGraph>;

  using CandidateSetType = std::map<typename VertexHandle< QueryGraph>::type,
                        std::vector<typename VertexHandle<TargetGraph>::type>>;

  #ifndef NDEBUG
  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone();
            map_it++) {
    assert(map_it->src_handle());
    assert(map_it->dst_handle());
  }
  #endif // NDEBUG

  // std::cout << "##  query_graph.CountVertex(): "
  //           <<      query_graph.CountVertex() << std::endl;

  // std::cout << "##  target_graph.CountVertex(): "
  //           <<      target_graph.CountVertex() << std::endl;

  // // by wenzhi
  // std::string query_graph_str;
  // query_graph_str << query_graph;
  // std::cout << "##  query graph str" << std::endl
  //           <<      query_graph_str  << std::endl;

  // std::cout << "candidate_set size()" << std::endl;

  // for (auto& [query_handle, 
  //             candidate_set] : candidate_set) {
  //   std::cout << "query_handle->id(): " 
  //             <<  query_handle->id() << std::endl
  //             << "candidate_set.size(): " 
  //             <<  candidate_set.size() << std::endl;
  // }
            
  if (!CheckMatchIsLegal<match_semantics>(partial_match)) {
    // partial match is not legal.  
    return false;
  }
  CandidateSetType temp_candidate_set 
                      = candidate_set;

  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone();
            map_it++) {
    UpdateCandidateSet(map_it->src_handle(), 
                       map_it->dst_handle(), 
                       temp_candidate_set, 
                       partial_match);
  }
  std::set<TargetVertexHandle> target_matched;
  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone();
            map_it++) {
    auto [ match_it,
           match_ret ] = target_matched.emplace(map_it->dst_handle());
    assert(match_ret);
  }
  size_t result_count = 0;
  // add lock to user_callback and prune_callback
  omp_lock_t user_callback_lock;
  omp_init_lock(&user_callback_lock);
  bool user_callback_has_return_false = false;
  std::function<bool(const Match<QueryGraph, TargetGraph>&)> 
    par_user_callback 
    = [&result_count, 
       &user_callback, 
       &user_callback_lock,
       &user_callback_has_return_false](const MatchType& match_state) -> bool{
        omp_set_lock(&user_callback_lock);
        bool ret_val = false;
        if (!user_callback_has_return_false) {
          ret_val = user_callback(match_state);
          if (!ret_val) {
            user_callback_has_return_false = true;
          }
          result_count++;
        }
        omp_unset_lock(&user_callback_lock);
        return ret_val;
      };

  omp_lock_t prune_callback_lock;
  omp_init_lock(&prune_callback_lock);
  std::function<bool(const Match<QueryGraph, TargetGraph>&)> 
    par_prune_callback
    = [&prune_callback_lock, 
       &prune_callback,
       //  &user_callback_lock,
       &user_callback_has_return_false](const MatchType& match_state) -> bool{
      // it might be unnecessary to set the lock here
      // user_callback_lock is read-only in this callback
      // and can only be set from false to true
      // omp_set_lock(&user_callback_lock);
      if (user_callback_has_return_false) {
        return true;
      }
      // omp_unset_lock(&user_callback_lock);
      omp_set_lock(&prune_callback_lock);
      bool ret_val = prune_callback(match_state);
      omp_unset_lock(&prune_callback_lock);
      return ret_val;
    };

  std:: map  <typename VertexHandle<QueryGraph>::type,
  std::vector<typename VertexHandle<QueryGraph>::type>> parent;

  #ifndef NDEBUG
  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone();
            map_it++) {
    assert(map_it->src_handle());
    assert(map_it->dst_handle());
  }
  #endif // NDEBUG

  QueryVertexHandle next_query_handle =
      _dp_iso_using_match::NextMatchVertex<use_dag_order>(candidate_set, partial_match, parent);

  if constexpr (use_dag_order) {
    std::set<typename VertexHandle<QueryGraph>::type> src_vertex_handle_set;

    if (!partial_match.empty()) {
      // has specified partial match, build the dag rooted at
      // the matched vertexes
      for (auto map_it = partial_match.MapBegin();
               !map_it.IsDone();
                map_it++) {
        assert(map_it->src_handle());
        assert(map_it->dst_handle());
        auto [ src_vertex_handle_it,
               src_vertex_handle_ret ]
             = src_vertex_handle_set.emplace(map_it->src_handle());
        assert(src_vertex_handle_ret);
      }
    }

    assert(src_vertex_handle_set.size() <= query_graph.CountVertex());

    if (src_vertex_handle_set.size() < query_graph.CountVertex()) {
      // the src_vertex_handle_set might not cover all the connected
      // component in the query graph

      std::set<typename VertexHandle<QueryGraph>::type> 
                new_src_vertex_handle_set
                  = src_vertex_handle_set;

      std::set<typename VertexHandle<QueryGraph>::type> vertexes_to_add;

      for (auto vertex_it = query_graph.VertexBegin();
              !vertex_it.IsDone();
                vertex_it++) {
        auto [ vertexes_to_add_it,
              vertexes_to_add_ret ]
            = vertexes_to_add.emplace(vertex_it);
        assert(vertexes_to_add_ret);
      }

      assert(vertexes_to_add.size() == query_graph.CountVertex());

      // to mark which vertexes have been considered in the query graph
      Match<QueryGraph, 
           TargetGraph> vertex_considered;

      while (true) {
        // add vertexes into the new_src_vertex_handle_set until it does not cover 
        // all conneceted components

        // std::cout << " wenzhi here0 " << std::endl;

        auto remove_vertexes_to_add_callback 
                = [&vertexes_to_add,
                  #ifndef NDEBUG
                   &query_graph,
                  #endif // NDEBUG
                   &vertex_considered](const QueryVertexHandle& query_vertex_handle) {

          assert(query_graph.CountVertex() == vertexes_to_add.size()
                                          + vertex_considered.size());

          // if (vertexes_to_add.find(query_vertex_handle)
          //  == vertexes_to_add.end()) {
          //   std::cout << "wenzhi! vertexes_to_add cannot find query_vertex_handle!!"
          //             << std::endl;
          // }
          // std::cout << "vertexes_to_add.size(): " << vertexes_to_add.size() << std::endl;
          typename std::set<typename VertexHandle<QueryGraph>::type>::size_type 
            erase_size = vertexes_to_add.erase(query_vertex_handle);
          assert(erase_size > 0);
          bool ret = vertex_considered.AddMap(query_vertex_handle, TargetVertexHandle());
          assert(ret);

          assert(query_graph.CountVertex() == vertexes_to_add.size()
                                          + vertex_considered.size());
          return true;
        };

        assert(query_graph.CountVertex() == vertexes_to_add.size()
                                        + vertex_considered.size());

        // std::cout << " wenzhi new_src_vertex_handle_set.size(): " 
        //           <<          new_src_vertex_handle_set.size() << std::endl;

        Bfs<true>(query_graph, new_src_vertex_handle_set, remove_vertexes_to_add_callback);

        // std::cout << " wenzhi here1 " << std::endl;

        assert(query_graph.CountVertex() == vertexes_to_add.size()
                                        + vertex_considered.size());

        if (vertexes_to_add.empty()) {
          // the vertexes in query graph have beenb covered 
          assert(vertex_considered.size() == query_graph.CountVertex());
          break;
        }
        // there are still vertexes in the query graph that has not been added
        // e.g. there still connected component that has not been covered by
        // the vertex set in src_vertex_handle_set
        
        assert(parent.empty());
        // then find the next vertex to be added
        QueryVertexHandle next_query_handle =
          _dp_iso_using_match::NextMatchVertex<use_dag_order>(candidate_set, vertex_considered, parent);
        // should not be null
        assert(next_query_handle);
        // should not be found in vertex_considered
        assert(!vertex_considered.HasMap(next_query_handle));
        // should be found in vertexes_to_add
        assert(vertexes_to_add.find(next_query_handle)
            != vertexes_to_add.end());
        new_src_vertex_handle_set.clear();
        new_src_vertex_handle_set.emplace(next_query_handle);
        src_vertex_handle_set.emplace(next_query_handle);
      }
    }

    // the first vertex to query should not be contained in the 
    // partial match
    assert(!partial_match.empty()
        || (src_vertex_handle_set.find(next_query_handle)
         != src_vertex_handle_set.end()));

    // std::cout << "## src_vertex_handle_set ##" << std::endl;
    // for (const auto& src_vertex_handle 
    //                : src_vertex_handle_set) {
    //   std::cout <<"\tsrc_vertex_handle->id(): " << src_vertex_handle->id() << std::endl;
    // }

    BuildDag(query_graph, src_vertex_handle_set, candidate_set, parent);

    #ifndef NDEBUG
    assert( Bfs<true>(query_graph, src_vertex_handle_set, 
             [](const QueryVertexHandle& query_vertex_handle){return true;})
                   == query_graph.CountVertex());
    #endif // NDEBUG

    // std::cout << "## parent ##" << std::endl;
    // for (const auto& [query_vertex_handle,
    //                   candidate_set] : parent) {
    //   std::cout << "query id: " << query_vertex_handle->id() 
    //             << std::endl;
    //   for (const auto& handle : candidate_set) {
    //     std::cout << "\t" << handle->id();
    //   }
    //   std::cout << std::endl;
    // }
  }

  const bool kUseFailSet = query_graph.CountEdge() >= large_query_edge;

  assert(!partial_match.HasMap(next_query_handle));
  if (!next_query_handle) {
    // all vertexes have been matched, cannot 
    // find the next vertex to match
    if (par_prune_callback(partial_match)) {
      return 0;
    }
    par_user_callback(partial_match);
    return static_cast<int>(result_count);
  }
  // partition next ptr's candiate
  assert(candidate_set.find(next_query_handle)
      != candidate_set.end());
  const auto &match_handle_candidate 
            = candidate_set.find(next_query_handle)->second;
  auto begin_time = std::time(NULL);
  for (int i = 0; i < match_handle_candidate.size(); i++) {
    /* ######################################################## *
     * ##  it might be unnecessary to set the lock here      ## *
     * ##  user_callback_lock is read-only in this callback  ## *
     * ##  and can only be set from false to true            ## *
     * ######################################################## *
     * ##  omp_set_lock(&user_callback_lock);                ## *
     * ######################################################## */
    if (user_callback_has_return_false) {
      continue;
    }
    const auto &match_target_handle = match_handle_candidate[i];
    if (!IsJoinable<match_semantics>(
            next_query_handle, 
          match_target_handle, partial_match,
                target_matched)) {
      continue;
    }
    auto temp_match_state    = partial_match;
    auto temp_target_matched = target_matched;
    auto temp_candidate_set  = candidate_set;
    _dp_iso_using_match::UpdateState(next_query_handle, 
                                    match_target_handle,
                                    temp_match_state, 
                                    temp_target_matched);
    _dp_iso_using_match::UpdateCandidateSet(
          next_query_handle, 
        match_target_handle, 
        temp_candidate_set,
        temp_match_state);
    if (!kUseFailSet) {
      std:: map  <typename VertexHandle<QueryGraph>::type,
      std::vector<typename VertexHandle<QueryGraph>::type>> temp_parent;
      std::vector<typename VertexHandle<QueryGraph>::type>  temp_fail_set;
      if constexpr (use_dag_order) {
        temp_parent = parent;
      }

      user_callback_has_return_false 
        = !_dp_iso_using_match::_DPISOUsingMatch<match_semantics, false, use_dag_order>(
            temp_candidate_set, 
            temp_match_state,
            temp_target_matched,
            temp_parent,
            temp_fail_set,
            par_user_callback,
            par_prune_callback, 
            begin_time, query_limit_time);
      // does not need to restore, temp_match_state is a copy
      // from match_state
      continue;
    }
    std:: map  <typename VertexHandle<QueryGraph>::type,
    std::vector<typename VertexHandle<QueryGraph>::type>> temp_parent;
    std::vector<typename VertexHandle<QueryGraph>::type>  temp_fail_set;
    if constexpr (use_dag_order) {
      temp_parent = parent;
    }
    else {
      temp_parent.emplace(next_query_handle, 
                          std::vector<typename VertexHandle<QueryGraph>::type>{next_query_handle});
    }
    user_callback_has_return_false 
      = !_dp_iso_using_match::_DPISOUsingMatch<match_semantics, true, use_dag_order>(
          temp_candidate_set, 
          temp_match_state,
          temp_target_matched,
          temp_parent,
          temp_fail_set,
          par_user_callback,
          par_prune_callback, 
          begin_time, query_limit_time);
    // does not need to restore, temp_match_state is a copy
    // from match_state

    // since user_callback_lock is not set, 
    // there is also no need to unset it
    // omp_unset_lock(&user_callback_lock);
  }
  return static_cast<int>(result_count);
}

}  // namespace _dp_iso_using_match

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          typename  QueryGraph,
          typename TargetGraph>
inline int IncreamentDPISOUsingMatch(
     QueryGraph  &query_graph,
    TargetGraph &target_graph,
    std::vector<typename VertexHandle<TargetGraph>::type> &delta_target_graph,
    const std::function<bool(const Match<QueryGraph, 
                                        TargetGraph>&)>& match_callback, 
    double query_limit_time = -1) {
  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;
  using CandidateSet = std::map<QueryVertexHandle, std::vector<TargetVertexHandle>>;
  CandidateSet candidate_set;
  if (!_dp_iso_using_match::InitCandidateSet<match_semantics>(
           query_graph, 
          target_graph, candidate_set)) {
    return 0;
  }
  if (!_dp_iso_using_match::RefineCandidateSet(query_graph, 
                                              target_graph, candidate_set)) {
    return 0;
  }
  std::sort(delta_target_graph.begin(), 
            delta_target_graph.end());
  std::vector<QueryVertexHandle> has_delta_target_graph_pattern_vertex;
  for (auto &[query_handle, target_list] : candidate_set) {
    bool found_new_vertex = false;
    for (auto &target_handle : target_list) {
      if (std::binary_search(delta_target_graph.begin(),
                             delta_target_graph. end (),
                                   target_handle)) {
        found_new_vertex = true;
        break;
      }
    }
    if (!found_new_vertex) {
      continue;
    }
    has_delta_target_graph_pattern_vertex.emplace_back(query_handle);
  }
  std::sort(has_delta_target_graph_pattern_vertex.begin(),
            has_delta_target_graph_pattern_vertex.end());
  int total_mask = (1 << (has_delta_target_graph_pattern_vertex.size()));
  for (int mask = 1; mask < total_mask; mask++) {
    std::vector<QueryVertexHandle> this_mask_vertex;
    for (int bit_pos = 0;
             bit_pos < has_delta_target_graph_pattern_vertex.size(); 
             bit_pos++) {
      if (mask & (1 << bit_pos)) {
        this_mask_vertex.emplace_back(
            has_delta_target_graph_pattern_vertex[bit_pos]);
      }
    }
    CandidateSet copy_candidate_set{candidate_set};
    for (auto &[query_handle, target_list] : copy_candidate_set) {
      if (std::binary_search(this_mask_vertex.begin(), 
                             this_mask_vertex.end(),
                             query_handle)) {
        std::vector<TargetVertexHandle> this_vertex_target_list;
        for (auto &target_handle : target_list) {
          if (std::binary_search(delta_target_graph.begin(),
                                 delta_target_graph.end(), 
                                       target_handle)) {
            this_vertex_target_list.emplace_back(target_handle);
          }
        }
        std::swap(this_vertex_target_list, 
                              target_list);
        continue;
      }
      std::vector<TargetVertexHandle> this_vertex_target_list;
      for (auto &target_handle : target_list) {
        if (!std::binary_search(delta_target_graph.begin(),
                                delta_target_graph.end(), 
                                      target_handle)) {
          this_vertex_target_list.emplace_back(target_handle);
        }
      }
      std::swap(this_vertex_target_list, 
                            target_list);
    }

    std::function<bool(const Match<QueryGraph, TargetGraph>&)>
      prune_callback = [](const Match<QueryGraph, 
                                    TargetGraph>& match) -> bool{
      // prune nothing, continue matching
      return false;
    };
    Match<QueryGraph, 
         TargetGraph> match_state;
    _dp_iso_using_match::DPISOUsingMatch_Recursive<match_semantics>(
                query_graph, 
               target_graph,
                copy_candidate_set, 
                match_state,
                match_callback, prune_callback, query_limit_time);
  }
  return 1;
}

};  // namespace GUNDAM

#endif // _GUNDAM_ALGORITHM_DPISO_USING_MATCH_H
