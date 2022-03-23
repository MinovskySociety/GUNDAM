# **G**raph **U**nifined **N**on-overhead **D**istributed **A**daptive **M**odel

## Introduction

GUNDAM is a fundamental library for graph computing that provides a *unified*, *graph-level*, *static* abstraction without bring overhead. 

[Vision of project GUNDAM](/doc/user_doc/vision_of_project_gundam.pdf)

[开发指导](/doc/user_doc/ProjectGUNDAM开发指导.pdf)

#

## Getting start

All graph types currently supported are listed here:
* SmallGraph
* LargeGraph
* LargeGraph2
* Graph

Interface of all graph types see [here](/doc/接口统计_分类.pdf). Detailed illustration of those graph types see [here](/doc/prog_doc/graph_types.md)

#

## Graph construction

### Manual construction

The following examples show how to construct a graph with [SmallGraph](/include/gundam/graph_type/small_graph.h), [other graph types](/doc/prog_doc/graph_types.md) work the same:

```c++
/********************************
 * demonstration of the graph   *
 * to construct:                *
 *    3 <- 1 <- 2               *
 ********************************/

using GraphType = GUNDAM::SmallGraph<int, // VertexIDType
                                     int, // VertexLabelType
                                     int, //   EdgeIDType
                                     int>;//   EdgeLabelType

GraphType g0;
// add vertexes
g0.AddVertex(1, 0);
g0.AddVertex(2, 1);
g0.AddVertex(3, 0);

// add edges
g0.AddEdge(1, 3, 42, 1);
g0.AddEdge(2, 1, 42, 2);

std::cout << "g0.CountVertex(): " << g0.CountVertex() << std::endl
          << "g0.CountEdge():   " << g0.CountEdge()   << std::endl;
```

The output would be:
```
g0.CountVertex(): 3
g0.CountEdge():   2
```

A more complicated example with bi-directional edges:

```c++
/******************************
 * demonstration of the graph *
 * to construct:              *
 *    1 -> 2 -> 4             *
 *    |  / |  / |             *
 *    V /  V /  V             *
 *    3 -> 5 -> 7             *
 *    |  / |  / |             *
 *    V /  V /  V             *
 *    6 -> 8 -> 9             *
 ******************************/

using GraphType = GUNDAM::SmallGraph<int, // VertexIDType
                                     int, // VertexLabelType
                                     int, //   EdgeIDType
                                     int>;//   EdgeLabelType

GraphType g1;

// add vertexes
g1.AddVertex(1, 0);
g1.AddVertex(2, 1);
g1.AddVertex(3, 1);
g1.AddVertex(4, 2);
g1.AddVertex(5, 2);
g1.AddVertex(6, 2);
g1.AddVertex(7, 3);
g1.AddVertex(8, 3);
g1.AddVertex(9, 4);

// add edges
g1.AddEdge(1, 2, 42, 1);
g1.AddEdge(1, 3, 42, 2);
g1.AddEdge(2, 3, 42, 3);
g1.AddEdge(3, 2, 42, 4);
g1.AddEdge(2, 4, 42, 5);
g1.AddEdge(2, 5, 42, 6);
g1.AddEdge(3, 5, 42, 7);
g1.AddEdge(3, 6, 42, 8);
g1.AddEdge(4, 5, 42, 9);
g1.AddEdge(5, 4, 42, 10);
g1.AddEdge(5, 6, 42, 11);
g1.AddEdge(6, 5, 42, 12);
g1.AddEdge(4, 7, 42, 13);
g1.AddEdge(5, 7, 42, 14);
g1.AddEdge(5, 8, 42, 15);
g1.AddEdge(6, 8, 42, 16);
g1.AddEdge(7, 8, 42, 17);
g1.AddEdge(8, 7, 42, 18);
g1.AddEdge(7, 9, 42, 19);
g1.AddEdge(8, 9, 42, 20);

std::cout << "g1.CountVertex(): " << g1.CountVertex() << std::endl
          << "g1.CountEdge():   " << g1.CountEdge()   << std::endl;
```

The output would be:
```
g1.CountVertex(): 9
g1.CountEdge():   20
```

The detailed explanation of the relative methods, e.g. *AddVertex*, *AddEdge*, see [here](/doc/prog_doc/graph.md).

### Load graph from CSV files

Except the manual construction shown above, users are also allowed to directly load the data graph from the CSV files. The following example shows how a graph is stored in CSV files and how to load it.

