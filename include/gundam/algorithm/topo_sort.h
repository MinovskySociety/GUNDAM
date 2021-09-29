#ifndef _GUNDAM_ALGORITHM_TOPO_SORT_H
#define _GUNDAM_ALGORITHM_TOPO_SORT_H

#include "include/gundam/type_getter/vertex_handle.h"
#include "include/gundam/type_getter/edge_handle.h"
#include "include/gundam/type_getter/edge_id.h"

#include <set>
#include <queue>
#include <vector>
#include <functional>

namespace GUNDAM {

template <bool bidirectional = false,
          typename QueryGraph, 
          typename InDegreeContainer>
inline size_t TopoSort(QueryGraph& graph, 
                InDegreeContainer& in_degree,
  const std::function<bool(const typename VertexHandle<QueryGraph>::type&)>& topo_callback) {

  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;
  using   QueryEdgeIDType = typename       EdgeID<QueryGraph>::type;

  size_t counter = 0;

  std::queue<QueryVertexHandle> topo_sort_queue;
  std::set<QueryEdgeIDType> used_edge;
  // topo sort
  for (auto it = graph.VertexBegin(); 
           !it.IsDone(); it++) {
    QueryVertexHandle query_vertex_handle = it;
    assert(in_degree.find(query_vertex_handle)
        != in_degree.end());
    if (in_degree[query_vertex_handle] == 0) {
      topo_sort_queue.emplace(query_vertex_handle);
    }
  }
  while (!topo_sort_queue.empty()) {
    QueryVertexHandle query_vertex_handle = topo_sort_queue.front();
    topo_sort_queue.pop();
    counter++;
    if (!topo_callback(query_vertex_handle)) {
      return counter;
    }
    for (auto edge_it = query_vertex_handle->OutEdgeBegin(); 
             !edge_it.IsDone();
              edge_it++) {
      if (used_edge.count(edge_it->id())) {
        continue;
      }
      QueryVertexHandle next_vertex_handle 
                     = edge_it->dst_handle();
      assert(in_degree.find(next_vertex_handle)
          != in_degree.end());
      in_degree[next_vertex_handle]--;
      used_edge.insert(edge_it->id());
      if (in_degree[next_vertex_handle] == 0) {
        topo_sort_queue.emplace(next_vertex_handle);
      }
    }
    if constexpr (bidirectional) {
      for (auto edge_it = query_vertex_handle->InEdgeBegin(); 
               !edge_it.IsDone();
                edge_it++) {
        if (used_edge.count(edge_it->id())) {
          continue;
        }
        QueryVertexHandle next_vertex_handle 
                       = edge_it->src_handle();
        assert(in_degree.find(next_vertex_handle)
            != in_degree.end());
        in_degree[next_vertex_handle]--;
        used_edge.insert(edge_it->id());
        if (in_degree[next_vertex_handle] == 0) {
          topo_sort_queue.emplace(next_vertex_handle);
        }
      }
    }
  }
  return counter;
}

template <bool bidirectional = false,
          typename QueryGraph>
inline size_t TopoSort(QueryGraph& graph,
  const std::function<bool(const typename VertexHandle<QueryGraph>::type&)>& topo_callback) {

  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;

  std::map<QueryVertexHandle, size_t> in_degree;

  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    QueryVertexHandle vertex_handle = vertex_it;
    size_t vertex_in_degree = vertex_handle->CountOutEdge();
    if constexpr (bidirectional) {
      vertex_in_degree += vertex_handle->CountInEdge();
    }
    auto [ in_degree_it,
           in_degree_ret ] = in_degree.emplace(vertex_handle,
                                               vertex_in_degree);
    assert(in_degree_ret);
  }

  assert(in_degree.size() == graph.CountVertex());

  return TopoSort<bidirectional>(graph,
                                 in_degree,
                                 topo_callback);
}

template <bool bidirectional = false,
          typename QueryGraph>
inline size_t TopoSort(QueryGraph& graph,
  std::vector<typename VertexHandle<QueryGraph>::type> &topo_seq) {

  using QueryVertexHandle = typename VertexHandle<QueryGraph>::type;

  assert(topo_seq.empty());
  topo_seq.clear();

  std::function<bool(const QueryVertexHandle&)> 
    topo_callback 
       = [&topo_seq](const QueryVertexHandle& vertex_handle) -> bool {
    topo_seq.emplace_back(vertex_handle);
    return true;
  };

  return TopoSort<bidirectional>(graph, topo_callback);
}
 
};

#endif // _GUNDAM_ALGORITHM_TOPO_SORT_H