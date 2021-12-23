# Tools

## ConnectedComponent

[head file](/include/gundam/tool/connected_component.h):

```
include/gundam/tool/connected_component.h
```

Declaration:

```c++
template <bool bidirectional = true,
          typename GraphType>
GraphType ConnectedComponent(GraphType& graph,
       typename VertexHandle<GraphType>::type vertex_handle);
```

Description:

Find a connected component in graph connected 

Example:

```c++
GraphType graph;
/***************************
 *  initialize graph here  *
 ***************************/
typename VertexId    <GraphType>::type src_id = 0;
typename VertexHandle<GraphType>::type src_handle = graph.FindVertex(src_id);

// should not be null
assert(src_handle);

// find connected component connected to src vertex
GraphType       connected_component(ConnectedComponent<false>(graph, src_handle));
GraphType bidir_connected_component(ConnectedComponent< true>(graph, src_handle));

assert(Connected<false>(      connected_component));
assert(Connected< true>(bidir_connected_component));

assert(SubGraphOf(connected_component, 
            bidir_connected_component));

Match<GraphType, GraphType> same_id_match(connected_component,
                                    bidir_connected_component, "same_id_map");
assert(SubGraphOf(connected_component, 
            bidir_connected_component,
                  same_id_match));

```

More examples:
* [test_connected_component, does not existed now](/test/test_connected_component.cc)

See also :
* [SubGraphOf](#SubGraphOf)
* [Connected](#Connected)
* [Match](/doc/prog_doc/match.md)

#

## Connected

[head file](/include/gundam/tool/connected.h):

```
include/gundam/tool/connected.h
```

Declaration:

```c++
template <bool bidirectional = true,
          typename GraphType>
inline bool Connected(const GraphType& graph);
```

Description:

Return whether the input graph is connected.

Example:

```c++
GraphType graph;
/***************************
 *  initialize graph here  *
 ***************************/
bool       graph_is_connected = Connected<false>(graph);
bool bidir_graph_is_connected = Connected< true>(graph);

// if bidir_graph_is_connected is true, then graph_is_connected should also be true
assert(!bidir_graph_is_connected || graph_is_connected);
```

More examples:
* [test_connected](/test/test_connected.cc)

See also:
* [ConnectedComponent](#ConnectedComponent)

## copy_to

## diameter

## edge_id_continued

## has_edge

## is_link

## isolate_vertex

## k_hop

## map_edge_to

## max_id

## maximal_common_subgraph

## merge_by_id

## preserve

## radius

## random_walk

## same_pattern

## SubGraphOf

## unique_patterns

## vertex_id_continued