#ifndef _GUNDAM_TOOL_HAS_ISOLATE_VERTEX_H
#define _GUNDAM_TOOL_HAS_ISOLATE_VERTEX_H

#include "gundam/type_getter/vertex_id.h"
#include "gundam/type_getter/vertex_handle.h"
namespace GUNDAM {

template <typename GraphType>
inline bool HasIsolateVertex(const GraphType& graph) {
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    if (vertex_it->CountOutEdge() == 0
     && vertex_it->CountInEdge() == 0){
      // this vertex is isolated
      return true;
    }
  }
  return false;
}

template <typename GraphType>
inline std::set<typename VertexHandle<GraphType>::type> IsolateVertexSet(GraphType& graph) {
  std::set<typename VertexHandle<GraphType>::type> isolate_vertex_set;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    if (vertex_it->CountOutEdge() == 0
     && vertex_it->CountInEdge() == 0){
      // this vertex is isolated
      isolate_vertex_set.emplace(vertex_it);
    }
  }
  return isolate_vertex_set;
}

template <typename GraphType>
inline std::set<typename VertexID<GraphType>::type> IsolateVertexIDSet(const GraphType& graph) {
  std::set<typename VertexID<GraphType>::type> isolate_vertex_id_set;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    if (vertex_it->CountOutEdge() != 0
     || vertex_it->CountInEdge()  != 0){
      continue;
    }
    // this vertex is isolated
    isolate_vertex_id_set.emplace(vertex_it->id());
  }
  return isolate_vertex_id_set;
}

template <typename GraphType>
inline size_t CountIsolateVertex(const GraphType& graph) {
  size_t counter = 0;
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    if (vertex_it->CountOutEdge() == 0
     && vertex_it->CountInEdge() == 0){
      // this vertex is isolated
      counter++;
    }
  }
  return counter;
}

// remove all isolated vertexes and 
// return how many vertexes are removed
template <typename GraphType>
inline size_t RemoveIsolateVertex(GraphType& graph) {
  static_assert(!std::is_const_v<GraphType>, "graph should not be constant");
  using VertexIDType = typename GraphType::VertexType::IDType;
  std::set<VertexIDType> isolated_vertex_id_set;
  // just begin bfs at a random vertex and find whether
  // it can reach all vertexes
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    if (vertex_it->CountOutEdge() != 0
     || vertex_it->CountInEdge() != 0){
      // this vertex is not isolated
      continue;
    }
    // this vertex is isolated
    auto [ret_it, ret] = isolated_vertex_id_set.emplace(vertex_it->id());
    assert(ret);
  }
  for (const auto& isolated_vertex_id
                 : isolated_vertex_id_set){
    graph.EraseVertex(isolated_vertex_id);
  }
  return isolated_vertex_id_set.size();
}

// remove all isolated vertexes and 
// return  ids of removed vertices
template <typename GraphType>
inline uint64_t RemoveIsolateVertex(GraphType& graph,
        std::set<typename GraphType::VertexType::IDType>& isolated_vertex_id_set) {
  static_assert(!std::is_const_v<GraphType>, "graph should not be constant");
  using VertexIDType = typename GraphType::VertexType::IDType;
  isolated_vertex_id_set.clear();
  // just begin bfs at a random vertex and find whether
  // it can reach all vertexes
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++){
    if (vertex_it->CountOutEdge() != 0
     || vertex_it->CountInEdge() != 0){
      // this vertex is not isolated
      continue;
    }
    // this vertex is isolated
    auto [ret_it, ret] = isolated_vertex_id_set.emplace(vertex_it->id());
    assert(ret);
  }
  for (const auto& isolated_vertex_id
                 : isolated_vertex_id_set){
    graph.EraseVertex(isolated_vertex_id);
  }
  return true;
}

}  // namespace GUNDAM

#endif // _GUNDAM_TOOL_HAS_ISOLATE_VERTEX_H