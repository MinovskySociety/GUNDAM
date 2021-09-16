#ifndef _GUNDAM_ALGORITHM_NEIGHBORHOOD_EQUIVALENCE_CLASS
#define _GUNDAM_ALGORITHM_NEIGHBORHOOD_EQUIVALENCE_CLASS

#include "gundam/match/match.h"

#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {

namespace _neighborhood_equivalence_class {

enum EdgeState { 
  kIn, 
  kOut 
};

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

};

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
           std::set<std::tuple<_neighborhood_equivalence_class::EdgeState,
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
    auto adjacent_vertex = _neighborhood_equivalence_class::AdjacentVertex<EdgeLabelType, VertexIDType>(vertex_handle);
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

template <typename GraphType>
std::vector<std::vector<typename VertexHandle<GraphType>::type>> 
                Nec(GraphType& graph)  {
  return Nec(graph, Match<GraphType, GraphType>());
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

#endif // _GUNDAM_ALGORITHM_NEIGHBORHOOD_EQUIVALENCE_CLASS
