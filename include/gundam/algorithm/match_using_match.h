#ifndef _GUNDAM_ALGORITHM_DPISO_USING_MATCH_H
#define _GUNDAM_ALGORITHM_DPISO_USING_MATCH_H
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

#include "gundam/algorithm/dp_iso.h"
#include "gundam/algorithm/vf2.h"

#include "gundam/match/match.h"

#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {

enum MergeNecConfig : uint8_t {
  kMerge,    // merge if there exists nec
  kNotMerge, // not merge even if there exists nec
  kAdaptive  // depends on the size of data graph
};

namespace _match_using_match {

enum EdgeState { kIn, kOut };

static constexpr int kLargeGraphSize = 1000;

template<typename    EdgeLabelType,
         typename     VertexIDType,
         typename VertexHandleType>
auto AdjacentVertex(const VertexHandleType& vertex) {
  std::set<std::tuple<EdgeState,
                      EdgeLabelType,
                       VertexIDType>> adjacent_vertex_set;
  for (auto in_edge_it = vertex->InEdgeBegin();
           !in_edge_it.IsDone();
            in_edge_it++) {
    adjacent_vertex_set.emplace(EdgeState::kIn,
                             in_edge_it->label(),
                             in_edge_it->src_handle()->id());
  }
  for (auto out_edge_it = vertex->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    adjacent_vertex_set.emplace(EdgeState::kOut,
                            out_edge_it->label(),
                            out_edge_it->dst_handle()->id());
  }
  return std::move(adjacent_vertex_set);
}

template <typename  QueryGraph,
          typename TargetGraph>
std::vector<std::vector<typename VertexHandle<QueryGraph>::type>> 
                Nec(QueryGraph& graph,
        const Match<QueryGraph,
                   TargetGraph>& partial_match) {
  using VertexHandleType = typename VertexHandle<QueryGraph>::type;
  using VertexLabelType = typename QueryGraph::VertexType::LabelType;
  using   EdgeLabelType = typename QueryGraph::  EdgeType::LabelType;
  using    VertexIDType = typename QueryGraph::VertexType::   IDType;
  std::map<std::pair<VertexLabelType,
           std::set<std::tuple<EdgeState,
                               EdgeLabelType,
                               VertexIDType>>>,
           std::vector<VertexHandleType>> adjacent_vertex_set;

  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    // does not consider those vertexes that have already been mapped
    // on the target graph
    if (partial_match.HasMap(vertex_it)) {
      continue;
    }
    VertexHandleType vertex_handle = vertex_it;
    auto adjacent_vertex = AdjacentVertex<EdgeLabelType, VertexIDType>(vertex_handle);
    // add if not exist
    adjacent_vertex_set[std::pair(vertex_handle->label(),
                                  adjacent_vertex)].emplace_back(vertex_it);
  }

  std::vector<std::vector<VertexHandleType>> nec_set;
  for (const auto& adjacent_vertex
                 : adjacent_vertex_set) {
    assert(!adjacent_vertex.second.empty());
    if (adjacent_vertex.second.size() == 1) {
      continue;
    }
    nec_set.emplace_back(adjacent_vertex.second);
  }
  return std::move(nec_set);
}

template <typename  QueryGraph,
          typename TargetGraph>
