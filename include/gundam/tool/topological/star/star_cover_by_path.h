#ifndef _GUNDAM_TOOL_TOPOLOGICAL_STAR_STAR_COVER_BY_PATH_H
#define _GUNDAM_TOOL_TOPOLOGICAL_STAR_STAR_COVER_BY_PATH_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/tool/connected.h"
#include "gundam/algorithm/vf2.h"
#include "gundam/match/match.h"
#include "gundam/algorithm/match_using_match.h"
#include "gundam/tool/topological/path/is_path.h"
#include "gundam/tool/topological/star/is_star.h"
#include <iostream>

namespace GUNDAM {

/* ########################################################## *
 * ## return false if:                                     ## *
 * ##   1, input star is not a star                        ## *
 * ##  (REMOVED) 2, any pattern in path_set is not a path  ## *
 * ##   3, path_set is empty                               ## *
 * ##   4, any of pathes in input star cannot              ## *
 * ##      be covered by the input path_set                ## *
 * ## else, return true                                    ## *
 * ########################################################## */
template<typename StarGraphType,
         typename PathGraphType>
bool StarCoverByPath(StarGraphType&  star,
         std::vector<PathGraphType>& path_set) {
  static constexpr bool bidirectional = true;


  using StarVertexHandleType = typename VertexHandle<StarGraphType>::type;
  using PathVertexHandleType = typename VertexHandle<PathGraphType>::type;
  using PathVertexIDType = typename PathGraphType::VertexType::IDType;
  using StarVertexIDType = typename StarGraphType::VertexType::IDType;

  if (star.CountVertex() == 1) {
    // the star is an isolated vertex
    auto vertex_it = star.VertexBegin();
    for (auto path_it = path_set.begin(); path_it != path_set.end();
              path_it++) {
      auto [path_src_handle, path_dst_handle] = PathEndPoints<true>(*path_it);
      if (path_src_handle->label() == vertex_it->label()
          || path_dst_handle->label() == vertex_it->label()) {
        return true;
      }
    }
    return false;
  }

  const auto [end_vertex_set,
          central_vertex_handle] = StarEndPoints<bidirectional>(star);
  if (end_vertex_set.empty()) {
    assert(!central_vertex_handle);
    return false;
  }

  auto tmp = central_vertex_handle;

  if (path_set.empty()) {
    return false;
  }

  if (!central_vertex_handle) {
    // the star is a path (a star has onlt 1 or 2 branches)
    auto [pattern_src_handle, pattern_dst_handle] = PathEndPoints<bidirectional>(star);
    for (auto c_vertex_it = star.VertexBegin();
              !c_vertex_it.IsDone(); c_vertex_it++) {
      StarVertexHandleType c_vertex_handle = c_vertex_it;
      if (c_vertex_handle == pattern_dst_handle) continue;  // already been checked when v_vertex_it == pattern_src_handle

      std::set<StarVertexIDType> unmatched_ends;
      unmatched_ends.insert(pattern_dst_handle->id());

      if (c_vertex_handle != pattern_src_handle) {
        unmatched_ends.insert(pattern_src_handle->id());
      }

      for (auto path_it = path_set.begin(); path_it != path_set.end();
                path_it++) {
        auto [path_src_handle, path_dst_handle] = PathEndPoints<true>(*path_it);

        std::function<bool(const Match<PathGraphType, StarGraphType>&)> 
            prune_callback = [](const Match<PathGraphType, StarGraphType>& partial_match) -> bool {
            // prune nothing
          return false;
        };

        auto path_target_handle = path_dst_handle;
        std::function<bool(const Match<PathGraphType, StarGraphType>&)> 
            match_callback = [&unmatched_ends, &path_target_handle]
                    (const Match<PathGraphType, StarGraphType>& match_state) -> bool {

            // matches all endpoints until no unmatched endpoints exist
          const StarVertexIDType id = match_state.MapTo(path_target_handle)->id();
          if (unmatched_ends.find(match_state.MapTo(path_target_handle)->id())
              != unmatched_ends.end()) {
            unmatched_ends.erase(match_state.MapTo(path_target_handle)->id());
            if (unmatched_ends.empty()) {
              return false;
            }
          }
          return true;
         };

        {
          path_target_handle = path_dst_handle;
          Match<PathGraphType, StarGraphType> match;
          match.AddMap(path_src_handle, c_vertex_handle);

          MatchUsingMatch<MatchSemantics::kIsomorphism,
                          MatchAlgorithm::kVf2,
                          MergeNecConfig::kNotMerge>(
                              *path_it, star, match,
                              prune_callback, match_callback
                          );
        }

        if (unmatched_ends.empty()) {
          return true;
        }

        {
          path_target_handle = path_src_handle;
          Match<PathGraphType, StarGraphType> match;
          match.AddMap(path_dst_handle, c_vertex_handle);

          MatchUsingMatch<MatchSemantics::kIsomorphism,
                          MatchAlgorithm::kVf2,
                          MergeNecConfig::kNotMerge>(
                              *path_it, star, match,
                              prune_callback, match_callback
                          );
        }

        if (unmatched_ends.empty()) {
          return true;
        }
      }
    }
    return false;
  } else {
    // the star has more than 2 branches
    std::set<StarVertexIDType> unmatched_ends;
    for (size_t end_vertex_idx = 0;
         end_vertex_idx < end_vertex_set.size(); end_vertex_idx++) {
      unmatched_ends.insert(end_vertex_set[end_vertex_idx]->id());
    }

    for (auto path_it = path_set.begin();
         path_it != path_set.end();
         path_it++) {
      auto [path_src_handle, path_dst_handle] = PathEndPoints<true>(*path_it);
 
      std::function<bool(const Match<PathGraphType, StarGraphType>&)> 
          prune_callback = [](const Match<PathGraphType, StarGraphType>& partial_match) -> bool {
          // prune nothing
        return false;
      };

      PathVertexHandleType path_target_handle;
      std::function<bool(const Match<PathGraphType, StarGraphType>&)> 
          match_callback = [&unmatched_ends, &path_target_handle]
                    (const Match<PathGraphType, StarGraphType>& match) -> bool {

            // matches all endpoints until no unmatched endpoints exist
        if (unmatched_ends.find((match.MapTo(path_target_handle))->id())
            != unmatched_ends.end()) {
          unmatched_ends.erase((match.MapTo(path_target_handle))->id());
          if (unmatched_ends.empty()) {
            return false;
          }
        }
        return true;
      };

      {
        path_target_handle = path_dst_handle;
        Match<PathGraphType, StarGraphType> match;
        match.AddMap(path_src_handle, central_vertex_handle);

        MatchUsingMatch<MatchSemantics::kIsomorphism,
                        MatchAlgorithm::kVf2,
                        MergeNecConfig::kNotMerge>(
                            *path_it, star, match,
                            prune_callback, match_callback
                        );
      }

      if (unmatched_ends.empty()) {
        return true;
      }

      {
        path_target_handle = path_src_handle;
        Match<PathGraphType, StarGraphType> match;
        match.AddMap(path_dst_handle, central_vertex_handle);

        MatchUsingMatch<MatchSemantics::kIsomorphism,
                        MatchAlgorithm::kVf2,
                        MergeNecConfig::kNotMerge>(
                            *path_it, star, match,
                            prune_callback, match_callback
                        );
      }

      if (unmatched_ends.empty()) {
        return true;
      }
    }
    return false;
  }

  assert(false);// should not reach here
  return false;

//optimize:: this is codes for decomposing star into paths
/*
  std::vector<std::vector<StarGraphType>> decomposed_patterns_vec;
  std::vector<VertexHandleType> centres_vec;

  if (!central_vertex_handle) {
    std::vector<StarGraphType> decomposed_patterns;
    
    decomposed_patterns.emplace_back(star);
    decomposed_patterns_vec.emplace_back(decomposed_patterns);
    for (auto centre_vertex_it = star.VertexBegin();
         !centre_vertex_it.IsDone(); centre_vertex_it++) {
      if (centre_vertex_it->CountInEdge() + centre_vertex_it->CountOutEdge() == 1) {
        continue;
      }
      centres_vec.emplace_back(centre_vertex_it);
    }
  } else {
    centres_vec.emplace_back(centre_vertex_handle);
  }

  for (size_t centres_vec_idx = 0; centres_vec_idx < centres_vec.size();
              centres_vec_idx++) {
    VertexHandleType centre_it = centres_vec[centres_vec_idx];

    std::vector<StarTypeGraph> decomposed_patterns;
    for (auto in_edge_it = centre_vertex_it->InEdgeBegin(); 
          !in_edge_it.IsDone(); in_edge_it++) {
      GraphType decomposed_path;

      VertexHandleType current_v = in_edge_it->src_handle();
      VertexHandleType prev_v = centre_vertex_it;

      decomposed_path.AddVertex(prev_v->id(), prev_v->label());
      decomposed_path.AddVertex(current_v->id(), current_v->label());
      decomposed_path.AddEdge(in_edge_it->src_handle()->id(),
                              in_edge_it->dst_handle()->id(),
                              in_edge_it->label(),
                              in_edge_it->id());
      while (true) {
        for (auto c_in_edge_it = current_v->InEdgeBegin();
             !c_in_edge_it.IsDone(); c_in_edge_it++) {
          if (c_in_edge_it->src_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_in_edge_it->src_handle();

          decomposed_path.AddVertex(current_v->id(), current_v->label());
          decomposed_path.AddEdge(c_in_edge_it->src_handle()->id(),
                                  c_in_edge_it->dst_handle()->id(),
                                  c_in_edge_it->label(),
                                  c_in_edge_it->id());
          break;
        }

        for (auto c_out_edge_it = current_v->OutEdgeBegin();
             !c_out_edge_it.IsDone(); c_out_edge_it++) {
          if (c_out_edge_it->dst_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_out_edge_it->dst_handle();

          decomposed_path.AddVertex(current_v->id(), current_v->label());
          decomposed_path.AddEdge(c_out_edge_it->src_handle()->id(),
                                  c_out_edge_it->dst_handle()->id(),
                                  c_out_edge_it->label(),
                                  c_out_edge_it->id());
          break;
        }

        if (current_v->CountInEdge() + current_v->CountOutEdge() == 1) {
          break;
        }
      }
      decomposed_patterns.emplace_back(decomposed_path);
    }

    for (auto out_edge_it = centre_vertex_it->OutEdgeBegin(); 
          !out_edge_it.IsDone(); out_edge_it++) {
      GraphType decomposed_path;

      VertexHandleType current_v = out_edge_it->dst_handle();
      VertexHandleType prev_v = centre_vertex_it;

      decomposed_path.AddVertex(prev_v->id(), prev_v->label());
      decomposed_path.AddVertex(current_v->id(), current_v->label());
      decomposed_path.AddEdge(out_edge_it->src_handle()->id(),
                              out_edge_it->dst_handle()->id(),
                              out_edge_it->label(),
                              out_edge_it->id());
      while (true) {
        for (auto c_in_edge_it = current_v->InEdgeBegin();
             !c_in_edge_it.IsDone(); c_in_edge_it++) {
          if (c_in_edge_it->src_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_in_edge_it->src_handle();

          decomposed_path.AddVertex(current_v->id(), current_v->label());
          decomposed_path.AddEdge(c_in_edge_it->src_handle()->id(),
                                  c_in_edge_it->dst_handle()->id(),
                                  c_in_edge_it->label(),
                                  c_in_edge_it->id());
          break;
        }

        for (auto c_out_edge_it = current_v->OutEdgeBegin();
             !c_out_edge_it.IsDone(); c_out_edge_it++) {
          if (c_out_edge_it->dst_handle() == prev_v) continue;

          prev_v = current_v;
          current_v = c_out_edge_it->dst_handle();

          decomposed_path.AddVertex(current_v->id(), current_v->label());
          decomposed_path.AddEdge(c_out_edge_it->src_handle()->id(),
                                  c_out_edge_it->dst_handle()->id(),
                                  c_out_edge_it->label(),
                                  c_out_edge_it->id());
          break;
        }

        if (current_v->CountInEdge() + current_v->CountOutEdge() == 1) {
          break;
        }
      }
      decomposed_patterns.emplace_back(decomposed_path);
    }
    decomposed_patterns_vec.emplace_back(decomposed_patterns);
  }

  std::set<IDType> unmatched_vertices;

  for (auto vertex_it = star.VertexBegin();
        !vertex_it.IsDone(); vertex_it++) {
    unmatched_vertices.insert(vertex_it->id());
  }

  for (size_t centre_idx = 0; centre_idx < decomposed_patterns_vec.size();
              centre_idx++) {
    auto& patterns = decomposed_patterns_vec[centre_idx];
    std::set unmatched_v = unmatched_vertices;
    for (size_t pattern_idx = 0; pattern_idx < patterns.size();
                pattern_idx++) {
      auto [pattern_link_src_handle, pattern_link_dst_handle] = PathEndPoints<true>(patterns[patterns_idx]);
      for (auto path_candidate_it = path_set.begin();
                path_candidate_it != path_set.end();
                path_candidate_it++) {
        auto [candidate_link_src_handle, candidate_link_dst_handle]
                = PathEndPoints<true>(*path_candidate_it);

        Match<StarGraphType, PathGraphType> match;
        match.AddMap(pattern_link_src_handle, candidate_link_src_handle);
        MatchUsingMatch<MatchSemantics::kIsomorphism,
                        MatchAlgorithm::kVf2,
                        MatchNecConfid::kNotMerge>(
                          patterns[patterns_idx], *path_candidate_it,
                          match, 1)
      }
    }

    
  }*/

/*  if (!central_vertex_handle) {
    // The pattern is considered as a path
    decomposed_patterns.emplace_back(star);
    for (auto centre_vertex_it = star.VertexBegin();
         !centre_vertex_it.IsDone(); centre_vertex_it++) {
      if (centre_vertex_it->CountInEdge() + centre_vertex_it->CountOutEdge() == 1) {
        continue;
      }
      assert(centre_vertex_it->CountInEdge() + centre_vertex_it->CountOutEdge() == 2);

      GraphType half_path;
      for (auto in_edge_it = centre_vertex_it->InEdgeBegin(); 
           !in_edge_it.IsDone(); in_edge_it++) {
        VertexHandleType current_v = in_edge_it->src_handle();
        VertexHandleType prev_v = centre_vertex_it;
        while (true) {
          if (current_v->CountInEdge() + current_v->CountOutEdge() == 1) {
            half_path.AddVertex(current_v);
            break;
          }

          for (auto c_in_edge_it = current_v->InEdgeBegin();
               !c_in_edge_it.IsDone(); c_in_edge_it++) {
            if (c_in_edge_it->src_handle() == prev_v) continue;

            prev_v = current_v;
            current_v = c_in_edge_it->src_handle();
            break;
          }

          for (auto c_out_edge_it = current_v->OutEdgeBegin();
               !c_out_edge_it.IsDone(); c_out_edge_it++) {
            if (c_out_edge_it->dst_handle() == prev_v) continue;

            prev_v = current_v;
            current_v = c_out_edge_it->dst_handle();
            break;
          }

        }
      }
    }
  } else {
    // The pattern is a star(more than 2 branches)

  }
*/



}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_STAR_STAR_COVER_BY_PATH_H
