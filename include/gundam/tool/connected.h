#ifndef _GUNDAM_TOOL_CONNECTED_H
#define _GUNDAM_TOOL_CONNECTED_H

#include "gundam/algorithm/dfs.h"

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_id.h"

namespace GUNDAM {

template <bool bidirectional = true,
          typename GraphType>
inline bool Connected(const GraphType& graph) {
  // just begin dfs at a random vertex and find whether
  // it can reach all vertexes
  assert(graph.CountVertex() > 0);
  typename VertexHandle<const GraphType>::type
           vertex_handle = graph.VertexBegin();
  assert(vertex_handle);
  return GUNDAM::Dfs<bidirectional>(graph,vertex_handle)
                                 == graph.CountVertex();
}

template <bool bidirectional = true,
          typename GraphType>
inline bool Connected(const GraphType& graph,
          const std::set<
          typename VertexID<GraphType>::type>& vertex_id_set) {
  assert(!vertex_id_set.empty());
  if (vertex_id_set.size() == 1) {
    return true;
  }
  using VertexIDType = typename VertexID<GraphType>::type;
  // remove all vertexes not contained in vertex_id_set
  // then find whether the remained graph are still connected
  std::vector<VertexIDType> vertex_id_need_to_be_removed;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    if (vertex_id_set.find(vertex_it->id())
     != vertex_id_set.end()) {
      // contained in literals, cannot be removed
      continue;
    }
    vertex_id_need_to_be_removed.emplace_back(vertex_it->id());
  }
  assert(vertex_id_need_to_be_removed.size()
       + vertex_id_set.size() == graph.CountVertex());

  GraphType remained_vertexes(graph);
  for (const auto& vertex_id : vertex_id_need_to_be_removed) {
    auto ret = remained_vertexes.EraseVertex(vertex_id);
    assert(ret);
  }
  return Connected(remained_vertexes);
}

}  // namespace GUNDAM

#endif // _GUNDAM_TOOL_CONNECTED_H