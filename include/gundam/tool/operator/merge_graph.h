#ifndef _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H
#define _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H

namespace GUNDAM {

/* ##################################################### *
 * ##  merge the input set of graph, join the set of  ## *
 * ##  vertex with given id                           ## *
 * ##################################################### */
template <typename GraphType>
inline std::remove_cv_t<
       std::remove_reference_t<GraphType>> 
       MergeGraphSet(const std::vector<GraphType>& input_graph_set,
                     const std::vector<
                     typename VertexID<GraphType>::type>& join_vertex_id_set
                   = const std::vector<
                     typename VertexID<GraphType>::type>() ) {

  using ReturnGraphType = std::remove_cv_t<
                   std::remove_reference_t<GraphType>>;

  #ifndef NDEBUG
  for (const auto& join_vertex_id : join_vertex_id_set) {
    for (const auto& input_graph : input_graph_set) {
      // should be contained in all input graphs
      assert(input_graph.FindVertex(join_vertex_id));
    }
  }
  #endif // NDEBUG
  
  ReturnGraphType merged_graph;

  /* ######################### *
   * ##  do something here  ## *
   * ######################### */
 
  #ifndef NDEBUG
  for (const auto& join_vertex_id : join_vertex_id_set) {
    // should also be contained in the merged graph
    assert(merged_graph.FindVertex(join_vertex_id));
  }
  #endif // NDEBUG
  return merged_graph;
};

#endif // _GUNDAM_TOOL_OPERATOR_MERGE_GRAPH_SET_H