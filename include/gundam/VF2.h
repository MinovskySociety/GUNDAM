#include "../../include/GUNDAM/include/gundam/graph.h"
#include "../../include/stardand/errcode.h"
#include "../../include/stardand/paratype.h"
#include <iostream>
#include <algorithm>
#include <set>
//enum class ...
///enum   match tyep bool is_iso,1/// iso or homo


enum class MatchSemantics:bool {
    kIsomorphism_,
    kHomomorphism_
};
enum class EdgeState:bool {
    kIn_,
    kOut_
};
//Cal Degree
template<enum EdgeState edge_state, typename VertexPtr>
int Degree(VertexPtr vertex_ptr){
    int vertex_degree = 0;
    for (auto it = ((edge_state==EdgeState::kIn_)?
            vertex_ptr->InEdgeCBegin():
            vertex_ptr->OutEdgeCBegin());
        !it.IsDone(); it++){
        ++vertex_degree;
    }
    return vertex_degree;
}
//Init Candidate Set
template<enum MatchSemantics match_semantics,
         template <typename...> class GraphType0, typename... configures0,
         template <typename...> class GraphType1, typename... configures1>
int InitCandidateSet(IN const GraphType0<configures0...>& query_graph,
        IN const GraphType1<configures1...>& target_graph,
        IN std::map<typename  GraphType0<configures0...>::VertexConstPtr,
        std::vector<typename  GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set
){
    using PatternType = GraphType0<configures0...>;
    using DataGraphType = GraphType1<configures1...>;
    using PatternIDType = typename PatternType::VertexType::IDType;
    using DataGraphIDType = typename DataGraphType::VertexType::IDType;
    using PatternVertexType = typename PatternType::VertexType;
    using PatternVertexPtr  = typename PatternType::VertexConstPtr;
    using DataGraphVertexType = typename DataGraphType::VertexType;
    using DataGraphVertexPtr  = typename DataGraphType::VertexConstPtr;
    for (auto query_it = query_graph.VertexCBegin();
        !query_it.IsDone(); query_it++){
        PatternVertexPtr query_ptr = query_graph.FindConstVertex(query_it->id());
        for (auto target_it = target_graph.VertexCBegin();
            !target_it.IsDone(); target_it++){
            if (!(query_it->label() == target_it->label())){
                continue;
            }
            DataGraphVertexPtr target_ptr =
                target_graph.FindConstVertex(target_it->id());
            int query_in_count = 0, query_out_count = 0;
            int target_in_count = 0, target_out_count = 0;
            query_in_count   = Degree<EdgeState::kIn_>(query_ptr);
            query_out_count  = Degree<EdgeState::kOut_>(query_ptr);
            target_in_count  = Degree<EdgeState::kIn_>(target_ptr);
            target_out_count = Degree<EdgeState::kOut_>(target_ptr);
            if ((target_in_count < query_in_count)||
                (target_out_count < query_out_count)){
                    continue;
            }
            candidate_set[query_ptr].push_back(target_ptr);
        }
    }
    return E_OK;
}





//Check target_id is in C(query_vertex_ptr)
template<typename QueryPtr, typename TargetPtr>
int CheckIsInCandidateSet(QueryPtr query_vertex_ptr,
    std::map<QueryPtr, std::vector<TargetPtr>> &candidate_set,
    int target_id
){
    for (auto it : candidate_set[query_vertex_ptr]){
        if (it->id() == target_id){
            return E_OK;
        }
    } 
    return E_ERROR_NOT_INCANDIDATE_SET;  
}

template<enum EdgeState edge_state,
         typename PatternVertexPtr,
         typename DataGraphVertexPtr>
int JoinAbleCheck(
    IN PatternVertexPtr query_vertex_ptr,
    IN DataGraphVertexPtr target_vertex_ptr,
    IN std::map<PatternVertexPtr, DataGraphVertexPtr> 
    &match_state
){  
    for (auto it = ((edge_state == EdgeState::kIn_)?
            query_vertex_ptr->InEdgeCBegin():
            query_vertex_ptr->OutEdgeCBegin());
        !it.IsDone(); it++){
        PatternVertexPtr temp_vertex_ptr = (edge_state == EdgeState::kIn_)?
            it->const_src_ptr() : it->const_dst_ptr();
        if (!match_state.count(temp_vertex_ptr)) continue;
        DataGraphVertexPtr temp_target_vertex_ptr = 
            match_state[temp_vertex_ptr];
        int find_target_flag = 0;
        for (auto bit = ((edge_state == EdgeState::kIn_)?
                target_vertex_ptr->InEdgeCBegin(it->label()):
                target_vertex_ptr->OutEdgeCBegin(it->label()));
            !bit.IsDone(); bit++){
            auto temp_id = (edge_state == EdgeState::kIn_)?
                bit->const_src_ptr()->id() : bit->const_dst_ptr()->id();
            if (temp_id == temp_target_vertex_ptr->id()){
                find_target_flag=1;
                break;
            }
        }
        if (find_target_flag == 0) 
            return E_ERROR_NOT_JOINABLE;   
    }
    return E_OK;
}

