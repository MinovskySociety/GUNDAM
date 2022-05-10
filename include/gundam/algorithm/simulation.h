#ifndef _GUNDAM_ALGORITHM_SIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_H

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/algorithm/dp_iso_using_match.h"
#include "gundam/algorithm/match_semantics.h"

#include "gundam/tool/connected_component.h"
#include "gundam/tool/diameter.h"
#include "gundam/tool/operator/preserve.h"

#include "gundam/util/hash.h"

#include "gundam/match/match.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace GUNDAM {

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kDualSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t Simulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
     std::map<typename VertexHandle< QueryGraph>::type,
  std::vector<typename VertexHandle<TargetGraph>::type>>& match_set) {

  std::cout << "join the simulation " << std::endl;
  assert(match_set.empty());
  match_set.clear();

  static_assert(match_semantics == MatchSemantics::kDualSimulation);

  if (!_dp_iso_using_match::InitCandidateSet<match_semantics>(
                                                              query_graph,
                                                             target_graph,
                                                              match_set)) {
    return 0;
  }

   if (!_dp_iso_using_match::RefineCandidateSet(query_graph, 
                                               target_graph, 
                                                match_set)) {
     return 0;
   }


  size_t match_count = 1;
  for (const auto& [query_vertex_handle,
                    query_vertex_candidate_set] : match_set) {
    match_count *= query_vertex_candidate_set.size();
  }
  //std::cout << "match count is " << match_count << std::endl;
  return match_count;
};


template <enum MatchSemantics match_semantics 
             = MatchSemantics::kStrongSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline std::vector<TargetGraph> StrongSimulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph) {

  static_assert(match_semantics == MatchSemantics::kStrongSimulation);

  using  QueryVerteHandleType = typename VertexHandle< QueryGraph>::type;
  using TargetVerteHandleType = typename VertexHandle<TargetGraph>::type;
  using  TargetEdgeHandleType = typename   EdgeHandle<TargetGraph>::type;

  using CandidateSet =  std::map< QueryVerteHandleType,
                     std::vector<TargetVerteHandleType>>;

  const size_t dQ = Diameter<true>(query_graph);

  std::vector<TargetGraph> maximum_perfect_subgraphs;

  for (auto vertex_it = target_graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    TargetVerteHandleType w_vertex_handle = vertex_it;
    // to obtain the ball
    TargetGraph ball = KHop<true>(target_graph, w_vertex_handle, 2 * dQ);
    CandidateSet match_set_in_ball;
    if (Simulation<MatchSemantics::kDualSimulation>(
                    query_graph, ball,
                    match_set_in_ball) == 0) {
      continue;
    }

    TargetVerteHandleType w_vertex_handle_in_ball
        = ball.FindVertex(w_vertex_handle->id());
    assert(w_vertex_handle_in_ball);

    // ExtractMaxPG(query_graph, ball, match_set_in_ball);
    std::vector<TargetVerteHandleType> vertex_handle_to_preserve;
    bool contained_in_sw = false;
    for (auto& [query_handle,
                candidate_set] : match_set_in_ball) {
      std::sort(candidate_set.begin(),
                candidate_set.end());
      std::copy(candidate_set.begin(), 
                candidate_set.end(), 
                std::back_inserter(vertex_handle_to_preserve));
      if (std::binary_search(candidate_set.begin(),
                             candidate_set.end(), 
                             w_vertex_handle_in_ball)) {
        contained_in_sw = true;
      }
    }
    if (!contained_in_sw) {
      continue;
    }
    std::sort( vertex_handle_to_preserve.begin(), 
               vertex_handle_to_preserve.end() );
    vertex_handle_to_preserve.erase(
      std::unique( vertex_handle_to_preserve.begin(),
                   vertex_handle_to_preserve.end() ),
                   vertex_handle_to_preserve.end() );
    assert(vertex_handle_to_preserve.size() <= ball.CountVertex());
    // to select the edge, map each edge in query_graph
    std::vector<TargetEdgeHandleType> edge_handle_to_preserve;
    for (auto vertex_it = query_graph.VertexBegin();
             !vertex_it.IsDone();
              vertex_it++) {
      for (auto out_edge_it = vertex_it->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        // to find the set of edges that can be matched by
        // this edge can
        assert(match_set_in_ball.find(vertex_it)
            != match_set_in_ball. end());
        const auto& candidate_set_u0 = match_set_in_ball.find(vertex_it)->second;
        assert(match_set_in_ball.find(out_edge_it->dst_handle())
            != match_set_in_ball. end());
        const auto& candidate_set_u1 = match_set_in_ball.find(out_edge_it->dst_handle())->second;
        for (const auto& dst_handle_u0 : candidate_set_u0) {
          for (const auto& dst_handle_u1 : candidate_set_u1) {
            // to find whether there is an edge from dst_handle_u0 
            // to dst_handle_u1
            for (auto out_edge_it = dst_handle_u0->OutEdgeBegin();
                     !out_edge_it.IsDone();
                      out_edge_it++) {
              if (out_edge_it->dst_handle() != dst_handle_u1) {
                continue;
              }
              edge_handle_to_preserve.emplace_back(out_edge_it);
              // in case there are more than one edge between these two 
              // vertexes
              // break;
            }
          }
        }
      }
    }

    std::sort( edge_handle_to_preserve.begin(),
               edge_handle_to_preserve.end() );
    edge_handle_to_preserve.erase(
      std::unique( edge_handle_to_preserve.begin(),
                   edge_handle_to_preserve.end() ),
                   edge_handle_to_preserve.end() );

    TargetGraph Gm = Preserve(ball, vertex_handle_to_preserve,
                                      edge_handle_to_preserve);
    assert(Gm.FindVertex(w_vertex_handle->id()));
    // find connected component containing w
    maximum_perfect_subgraphs.emplace_back(ConnectedComponent<true>(Gm, Gm.FindVertex(w_vertex_handle->id())));
  }
  return std::move(maximum_perfect_subgraphs);
};



