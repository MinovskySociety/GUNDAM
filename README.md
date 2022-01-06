# **G**raph **U**nifined **N**on-overhead **D**istributed **A**daptive **M**odel

## Introduction

GUNDAM is a fundamental library for graph computing that provides a unified graph-level abstraction without bring overhead.

## Getting start

All graph types currently supported are listed here:
* SmallGraph
* LargeGraph
* LargeGraph2
* Graph

Interface of all graph types see [here](/doc/接口统计_分类.pdf). Detailed illustration of those graph types see [here](###GraphTypes)

GUNDAM provides a graph-level abstraction that only allows programers to access *Vertex* & *Edge* through the *Handle* & *Iterator*. The following example shows how to access vertex in graph through id:

```c++
GraphType graph;
typename GUNDAM::VertexHandle<GraphType>::type vertex_handle = graph.FindVertex(0);
// to simplify the code, use:
// auto vertex_handle = graph.FindVertex(0);
if (!vertex_handle) {
  std::cout << "graph does not have vertex with id 0" << std::endl;
  return;
}
assert(vertex_handle->id() == 0);
```

To store the 

```c++
// work but low efficiency 
GraphType0 graph_0;
GraphType0 graph_1;

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

### GraphTypes

#

#### SmallGraph

* Does not support index.
* Does not support attribute on vertex.
* Does not support attribute on edge.

[head file](/include/gundam/graph_type/small_graph.h): 
```
/include/gundam/graph_type/small_graph.h
```
#
```c++
template <class VertexIDType, class VertexLabelType, 
          class   EdgeIDType, class   EdgeLabelType>
class SmallGraph;
```
#
Description

Light-weight graph type for small graph to save memory, does not support index. Neither support attribute on vertex nor edge.

#

#### LargeGraph

* Support index.
* Support attribute on vertex.
* Support attribute on edge.

[head file](/include/gundam/graph_type/large_graph.h): 
```
/include/gundam/graph_type/large_graph.h
```
#
```c++
template <class VertexIDType, class VertexLabelType, class VertexAttributeKeyType, 
          class   EdgeIDType, class   EdgeLabelType, class   EdgeAttributeKeyType>
class LargeGraph;
```

#
Description

Heavy-weight graph type for large graph for efficiency. Support index and also support attribute on both vertex and edge.

Use map/set (red-black tree) as low level container, fast to load graph no matter how the vertex/edge are sorted in CSV file but can cause performance problems since it is not cache-friendly.

#

#### LargeGraph2

* Support index.
* Support attribute on vertex.
* Support attribute on edge.

[head file](/include/gundam/graph_type/large_graph2.h): 
```
/include/gundam/graph_type/large_graph2.h
```
#
```c++
template <class VertexIDType, class VertexLabelType, class VertexAttributeKeyType, 
          class   EdgeIDType, class   EdgeLabelType, class   EdgeAttributeKeyType>
class LargeGraph2;
```

#
Description

Heavy-weight graph type for large graph for efficiency. Support index and also support attribute on both vertex and edge.

The only difference compare to LargeGraph is this data type use vector as low level container, which makes it more cache-friendly and brings higher performance. However, the time complexity for loading graph can become O(N2) when the vertex/edge are not stored properly in the CSV file.

#

#### Graph

* Support index.
* Support attribute on vertex, separated or grouped.
* Support attribute on edge, separated or grouped.

[head file](/include/gundam/graph_type/graph.h): 
```
/include/gundam/graph_type/graph.h
```
#
```c++
template <typename... configures>
class Graph;
```

#

### Vertex
#### VertexHandle
```c++
using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type
```
#### VertexID
```c++
using VertexIDType = typename GUNDAM::VertexID<GraphType>::type
```
#### VertexLabel
```c++
using VertexLabelType = typename GUNDAM::VertexLabel<GraphType>::type
```
#### VertexAttributeHandle
```c++
using VertexAttributeHandleType = typename GUNDAM::VertexAttributeHandle<GraphType>::type
```

#

### Edge
#### EdgeHandle 
```c++
using EdgeHandleType = typename GUNDAM::EdgeHandle<GraphType>::type
```
#### EdgeID
```c++
using EdgeIDType = typename GUNDAM::EdgeID<GraphType>::type
```
#### EdgeLabel
```c++
using EdgeLabelType = typename GUNDAM::EdgeLabel<GraphType>::type
```
#### EdgeAttributeHandle
```c++
using EdgeAttributeHandleType = typename GUNDAM::EdgeAttributeHandle<GraphType>::type
```

## Interface
Graph

## Match

[Match](/doc/prog_doc/match.md)

## MatchUsingMatch

[MatchUsingMatch](/doc/prog_doc/match_using_match.md)

## Algorithem

VF2

Dpiso

Bfs

Dfs

## Tools

[tools](/doc/prog_doc/tools.md)

## Works with publication
![](/doc/images/all_works.svg)