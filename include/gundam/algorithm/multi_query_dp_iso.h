#ifndef _GUNDAM_ALGORITHM_MULTI_QUERY_DPISO_USING_MATCH_H
#define _GUNDAM_ALGORITHM_MULTI_QUERY_DPISO_USING_MATCH_H

#include "gundam/graph_type/graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/match/match.h"

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/algorithm/bfs.h"
#include "gundam/algorithm/dfs.h"

#include "gundam/algorithm/dp_iso.h"
#include "gundam/algorithm/dp_iso_using_match.h"

#include "gundam/tool/same_pattern.h"

namespace GUNDAM{

namespace _multi_query_dp_iso{

template <typename VertexHandle>
inline int GetPatternIdx(VertexHandle vertex_handle){
  return vertex_handle->id();
}

template <typename TriVertexPatternType>
class Tls{
 public:
  Tls(const TriVertexPatternType& tri_vertex_pattern)
             :tri_vertex_pattern_(tri_vertex_pattern){
    auto automorphism = DpisoUsingMatch(this->tri_vertex_pattern_,
                                        this->tri_vertex_pattern_);
    assert(automorphism == 1
        || automorphism == 2);
    if (automorphism == 2){
      this->symmetrical_ = true;
      return;
    }
    this->symmetrical_ = false;
    return;
  }

  TriVertexPatternType& tri_vertex_pattern(){
    return this->tri_vertex_pattern_;
  }

  const TriVertexPatternType& tri_vertex_pattern() const{
    return this->tri_vertex_pattern_;
  }
  
  inline bool symmetrical() const {
    return this->symmetrical_;
  }

  inline bool operator==(const Tls& tls) const {
    return SamePattern(this->tri_vertex_pattern_, 
                         tls.tri_vertex_pattern_);
  }

  inline bool operator!=(const Tls& tls) const {
    return !((*this) == tls);
  }

 private:
  TriVertexPatternType tri_vertex_pattern_;
  bool symmetrical_;
};

template <typename TriVertexPatternType>
class TlsSet{
 private:
  using TlsType = Tls<TriVertexPatternType>;

  using ContainerTlsContainerType 
      = Container<ContainerType::Vector,
                       SortType::Default,
                        TlsType>;

 public:
  using       iterator = typename ContainerTlsContainerType::      iterator;
  using const_iterator = typename ContainerTlsContainerType::const_iterator;

  TlsSet(){
    return;
  }

  inline std::pair<iterator, bool> AddTls(const TlsType& tls){
    return this->tls_set_.Insert(tls);
  }

  inline size_t Size() const{
    return this->tls_set_.size();
  }

  inline iterator TlsBegin() {
    return this->tls_set_.begin();
  }

  inline iterator TlsEnd() {
    return this->tls_set_.end();
  }

  inline const_iterator TlsBegin() const {
    return this->tls_set_.cbegin();
  }

  inline const_iterator TlsEnd() const {
    return this->tls_set_.cend();
  }