QueryGraph RemoveNecVertex(
                     QueryGraph& query_graph,
         const Match<QueryGraph,
                    TargetGraph>& partial_match) {
  auto nec_set = Nec(query_graph, partial_match);
  std::remove_const_t<QueryGraph> query_graph_removed(query_graph);
  for (const auto& nec : nec_set){
    assert(nec.size() >= 2);
    auto vertex_remove_begin_it = nec.begin();
    assert(vertex_remove_begin_it != nec.end());
    // preserve the first one in nec
    while (true) {
      vertex_remove_begin_it++;
      if (vertex_remove_begin_it == nec.end()){
        break;
      }
      auto erase_vertex_ret = query_graph_removed.EraseVertex(
                                 (*vertex_remove_begin_it)->id());
      assert(erase_vertex_ret);
    }
  }
  return std::move(query_graph_removed);
}

};

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const Match<QueryGraph,
             TargetGraph>& partial_match,
  const std::map<typename VertexHandle< QueryGraph>::type,
     std::vector<typename VertexHandle<TargetGraph>::type>>& candidate_set,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0) {

  using namespace _match_using_match;

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using  QueryEdgeHandle = typename EdgeHandle< QueryGraph>::type;
  using TargetEdgeHandle = typename EdgeHandle<TargetGraph>::type;

  using MatchMap = std::map<QueryVertexHandle, 
                           TargetVertexHandle>;

  using MatchType = Match<QueryGraph, TargetGraph>;

  auto prune_callback_using_map 
   = [&prune_callback](const MatchMap& match_state) -> bool {
    Match<QueryGraph, TargetGraph> match;
    for (const auto& map : match_state){
      auto ret = match.AddMap(map.first, map.second);
      assert(ret);
    }
    return prune_callback(match);
  };

  auto match_callback_using_map 
   = [&match_callback](const MatchMap& match_state) -> bool {
    Match<QueryGraph, TargetGraph> match;
    for (const auto& map : match_state){
      auto ret = match.AddMap(map.first, map.second);
      assert(ret);
    }
    return match_callback(match);
  };

  using CandidateSetType = std::map<typename VertexHandle< QueryGraph>::type,
                        std::vector<typename VertexHandle<TargetGraph>::type>>;
  
  #ifndef NDEBUG
  for (const auto& candidate : candidate_set) {
    assert(candidate.first);
    assert(candidate.first == query_graph.FindVertex(candidate.first->id()));
  }

  for (auto map_it = partial_match.MapBegin();
           !map_it.IsDone();
            map_it++) {
    assert(map_it->src_handle());
    assert(map_it->src_handle() ==  query_graph.FindVertex(map_it->src_handle()->id()));
    assert(map_it->dst_handle());
    assert(map_it->dst_handle() == target_graph.FindVertex(map_it->dst_handle()->id()));
  }
  #endif // NDEBUG

  // refine at each partial match could slow the matching algorithem
  // CandidateSetType refined_candidate_set 
  //                        = candidate_set;
  // if (!partial_match.empty()) {
  //   if (!GUNDAM::_dp_iso::RefineCandidateSet(
  //                  query_graph,
  //                 target_graph,
  //                 refined_candidate_set,
  //                 partial_match)) {
  //     return 0;
  //   }
  // }

  if (merge_nec_config == MergeNecConfig::kMerge
   ||(merge_nec_config == MergeNecConfig::kAdaptive
   && target_graph.CountVertex() >= _match_using_match::kLargeGraphSize)) {
    QueryGraph query_graph_removed = RemoveNecVertex(query_graph, partial_match);
    if (query_graph_removed.CountVertex()
              < query_graph.CountVertex()) {
      // have vertex to merge
      Match<QueryGraph,
            QueryGraph> query_graph_removed_to_query_graph(
                        query_graph_removed,   query_graph,
                        "same_id_map");

      std::function<bool(const MatchType&)> 
           prune_callback_from_removed 
       = [&prune_callback,
          &query_graph_removed_to_query_graph](
            const MatchType& match_state_from_removed){
        MatchType match_from_query_graph 
                = match_state_from_removed(
          query_graph_removed_to_query_graph.Reverse());
             
        return prune_callback(match_from_query_graph);
      };

      bool match_callback_return = true;
      std::function<bool(const MatchType&)> 
           match_callback_save_return 
       = [&match_callback,
          &match_callback_return](const MatchType& match_state_from_query_graph){
        match_callback_return = match_callback(match_state_from_query_graph);
        return match_callback_return;
      };

      size_t match_counter = 0;

      std::function<bool(const MatchType&)> 
           match_callback_from_removed 
       = [&prune_callback,
          &match_callback_save_return,
          &match_callback_return,
          &match_counter,
          &target_graph,
           &query_graph,
           &query_graph_removed_to_query_graph,
          &candidate_set,
          &time_limit](
            const MatchType& partial_match_from_query_graph){

        const CandidateSetType& candidate_set_from_query_graph = candidate_set;

        match_counter += MatchUsingMatch<match_semantics,
                                         match_algorithm,
                                         MergeNecConfig::kNotMerge>(
                                           query_graph,
                                          target_graph,
                        partial_match_from_query_graph,
                        candidate_set_from_query_graph,
                                        prune_callback,
                                        match_callback_save_return,
                                         time_limit);

        return match_callback_return;
      };

      MatchMap match_state;
      if (!partial_match.empty()){
        for (auto vertex_it = query_graph.VertexBegin(); 
                !vertex_it.IsDone();
                  vertex_it++) {
          const QueryVertexHandle src_handle = vertex_it;
          if (!partial_match.HasMap(src_handle)) {
            continue;
          }
          const auto dst_handle = partial_match.MapTo(src_handle);
          assert(dst_handle);
          match_state.emplace(src_handle, dst_handle);
        }
        assert(match_state.size() == partial_match.size());
      }

      MatchType partial_match_from_removed
              = partial_match(
                       query_graph_removed_to_query_graph);

      CandidateSetType candidate_set_from_removed;

      Match query_graph_to_query_graph_removed(
            query_graph,   query_graph_removed,
            "same_id_map");

      for (const auto& candidate_it : candidate_set){
        auto vertex_handle_in_removed 
                = query_graph_to_query_graph_removed.MapTo(candidate_it.first);
        if (!vertex_handle_in_removed){
          assert(!query_graph_removed.FindVertex(candidate_it.first->id()));
          continue;
        }
        candidate_set_from_removed.emplace(vertex_handle_in_removed,
                                           candidate_it.second);
      }

      MatchUsingMatch<match_semantics,
                      match_algorithm,
                      MergeNecConfig::kNotMerge>(
                      query_graph_removed,
                     target_graph, 
                     partial_match_from_removed,
                     candidate_set_from_removed,
                    prune_callback_from_removed,
                    match_callback_from_removed,
                     time_limit);

      return match_counter;
    }
  }

  MatchMap match_state;
  if (!partial_match.empty()){
    for (auto vertex_it = query_graph.VertexBegin(); 
             !vertex_it.IsDone();
              vertex_it++) {
      const QueryVertexHandle src_handle = vertex_it;
      if (!partial_match.HasMap(src_handle)) {
        continue;
      }
      const auto dst_handle = partial_match.MapTo(src_handle);
      assert(dst_handle);
      match_state.emplace(src_handle, dst_handle);
    }
    assert(match_state.size() == partial_match.size());
  }

  CandidateSetType temp_candidate_set = candidate_set;

  if constexpr (match_algorithm
              == MatchAlgorithm::kDagDp) {
    return _dp_iso::DPISO_Recursive<match_semantics>(
                       query_graph,
                      target_graph,
                      temp_candidate_set, 
                        match_state, 
                        match_callback_using_map,
                        prune_callback_using_map,
                        time_limit);
  }
  else if constexpr (match_algorithm
                   == MatchAlgorithm::kVf2) {
    size_t match_counter = 0;
    auto user_callback_using_map
    = [&prune_callback_using_map,
       &match_callback_using_map,
       &match_counter](const MatchMap& match_state){
      if (prune_callback_using_map(match_state)){
        // should have been pruned before, should not call this match_state
        // at begin
        return true;
      }
      match_counter++;
      return match_callback_using_map(match_state);
    };

    VF2(query_graph, 
       target_graph,
       temp_candidate_set,
        match_state,
      _vf2::LabelEqual<QueryEdgeHandle, 
                      TargetEdgeHandle>(), 
       user_callback_using_map);
    return match_counter;
  }
  else{
    // trick the compiler, equal to static_assert(false)
    static_assert(match_algorithm
              == MatchAlgorithm::kDagDp, "unsupported match algorithm");
    static_assert(match_algorithm
              != MatchAlgorithm::kDagDp, "unsupported match algorithm");
  }
  return 0;
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  const std::map<typename VertexHandle< QueryGraph>::type, 
     std::vector<typename VertexHandle<TargetGraph>::type>>& candidate_set,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0) {

  Match<QueryGraph, TargetGraph> match_state;

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(
                         query_graph,
                        target_graph,
                         match_state,
                       candidate_set,
                      prune_callback,
                      match_callback,
                          time_limit);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const Match<QueryGraph, 
             TargetGraph>& partial_match,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0) {

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using CandidateSetType = std::map<QueryVertexHandle, 
                       std::vector<TargetVertexHandle>>;

  CandidateSetType candidate_set;
  if (!_dp_iso::InitCandidateSet<match_semantics>(query_graph,
                                                 target_graph,
                                                  candidate_set)) {
    return 0;
  }
  if (!_dp_iso::RefineCandidateSet(query_graph, 
                                  target_graph, 
                                   candidate_set)) {
    return 0;
  }

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(
                         query_graph,
                        target_graph,
                       partial_match,
                       candidate_set,
                      prune_callback,
                      match_callback,
                          time_limit);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const Match<QueryGraph, TargetGraph>& partial_match,
   int64_t max_match = -1,
   double time_limit = -1.0) {
                           
  using MatchType = Match<QueryGraph, TargetGraph>;

  std::function<bool(const MatchType&)>
    prune_callback = [](const MatchType& match) -> bool{
    // prune nothing, continue matching
    return false;
  };

  int64_t match_counter = 0;

  std::function<bool(const MatchType&)>
    match_callback = [&max_match,
                      &match_counter](const MatchType& match) -> bool {
    if (max_match == -1) {
      // does not have support nothing
      // do nothing continue matching
      return true;
    }
    match_counter++;
    if (match_counter >= max_match){
      // reach max match, end matching
      return false;
    }
    // does not reach max match, continue matching
    return true;
  };

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(
                         query_graph,
                        target_graph,
                       partial_match,
                      prune_callback,
                      match_callback,
                          time_limit);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0) {

  Match<QueryGraph, TargetGraph> partial_match;

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(
                         query_graph,
                        target_graph,
                       partial_match,
                      prune_callback,
                      match_callback,
                          time_limit);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
   int64_t max_match = -1,
   double time_limit = -1.0) {

  using MatchType = Match<QueryGraph, TargetGraph>;

  std::function<bool(const MatchType&)>
    prune_callback = [](const MatchType& match) -> bool{
    // prune nothing, continue matching
    return false;
  };

  int64_t match_counter = 0;

  std::function<bool(const MatchType&)>
    match_callback = [&max_match,
                      &match_counter](const MatchType& match) -> bool{
    if (max_match == -1) {
      // does not have support nothing
      // do nothing continue matching
      return true;
    }
    match_counter++;
    if (match_counter >= max_match){
      // reach max match, end matching
      return false;
    }
    // does not reach max match, continue matching
    return true;
  };

  // the initial partial match is empty
  MatchType match_state;

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(
                         query_graph,
                        target_graph,
                         match_state,
                      prune_callback,
                      match_callback, 
                          time_limit);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  const Match<QueryGraph, TargetGraph>& partial_match,
     MatchSet<QueryGraph, TargetGraph>& match_result,
   int64_t max_match = -1,
   double time_limit = -1.0){

  using MatchType = Match<QueryGraph, TargetGraph>;

  std::function<bool(const MatchType&)>
    prune_callback = [](const MatchType& match){
    // prune nothing, continue matching
    return false;
  };

  std::function<bool(const MatchType&)>
    match_callback = [&match_result,
                         &max_match](const MatchType& match){
    // add this match to match_result, continue matching
    match_result.AddMatch(match);
    if ((max_match != -1) && (match_result.size() >= max_match)){
      // reach max match, end matching
      return false;
    }
   return true;
  };

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(query_graph, 
                                          target_graph,
                                         partial_match,
                                          prune_callback,
                                          match_callback);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  MatchSet<QueryGraph, TargetGraph>& match_result,
   int64_t max_match = -1,
   double time_limit = -1.0) {

  using MatchType = Match<QueryGraph, TargetGraph>;

  std::function<bool(const Match<QueryGraph, TargetGraph>&)>
    prune_callback = [](const Match<QueryGraph, 
                                   TargetGraph>& match) -> bool{
    // prune nothing, continue matching
    return false;
  };

  std::function<bool(const Match<QueryGraph, TargetGraph>&)>
    match_callback = [&match_result,
                         &max_match](const Match<QueryGraph, 
                                                TargetGraph>& match) -> bool{
    // add this match to match_result, continue matching
    match_result.AddMatch(match);
    if ((max_match != -1) && (match_result.size() >= max_match)){
      // reach max match, end matching
      return false;
    }
   return true;
  };

  // the initial partial match is empty
  MatchType match_state;

  return MatchUsingMatch<match_semantics,
                         match_algorithm,
                         merge_nec_config>(
                         query_graph,
                        target_graph,
                         match_state,
                      prune_callback,
                      match_callback, 
                          time_limit);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          typename    QueryGraph, 
          typename   TargetGraph>
inline int IncrementalMatchUsingMatch(
                      QueryGraph &query_graph, 
                     TargetGraph &target_graph,
     std::vector<typename VertexHandle<TargetGraph>::type> &delta_target_graph,
         std::function<bool(const Match<QueryGraph, 
                                       TargetGraph>&)> match_callback,
                           double query_limit_time = -1){
  static_assert(match_algorithm
             == MatchAlgorithm::kDagDp, "unsupported match algorithm for incremental match");

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using MatchMap = std::map<QueryVertexHandle, 
                           TargetVertexHandle>;

  using MatchType = Match<QueryGraph, TargetGraph>;

  auto match_callback_using_map 
   = [&match_callback](const MatchMap& match) {
    MatchType new_match;
    for (const auto& map : match){
      auto ret = new_match.AddMap(map.first, map.second);
      // should added successfully
      assert(ret);
    }
    return match_callback(new_match);
  };

  return IncreamentDPISO<match_semantics>(
                         query_graph, 
                        target_graph,
                  delta_target_graph,
                         match_callback_using_map,
                         query_limit_time);
}

}  // namespace GUNDAM

#endif //_DPISO_USING_MATCH_H
