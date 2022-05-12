#ifndef _GUNDAM_TOOL_TOPOLOGICAL_TREE_TO_TREE_H
#define _GUNDAM_TOOL_TOPOLOGICAL_TREE_TO_TREE_H

namespace GUNDAM {

// convert one general graph to tree,
// rooted at input root
template <typename GraphType>
auto ToTree(GraphType& graph,
  const typename VertexHandle<GraphType>::type& root) {
  std::remove_cv_t<GraphType> trans_graph;
  using VertexHandleType = typename VertexHandle<GraphType>::type;
  std::queue<VertexHandleType> vertex_queue = {root};
  auto [ vertex_it,
         vertex_ret ] = trans_graph.AddVertex(root->id(), 
                                              root->label());
  assert(vertex_ret);
  while (!vertex_queue.empty()) {
    auto current_handle = vertex_queue.front();
    vertex_queue.pop();
    for () {
      
    }
  }
  return;
}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_TREE_TO_TREE_H