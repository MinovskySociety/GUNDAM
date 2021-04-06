#ifdef _GUNDAM_ALGORITHM_MULTI_QUERY_DPISO_USING_MATCH_H
#define _GUNDAM_ALGORITHM_MULTI_QUERY_DPISO_USING_MATCH_H

namespace GUNDAM{

namespace _multi_query_dp_iso{

template<typename VertexHandle>
int GetPatternIdx(VertexHandle vertex_handle){
  const std::string kPatternIdAttributeKey = "pattern_idx";
  auto attr_handle = vertex_handle->FindAttribute(kPatternIdAttributeKey);
  assert(attr_handle);
  return attr_handle->template value<int>();
}

};

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          typename PatternTreeGraph,
          typename       QueryGraph,
          typename      TargetGraph>
bool MatchFromParentToChild(
            typename VertexHandle<PatternTreeGraph>::type current_pattern_handle,
        std::map<typename VertexHandle< QueryGraph>::type,
                 typename VertexHandle<TargetGraph>::type>& match,
     std::vector<
        std::map<typename VertexHandle< QueryGraph>::type,
     std::vector<typename VertexHandle<TargetGraph>::type>>>& candidate_set_list,
                            std::vector<QueryGraph>& query_graph_list,
                            std::vector<bool>& call_match_callback,
                                       TargetGraph& target_graph,
  std::function<bool(int,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> prune_callback,
  std::function<bool(int,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> match_callback){

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using CandidateSetType = std::map<QueryVertexHandle,
                       std::vector<TargetVertexHandle>>;

  using MatchMap = std::map<QueryVertexHandle,
                           TargetVertexHandle>;

  assert(candidate_set_list.size()
        == query_graph_list.size());

  int current_pattern_idx = GetPatternIdx(current_pattern_handle);
  assert(current_pattern_idx >= 0
      && current_pattern_idx < candidate_set_list.size());

  // the hold the current match from parent pattern to child pattern
  // the match from parent patten to data graph can be viewed as a
  // parital match from the child pattern to data graph                              
  auto& current_candidate_set = candidate_set_list[current_pattern_idx];
  auto&   current_query_graph =   query_graph_list[current_pattern_idx];

  auto current_pattern_prune_callback 
   = [&current_pattern_idx,
        &child_pattern_idx_set,
        &prune_callback](const MatchMap& match){

    if (!call_match_callback[current_pattern_idx]) {
      // already marked as not call
      return false;
    }
    
    if (prune_callback(current_pattern_idx, match)){
      call_match_callback[current_pattern_idx] = false;
      bool all_child_pattern_does_not_need_to_be_called = true;
      for (const auto& child_pattern_idx
                     : child_pattern_idx_set){
        if (call_match_callback[child_pattern_idx]){
          all_child_pattern_does_not_need_to_be_called = false;
          break;
        }
      }
      // would be true if it has no children or the match callback
      // for all children does not need to call
      if (all_child_pattern_does_not_need_to_be_called){
        // the match callback of child patten has been called
        // this match can be pruned
        return true;
      }
    }
    return false;
  };

  auto current_pattern_match_callback 
   = [&current_pattern_idx,
        &child_pattern_idx_set,
      &candidate_set_list,
        &query_graph_list,
      &prune_callback,
      &match_callback](const MatchMap& match){
                            
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
      
      auto& child_pattern = child_pattern_list[child_pattern_idx];

      MatchPatternToPatternType child_to_parent_match;

      MatchPatternToDataGraphType parent_to_data_graph_match;

      MatchPatternToDataGraphType child_to_data_graph_partial_match
                                = parent_to_data_graph_match(
                                  child_to_parent_match);
      
      bool child_match_ret = MatchFromParentToChild(child_pattern,
                                                    child_to_data_graph_partial_match, 
                                                            data_graph,
                                                    match_callback); 
      if (call_match_callback[child_pattern_idx]){
        all_child_pattern_does_not_need_to_be_called = false;
      }
    }

    if (!call_match_callback[current_pattern_idx]
      && all_child_pattern_does_not_need_to_be_called){
      // the match callback for this pattern does not need to be called
      return false;
    }
    return true;
  };

  return DpisoUsingMatch(current_query_graph,
                                target_graph,
                         current_candidate_set,
                         current_pattern_prune_callback,
                         current_pattern_match_callback);
}

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          typename  QueryGraph,
          typename TargetGraph>
inline void DpisoUsingMatch(
  std::vector<QueryGraph>&  query_graph_list,
             TargetGraph & target_graph,
  std::function<bool(QueryGraph& query_graph,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> prune_callback,
  std::function<bool(QueryGraph& query_graph,
                     const std::map<typename VertexHandle< QueryGraph>::type, 
                                    typename VertexHandle<TargetGraph>::type>&)> match_callback,
   double time_limit = -1.0) {

  using PcmTreeType = Graph;
  PcmTreeType pcm_tree;
  auto root_handle = BuildPCM(pcm_tree, tree);
  assert(root_handle);

  using  QueryVertexHandle = typename VertexHandle< QueryGraph>::type;
  using TargetVertexHandle = typename VertexHandle<TargetGraph>::type;

  using CandidateSetType = std::map<QueryVertexHandle, 
                       std::vector<TargetVertexHandle>>;

  using MatchMap = std::map<QueryVertexHandle,
                           TargetVertexHandle>;

  MatchMap match;

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
      assert(false);
      candidate_set_list.emplace_back();
      continue;
    }
    candidate_set_list.emplace_back(std::move(candidate_set));
  }

  std::vector<bool> call_match_callback(query_graph_list.size(), true);

  MatchFromParentToChild(root_handle, match, 
                         candidate_set_list,
                           query_graph_list,
                        call_match_callback,
                               target_graph,
                             prune_callback,
                             match_callback);
  return;
}

};

#endif // _GUNDAM_ALGORITHM_MULTI_QUERY_DPISO_USING_MATCH_H