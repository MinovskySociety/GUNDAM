#ifndef _GUNDAM_ALGORITHM_NEIGHBORHOOD_EQUIVALENCE_CLASS
#define _GUNDAM_ALGORITHM_NEIGHBORHOOD_EQUIVALENCE_CLASS

#include "gundam/match/match.h"

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/component/disjoint_set.h"

#include "gundam/util/util.h"

namespace GUNDAM {

namespace _neighborhood_equivalence_class {

template<typename    EdgeLabelType,
         typename     VertexIDType,
         typename VertexHandleType>
auto AdjacentVertex(const VertexHandleType& vertex) {
  std::set<std::tuple<enum EdgeDirection,
                           EdgeLabelType,
                           VertexIDType>> adjacent_vertex_set;
  for (auto in_edge_it = vertex->InEdgeBegin();
           !in_edge_it.IsDone();
            in_edge_it++) {
    adjacent_vertex_set.emplace(EdgeDirection::kIn,
                             in_edge_it->label(),
                             in_edge_it->src_handle()->id());
  }
  for (auto out_edge_it = vertex->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    adjacent_vertex_set.emplace(EdgeDirection::kOut,
                            out_edge_it->label(),
                            out_edge_it->dst_handle()->id());
  }
  return adjacent_vertex_set;
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
           std::set<std::tuple<enum EdgeDirection,
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
    // add if not existnec_set
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
  return nec_set;
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
  return query_graph_removed;
}

// remove duplicate target vertexes that in the same Nec from the
// candidate set
template <typename  QueryGraph,
          typename TargetGraph>
void RemoveDuplicateCandidate(std::  map <typename VertexHandle< QueryGraph>::type,
                              std::vector<typename VertexHandle<TargetGraph>::type>>& candidate_set,
                        const std::vector<
                              std::vector<typename VertexHandle<TargetGraph>::type>>& nec_set) {

  using TargetVertexHandleType = typename VertexHandle<TargetGraph>::type;

  DisjointSet<TargetVertexHandleType, false> disjoint_set;
  for (const auto& nec : nec_set) {
    for (const TargetVertexHandleType& target_handle : nec) {
      disjoint_set.Merge(target_handle, nec.front());
    }
  }
  
  for (auto& [query_handle, candidate] : candidate_set) {
    std::set<TargetVertexHandleType> contained_nec_set;
    for (auto candidate_it  = candidate.begin(); 
              candidate_it != candidate. end ();) {
      const TargetVertexHandleType representive_handle = disjoint_set.Find(*candidate_it);
      if (contained_nec_set.emplace(representive_handle).second) {
        // added successfully, the candidate in this nec have been considered before
        candidate_it++;
        continue;
      }
      // added failed, has already considered this nec before, remove it from the candidate set
      candidate_it = candidate.erase(candidate_it);
    }
  }

  return;
}

};

#endif // _GUNDAM_ALGORITHM_NEIGHBORHOOD_EQUIVALENCE_CLASS