template<enum MatchSemantics match_semantics,
         template <typename...> class GraphType0, typename... configures0,
         template <typename...> class GraphType1, typename... configures1>
int IsJoinAble(IN const GraphType0<configures0...>& query_graph,
        IN const GraphType1<configures1...>& target_graph,
        IN typename GraphType0<configures0...>::VertexConstPtr next_query_vertex,
        IN typename GraphType1<configures1...>::VertexConstPtr next_target_vertex,
        IN std::map<typename GraphType0<configures0...>::VertexConstPtr,
                    typename GraphType1<configures1...>::VertexConstPtr>
        &match_state,
        std::set<typename GraphType1<configures1...>::VertexConstPtr>
        &target_graph_used_node
){
    using PatternType = GraphType0<configures0...>;
    using DataGraphType = GraphType1<configures1...>;
    using PatternIDType = typename PatternType::VertexType::IDType;
    using DataGraphIDType = typename DataGraphType::VertexType::IDType;
    using PatternVertexType = typename PatternType::VertexType;
    using PatternVertexPtr  = typename PatternType::VertexConstPtr;
    using DataGraphVertexType = typename DataGraphType::VertexType;
    using DataGraphVertexPtr  = typename DataGraphType::VertexConstPtr;    
    if (match_semantics == MatchSemantics::kIsomorphism_)
        if (target_graph_used_node.count(next_target_vertex)) 
            return E_ERROR_NOT_JOINABLE;
    if (!E_SUCCEEDED(JoinAbleCheck<EdgeState::kIn_>(next_query_vertex,
        next_target_vertex, match_state))){
        return E_ERROR_NOT_JOINABLE;
    }
    if (!E_SUCCEEDED(JoinAbleCheck<EdgeState::kOut_>(next_query_vertex,
        next_target_vertex, match_state))){
        return E_ERROR_NOT_JOINABLE;
    }
    return E_OK;
}




template<template <typename...> class GraphType0, typename... configures0,
         template <typename...> class GraphType1, typename... configures1>
int DetermineMatchOrder(IN const GraphType0<configures0...>& query_graph,
    IN const GraphType1<configures1...>& target_graph,
    IN std::map<typename GraphType0<configures0...>::VertexConstPtr,
       std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
    &candidate_set,
    IN std::map<typename GraphType0<configures0...>::VertexConstPtr,
                typename GraphType1<configures1...>::VertexConstPtr>
    &match_state,
    OUT std::vector<
        std::pair<int,typename GraphType0<configures0...>::VertexConstPtr>>
    &query_vertex_sequence
){
    using PatternType = GraphType0<configures0...>;
    using DataGraphType = GraphType1<configures1...>;
    using PatternIDType = typename PatternType::VertexType::IDType;
    using DataGraphIDType = typename DataGraphType::VertexType::IDType;
    using PatternVertexType = typename PatternType::VertexType;
    using PatternVertexPtr  = typename PatternType::VertexConstPtr;
    using DataGraphVertexType = typename DataGraphType::VertexType;
    using DataGraphVertexPtr  = typename DataGraphType::VertexConstPtr;
    std::set<PatternVertexPtr> next_query_set;  
    for (auto &it : match_state){
        PatternVertexPtr  query_u_ptr = it.first;
        for (auto bit = query_u_ptr->OutEdgeCBegin(); !bit.IsDone(); bit++){
            if (match_state.count(bit->const_dst_ptr())) continue;
            next_query_set.insert(bit->const_dst_ptr());
        }
        for (auto bit = query_u_ptr->InEdgeCBegin(); !bit.IsDone(); bit++){
            if (match_state.count(bit->const_src_ptr())) continue;
            next_query_set.insert(bit->const_src_ptr());
        }
    }  
    for (auto it : next_query_set){
        query_vertex_sequence.push_back(
            std::make_pair((int)candidate_set[it].size(), it));
    }
    sort(query_vertex_sequence.begin(), query_vertex_sequence.end());
    return E_OK;
}

template<typename QueryPtr,typename TargetPtr>
int UpdateState(QueryPtr query_vertex_ptr, TargetPtr target_vertex_ptr,
    std::map<QueryPtr,TargetPtr> &match_state,
    std::set<TargetPtr> &target_graph_used_node
){
    match_state.insert(std::make_pair(query_vertex_ptr, target_vertex_ptr));
    target_graph_used_node.insert(target_vertex_ptr);
    return E_OK;
}
template<typename QueryPtr,typename TargetPtr>
int RestoreState(QueryPtr query_vertex_ptr, TargetPtr target_vertex_ptr,
    std::map<QueryPtr, TargetPtr> &match_state,
    std::set<TargetPtr> &target_graph_used_node
){
    match_state.erase(match_state.find(query_vertex_ptr));
    target_graph_used_node.erase(target_vertex_ptr);
    return E_OK;
}