template <enum MatchSemantics match_semantics 
             = MatchSemantics::kDualSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t DualSimulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
      std::map<typename VertexHandle< QueryGraph>::type,
   std::vector<typename VertexHandle<TargetGraph>::type>>& match_set) {


  std::cout << "join new dual simulation " << std::endl;

using QueryVertexHandleType = typename VertexHandle<QueryGraph>::type;
  using QueryVertexLabelType = typename QueryGraph::VertexType::LabelType;
  using QueryEdgeLabelType = typename QueryGraph::EdgeType::LabelType;

  using TargetVertexHandleType = typename VertexHandle<TargetGraph>::type;
  using TargetVertexLabelType = typename TargetGraph::VertexType::LabelType;

  std::unordered_map<QueryVertexLabelType, std::vector<QueryVertexHandleType>> tag2handle;
  std::unordered_map<QueryVertexHandleType, size_t, v_handle_hash> handle2vec_id;

  std::unordered_map<QueryVertexHandleType,
                    std::unordered_map<QueryVertexHandleType, QueryEdgeLabelType, v_handle_hash>, v_handle_hash> pre_p;
  std::unordered_map<QueryVertexHandleType,
                    std::unordered_map<QueryVertexHandleType, QueryEdgeLabelType, v_handle_hash>, v_handle_hash> post_p;
  std::unordered_map<QueryVertexHandleType,
                    std::unordered_map<QueryEdgeLabelType, std::vector<QueryVertexHandleType>>, v_handle_hash> pre_e;
  std::unordered_map<QueryVertexHandleType,
                    std::unordered_map<QueryEdgeLabelType, std::vector<QueryVertexHandleType>>, v_handle_hash> post_e;

  std::unordered_map<TargetVertexHandleType, std::vector<std::unordered_map<QueryVertexHandleType, size_t, v_handle_hash>>, v_handle_hash> pre_cnt;
  std::unordered_map<TargetVertexHandleType, std::vector<std::unordered_map<QueryVertexHandleType, size_t, v_handle_hash>>, v_handle_hash> post_cnt;
//  std::unordered_map<QueryVertexHandleType, std::unordered_set<TargetVertexHandleType, v_handle_hash>, v_handle_hash> remove_set;
  std::unordered_map<QueryVertexHandleType, std::vector<TargetVertexHandleType>, v_handle_hash> remove_set;
  std::unordered_map<TargetVertexHandleType, std::vector<bool>, v_handle_hash> active;
  for (auto vertex_it = query_graph.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandleType vertex_handle(vertex_it);
    tag2handle[vertex_it->label()].emplace_back(vertex_handle);
    handle2vec_id[vertex_handle] = tag2handle[vertex_it->label()].size() - 1;

    pre_p[vertex_handle] = std::unordered_map<QueryVertexHandleType, QueryEdgeLabelType, v_handle_hash>();
    post_p[vertex_handle] = std::unordered_map<QueryVertexHandleType, QueryEdgeLabelType, v_handle_hash>();
    pre_e[vertex_handle] = std::unordered_map<QueryEdgeLabelType, std::vector<QueryVertexHandleType>>();
    post_e[vertex_handle] = std::unordered_map<QueryEdgeLabelType, std::vector<QueryVertexHandleType>>();

    for (auto in_edge_it = vertex_it->InEdgeBegin();
              !in_edge_it.IsDone();
              in_edge_it++) {
      pre_p[vertex_handle][in_edge_it->src_handle()] = in_edge_it->label();
      pre_e[vertex_handle][in_edge_it->label()].emplace_back(in_edge_it->src_handle());
    }

    for (auto out_edge_it = vertex_it->OutEdgeBegin();
              !out_edge_it.IsDone();
              out_edge_it++) {
      post_p[vertex_handle][out_edge_it->dst_handle()] = out_edge_it->label();
      post_e[vertex_handle][out_edge_it->label()].emplace_back(out_edge_it->dst_handle());
    }

    //remove_set[vertex_handle] = std::unordered_set<TargetVertexHandleType, v_handle_hash>();
    remove_set[vertex_handle] = std::vector<TargetVertexHandleType>();
  }

  for (auto& [qg_vertex_handle, tg_v_handle_vec] : match_set) {
    for (auto &tg_vertex_handle : tg_v_handle_vec) {
      size_t tag_count = tag2handle[qg_vertex_handle->label()].size();

      if (active.find(tg_vertex_handle) == active.end()) {
        active[tg_vertex_handle].resize(tag_count, false);
      }
      active[tg_vertex_handle][handle2vec_id[qg_vertex_handle]] = true;

      pre_cnt[tg_vertex_handle].resize(tag_count);
      post_cnt[tg_vertex_handle].resize(tag_count);
    }
  }

  for (auto &v2vx : active) {
    auto tg_v_handle = v2vx.first;
    auto tag = tg_v_handle->label();
    auto &qg_v_handle_vec = tag2handle[tag];


    for (size_t qg_v_handle_idx = 0;
         qg_v_handle_idx < qg_v_handle_vec.size();
         qg_v_handle_idx++) {
      auto qg_v_handle = qg_v_handle_vec[qg_v_handle_idx];

      if (!active[tg_v_handle][qg_v_handle_idx]) continue;

      if constexpr (GraphParameter<TargetGraph>::vertex_level_edge_label_index) {
        for (auto &[e_tag, qg_v_handle_vec] : pre_e[qg_v_handle]) {
          for (auto in_edge_it = tg_v_handle->InEdgeBegin(e_tag);
                    !in_edge_it.IsDone();
                    in_edge_it++) {
            TargetVertexHandleType tg_v_handle_2 = in_edge_it->src_handle();
            if (active.find(tg_v_handle_2) == active.end()) continue;

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            for (auto &qg_v_handle_2 : qg_v_handle_vec) {
              if (tag_2 != qg_v_handle_2->label()) continue;
              size_t idx = handle2vec_id[qg_v_handle_2];
              if (active[tg_v_handle_2][idx]) {
                post_cnt[tg_v_handle_2][idx][qg_v_handle]++;
              }
            }
          }
        }

        for (auto &[e_tag, qg_v_handle_vec] : post_e[qg_v_handle]) {
          for (auto out_edge_it = tg_v_handle->OutEdgeBegin(e_tag);
                    !out_edge_it.IsDone();
                    out_edge_it++) {
            TargetVertexHandleType tg_v_handle_2 = out_edge_it->dst_handle();
            if (active.find(tg_v_handle_2) == active.end()) continue;

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            for (auto &qg_v_handle_2 : qg_v_handle_vec) {
              if (tag_2 != qg_v_handle_2->label()) continue;
              size_t idx = handle2vec_id[qg_v_handle_2];
              if (active[tg_v_handle_2][idx]) {
                pre_cnt[tg_v_handle_2][idx][qg_v_handle]++;
              }
            }
          }
        }
      } else { 
        if (!pre_p[qg_v_handle].empty()) {

          for (auto in_edge_it = tg_v_handle->InEdgeBegin();
                    !in_edge_it.IsDone();
                    in_edge_it++) {
            TargetVertexHandleType tg_v_handle_2 = in_edge_it->src_handle();
            if (active.find(tg_v_handle_2) == active.end()) continue;

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            auto &qg_v_handle_vec_2 = tag2handle[tag_2];
            for (size_t qg_v_handle_idx_2 = 0;
                qg_v_handle_idx_2 < qg_v_handle_vec_2.size();
                qg_v_handle_idx_2++) {
              if (!active[tg_v_handle_2][qg_v_handle_idx_2]) continue;

              QueryVertexHandleType qg_v_handle_2 = qg_v_handle_vec_2[qg_v_handle_idx_2];
              if (pre_p[qg_v_handle].find(qg_v_handle_2) != pre_p[qg_v_handle].end()
                    && pre_p[qg_v_handle][qg_v_handle_2] == in_edge_it->label()) {
                post_cnt[tg_v_handle_2][qg_v_handle_idx_2][qg_v_handle]++;
              }
            }
          }
        }

        if (!post_p[qg_v_handle].empty()) {
          for (auto out_edge_it = tg_v_handle->OutEdgeBegin();
                    !out_edge_it.IsDone();
                    out_edge_it++) {
            TargetVertexHandleType tg_v_handle_2 = out_edge_it->dst_handle();
            if (active.find(tg_v_handle_2) == active.end()) continue;

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            auto &qg_v_handle_vec_2 = tag2handle[tag_2];
            for (size_t qg_v_handle_idx_2 = 0;
                qg_v_handle_idx_2 < qg_v_handle_vec_2.size();
                qg_v_handle_idx_2++) {
              if (!active[tg_v_handle_2][qg_v_handle_idx_2]) continue;

              QueryVertexHandleType qg_v_handle_2 = qg_v_handle_vec_2[qg_v_handle_idx_2];
              if (post_p[qg_v_handle].find(qg_v_handle_2) != post_p[qg_v_handle].end()
                    && post_p[qg_v_handle][qg_v_handle_2] == out_edge_it->label()) {
                pre_cnt[tg_v_handle_2][qg_v_handle_idx_2][qg_v_handle]++;
              }
            }
          }
        }
      }
    }
  }

  for (auto &[tg_v_handle, qg_vec] : pre_cnt) {
    auto tag = tg_v_handle->label();
    for (size_t idx = 0; idx < qg_vec.size(); idx++) {
      if (!active[tg_v_handle][idx]) continue;

      auto qg_v_handle = tag2handle[tag][idx];
      auto size = pre_p[qg_v_handle].size() + post_p[qg_v_handle].size();
      if (pre_cnt[tg_v_handle][idx].size() + post_cnt[tg_v_handle][idx].size() < size) {
        //remove_set[qg_v_handle].insert(tg_v_handle);
        remove_set[qg_v_handle].emplace_back(tg_v_handle);
        active[tg_v_handle][idx] = false;
      }
    }
  }

  std::queue<QueryVertexHandleType> que;
  std::map<QueryVertexHandleType, bool> in_queue;

  for (auto vertex_it = query_graph.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandleType v_handle(vertex_it);
    in_queue[v_handle] = false;

    if (!remove_set[v_handle].empty()) {
      que.push(v_handle);
      in_queue[v_handle] = true;
    }
  }

  if (que.empty()) {
    std::cout << "empty queue " << std::endl;
  }

  for (; !que.empty(); que.pop()) {
    QueryVertexHandleType qg_v_handle = que.front();
    in_queue[qg_v_handle] = false;

    for (auto& tg_v_handle : remove_set[qg_v_handle]) {
      auto idx = handle2vec_id[qg_v_handle];

//      active[tg_v_handle][idx] = false;

      if constexpr (GraphParameter<TargetGraph>::vertex_level_edge_label_index) {
        for (auto &[e_tag, qg_v_handle_vec] : pre_e[qg_v_handle]) {
          for (auto in_edge_it = tg_v_handle->InEdgeBegin(e_tag);
                    !in_edge_it.IsDone();
                    in_edge_it++) {
            TargetVertexHandleType tg_v_handle_2 = in_edge_it->src_handle();
            if (active.find(tg_v_handle_2) == active.end()) continue;

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            for (auto &qg_v_handle_2 : qg_v_handle_vec) {
              if (tag_2 != qg_v_handle_2->label()) continue;

              size_t idx = handle2vec_id[qg_v_handle_2];
              if (active[tg_v_handle_2][idx]) {
                post_cnt[tg_v_handle_2][idx][qg_v_handle]--;

                if (post_cnt[tg_v_handle_2][idx][qg_v_handle] == 0) {
                  //remove_set[qg_v_handle_2].insert(tg_v_handle_2);
                  remove_set[qg_v_handle_2].emplace_back(tg_v_handle_2);
                  active[tg_v_handle_2][idx] = false;

                  if (!in_queue[qg_v_handle_2]) {
                    que.push(qg_v_handle_2);
                    in_queue[qg_v_handle_2] = true;
                  }
                }
              }
            }
          }
        }

        for (auto &[e_tag, qg_v_handle_vec] : post_e[qg_v_handle]) {
          for (auto out_edge_it = tg_v_handle->OutEdgeBegin(e_tag);
                    !out_edge_it.IsDone();
                    out_edge_it++) {
            TargetVertexHandleType tg_v_handle_2 = out_edge_it->dst_handle();
            if (active.find(tg_v_handle_2) == active.end()) continue;

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            for (auto &qg_v_handle_2 : qg_v_handle_vec) {
              if (tag_2 != qg_v_handle_2->label()) continue;

              size_t idx = handle2vec_id[qg_v_handle_2];
              if (active[tg_v_handle_2][idx]) {
                pre_cnt[tg_v_handle_2][idx][qg_v_handle]--;

                if (pre_cnt[tg_v_handle_2][idx][qg_v_handle] == 0) {
                  //remove_set[qg_v_handle_2].insert(tg_v_handle_2);
                  remove_set[qg_v_handle_2].emplace_back(tg_v_handle_2);
                  active[tg_v_handle_2][idx] = false;

                  if (!in_queue[qg_v_handle_2]) {
                    que.push(qg_v_handle_2);
                    in_queue[qg_v_handle_2] = true;
                  }
                }
              }
            }
          }
        }
      } else {
        if (!pre_p[qg_v_handle].empty()) {
          for (auto in_edge_it = tg_v_handle->InEdgeBegin();
                    !in_edge_it.IsDone();
                    in_edge_it++) {

            TargetVertexHandleType tg_v_handle_2 = in_edge_it->src_handle();
            if (active.find(tg_v_handle_2) == active.end()) {
              continue;
            }

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            auto &qg_v_handle_vec_2 = tag2handle[tag_2];
            for (size_t qg_v_handle_idx_2 = 0;
                        qg_v_handle_idx_2 < qg_v_handle_vec_2.size();
                        qg_v_handle_idx_2++) {
              if (!active[tg_v_handle_2][qg_v_handle_idx_2]) continue;

              QueryVertexHandleType qg_v_handle_2 = qg_v_handle_vec_2[qg_v_handle_idx_2];
              if (pre_p[qg_v_handle].find(qg_v_handle_2) != pre_p[qg_v_handle].end()
                    && pre_p[qg_v_handle][qg_v_handle_2] == in_edge_it->label()) {

                post_cnt[tg_v_handle_2][qg_v_handle_idx_2][qg_v_handle]--;

                if (post_cnt[tg_v_handle_2][qg_v_handle_idx_2][qg_v_handle] == 0) {
                  //remove_set[qg_v_handle_2].insert(tg_v_handle_2);
                  remove_set[qg_v_handle_2].emplace_back(tg_v_handle_2);
                  active[tg_v_handle_2][idx] = false;

                  if (!in_queue[qg_v_handle_2]) {
                    que.push(qg_v_handle_2);
                    in_queue[qg_v_handle_2] = true;
                  }
                }
              }
            }
          }
        }

        if (!post_p[qg_v_handle].empty()) {
          for (auto out_edge_it = tg_v_handle->OutEdgeBegin();
                    !out_edge_it.IsDone();
                    out_edge_it++) {

            TargetVertexHandleType tg_v_handle_2 = out_edge_it->dst_handle();
            if (active.find(tg_v_handle_2) == active.end()) {
              continue;
            }

            QueryVertexLabelType tag_2 = tg_v_handle_2->label();

            auto &qg_v_handle_vec_2 = tag2handle[tag_2];
            for (size_t qg_v_handle_idx_2 = 0;
                        qg_v_handle_idx_2 < qg_v_handle_vec_2.size();
                        qg_v_handle_idx_2++) {
              if (!active[tg_v_handle_2][qg_v_handle_idx_2]) continue;

              QueryVertexHandleType qg_v_handle_2 = qg_v_handle_vec_2[qg_v_handle_idx_2];
              if (post_p[qg_v_handle].find(qg_v_handle_2) != post_p[qg_v_handle].end()
                    && post_p[qg_v_handle][qg_v_handle_2] == out_edge_it->label()) {

                pre_cnt[tg_v_handle_2][qg_v_handle_idx_2][qg_v_handle]--;

                if (pre_cnt[tg_v_handle_2][qg_v_handle_idx_2][qg_v_handle] == 0) {
                  //remove_set[qg_v_handle_2].insert(tg_v_handle_2);
                  remove_set[qg_v_handle_2].emplace_back(tg_v_handle_2);
                  active[tg_v_handle_2][idx] = false;

                  if (!in_queue[qg_v_handle_2]) {
                    que.push(qg_v_handle_2);
                    in_queue[qg_v_handle_2] = true;
                  }
                }
              }
            }
          }
        }
      }
    }
    remove_set[qg_v_handle].clear();
  }

  match_set.clear();

  for (auto &v2vx: active) {
    auto tg_v_handle = v2vx.first;
    auto tg_v_label = tg_v_handle->label();
    for (size_t idx = 0; idx < v2vx.second.size(); idx++) {
      if (!(v2vx.second)[idx]) continue;
      match_set[tag2handle[tg_v_label][idx]].emplace_back(tg_v_handle);
    }
  }

  size_t match_count = 1;
  if (match_set.empty()) {
    return 0;
  }
  for (const auto& [query_vertex_handle,
                    query_vertex_candidate_set] : match_set) {
    match_count *= query_vertex_candidate_set.size();
  }


  return match_count;
};











};

#endif // _GUNDAM_ALGORITHM_SIMULATION_H
