# GraphTypes

## SmallGraph

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

## LargeGraph

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

## LargeGraph2

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

## Graph

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