All vertexes in the graph is stored in example_v.csv as:

```sh
$ cat ./example_v.csv
vertex_id:int,label_id:int
1,0
2,1
3,0
```

And all edges are stored in example_e.csv as shown below:

```sh
$ cat ./example_e.csv
edge_id:int,source_id:int,target_id:int,label_id:int
1,1,3,42
2,2,1,42
```

The following code show how to load the above two csv files to construct the data graph:

```c++
using GraphType = GUNDAM::SmallGraph<int, int, int, int>;

GraphType g0;
GUNDAM::ReadCSVGraph(g0, "./example_v.csv",
                         "./example_e.csv");
```

The *g0* load from the csv files in this example is exactly the same as the *g0* constructed manually [above](#graph-construction).

The detailed illustration of CSV format see [here](/doc/user_doc/csv_format.md), and the detailed explanation of IO methods see [here](/doc/prog_doc/csv_graph.md).

#

## Access vertex in graph

GUNDAM provides a graph-level abstraction that only allows users to access *Vertex* through *Handle* and *Iterator*. 
The graph-level methods to get the *Handle* and *Iterator* see [here](/doc/prog_doc/graph.md), the detailed vertex-level interface see [here](/doc/prog_doc/vertex.md).

### Access vertex through id

Each vertex has an unique ID in the data graph which allows the user to directly access the vertex through. Take the [*g0*](#graph-construction) constructed above as an example:

```c++
std::cout << "label of vertex 1: " << g0.FindVertex(1)->label() << std::endl;
std::cout << "label of vertex 2: " << g0.FindVertex(2)->label() << std::endl;
std::cout << "label of vertex 3: " << g0.FindVertex(3)->label() << std::endl;
```

Output:
```
label of vertex 1: 0
label of vertex 2: 1
label of vertex 3: 0
```

Such a method is correct but has limited efficiency and is suggested to be used *as least as possible*. For higher efficiency, the vertexes should be [accessed through handle](#access-vertex-through-handle) carefully.

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
  std::cout << "accessing vertex id: " << vertex_it->id()
                         << " label: " << vertex_it->label() << std::endl;
  /* ########################################### *
   *     do something through vertex_it here     *
   * ########################################### */
}
```

Take the graph [*g0*](#graph-construction) constructed above as an example, we have:

```c++
for (auto vertex_it = g0.VertexBegin();
         !vertex_it.IsDone();
          vertex_it++) {
  std::cout << "accessing vertex id: " << vertex_it->id()
                         << " label: " << vertex_it->label() << std::endl;
}
```

Output:
```
accessing vertex id: 1 label: 0
accessing vertex id: 2 label: 1
accessing vertex id: 3 label: 0
```

It should be notice that the above access order of vertexes might variety between different [graph types](/doc/prog_doc/graph_types.md), different graph objects with the same type and even the same graph object after modification.

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
  std::cout << "accessing vertex id: " << vertex_it->id()
                         << " label: " << vertex_it->label() << std::endl;
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
  std::cout << "accessing vertex id: " << vertex_it->id()
                         << " label: " << vertex_it->label() << std::endl;
  /* ########################################### *
   *     do something through vertex_it here     *
   * ########################################### */
}
```

Since *SmallGraph* does not have graph-level vertex-label-index (see [here](/doc/prog_doc/graph_types.md)), the *LargeGraph* is utilized for the example here:

```c++
using GraphType = GUNDAM::LargeGraph<int, int, int,
                                     int, int, int>;
GraphType g0;
GUNDAM::ReadCSVGraph(g0, "./example_v.csv",
                         "./example_e.csv");
for (auto vertex_it = g0.VertexBegin(0);
         !vertex_it.IsDone();
          vertex_it++) {
  std::cout << "accessing vertex id: " << vertex_it->id()
                         << " label: " << vertex_it->label() << std::endl;
}
```

Output:
```
accessing vertex id: 1 label: 0
accessing vertex id: 3 label: 0
```

Similarly, the access order of the vertexes is also not stable.

Also similiar to the containers in STL, modifying the graph may lead to the invalidation of the iterator:

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

Except through id and iterator, vertexes are also allowed to be access through *Handle*, which is a recommanded high-efficient manner although needs to be used carefully.

Take the [*g0*](#graph-construction) constructed above as an example, there are:

```c++
using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;
VertexHandleType vertex_0_handle = g0.FindVertex(0);
// to simplify the code, use:
// auto vertex_handle = graph.FindVertex(0);
if (!vertex_0_handle) {
  std::cout << "g0 does not have vertex with id 0" << std::endl;
}
else {
  std::cout << "vertex 0 has label: " << vertex_0_handle->label() << std::endl;
}

VertexHandleType vertex_1_handle = g0.FindVertex(1);
// to simplify the code, use:
// auto vertex_handle = graph.FindVertex(0);
if (!vertex_1_handle) {
  std::cout << "g0 does not have vertex with id 1" << std::endl;
}
else {
  std::cout << "vertex 1 has label: " << vertex_1_handle->label() << std::endl;
}
```

Output:
```
g0 does not have vertex with id 0
vertex 1 has label: 0
```

*Handle* can also convert from *Iterator* as:

```c++
using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;

for (auto vertex_it = g0.VertexBegin();
         !vertex_it.IsDone();
          vertex_it++) {
  VertexHandleType vertex_handle = vertex_it;
  std::cout << "accessing vertex id: " << vertex_handle->id()
                         << " label: " << vertex_handle->label() << std::endl;
}
```

Output:

```
accessing vertex id: 1 label: 0
accessing vertex id: 2 label: 1
accessing vertex id: 3 label: 0
```

The access order is not stable.

The *Hanldes* for the same vertex in the same graph object are equal to each others no matter how they are obtained:

```c++
using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type;

VertexHandleType vertex_1_handle = g0.FindVertex(1);

const auto& g0_const_ref = g0;

VertexHandleType vertex_1_const_handle = g0_const_ref.FindVertex(1);

std::cout << "vertex_1_handle == vertex_1_const_handle: "
          <<  vertex_1_handle == vertex_1_const_handle << std::endl;

for (auto vertex_it = g0.VertexBegin();
         !vertex_it.IsDone();
          vertex_it++) {
  if (vertex_it->id() != 1) {
    continue;
  }
  std::cout << "vertex_1_handle == vertex_it: "
            <<  vertex_1_handle == vertex_it << std::endl;
  break;
}

for (auto vertex_cit = g0_const_ref.VertexBegin();
         !vertex_cit.IsDone();
          vertex_cit++) {
  if (vertex_cit->id() != 1) {
    continue;
  }
  std::cout << "vertex_1_handle == vertex_cit: "
            <<  vertex_1_handle == vertex_cit << std::endl;
  break;
}
```

Output:

```
vertex_1_handle == vertex_1_const_handle: 1
vertex_1_handle == vertex_it: 1
vertex_1_handle == vertex_cit: 1
```

It should be noticed that comparing two vertex handles from different graph object would bring undefined behaviour:

```c++
GUNDAM::SmallGraph<int, int, int, int> g0_copy(g0);

std::cout << "g0_copy.FindVertex(1) == g0.FindVertex(1): "
          <<  g0_copy.FindVertex(1) == g0.FindVertex(1) << std::endl;
```

The behaviour is undefined and the output is unpredictable.

To access the attribute on vertex, see 
[the document for attribute](/doc/prog_doc/attribute.md).


<!-- ```c++
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

``` -->

#

## Access edge in graph

Similar to the vertex, edges in graph are also only allowed to be access through Handle/Iterator. 
The interface of edge is much more simpler than vertexes can is detailed illustrated [here](/doc/prog_doc/edge.md).

## Match

Pattern matching is one of the the most esensial functions that support most of the useful methods and tools in this library. The class *Match* is the most important interface for pattern matching and is detailed explained in [here](/doc/prog_doc/match.md).

## MatchUsingMatch

[MatchUsingMatch](/doc/prog_doc/match_using_match.md)

[Simulation](/doc/prog_doc/simulation.md)

## Algorithem

Library GUNDAM also contains.

Bfs

Dfs

## Serialize

Graph types are allowed to be serialized as string as follow:

```c++
GraphType0 g0;
/* ############################# *
 *    initialize g0 here    *
 * ############################# */

std::string str;
str << g0;

std::cout << str << std::endl;

GraphType1 g1;
str >> g1;
```

The serialized graph can be easily transformed in distributed environment or stored in file. 

Detailed explanation see [here](/doc/prog_doc/serialize.md).

## Tools

[tools](/doc/prog_doc/tools.md)

## Works with publication
![](/doc/images/all_works.svg)
