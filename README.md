# **G**raph **U**nifined **N**on-overhead **D**istributed **A**daptive **M**odel

## Introduction

GUNDAM is a fundamental library for graph computing that provides a *unified*, *graph-level*, *static* abstraction without bring overhead.

#

## Getting start

All graph types currently supported are listed here:
* SmallGraph
* LargeGraph
* LargeGraph2
* Graph

Interface of all graph types see [here](/doc/接口统计_分类.pdf). Detailed illustration of those graph types see [here](/doc/prog_doc/graph_types.md)

#

## Initialization

### Load graph from CSV file

### Modify graph

##### Add Vertex

##### Erase Vertex

##### Add Edge

##### Erase Edge

#

## Access vertex in graph

GUNDAM provides a graph-level abstraction that only allows programers to access *Vertex* through the *Handle* and *Iterator*.

### Access vertex through iterator

Similar to the containers in STL, graph types in GUNDAM allow users to iterate over all vertexes in the graph:

```c++
GraphType graph;
/* ########################### *
 *    initialize graph here    *
 * ########################### */

for (auto vertex_it = graph.VertexBegin();
         !vertex_it.IsDone();
          vertex_it++) {
  std::cout << "accessing vertex: " << vertex_it->id() << std::endl;
  /* ########################################### *
   *     do something through vertex_it here     *
   * ########################################### */
}
```

For the graph types that support graph-level vertex-label-index, it is also allowed to iterate over all vertexes with the specified label:

```c++
GraphType graph;
/* ########################### *
 *    initialize graph here    *
 * ########################### */

typename GUNDAM::VertexLabel<GraphType>::type vertex_label = /* some vertex label */;
for (auto vertex_it = graph.VertexBegin(vertex_label);
         !vertex_it.IsDone();
          vertex_it++) {
  std::cout << "accessing vertex: " << vertex_it->id() 
            <<      " with label: " << vertex_it->label() << std::endl;
  /* ########################################### *
   *     do something through vertex_it here     *
   * ########################################### */
}
```

The above example works the same but is more efficient compared to the naive implementation without using index:

```c++
GraphType graph;
/* ########################### *
 *    initialize graph here    *
 * ########################### */

typename GUNDAM::VertexLabel<GraphType>::type vertex_label = /* some vertex label */;
for (auto vertex_it = graph.VertexBegin();
         !vertex_it.IsDone();
          vertex_it++) {
  if (vertex_it->label() != vertex_label) {
    continue;
  }
  std::cout << "accessing vertex: " << vertex_it->id() 
            <<      " with label: " << vertex_it->label() << std::endl;
  /* ########################################### *
   *     do something through vertex_it here     *
   * ########################################### */
}
```

It should be noticed that, similiar to the containers in STL, modifying the graph may invalidate the iterator:

```c++
GraphType graph;
/* ########################### *
 *    initialize graph here    *
 * ########################### */
typename GUNDAM::VertexID<GraphType>::type vertex_id_allocator = /* maximum vertex id in graph */;
for (auto vertex_it = graph.VertexBegin();
         !vertex_it.IsDone();
          vertex_it++) {
  // add new vertex while iterating
  graph.AddVertex(vertex_id_allocator++, vertex_it->label());
  /* ################################## *
   *    vertex_it is invalidate here    *
   * ################################## */
}
```

### Access vertex through handle

Expect the iterator, Each vertex has an unique ID in the data graph  which allows the user to directly access the vertex through ID as shown in the following example:

```c++
GraphType graph;
/* ########################### *
 *    initialize graph here    *
 * ########################### */

typename GUNDAM::VertexHandle<GraphType>::type vertex_handle = graph.FindVertex(0);
// to simplify the code, use:
// auto vertex_handle = graph.FindVertex(0);
if (!vertex_handle) {
  std::cout << "graph does not have vertex with id 0" << std::endl;
  return;
}
assert(vertex_handle->id() == 0);
```

```c++
// work but low efficiency 
GraphType0 graph_0;
/* ############################# *
 *    initialize graph_0 here    *
 * ############################# */

GraphType0 graph_1;
/* ############################# *
 *    initialize graph_1 here    *
 * ############################# */

using VertexIDType0 = typename GUNDAM::VertexID<GraphType0>::type;
using VertexIDType1 = typename GUNDAM::VertexID<GraphType1>::type;

std::map<VertexIDType0, std::vector<VertexIDType1>> vertex_candidate_set;

for (auto vertex_0_it = graph_0.VertexBegin();
         !vertex_0_it.IsDone();
          vertex_0_it++) {
  std::vector<VertexIDType1>& candidate_set
                     = vertex_candidate_set[vertex_0_it->id()];
  // to simplify the code, use:
  // auto& candidate_set = vertex_candidate_set[vertex_0_it->id()];
  for (vertex_1_it = graph_1.VertexBegin();
      !vertex_1_it.IsDone();
       vertex_1_it++) {
    if (vertex_0_it->label() 
     != vertex_1_it->label()) {
      continue;
    }
    candidate_set.emplace_back(vertex_1_it->id());
  }
}

for (const auto& [vertex_id, candidate_set] 
                    : vertex_candidate_set) {
  for () {
    
  }
}
```

The above example works but is not an efficient implementation since it requires access the .

```c++
GraphType graph;
/* ########################### *
 *    initialize graph here    *
 * ########################### */

using VertexIDType = typename GUNDAM::VertexID<GraphType>::type;
std::vector<VertexIDType> vertex_id_set_to_collect = {0, 1, 2, 3};

using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;
std::vector<VertexHandleType> vertex_handle_set_collected;

for (const auto& vertex_id : vertex_id_set_to_collect) {
  auto vertex_handle = graph.FindVertex(vertex_id);
  if (!vertex_handle) {
    continue;
  }
  vertex_handle_set_collected.emplace_back(vertex_handle);
}

```

#

## Access edge in graph

### Access edge through iterator

### Access edge through handle

## Match

Pattern matching is one of the the most esensial functions that support most of the useful methods and tools in this library. The class *Match* is the most important interface for pattern matching and is detailed explained in [here](/doc/prog_doc/match.md).

## MatchUsingMatch

[MatchUsingMatch](/doc/prog_doc/match_using_match.md)

## Algorithem

Library GUNDAM also contains.

Bfs

Dfs

## Tools

[tools](/doc/prog_doc/tools.md)

## Works with publication
![](/doc/images/all_works.svg)