 private:
  ContainerTlsContainerType tls_set_;
};

template <typename     GraphPatternType,
          typename TriVertexPatternType>
size_t Li(GraphPatternType&  q,
   TlsSet<TriVertexPatternType>& tls_set){
  size_t li_q_tls = 0;
  for (auto tls_it  = tls_set.TlsBegin();
            tls_it != tls_set.TlsEnd();
            tls_it++) {
    auto automorphism = DpisoUsingMatch(tls_it->tri_vertex_pattern(), q);
    if (tls_it->symmetrical()) {
      assert(automorphism % 2 == 0);
      automorphism /= 2;
    }
    li_q_tls += automorphism;
  }
  return li_q_tls;
}

template <typename TriVertexPatternType>
TlsSet<TriVertexPatternType> Intersect(TlsSet<TriVertexPatternType>& tls_qi_set,
                                       TlsSet<TriVertexPatternType>& tls_qj_set){
  TlsSet<TriVertexPatternType> tls_qi_qj_set;
  
  for (auto tls_qi_it  = tls_qi_set.TlsBegin();
            tls_qi_it != tls_qi_set.TlsEnd();
            tls_qi_it++) {
    // to find whether this tls is contained in tls_qj_it
    auto& tls_qi = *tls_qi_it;
    for (auto tls_qj_it  = tls_qi_set.TlsBegin();
              tls_qj_it != tls_qi_set.TlsEnd();
              tls_qj_it++) {
      if (SamePattern(tls_qi_it->tri_vertex_pattern(),
                      tls_qj_it->tri_vertex_pattern())){
        // this tri_vertex_pattern is contained in both 
        // tls_qi_set and tls_qj_set
        // add into tls_qi_qj_set
        tls_qi_qj_set.AddTls(tls_qi_it->tri_vertex_pattern());
        break;
      }
    }
  }
  return tls_qi_qj_set;
}

template <typename TriVertexPatternType>
double Gf(TriVertexPatternType& qi, TlsSet<TriVertexPatternType>& tls_qi_set, size_t tls_qi_size,
          TriVertexPatternType& qj, TlsSet<TriVertexPatternType>& tls_qj_set, size_t tls_qj_size){

  TlsSet<TriVertexPatternType> tls_qi_qj_set 
                   = Intersect(tls_qi_set,
                               tls_qj_set);
  
  size_t li_qi_tls_qi_qj = Li(qi, tls_qi_qj_set);
  size_t li_qj_tls_qi_qj = Li(qj, tls_qi_qj_set);

  size_t numerator = li_qi_tls_qi_qj < li_qj_tls_qi_qj?
                     li_qi_tls_qi_qj : li_qj_tls_qi_qj;

  size_t denominator = tls_qi_size < tls_qj_size?
                       tls_qi_size : tls_qj_size;

  return ((double)numerator) / ((double)denominator);
}

template <typename     GraphPatternType,
          typename TriVertexPatternType>
void BuildTls(GraphPatternType&  q,
   TlsSet<TriVertexPatternType>& q_tls_set){
  assert(q_tls_set.Size() == 0);

  using VertexHandleType = typename VertexHandle<GraphPatternType>::type;
  using VertexIDType     = typename GraphPatternType::VertexType::IDType;

  std::vector<VertexIDType> vertex_id_set;
  vertex_id_set.reserve(q.CountVertex());
  for (auto vertex_it = q.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    vertex_id_set.emplace_back(vertex_it->id());
  }
  assert(vertex_id_set.size() == q.CountVertex());

  for (auto vertex_it = q.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    // enumerate the src vertex of the tri_vertex_pattern
    std::vector<VertexHandleType> tri_vertex_handle_set;
    tri_vertex_handle_set.resize(3, VertexHandleType());
    
    // construct the tri_vertex_pattern
    auto dfs_callback = [&tri_vertex_handle_set,
                         &q,
                         #ifndef NDEBUG
                         &vertex_it,
                         #endif // NDEBUG
                         &q_tls_set,
                         &vertex_id_set](
             VertexHandleType vertex_handle, int dfs_idx, 
                                             int dfs_depth){
      assert(tri_vertex_handle_set.size() == 3);
      assert(tri_vertex_handle_set.at(0) == vertex_it);
      assert(0 <= dfs_depth && dfs_depth <= 2);
      tri_vertex_handle_set.at(dfs_depth) = vertex_handle;
      if (dfs_depth < 2) {
        // does not need further process
        return true;
      }
      // has already get a tri-vertex tuple
      TriVertexPatternType tri_vertex_pattern(q);
      // get a tri-vertex pattern, obtain it from the graph pattern
      for (const auto& vertex_id : vertex_id_set) {
        bool in_tri_vertex_pattern = false;
        for (const auto& tri_vertex_handle
                       : tri_vertex_handle_set) {
          // should not be null
          assert(tri_vertex_handle);
          if (tri_vertex_handle->id() == vertex_id) {
            in_tri_vertex_pattern = true;
            break;
          }
        }
        if (in_tri_vertex_pattern) {
          // this vertex does not need to be removed
          continue;
        }
        // this vertex is not contained in the tri-vertex-pattern
        // need to be removed 
        auto erase_size_ret = tri_vertex_pattern.EraseVertex(vertex_id);
        // should have removed successfully
        assert(erase_size_ret == 1);
      }
      assert(tri_vertex_pattern.CountVertex() == 3);
      q_tls_set.AddTls(tri_vertex_pattern);
      return true;
    };
    // use bidir edge, does not remove duplicated vertex
    Dfs<true, false>(q, vertex_it, dfs_callback, 3);
  }
  return;
}

template <typename GraphPatternType>
void TLSGroupMatrix(std::vector<GraphPatternType>& query_graph_list,
                    std::vector<std::vector<std::pair<int, bool>>> clique_set,
                    double threshold){
  assert(threshold >= 0.0 && threshold <= 1.0);

  using CliqueVertexLabelType = int;
  using CliqueVertexIdType = int;

  using CliqueEdgeLabelType = int;
  using CliqueEdgeIdType = int;

  using CliqueVertexAttrKeyType = std::string;
  using CliqueEdgeAttrKeyType   = std::string;

  using CliqueGraphType = LargeGraph<CliqueVertexIdType, 
                                     CliqueVertexLabelType, 
                                     CliqueVertexAttrKeyType,
                                     CliqueEdgeIdType, 
                                     CliqueEdgeLabelType, 
                                     CliqueEdgeAttrKeyType>;
  using ClusterVertexHandleType = typename VertexHandle<CliqueGraphType>::type;

  CliqueGraphType m;

  constexpr CliqueVertexLabelType kDefaultVertexLabel = 0;
  constexpr   CliqueEdgeLabelType kDefaultEdgeLabel   = 0;
  const   CliqueVertexAttrKeyType kAddedToCliqueKey   = "add_to_key";

  using TriVertexPatternType = GraphPatternType;

  using TlsSetType = TlsSet<TriVertexPatternType>;
  std::vector<std::pair<TlsSetType, size_t>> query_graph_tls_set_list;
  query_graph_tls_set_list.reserve(query_graph_list.size());
  for (auto& q : query_graph_list) {
    TlsSetType q_tls_set;
    BuildTls(q, q_tls_set);
    auto li = Li(q, q_tls_set);
    query_graph_tls_set_list.emplace_back(std::move(q_tls_set), std::move(li));
  }

  for (int i = 0; i < query_graph_list.size(); i++) {
    auto [ vertex_handle, 
           vertex_ret ] = m.AddVertex(i, kDefaultVertexLabel);
    // should add successfully
    assert(vertex_ret);
  }

  CliqueEdgeIdType edge_id_counter = 0;
  for (int i = 0; i < query_graph_list.size(); i++) {
    for (int j = i + 1; j < query_graph_list.size(); j++) {
      double gf_qi_qj = Gf(query_graph_list[i], 
                           query_graph_tls_set_list[i].first,  
                           query_graph_tls_set_list[i].second,
                           query_graph_list[j], 
                           query_graph_tls_set_list[j].first,  
                           query_graph_tls_set_list[j].second);
      if (gf_qi_qj >= threshold) {
        // only add one direction would be enough
        m.AddEdge(i, j, kDefaultEdgeLabel, edge_id_counter);
        edge_id_counter++;
      }
    }
  }

  for (auto vertex_it = m.VertexBegin();
           !vertex_it.IsDone(); vertex_it++) {
    if (vertex_it->FindAttribute(kAddedToCliqueKey)) {
      // this vertex has already been added into the cluster
      continue;
    }
    // find 
    auto& clique = clique_set.emplace_back();
    auto dfs_callback = [&clique,
                         &kAddedToCliqueKey](ClusterVertexHandleType vertex_handle){
      // should not contain this vertex
      assert(!vertex_handle->FindAttribute(kAddedToCliqueKey));
      clique.emplace_back(vertex_handle->id(), true);
      auto [attr_handle,
            attr_ret] = vertex_handle->AddAttribute(kAddedToCliqueKey, 0);
      // should add successfully
      assert(attr_ret);
      return true;
    };
    Dfs<true>(vertex_it, m, dfs_callback);
    assert(!clique.empty());
  }
  return;
}

template <typename GraphPatternType,
          typename VertexHandle>
inline Match<GraphPatternType,
             GraphPatternType>&
    GetMatchToParent(VertexHandle vertex_handle) {
  const std::string kMatchToParentAttributeKey = "match_to_parent";
  using MatchType = Match<GraphPatternType,
                          GraphPatternType>;
  auto attr_handle = vertex_handle->FindAttribute(kMatchToParentAttributeKey);
  assert(attr_handle);
  return attr_handle->template value<MatchType>();
}

template <typename GraphPatternType,
          typename VertexHandle>
inline Match<GraphPatternType,
             GraphPatternType>&
    AddMatchToParent(VertexHandle vertex_handle,
                GraphPatternType& parent_graph,
                GraphPatternType&  child_graph) {

  const std::string kMatchToParentAttributeKey = "match_to_parent";

  using MatchType 
      = Match<GraphPatternType,
              GraphPatternType>;

  using MatchSetType 
      = MatchSet<GraphPatternType,
                 GraphPatternType>;

  MatchSetType match_set_parent_to_child;

  auto match_counter = DpisoUsingMatch(parent_graph,
                                        child_graph,
                          match_set_parent_to_child, 1);
  assert(match_counter == 1);
  assert(match_set_parent_to_child.size() == 1);
  
  MatchType match_child_to_parent(*(match_set_parent_to_child.MatchBegin()),
                                 "reverse");

  auto [attr_handle,
        attr_ret] = vertex_handle->AddAttribute(kMatchToParentAttributeKey,
                                                 match_child_to_parent);
  // should added successfully
  assert(attr_ret);
  return attr_handle->template value<MatchType>();
}

template <typename      PcmTreeType,
          typename GraphPatternType>
typename VertexHandle<PcmTreeType>::type 
             BuildPCM(PcmTreeType&  pcm_tree,
             // to store the query graph
     std::vector<GraphPatternType>&      query_graph_list,
             // to store the additional graph
     std::vector<GraphPatternType>& additional_graph_list) {

  using PcmTreeVertexLabelType = typename PcmTreeType::VertexType::LabelType;
  using   PcmTreeEdgeLabelType = typename PcmTreeType::  EdgeType::LabelType;

  const PcmTreeVertexLabelType kDefaultVertexLabel = 0;
  const   PcmTreeEdgeLabelType   kDefaultEdgeLabel = 0;

  const std::string kMatchToParentAttributeKey = "match_to_parent";

  assert(    !query_graph_list.empty());
  assert(additional_graph_list.empty());
  
  // // only to compile pass
  // for (int i = 0; i < query_graph_list.size(); i++) {
  //   auto [ vertex_handle,
  //          vertex_ret ] = pcm_tree.AddVertex(i, kDefaultVertexLabel);
  //   assert(vertex_handle);
  //   assert(vertex_ret);
  //   if (i == 0){
  //     continue;
  //   }
  //   Match match_from_pattern(query_graph_list[i - 1],
  //                            query_graph_list[i],
  //                            "same_id_map");
  //   vertex_handle->AddAttribute(kMatchToParentAttributeKey, match_from_pattern);
  //   pcm_tree.AddEdge(i - 1, i, kDefaultEdgeLabel, i - 1);
  // }
  // assert(pcm_tree.CountVertex() == query_graph_list.size());
  // auto root_handle = pcm_tree.FindVertex(0);
  // assert(root_handle);
  // return root_handle;

  std::vector<std::vector<std::pair<int, bool>>> cliques;

  constexpr double threshold = 0.5;

  TLSGroupMatrix(query_graph_list, cliques, threshold);

  for (const auto& clique : cliques) {
    // the next level group
    std::vector<std::pair<int, bool>> next_level_group;
    std::vector<std::pair<int, bool>> query_graph_id_set = clique;
    while (true) {
      assert(next_level_group.empty());
      assert(!query_graph_id_set.empty());
      while (query_graph_id_set.size() >= 2) {
        // randomly pick a pair of pattern in the remained
        // patterns in this clique
        size_t query_graph_id_set_idx_i  = rand() % query_graph_id_set.size();
        size_t query_graph_id_set_idx_j  = query_graph_id_set_idx_i;
        while (query_graph_id_set_idx_j == query_graph_id_set_idx_i){
          query_graph_id_set_idx_j  = rand() % query_graph_id_set.size();
        }
        assert(query_graph_id_set_idx_i
            != query_graph_id_set_idx_j);
        // has picked a random pair from current clique 
        std::pair<int, bool>
          query_graph_id_i = query_graph_id_set[query_graph_id_set_idx_i],
          query_graph_id_j = query_graph_id_set[query_graph_id_set_idx_j];

        // removed the picked pair from current clique
        query_graph_id_set.erase(query_graph_id_set.begin() 
                               + query_graph_id_set_idx_i);
        query_graph_id_set.erase(query_graph_id_set.begin() 
                               + query_graph_id_set_idx_j);

        auto& qi = GetPatternRef(query_graph_id_i, query_graph_list, 
                                              additional_graph_list);
        auto& qj = GetPatternRef(query_graph_id_j, query_graph_list,
                                              additional_graph_list);

        if (SubGraphOf(qi, qj)) {
          // qi is the subgraph of qj
          // add an edge from qj to qi
          auto [edge_handle, 
                edge_ret] = pcm_tree.AddEdge(query_graph_id_j,
                                             query_graph_id_i,
                                            kDefaultEdgeLabel,
                                             edge_counter++);
          assert(edge_ret);
          // and add qj into next_level_group 
          next_level_group.emplace_back(query_graph_id_j);
          continue;
        }
        if (SubGraphOf(qj, qi)) {
          // qj is the subgraph of qi
          // add an edge from qj to qi
          auto [edge_handle, 
                edge_ret] = pcm_tree.AddEdge(query_graph_id_i,
                                             query_graph_id_j,
                                            kDefaultEdgeLabel,
                                             edge_counter++);
          assert(edge_ret);
          // and add qi into next_level_group 
          next_level_group.emplace_back(query_graph_id_i);
          continue;
        }

        auto mcs_qi_qj_set = MaximalCommonSubgraph(qi, qj);
        assert(!mcs_qi_qj_set.empty());
        bool has_been_contained = false;
        for (const auto& mcs : mcs_qi_qj_set) {
          // to find whether the mcs has already been contained
          // in this clique or next_level_group
          for (int query_graph_id_set_idx = 0;
                   query_graph_id_set_idx < query_graph_id_set.size();
                   query_graph_id_set_idx++){
            const auto& query_graph_id 
                      = query_graph_id_set[query_graph_id_set_idx];
            // find in this clique to find a new
            auto& query_graph = GetPatternRef(query_graph_id, 
                                              query_graph_list, 
                                              additional_graph_list);
            if (SamePattern(query_graph, mcs)) {
              // add an edge from query_graph_id to qi and qj
              auto [edge_handle_i, 
                    edge_ret_i] = pcm_tree.AddEdge(query_graph_id,
                                                   query_graph_id_i,
                                                  kDefaultEdgeLabel,
                                                   edge_counter++);
              assert(edge_ret_i);
              auto [edge_handle_j, 
                    edge_ret_j] = pcm_tree.AddEdge(query_graph_id,
                                                   query_graph_id_j,
                                                  kDefaultEdgeLabel,
                                                   edge_counter++);
              assert(edge_ret_j);
              // remove query_graph_id from 
              query_graph_id_set.erase(query_graph_id_set.begin() 
                                     + query_graph_id_set_idx);
              // and add query_graph_id into next_level_group 
              next_level_group.emplace_back(query_graph_id);
              has_been_contained = true;
              break;
            }
          }
          if (has_been_contained){
            continue;
          }
        }
        // if 
        if (!has_been_contained) {

        }
      }
      assert(query_graph_id_set.size() == 0
          || query_graph_id_set.size() == 1);
      
      if (query_graph_id_set.size() == 1){
        // there still one pattern remained in this clique
        // add it into the next_level_group
        next_level_group.emplace(*query_graph_id_set.begin());
      }
      if (next_level_group.size() == 1) {
        // does not need further grouping
        break;
      }
      query_graph_id_set.clear();
      next_level_group.swap(query_graph_id_set);
    }
    
    if () {

    }
    
    auto query_graph_id_set = query_graph_id_set;
    for (auto query_graph_it  = query_graph_id_set.begin();
              query_graph_it != query_graph_id_set.end();
              query_graph_it++) {
      // enumerate the pattern pairs in this clique
      // find mcs of them, if the mcs does not exist in
      // the current clique, then add it into the additional_graph_list
      next_level_group.clear();
      auto query_graph_id = *query_graph_it;
      while (true) {
        // the 
        for (auto current_query_graph_id_it  = current_query_graph_id.begin();
                  current_query_graph_id_it != current_query_graph_id.end();){
          auto next_current_query_graph_id_it = current_query_graph_id_it;
          next_current_query_graph_id_it++;
          if (next_current_query_graph_id_it 
                == current_query_graph_id.end()){
            // is odd
            assert(current_query_graph_id.size() % 2 == 1);
            next_level_group.emplace_back(*current_query_graph_id_it);
            continue;
          }
          auto qi_idx =      *current_query_graph_id_it;
          auto qj_idx = *next_current_query_graph_id_it;

          auto& qi;
          auto& qj;

          auto mcs_qi_qj = MaximalCommonSubgraph(qi, qj);

          // these two pattern does not have common subgraph
          if (mcs_qi_qj.empty()){
            continue;
          }

          for (auto ){

          }

          // add one for them to be added 
          for (auto vertex_it = pcm_tree;) {
            
          }
          for (){

          }
          current_query_graph_id_it = ++next_current_query_graph_id_it;
        }

        if (next_level_group.size() <= 1){
          break;
        }
        // next_level_group.size() > 1
        current_query_graph_id.swap(next_level_group);
      }
    }
  }
  return VertexHandle();
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          typename PatternTree,
          typename  QueryGraph,
          typename TargetGraph>
bool MatchFromParentToChild(
            PatternTree& pcm_tree,
   typename VertexHandle<PatternTree>::type current_pattern_handle,
                    Match<QueryGraph, 
                         TargetGraph> match,
     std::vector<
        std::map<typename VertexHandle< QueryGraph>::type,
     std::vector<typename VertexHandle<TargetGraph>::type>>>& candidate_set_list,
                            std::vector<QueryGraph>& query_graph_list,
                            std::vector<bool> call_match_callback,
              std::vector<bool> call_child_pattern_match_callback,
                                         TargetGraph& target_graph,
  std::function<bool(int,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> prune_callback,
  std::function<bool(int,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> match_callback){

  using PcmTreeVertexHandle = typename VertexHandle<PatternTree>::type;
  using   QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using  TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using CandidateSetType = std::map<QueryVertexHandle,
                       std::vector<TargetVertexHandle>>;

  using MatchMap = std::map<QueryVertexHandle,
                           TargetVertexHandle>;

  using MatchPatternToPatternType   = Match<QueryGraph,  QueryGraph>;
  using MatchPatternToDataGraphType = Match<QueryGraph, TargetGraph>;

  assert(candidate_set_list.size()
        == query_graph_list.size());

  assert(call_match_callback.size()
         == query_graph_list.size());

  int current_pattern_idx = GetPatternIdx(current_pattern_handle);
  assert(current_pattern_idx >= 0
      && current_pattern_idx < candidate_set_list.size());

  // the hold the current match from parent pattern to child pattern
  // the match from parent patten to data graph can be viewed as a
  // parital match from the child pattern to data graph                              
  auto& current_candidate_set = candidate_set_list[current_pattern_idx];
  auto&   current_query_graph =   query_graph_list[current_pattern_idx];

  // for each pattern, 
  // the current match need to be prune if and only if the prune_callback
  // of it and all its children return true
  auto current_pattern_prune_callback 
   = [&current_pattern_handle,
             &prune_callback,
             &pcm_tree](const MatchMap& match){
    // one-direction bfs at this vertex, if meet one prune_callback return false,
    // then end matching
    bool all_prune_return_true = true;
    auto bfs_callback = [&all_prune_return_true,
                         &prune_callback,
                         &match,
                         &pcm_tree](PcmTreeVertexHandle pcm_tree_vertex_handle) {
      auto pattern_idx = GetPatternIdx(pcm_tree_vertex_handle);
      if (!prune_callback(pattern_idx, match)) {
        // has found a pattern that cannot prune at this 
        // partial match, mark all_prune_return_true as false
        // and end the bfs process
        all_prune_return_true = false;
        return false;
      }
      return true;
    };

    Bfs(pcm_tree, current_pattern_handle, bfs_callback);

    return all_prune_return_true;
  };

  // for each pattern, hold a flage to store whether the return value
  // has been true
  // and another flage stores whether all its children return true,
  // return false if and only if the match callback of this pattern
  // and all its child pattern return false
  // if the match callback for this pattern has already been marked
  // as false, then do not call the match callback for this pattern
  // and only all the child pattern that has not been marked as false
  auto current_pattern_match_callback 
   = [&current_pattern_idx,
      &current_pattern_handle,
        &candidate_set_list,
          &query_graph_list,
        &pcm_tree,
        &call_match_callback,
        &call_child_pattern_match_callback,
        &target_graph,
        &prune_callback,
        &match_callback](const MatchMap& match){
      
    assert(call_child_pattern_match_callback[current_pattern_idx]);
    
    // call match_callback only when the flage for this pattern
    // is not marked as false
    if (call_match_callback[current_pattern_idx]){
        call_match_callback[current_pattern_idx]
           = match_callback(current_pattern_idx, match);
    }

    bool all_child_pattern_does_not_need_to_be_called = true;
    // get a match from parent pattern to child pattern
    for (auto child_pattern_it = current_pattern_handle->OutVertexBegin();
             !child_pattern_it.IsDone();
              child_pattern_it++) {

      assert(candidate_set_list.size()
            == query_graph_list.size());

      int child_pattern_idx = GetPatternIdx(child_pattern_it);
      assert(child_pattern_idx >= 0
          && child_pattern_idx < candidate_set_list.size());

      if (!call_child_pattern_match_callback[child_pattern_idx]) {
        // the match_callback of this child pattern
        // does not need to be called 
        continue;
      }

      MatchPatternToPatternType child_to_parent_match 
             = GetMatchToParent<QueryGraph,
                                QueryGraph>(child_pattern_it);

      MatchPatternToDataGraphType parent_to_target_graph_match;

      MatchPatternToDataGraphType child_to_target_graph_partial_match
                               = parent_to_target_graph_match(
                                  child_to_parent_match);
      
      bool child_match_ret = MatchFromParentToChild(
                              pcm_tree,
                              child_pattern_it, 
                              child_to_target_graph_partial_match, 
                              candidate_set_list,
                                query_graph_list,
                              call_match_callback,
                call_child_pattern_match_callback,
                                    target_graph,
                                  prune_callback,
                                  match_callback);

      if (call_match_callback[child_pattern_idx]){
        all_child_pattern_does_not_need_to_be_called = false;
      }
    }

    assert(call_child_pattern_match_callback[current_pattern_idx]);

    call_child_pattern_match_callback[current_pattern_idx]
                = call_match_callback[current_pattern_idx]
                  && all_child_pattern_does_not_need_to_be_called;

    return call_child_pattern_match_callback[current_pattern_idx];
  };

  return DpisoUsingMatch(current_query_graph,
                                target_graph,
                                match,
                         current_candidate_set,
                         current_pattern_prune_callback,
                         current_pattern_match_callback);
}

};

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          typename  QueryGraph,
          typename TargetGraph>
inline void MultiQueryDpiso(
  std::vector<QueryGraph>&  query_graph_list,
             TargetGraph & target_graph,
  std::function<bool(int,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> prune_callback,
  std::function<bool(int,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> match_callback,
   double time_limit = -1.0) {

  using VertexIDType = uint32_t;
  using   EdgeIDType = uint32_t;

  using VertexLabelType = uint32_t;
  using   EdgeLabelType = uint32_t;
  
  using PcmTreeType = GUNDAM::LargeGraph2<VertexIDType, VertexLabelType, std::string,
                                            EdgeIDType,   EdgeLabelType, std::string>;

  PcmTreeType pcm_tree;
  auto root_handle = _multi_query_dp_iso::BuildPCM(pcm_tree, query_graph_list);
  assert(root_handle);

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using CandidateSetType = std::map<QueryVertexHandle, 
                       std::vector<TargetVertexHandle>>;

  using MatchMap = std::map<QueryVertexHandle,
                           TargetVertexHandle>;

  std::vector<CandidateSetType> candidate_set_list;
  candidate_set_list.reserve(query_graph_list.size());
  // pre-process the candidate set from each pattern to data graph
  // in advance
  for (auto& query_graph : query_graph_list){

    using CandidateSetType = std::map<QueryVertexHandle, 
                         std::vector<TargetVertexHandle>>;

    CandidateSetType candidate_set;
    if (!_dp_iso::InitCandidateSet<match_semantics>(query_graph,
                                                  target_graph,
                                                    candidate_set)) {
      // there is a bug in Dpiso if the candidate set is empty
      assert(false);
      candidate_set_list.emplace_back();
      continue;
    }
    if (!_dp_iso::RefineCandidateSet(query_graph, 
                                    target_graph, 
                                    candidate_set)) {
      // there is a bug in Dpiso if the candidate set is empty
      assert(false);
      candidate_set_list.emplace_back();
      continue;
    }
    candidate_set_list.emplace_back(std::move(candidate_set));
  }

  assert(candidate_set_list.size()
        == query_graph_list.size());

      // to mark whether the match_callback of this pattern need to be called
  std::vector<bool> call_match_callback(query_graph_list.size(), true),
      // to mark whether the match_callback for the child patterns need to be called
      call_child_pattern_match_callback(query_graph_list.size(), true);
  
  Match<QueryGraph, TargetGraph> match;

  _multi_query_dp_iso::MatchFromParentToChild(
                         pcm_tree,
                         root_handle, match, 
                         candidate_set_list,
                           query_graph_list,
                        call_match_callback,
          call_child_pattern_match_callback,
                               target_graph,
                             prune_callback,
                             match_callback);
  return;
}

};

#endif // _GUNDAM_ALGORITHM_MULTI_QUERY_DPISO_USING_MATCH_H