#ifndef _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H
#define _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H

#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"

#include "gundam/match/match.h"
#include "gundam/tool/max_id.h"

#include <vector>
#include <set>
#include <map>
#include <iostream>

namespace GUNDAM {

enum MergeGraphReturnType : char {
  not_return,
  match_from_graph_1
};

// use merge to specify the vertex pair between graph_0 
// and graph_1 to merge
template <enum MergeGraphReturnType return_type 
             = MergeGraphReturnType::not_return,
          typename GraphType0,
          typename GraphType1>
inline auto MergeGraph(GraphType0& graph_0,
                       GraphType1& graph_1,
           const Match<GraphType1,
                       GraphType0>& match_graph_1_to_graph_0) {

  auto max_vertex_id = MaxVertexId(graph_0);
  max_vertex_id++;

  using VertexIDType0 = typename VertexID<GraphType0>::type;
  using VertexIDType1 = typename VertexID<GraphType1>::type;

  std::unordered_map<VertexIDType0,
                     VertexIDType1> id_map;

  // merge vertexes 
  for (auto graph_1_vertex_it = graph_1.VertexBegin();
           !graph_1_vertex_it.IsDone();
            graph_1_vertex_it++) {
    auto graph_0_vertex_handle = match_graph_1_to_graph_0.MapTo(graph_1_vertex_it);
    if ( graph_0_vertex_handle ) {
      // graph_0_vertex_handle is not null
      // graph_0_vertex_handle has already
      // been contained in graph_1
      continue;
    }
    auto [vertex_handle, add_vertex_ret] 
        = graph_0.AddVertex(max_vertex_id++,
                        graph_1_vertex_it->label());
    if (!add_vertex_ret) {
      // is not added successfully
      if constexpr (return_type == MergeGraphReturnType::not_return) {
        return false;
      }
      else {
        return Match<GraphType1, GraphType0>();
      }
    }
    id_map.emplace(graph_1_vertex_it    ->id(),
                           vertex_handle->id());
    if constexpr (GraphParameter<GraphType0>::vertex_has_attribute) {
      for (auto attr_it = graph_1_vertex_it->AttributeBegin();
               !attr_it.IsDone(); 
                attr_it++){
        auto [attr_handle, attr_ret] 
          = vertex_handle->AddAttribute(attr_it->key(), 
                                        attr_it->value_type(), 
                                        attr_it->value_str());
        assert(!attr_handle.IsNull());
        assert(attr_ret);
      }
    }
  }

  // merge edges
  for (auto graph_1_vertex_it = graph_1.VertexBegin();
           !graph_1_vertex_it.IsDone();
            graph_1_vertex_it++){
    for (auto out_edge_it = graph_1_vertex_it->OutEdgeBegin();
             !out_edge_it.IsDone(); 
              out_edge_it++){

      auto src_id_map_it = id_map.find(out_edge_it->src_handle()->id());
      auto dst_id_map_it = id_map.find(out_edge_it->dst_handle()->id());

      auto [edge_handle, add_edge_ret] 
        = graph_0.AddEdge(src_id_map_it == id_map.end()? out_edge_it->src_handle()->id() 
                        : src_id_map_it->second,
                          dst_id_map_it == id_map.end()? out_edge_it->dst_handle()->id() 
                        : dst_id_map_it->second,
                          out_edge_it->label(),
                          out_edge_it->id());
      if (!add_edge_ret) {
        // already existed
        continue;
      }
      assert(edge_handle);
      if constexpr (GraphParameter<GraphType0>::edge_has_attribute) {
        for (auto attr_it = out_edge_it->AttributeBegin();
                 !attr_it.IsDone(); 
                  attr_it++) {
          auto [attr_handle, attr_ret] 
              = edge_handle->AddAttribute(attr_it->key(), 
                                          attr_it->value_type(), 
                                          attr_it->value_str());
          assert(!attr_handle.IsNull());
          assert(attr_ret);
        }
      }
    }
  }

  if constexpr (return_type == MergeGraphReturnType::not_return) {
    return true;
  }
  else {
    using ReturnMatchType = Match<GraphType1, GraphType0>;

    ReturnMatchType match_to_merged_graph;

    for (const auto& [graph_1_id, 
                 merged_graph_id] : id_map) {
      match_to_merged_graph.AddMap(graph_1.FindVertex(     graph_1_id),
                                   graph_0.FindVertex(merged_graph_id));
    }
    return match_to_merged_graph;
  }
}


/* ##################################################### *
 * ##  merge the input set of graph, join the set of  ## *
 * ##  vertex with given id                           ## *
 * ##################################################### */
template <typename GraphType>
inline std::remove_cv_t<
       std::remove_reference_t<GraphType>> 
       MergeGraphSet(const std::vector<GraphType>& input_graph_vec,
                     const std::vector<
                     typename VertexID<GraphType>::type>& join_vertex_id_vec
                   = std::vector<
                     typename VertexID<GraphType>::type>() ) {

  using ReturnGraphType = std::remove_cv_t<
                   std::remove_reference_t<GraphType>>;
  using VertexIDType = typename VertexID<GraphType>::type;
  using EdgeIDType = typename EdgeID<GraphType>::type;
  using   EdgeHandleType = typename GUNDAM::  EdgeHandle<GraphType>::type;
  using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;
  using VertexConstHandleType = typename GUNDAM::VertexHandle<const GraphType>::type;

  ReturnGraphType merged_graph;
  if (input_graph_vec.empty()) return merged_graph;

  #ifndef NDEBUG
  for (const auto& join_vertex_id : join_vertex_id_vec) {
    for (const auto& input_graph : input_graph_vec) {
      // should be contained in all input graphs
      assert(input_graph.FindVertex(join_vertex_id));
    }
  }
  #endif // NDEBUG


  std::set<VertexIDType> join_vertex_id_set;
  for(const auto & join_vertex_id : join_vertex_id_vec) {
    join_vertex_id_set.insert(join_vertex_id);
  }

  VertexIDType unassigned_vertex_id = 0;
  EdgeIDType unassigned_edge_id = 0;


  for (const auto & input_graph : input_graph_vec) {
    std::map<VertexIDType, VertexHandleType> vid2handle;
    for (auto vertex_it = input_graph.VertexBegin();
              !vertex_it.IsDone();
              vertex_it++) {
      if (join_vertex_id_set.find(vertex_it->id()) == join_vertex_id_set.end()) {
        vid2handle[vertex_it->id()] = VertexHandleType();
      } else {
        vid2handle[vertex_it->id()] = merged_graph.FindVertex(vertex_it->id());
      }
    }

    for (auto vertex_it = input_graph.VertexBegin();
              !vertex_it.IsDone();
              vertex_it++) {
      VertexIDType merged_src_id;

      if ((vid2handle[vertex_it->id()])) {
        merged_src_id = (vid2handle[vertex_it->id()])->id();
      } else {
        if (join_vertex_id_set.find(vertex_it->id()) != join_vertex_id_set.end()) {
          merged_src_id = vertex_it->id();
        } else {
//          merged_src_id = MaxVertexId<GraphType>(merged_graph) + 1;
          merged_src_id = unassigned_vertex_id;
          unassigned_vertex_id++;

          while (join_vertex_id_set.find(merged_src_id) != join_vertex_id_set.end()) {
            merged_src_id = unassigned_vertex_id;;
            unassigned_vertex_id++;
          }
        }

        auto [merged_src_handle, merged_src_ret] = merged_graph.AddVertex(merged_src_id, vertex_it->label());
        vid2handle[vertex_it->id()] = merged_src_handle;
        if (!merged_src_ret) {
          std::cout << "error inserting src" << std::endl;
        }

        if constexpr (GUNDAM::GraphParameter<GraphType>::vertex_has_attribute) {
          // add attribute to new_vertex
          for (auto attr_it = vertex_it->AttributeBegin();
                    !attr_it.IsDone();
                    attr_it++){
            auto [attr_handle, ret] = merged_src_handle->AddAttribute(
                                            attr_it->key(),
                                            attr_it->value_type(),
                                            attr_it->value_str());
          }
        }
      }

      for (auto out_edge_it = vertex_it->OutEdgeBegin();
                !out_edge_it.IsDone();
                out_edge_it++) {
        VertexIDType merged_dst_id;
//        auto dst_handle = out_edge_it->dst_handle();
        VertexConstHandleType dst_handle = out_edge_it->dst_handle();

        if ((vid2handle[dst_handle->id()])) {
          merged_dst_id = (vid2handle[dst_handle->id()])->id();
        } else {
          if (join_vertex_id_set.find(dst_handle->id()) != join_vertex_id_set.end()) {
            merged_dst_id = dst_handle->id();
          } else {
//            merged_dst_id = MaxVertexId<GraphType>(merged_graph) + 1;
            merged_dst_id = unassigned_vertex_id;
            unassigned_vertex_id++;

            while (join_vertex_id_set.find(merged_dst_id) != join_vertex_id_set.end()) {
            merged_dst_id = unassigned_vertex_id;;
            unassigned_vertex_id++;
//              merged_dst_id++;
            }
          }

          auto [merged_dst_handle, merged_dst_ret] = merged_graph.AddVertex(merged_dst_id, dst_handle->label());
          vid2handle[dst_handle->id()] = merged_dst_handle;
          if (!merged_dst_ret) {
            std::cout << "error inserting dst" << std::endl;
          }

          if constexpr (GUNDAM::GraphParameter<GraphType>::vertex_has_attribute) {
            // add attribute to new_vertex
            for (auto attr_it = dst_handle->AttributeBegin();
                      !attr_it.IsDone();
                      attr_it++){
              auto [attr_handle, ret] = merged_dst_handle->AddAttribute(
                                              attr_it->key(),
                                              attr_it->value_type(),
                                              attr_it->value_str());
            }
          }
        }

        EdgeIDType merged_edge_id = unassigned_edge_id;
        unassigned_edge_id++;
//        EdgeIDType merged_edge_id = MaxEdgeId<GraphType>(merged_graph) + 1;
        auto [merged_edge_handle, merged_edge_ret] = merged_graph.AddEdge(merged_src_id,
                                                        merged_dst_id,
                                                        out_edge_it->label(),
                                                        merged_edge_id);
        if (!merged_edge_ret) {
          std::cout << "error inserting edge" << std::endl;
        }

        if constexpr (GUNDAM::GraphParameter<GraphType>::edge_has_attribute) {
          for (auto attr_it = out_edge_it->AttributeBegin();
                   !attr_it.IsDone();
                    attr_it++){
            auto [attr_handle, ret] = merged_edge_handle->AddAttribute(
                                            attr_it->key(),
                                            attr_it->value_type(),
                                            attr_it->value_str());
          }
        }
      }
    }
  }


  #ifndef NDEBUG
  for (const auto& join_vertex_id : join_vertex_id_set) {
    // should also be contained in the merged graph
    assert(merged_graph.FindVertex(join_vertex_id));
  }
  #endif // NDEBUG
  return merged_graph;
};
}
#endif // _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H
