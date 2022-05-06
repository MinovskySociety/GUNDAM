#ifndef _GUNDAM_ALGORITHM_CONNECTED_COMPONENT_H
#define _GUNDAM_ALGORITHM_CONNECTED_COMPONENT_H

#include "gundam/tool/max_id.h"
#include "gundam/type_getter/vertex_id.h"
#include "gundam/type_getter/vertex_handle.h"

#include <stack>
#include <vector>
#include <unordered_map>

namespace GUNDAM {

namespace _connected_component {

template <typename GraphType,
          typename IndexType>
void _StrongConnectedComponent(GraphType& graph,
   const typename VertexHandle<GraphType>::type& vertex_handle,
                               IndexType& index,
            std::unordered_map<
             typename VertexID<GraphType>::type, 
                    std::tuple<IndexType, // index
                               IndexType, // lowlink
                               bool>      // onStack
                                                   >& vertex_state_map,
 std::stack <typename VertexHandle<GraphType>::type>& vertex_stack,
 std::vector<
 std::vector<typename VertexHandle<GraphType>::type>>& strong_connected_component_vertex_set) {

  constexpr uint8_t kIndexIdx   = 0,
                    kLowLinkIdx = 1,
                    kOnStackIdx = 2;

  using VertexHandleSetType = std::vector<typename VertexHandle<GraphType>::type>;

  // function strongconnect(v)
  //     // Set the depth index for v to the smallest unused index
  //     v.index := index
  //     v.lowlink := index
  //     index := index + 1
  //     S.push(v)
  //     v.onStack := true
  
  auto [ vertex_state_it,
         vertex_state_ret ] 
       = vertex_state_map.emplace(vertex_handle->id(),
                                  std::tuple(index,  // index
                                             index,  // lowlink
                                             true)); // onStack
  assert(vertex_state_ret);
  index++;
  vertex_stack.emplace(vertex_handle);

  //     // Consider successors of v
  //     for each (v, w) in E do
  //         if w.index is undefined then
  //             // Successor w has not yet been visited; recurse on it
  //             strongconnect(w)
  //             v.lowlink := min(v.lowlink, w.lowlink)
  //         else if w.onStack then
  //             // Successor w is in stack S and hence in the current SCC
  //             // If w is not on stack, then (v, w) is an edge pointing to an SCC already found and must be ignored
  //             // Note: The next line may look odd - but is correct.
  //             // It says w.index not w.lowlink; that is deliberate and from the original paper
  //             v.lowlink := min(v.lowlink, w.index)
  //         end if
  //     end for

  for (auto out_edge_it = vertex_handle->OutEdgeBegin();
           !out_edge_it.IsDone();
            out_edge_it++) {
    // vertex_state_map
    auto successor_handle = out_edge_it->dst_handle();
    auto successor_it =  vertex_state_map.find(successor_handle->id());
    if ( successor_it == vertex_state_map. end()) {
      _StrongConnectedComponent(graph, successor_handle, index, 
                                vertex_state_map, 
                                vertex_stack,
     strong_connected_component_vertex_set);
      assert(vertex_state_map.find(   vertex_handle->id()) != vertex_state_map.end());
      assert(vertex_state_map.find(successor_handle->id()) != vertex_state_map.end());
      std::get<kLowLinkIdx>(vertex_state_map[vertex_handle->id()]) 
            = std::min(std::get<kLowLinkIdx>(vertex_state_map[   vertex_handle->id()]),
                       std::get<kLowLinkIdx>(vertex_state_map[successor_handle->id()]));
      continue;
    }
    if (!std::get<kOnStackIdx>(vertex_state_map[successor_handle->id()])) {
      continue;
    }
    // w.onStack
    // vertex_state_it could fail now
    assert(vertex_state_map.find(   vertex_handle->id()) != vertex_state_map.end());
    assert(vertex_state_map.find(successor_handle->id()) != vertex_state_map.end());
    std::get<kLowLinkIdx>(vertex_state_map[vertex_handle->id()]) 
          = std::min(std::get<kLowLinkIdx>(vertex_state_map[   vertex_handle->id()]),
                     std::get<kLowLinkIdx>(vertex_state_map[successor_handle->id()]));
  }

  //     // If v is a root node, pop the stack and generate an SCC
  //     if v.lowlink = v.index then
  //         start a new strongly connected component
  //         repeat
  //             w := S.pop()
  //             w.onStack := false
  //             add w to current strongly connected component
  //         while w ≠ v
  //         output the current strongly connected component
  //     end if
  // end function

  // vertex_state_it could fail now
  assert(vertex_state_map.find(vertex_handle->id()) 
      != vertex_state_map.end());
  if (std::get<  kIndexIdx>(vertex_state_map[vertex_handle->id()])
   != std::get<kLowLinkIdx>(vertex_state_map[vertex_handle->id()])) {
    return;
  }

  VertexHandleSetType vertex_handle_set;
  while (true) {
    auto in_stack_handle = vertex_stack.top();
    vertex_stack.pop();
    std::get<kOnStackIdx>(vertex_state_map[in_stack_handle->id()]) = false;
    vertex_handle_set.emplace_back(in_stack_handle);
    if ( in_stack_handle == vertex_handle ) {
      break;
    }
  }
  std::sort(vertex_handle_set.begin(),
            vertex_handle_set.end());
  strong_connected_component_vertex_set.emplace_back(std::move(vertex_handle_set));
  return;
}

}; // _connected_component

/* ############################ *
 * ##    tarjan algorithm    ## *
 * ############################ */
// input: graph G = (V, E)
// output: set of strongly connected components (sets of vertices)
template <typename GraphType>
std::vector<
std::vector<typename VertexHandle<GraphType>::type>> // OK to return vector after C11
         StrongConnectedComponent(GraphType& graph) {

  using VertexIDType     = typename VertexID    <GraphType>::type;
  using VertexHandleType = typename VertexHandle<GraphType>::type;

  VertexIDType kMaxVertexId = MaxVertexId(graph);

  using IndexType = VertexIDType;

  using VertexStateType = std::tuple<IndexType, // index
                                     IndexType, // lowlink
                                     bool>;     // onStack

  std::unordered_map<VertexIDType, 
                  VertexStateType> vertex_state_map;

  std::stack<VertexHandleType> vertex_stack;

  std::vector<
  std::vector<VertexHandleType>> strong_connected_component_vertex_set;

  IndexType index = 0;

  // index := 0
  // S := empty stack
  // for each v in V do
  //     if v.index is undefined then
  //         strongconnect(v)
  //     end if
  // end for
  for (auto vertex_it = graph.VertexBegin();
           !vertex_it.IsDone();
            vertex_it++) {
    if (vertex_state_map.find(vertex_it->id())
     != vertex_state_map. end()) {
      // this vertex has been considered
      continue;
    }
    // this vertex has not been considered
    _connected_component::_StrongConnectedComponent(graph, vertex_it, index, 
                                                           vertex_state_map, 
                                                           vertex_stack,
                                strong_connected_component_vertex_set);
  }
  return strong_connected_component_vertex_set;
}

};

#endif // _GUNDAM_ALGORITHM_CONNECTED_COMPONENT_H