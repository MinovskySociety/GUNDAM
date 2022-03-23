#ifndef _GUNDAM_ALGORITHM_SIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_H

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/algorithm/dp_iso_using_match.h"
#include "gundam/algorithm/match_semantics.h"

#include "gundam/tool/connected_component.h"
#include "gundam/tool/diameter.h"
#include "gundam/tool/operator/preserve.h"

#include "gundam/match/match.h"

#include <unordered_map>

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

  //std::cout << "join the simulation " << std::endl;
  assert(match_set.empty());
  match_set.clear();

  static_assert(match_semantics == MatchSemantics::kDualSimulation);

  if (!_dp_iso_using_match::InitCandidateSet<MatchSemantics::kHomomorphism>(
                                                              query_graph,
                                                             target_graph,
                                                              match_set)) {
    return 0;
  }

  // if (!_dp_iso_using_match::RefineCandidateSet(query_graph, 
  //                                             target_graph, 
  //                                              match_set)) {
  //   return 0;
  // }

  /* ########################################################
   * ##  for muyang to implement new Simulation algorithm  ##
   * ######################################################## */

  using QueryVertexHandleType = typename VertexHandle<QueryGraph>::type;
  using QueryVertexLabelType = typename QueryGraph::VertexType::LabelType;
  using QueryEdgeLabelType = typename QueryGraph::EdgeType::LabelType;

  using TargetVertexHandleType = typename VertexHandle<TargetGraph>::type;
  using TargetVertexLabelType = typename TargetGraph::VertexType::LabelType;

  //using MatchType = Match<QueryGraph,
  //                       TargetGraph>;

  //auto my_hash = [](const QueryVertexHandleType& handle) {
  //    return std::hash<int>{}(handle->id());
  //};


  std::unordered_map<QueryVertexLabelType, std::vector<QueryVertexHandleType>> tag2handle;
  std::map<QueryVertexHandleType, size_t> handle2vec_id;

  std::set<TargetVertexHandleType> pre_g;
  std::map<QueryVertexHandleType,
  std::map<QueryVertexHandleType, 
           QueryEdgeLabelType>> pre_p;
  std::map<QueryVertexHandleType, std::unordered_set<QueryEdgeLabelType>> in_e_label_set;
  std::map<TargetVertexHandleType, std::map<QueryVertexHandleType, size_t>> cnt;
  std::map<QueryVertexHandleType, std::set<TargetVertexHandleType>> remove_set;
  std::map<TargetVertexHandleType, std::vector<bool>> active;

  // for (auto& [qg_v_handle, tg_v_handle_vec] : match_set) {
  //   std::cout << "input qurey graph vertex " << qg_v_handle->id() << std::endl;
  //   for (auto tg_v_handle : tg_v_handle_vec) {
  //     std::cout << "        input target graph vertex " << tg_v_handle->id() << std::endl;
  //   }
  // }


  for (auto vertex_it = query_graph.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandleType vertex_handle(vertex_it);
    tag2handle[vertex_it->label()].emplace_back(vertex_handle);
    handle2vec_id[vertex_handle] = tag2handle[vertex_it->label()].size() - 1;
    //std::cout << "idx " << tag2handle[vertex_it->label()].size() - 1 << std::endl;
    pre_p[vertex_handle] = std::map<QueryVertexHandleType, QueryEdgeLabelType>();

    for (auto in_edge_it = vertex_it->InEdgeBegin();
              !in_edge_it.IsDone();
              in_edge_it++) {
      in_e_label_set[vertex_handle].insert(in_edge_it->label());
      pre_p[vertex_handle][in_edge_it->src_handle()] = in_edge_it->label();
    }

    remove_set[vertex_handle] = std::set<TargetVertexHandleType>();
  }

  //std::cout << "tag2handle size"  << tag2handle.size() << std::endl;

  for (auto& [qg_vertex_handle, tg_v_handle_vec] : match_set) {
    for (auto &tg_vertex_handle : tg_v_handle_vec) {
      size_t tag_count = tag2handle[qg_vertex_handle->label()].size();
      if (active.find(tg_vertex_handle) == active.end()) {
        active[tg_vertex_handle].resize(tag_count, false);
      }
      active[tg_vertex_handle][handle2vec_id[qg_vertex_handle]] = true;
      if (tg_vertex_handle->CountOutEdge()) {
        pre_g.insert(tg_vertex_handle);
      }
    }
  }

  // for (auto [tg_vertex_handle, bool_vec] : active) {
  //   std::cout << tg_vertex_handle->id() << std::endl;
  //   for (size_t i = 0; i < bool_vec.size(); i++) {
  //     if (bool_vec[i]) {
  //       std::cout << "a candidate match " << tag2handle[tg_vertex_handle->label()][i]->id() << std::endl;
  //     }
  //   }
  // }

  for (auto &v2vx : active) {
    auto tg_v_handle = v2vx.first;
    auto tag = tg_v_handle->label();
    auto &qg_v_handle_vec = tag2handle[tag];

    //std::cout << "tag " << tag << " size " << qg_v_handle_vec.size() << std::endl;

    for (size_t qg_v_handle_idx = 0;
         qg_v_handle_idx < qg_v_handle_vec.size();
         qg_v_handle_idx++) {
      auto qg_v_handle = qg_v_handle_vec[qg_v_handle_idx];

      if (!active[tg_v_handle][qg_v_handle_idx]) continue;

      //std::cout << "here xx " << qg_v_handle->id() << std::endl;
      //std::cout << "qg out " << qg_v_handle->CountOutEdge() << "tg out " << tg_v_handle->CountOutEdge() << std::endl;
      if ((!qg_v_handle->CountOutEdge()) || tg_v_handle->CountOutEdge()) {
        for (auto tg_in_edge_it = tg_v_handle->InEdgeBegin();
                  !tg_in_edge_it.IsDone();
                  tg_in_edge_it++) {
           //std::cout << "xx " << qg_v_handle->id() << std::endl;
          TargetVertexHandleType tg_v_handle_2 = tg_in_edge_it->src_handle();
          if (active.find(tg_v_handle_2) != active.end()
              && (in_e_label_set[qg_v_handle].find(tg_in_edge_it->label())
               != in_e_label_set[qg_v_handle].end())) {
            ++cnt[tg_v_handle_2][qg_v_handle];
            remove_set[qg_v_handle].insert(tg_v_handle_2);
          }
        }
      } else {
        //std::cout << "xxxxxxxxxx" << std::endl;
        active[tg_v_handle][qg_v_handle_idx] = false;
      }
    }
  }

  // for (auto [tg_vertex_handle, bool_vec] : active) {
  //   std::cout << tg_vertex_handle->id() << std::endl;
  //   for (size_t i = 0; i < bool_vec.size(); i++) {
  //     if (bool_vec[i]) {
  //       std::cout << "a candidate match " << tag2handle[tg_vertex_handle->label()][i]->id() << std::endl;
  //     }
  //   }
  // }

  // for (auto handle : pre_g) {
  //   std::cout << "pre g " << handle->id() << std::endl;
  // }

  //  for (auto& [tg_v_handle, qg_v_handle_vec] : remove_set) {
  //   std::cout << "reserve target graph vertex " << tg_v_handle->id() << std::endl;
  //   for (auto qg_v_handle : qg_v_handle_vec) {
  //     std::cout << "        reserve query graph vertex " << qg_v_handle->id() << std::endl;
  //   }
  // }

  for (auto vertex_it = query_graph.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandleType v_handle(vertex_it);
    std::set<TargetVertexHandleType> res;
    std::set_difference(pre_g.begin(), pre_g.end(), remove_set[v_handle].begin(),
                        remove_set[v_handle].end(), std::inserter(res, res.end()));
    remove_set[v_handle] = res;
  }

  //  for (auto& [tg_v_handle, qg_v_handle_vec] : remove_set) {
  //   std::cout << "target graph vertex " << tg_v_handle->id() << std::endl;
  //   for (auto qg_v_handle : qg_v_handle_vec) {
  //     std::cout << "        query graph vertex " << qg_v_handle->id() << std::endl;
  //   }
  // }

  std::queue<QueryVertexHandleType> que;
  std::map<QueryVertexHandleType, bool> in_queue;

  for (auto vertex_it = query_graph.VertexBegin();
            !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandleType v_handle(vertex_it);
    in_queue[v_handle] = false;
    if (!remove_set[v_handle].empty()) {
     // std::cout << "in queue " << v_handle->id() << std::endl;
      que.push(v_handle);
      in_queue[v_handle] = true;
    }
  }
  // for (auto &[tg_handle, qg_handle_map] : cnt) {
  //   std::cout << "tg_handle " << tg_handle->id() << std::endl;
  //   for (auto &p : qg_handle_map) {
  //     std::cout << "qg_handle " << p.first->id() << " number " << p.second << std::endl;
  //   }
  // }

  for (; !que.empty(); que.pop()) {
    QueryVertexHandleType qg_v_handle = que.front();
    in_queue[qg_v_handle] = false;

    for (auto tg_v_handle : remove_set[qg_v_handle]) {
      auto tag = tg_v_handle->label();

      if (tag2handle.find(tag) != tag2handle.end()) {
        auto &qg_v_handle_vec = tag2handle[tag];

        for (size_t qg_v_handle_idx = 0;
                    qg_v_handle_idx < qg_v_handle_vec.size();
                    qg_v_handle_idx++) {
          QueryVertexHandleType qg_v_handle_2 = qg_v_handle_vec[qg_v_handle_idx];

          if (pre_p[qg_v_handle].find(qg_v_handle_2) != pre_p[qg_v_handle].end()
              && active[tg_v_handle][qg_v_handle_idx]
              && tg_v_handle->CountInEdge(pre_p[qg_v_handle][qg_v_handle_2])) {
            active[tg_v_handle][qg_v_handle_idx] = false;

            for (auto in_edge_it = tg_v_handle->InEdgeBegin(pre_p[qg_v_handle][qg_v_handle_2]);
                     !in_edge_it.IsDone();
                      in_edge_it++) {
              TargetVertexHandleType tg_v_handle_2 = in_edge_it->src_handle();

              if (active.find(tg_v_handle_2) != active.end()
                  && std::find(active[tg_v_handle_2].begin(),
                               active[tg_v_handle_2].end(), true)
                               != active[tg_v_handle_2].end()){
                --cnt[tg_v_handle_2][qg_v_handle_2];

                if (cnt[tg_v_handle_2][qg_v_handle_2] == 0) {
                  remove_set[qg_v_handle_2].insert(tg_v_handle_2);
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


  // for (auto &[tg_handle, qg_handle_map] : cnt) {
  //   std::cout << "tg_handle " << tg_handle->id() << std::endl;
  //   for (auto &p : qg_handle_map) {
  //     std::cout << "qg_handle " << p.first->id() << " number " << p.second << std::endl;
  //   }
  // }
  match_set.clear();

  for (auto &v2vx: active) {
    auto tg_v_handle = v2vx.first;
    auto tg_v_label = tg_v_handle->label();
    for (size_t idx = 0; idx < v2vx.second.size(); idx++) {
      if (!(v2vx.second)[idx]) continue;
      //std::cout << "a candidate" << std::endl;
      match_set[tag2handle[tg_v_label][idx]].emplace_back(tg_v_handle);
    }
  }
  //std::cout << "match_set size " << match_set.size() << std::endl;
  //std::cout << "active size " << active.size() << std::endl;

  // for (auto& [qg_v_handle, tg_v_handle_vec] : match_set) {
  //   std::cout << "qurey graph vertex " << qg_v_handle->id() << std::endl;
  //   for (auto tg_v_handle : tg_v_handle_vec) {
  //     std::cout << "        target graph vertex " << tg_v_handle->id() << std::endl;
  //   }
  // }

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

};

#endif // _GUNDAM_ALGORITHM_SIMULATION_H
