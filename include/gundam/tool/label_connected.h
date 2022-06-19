#ifndef _GUNDAM_TOOL_LABEL_CONNECTED_H
#define _GUNDAM_TOOL_LABEL_CONNECTED_H

#include "include/gundam/type_getter/vertex_label.h"
#include "include/gundam/tool/max_id.h"
#include "include/gundam/algorithm/bfs.h"

#include <vector>

namespace GUNDAM {

// return whether the set of input vertex label
// are connected, e.g. whether there exists 
// a path, longer than two vertexes, contains all
// and only the labels in the input vertex label set
template <typename GraphType>
inline bool LabelConnected(const GraphType& graph,
          const std::vector<
          typename GUNDAM::VertexLabel<GraphType>::type>& vertex_label_set,
          const std::vector<
          typename GUNDAM::  EdgeLabel<GraphType>::type>&   edge_label_set) {

  using VertexHandleType = typename VertexHandle<const GraphType>::type;
  using   EdgeHandleType = typename   EdgeHandle<const GraphType>::type;

  auto sorted_vertex_label_set = vertex_label_set;
  std::sort(sorted_vertex_label_set.begin(),
            sorted_vertex_label_set.end());

  auto sorted_edge_label_set = edge_label_set;
  std::sort(sorted_edge_label_set.begin(),
            sorted_edge_label_set.end());

  auto max_vertex_id = MaxVertexId(graph);
  std::vector<bool> visited_vertex_set(max_vertex_id + 1, false);

  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {

    if (visited_vertex_set[vertex_it->id()]) {
      continue;
    }

    if (!std::binary_search(sorted_vertex_label_set.begin(), 
                            sorted_vertex_label_set.end(), 
                                   vertex_it->label())) {
      continue;
    }

    std::vector<bool> visited_vertex_label_set(
                       sorted_vertex_label_set.size(), false);
    std::vector<bool> visited_edge_label_set(
                       sorted_edge_label_set.size(), false);

    size_t vertex_label_counter = 0,
         visited_vertex_counter = 0;

    auto prune_distance_callback = [&vertex_label_counter,
                             &sorted_vertex_label_set,
                               &sorted_edge_label_set,
                            &visited_vertex_label_set,
                              &visited_edge_label_set](
              const VertexHandleType&  vertex_handle,
                const EdgeHandleType&    edge_handle) -> bool {
      if (!std::binary_search(sorted_vertex_label_set.begin(),
                              sorted_vertex_label_set.end(),
                                     vertex_handle->label())) {
        // is not in the set of vertex label to be considered
        // continue matching
        return true;
      }
      if (!std::binary_search(sorted_edge_label_set.begin(),
                              sorted_edge_label_set.end(),
                                     edge_handle->label())) {
        // is not in the set of edge label to be considered
        // continue matching
        return true;
      }
      // not prune
      auto sorted_vertex_label_set_it 
         = std::lower_bound(sorted_vertex_label_set.begin(), 
                            sorted_vertex_label_set.end(), 
                                   vertex_handle->label());
      assert((sorted_vertex_label_set_it != sorted_vertex_label_set.end()) 
          &&(*sorted_vertex_label_set_it ==        vertex_handle->label()));
      size_t sorted_vertex_label_idx 
           = sorted_vertex_label_set_it 
           - sorted_vertex_label_set.begin();
      vertex_label_counter += (1 - visited_vertex_label_set[sorted_vertex_label_idx]);
      visited_vertex_label_set[sorted_vertex_label_idx] = true;

      auto sorted_edge_label_set_it 
         = std::lower_bound(sorted_edge_label_set.begin(), 
                            sorted_edge_label_set.end(), 
                                   edge_handle->label());
      assert((sorted_edge_label_set_it != sorted_edge_label_set.end()) 
          &&(*sorted_edge_label_set_it ==        edge_handle->label()));
      size_t sorted_edge_label_idx 
           = sorted_edge_label_set_it 
           - sorted_edge_label_set.begin();
      visited_edge_label_set[sorted_edge_label_idx] = true;
      return false;
    };

    auto bfs_callback = [&visited_vertex_counter,
                         &vertex_label_counter,
                         &visited_vertex_set,
                         &sorted_vertex_label_set](const VertexHandleType& vertex_handle) -> bool {
      visited_vertex_counter++;
      visited_vertex_set[vertex_handle->id()] = true;
      if (sorted_vertex_label_set.size()
              == vertex_label_counter) {
        if (sorted_vertex_label_set.size() != 1) {
          // has reached all vertex label
          return false;
        }
        return !(visited_vertex_counter > 1);
      }
      return true;
    };

    GUNDAM::Bfs<true>(graph, vertex_it, bfs_callback, 
                             prune_distance_callback);

    if (sorted_vertex_label_set.size() != 1) {
      if (sorted_vertex_label_set.size()
              == vertex_label_counter) {
        return true;
      }
    }
    if (visited_vertex_counter > 1) {
      return true;
    }
  }
  return false;
}

};

#endif // _GUNDAM_TOOL_LABEL_CONNECTED_H