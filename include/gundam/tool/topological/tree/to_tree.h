#ifndef _GUNDAM_TOOL_TOPOLOGICAL_TREE_TO_TREE_H
#define _GUNDAM_TOOL_TOPOLOGICAL_TREE_TO_TREE_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_id.h"

#include "gundam/tool/max_id.h"

#include <queue>

namespace GUNDAM {

// convert one general graph to tree,
// rooted at input root
template <typename GraphType>
auto ToTree(GraphType& graph,
  const typename VertexHandle<GraphType>::type& root) {
  std::remove_cv_t<GraphType> trans_graph;
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  using VertexIdType     = typename VertexID    <GraphType>::type;
  using   EdgeIdType     = typename   EdgeID    <GraphType>::type;

  std::queue<VertexHandleType> vertex_queue;
  vertex_queue.emplace(root);

  auto [ vertex_it,
         vertex_ret ] = trans_graph.AddVertex(root->id(),
                                              root->label());
  assert(vertex_ret);

  VertexIdType max_vertex_id = MaxVertexId(graph);
  EdgeIdType   max_edge_id   = MaxEdgeId  (graph);
  max_vertex_id++;
  max_edge_id++;

  // the set of edges that have been added into the trans_graph
  std::vector<bool> added_edge_set  (max_edge_id,   false); 
  // the set of vertexes that have been added into the trans_graph
  std::vector<bool> added_vertex_set(max_vertex_id, false); 
  added_vertex_set[root->id()] = true;
  while (!vertex_queue.empty()) {
    auto current_handle = vertex_queue.front();
    vertex_queue.pop();
    for (auto out_edge_it = current_handle->OutEdgeBegin();
             !out_edge_it.IsDone();
              out_edge_it++) {
      auto dst_handle =  out_edge_it->dst_handle();
      if ( added_edge_set[out_edge_it->id()] ) {
        // this edge has been considered before
        assert(added_vertex_set[dst_handle->id()]);
        assert(trans_graph.FindEdge(out_edge_it->id()));
        continue;
      }
      added_edge_set[out_edge_it->id()] = true;
      VertexIdType new_vertex_id = dst_handle->id();
      if (added_vertex_set[new_vertex_id]) {
        // the dst handle has been considered in the previous 
        // searching process, add a new vertex to make the
        // trans_graph a tree
        assert(trans_graph.FindVertex(new_vertex_id));
        // alloc a new vertex id
        new_vertex_id = max_vertex_id++;
      }
      else {
        added_vertex_set[dst_handle->id()] = true;
        vertex_queue.emplace(dst_handle);
      }
      // whether new_vertex_id == dst_handle->id() or not,
      // dst_handle->id() should have already existed in trans_graph
      assert(added_vertex_set[dst_handle->id()]);
      auto [ vertex_it,
             vertex_ret ] = trans_graph.AddVertex(new_vertex_id, 
                                                  dst_handle->label());
      // should have been added successfully
      assert(vertex_ret);
      auto [ edge_handle,
             edge_ret ] = trans_graph.AddEdge(current_handle->id(),
                                              new_vertex_id,
                                              out_edge_it->label(),
                                              out_edge_it->id());
      assert(edge_ret);
    }
    for (auto in_edge_it = current_handle->InEdgeBegin();
             !in_edge_it.IsDone();
              in_edge_it++) {
      auto src_handle =  in_edge_it->src_handle();
      if ( added_edge_set[in_edge_it->id()] ) {
        // this vertex is the parent of the current vertex
        assert(added_vertex_set[src_handle->id()]);
        assert(trans_graph.FindEdge(in_edge_it->id()));
        continue;
      }
      added_edge_set[in_edge_it->id()] = true;
      VertexIdType new_vertex_id = src_handle->id();
      if (added_vertex_set[new_vertex_id]) {
        // the dst handle has been considered in the previous 
        // searching process, add a new vertex to make the
        // trans_graph a tree
        assert(trans_graph.FindVertex(new_vertex_id));
        // alloc a new vertex id
        new_vertex_id = max_vertex_id++;
      }
      else {
        added_vertex_set[src_handle->id()] = true;
        vertex_queue.emplace(src_handle);
      }
      // whether new_vertex_id == src_handle->id() or not,
      // src_handle->id() should have already existed in trans_graph
      assert(added_vertex_set[src_handle->id()]);
      auto [ vertex_it,
             vertex_ret ] = trans_graph.AddVertex(new_vertex_id, 
                                                  src_handle->label());
      // should have been added successfully
      assert(vertex_ret);
      auto [ edge_handle,
             edge_ret ] = trans_graph.AddEdge(new_vertex_id,
                                              current_handle->id(),
                                              in_edge_it->label(),
                                              in_edge_it->id());
      assert(edge_ret);
    }
  }
  assert(trans_graph.CountEdge()
            == graph.CountEdge());
  return trans_graph;
}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_TREE_TO_TREE_H