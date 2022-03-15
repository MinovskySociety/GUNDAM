#ifndef _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H
#define _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H

#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"
#include "gundam/tool/max_id.h"

#include <vector>
#include <set>
#include <map>
#include <iostream>

namespace GUNDAM {

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