template<enum MatchSemantics match_semantics,
         template <typename...> class GraphType0, typename... configures0,
         template <typename...> class GraphType1, typename... configures1>
int VF2(IN const GraphType0<configures0...>& query_graph,
        IN const GraphType1<configures1...>& target_graph,
        IN std::map<typename GraphType0<configures0...>::VertexConstPtr,
           std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
        &candidate_set,
        IN std::map<typename GraphType0<configures0...>::VertexConstPtr,
                    typename GraphType1<configures1...>::VertexConstPtr>
        &match_state,
        std::set<typename GraphType1<configures1...>::VertexConstPtr>
        &target_graph_used_node,
        OUT std::vector<
        std::map<typename GraphType0<configures0...>::VertexConstPtr,
                 typename GraphType1<configures1...>::VertexConstPtr>>
        &match_result,
        IN int top_k=2147483647
         ){
    using PatternType = GraphType0<configures0...>;
    using DataGraphType = GraphType1<configures1...>;
    using PatternIDType = typename PatternType::VertexType::IDType;
    using DataGraphIDType = typename DataGraphType::VertexType::IDType;
    using PatternVertexType = typename PatternType::VertexType;
    using PatternVertexLabelType = typename PatternType::VertexType::LabelType;
    using PatternVertexPtr  = typename PatternType::VertexConstPtr;
    using DataGraphVertexType = typename DataGraphType::VertexType;
    using DataGraphVertexPtr  = typename DataGraphType::VertexConstPtr;
    int find_match_tag = E_ERROR_NOT_FIND_MATCH;
    size_t query_size = query_graph.CountVertex();
    if (match_state.size() == query_size){
        match_result.push_back(match_state);
        return E_OK;
    }
    std::vector<std::pair<int, PatternVertexPtr>> query_vertex_sequence;
    DetermineMatchOrder(query_graph, target_graph,
        candidate_set, match_state, query_vertex_sequence);
    PatternVertexPtr next_query_vertex = query_vertex_sequence[0].second;
    for (auto next_target_vertex : candidate_set[next_query_vertex]){
        if (!IsJoinAble<match_semantics>(query_graph, target_graph,
            next_query_vertex, next_target_vertex,
            match_state, target_graph_used_node)){
            UpdateState(next_query_vertex, next_target_vertex,
                match_state, target_graph_used_node);
            VF2<match_semantics>(query_graph, target_graph,
                candidate_set, match_state, target_graph_used_node,
                match_result, top_k);
            if (match_result.size() >= top_k) return E_OK;
            RestoreState(next_query_vertex, next_target_vertex,
                match_state, target_graph_used_node);
        }
    }
    if (!match_result.empty()) return E_OK;
    else return E_ERROR_NOT_FIND_MATCH;
}

template<enum MatchSemantics match_semantics,
         template <typename...> class GraphType0, typename... configures0,
         template <typename...> class GraphType1, typename... configures1>
int VF2_(IN const GraphType0<configures0...>& query_graph,
         IN const GraphType1<configures1...>& target_graph,
         IN typename GraphType0<configures0...>::
         VertexType::IDType query_id,
         IN typename GraphType1<configures1...>::
         VertexType::IDType target_id,
         OUT std::vector<
         std::map<typename GraphType0<configures0...>::VertexConstPtr,
                  typename GraphType1<configures1...>::VertexConstPtr>>
         &match_result,
         int top_k=2147483647){
    using PatternType = GraphType0<configures0...>;
    using DataGraphType = GraphType1<configures1...>;
    using PatternIDType = typename PatternType::VertexType::IDType;
    using DataGraphIDType = typename DataGraphType::VertexType::IDType;
    using PatternVertexType = typename PatternType::VertexType;
    using PatternVertexPtr  = typename PatternType::VertexConstPtr;
    using DataGraphVertexType = typename DataGraphType::VertexType;
    using DataGraphVertexPtr  = typename DataGraphType::VertexConstPtr;
    std::map<typename GraphType0<configures0...>::VertexConstPtr,
        std::vector<typename GraphType1<configures1...>::VertexConstPtr>>
    candidate_set;  
    InitCandidateSet<match_semantics>(query_graph, target_graph, candidate_set);
    PatternVertexPtr query_ptr = query_graph.FindConstVertex(query_id);
    DataGraphVertexPtr  target_ptr = target_graph.FindConstVertex(target_id);
    if (!E_SUCCEEDED(CheckIsInCandidateSet(
        query_ptr, candidate_set, target_id))){
        return E_ERROR_NOT_FIND_ISO; 
    }
    std::map<PatternVertexPtr, DataGraphVertexPtr> match_state;
    std::set<DataGraphVertexPtr> target_used_node;
    UpdateState(query_ptr, target_ptr, match_state, target_used_node);
    if (E_SUCCEEDED(VF2<match_semantics>(query_graph, target_graph,
            candidate_set, match_state, target_used_node,
            match_result, top_k))){
        return E_OK;
    }
    return E_ERROR_NOT_FIND_ISO;
}