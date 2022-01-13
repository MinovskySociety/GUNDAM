# Vertex

## Types

### VertexHandle
```c++
using VertexHandleType = typename GUNDAM::VertexHandle<GraphType>::type
```
### VertexID
```c++
using VertexIDType = typename GUNDAM::VertexID<GraphType>::type
```
### VertexLabel
```c++
using VertexLabelType = typename GUNDAM::VertexLabel<GraphType>::type
```
### VertexAttributeHandle
```c++
using VertexAttributeHandleType = typename GUNDAM::VertexAttributeHandle<GraphType>::type
```

## Interface

### id
```c++
inline VertexIDType id() const;
```

### label
```c++
inline VertexLabelType label() const;
```

#

**Iterators**

### OutVertexBegin
```c++
auto OutVertexBegin();
auto OutVertexBegin() const;
auto OutVertexBegin(const EdgeLabelType& edge_label);
auto OutVertexBegin(const EdgeLabelType& edge_label) const;
```

### InVertexBegin
```c++
auto InVertexBegin();
auto InVertexBegin() const;
auto InVertexBegin(const EdgeLabelType& edge_label);
auto InVertexBegin(const EdgeLabelType& edge_label) const;
```

### OutEdgeBegin
```c++
// iterate over all output edges
auto OutEdgeBegin();
auto OutEdgeBegin() const;

// iterate over all output edges with specified edge label
auto OutEdgeBegin(const EdgeLabelType& edge_label);
auto OutEdgeBegin(const EdgeLabelType& edge_label) const;

// iterate over all output edges with specified edge label to a specified vertex
auto OutEdgeBegin(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle);
auto OutEdgeBegin(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle) const;

// iterate over all output edges to a specified vertex
auto OutEdgeBegin(const VertexHandleType& vertex_handle);
auto OutEdgeBegin(const VertexHandleType& vertex_handle) const;
```

### InEdgeBegin
```c++
// iterate over all output edges
auto InEdgeBegin();
auto InEdgeBegin() const;

// iterate over all output edges with specified edge label
auto InEdgeBegin(const EdgeLabelType& edge_label);
auto InEdgeBegin(const EdgeLabelType& edge_label) const;

// iterate over all output edges with specified edge label to a specified vertex
auto InEdgeBegin(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle);
auto InEdgeBegin(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle) const;

// iterate over all output edges to a specified vertex
auto InEdgeBegin(const VertexHandleType& vertex_handle);
auto InEdgeBegin(const VertexHandleType& vertex_handle) const;
```

# 

**Lookup**

### CountOutEdgeLabel
```c++
size_t CountOutEdgeLabel() const;
```

### CountInEdgeLabel
```c++
size_t CountInEdgeLabel() const;
```

### CountOutVertex
```c++
size_t CountOutVertex() const;
size_t CountOutVertex(const EdgeLabelType& edge_label) const;
```

### CountInVertex
```c++
size_t CountInVertex() const;
size_t CountInVertex(const EdgeLabelType& edge_label) const;
```

### CountOutEdge
```c++
size_t CountOutEdge() const;
size_t CountOutEdge(const EdgeLabelType& edge_label) const;
size_t CountOutEdge(const VertexHandleType& vertex_handle) const;
size_t CountOutEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle) const;
size_t CountOutEdge(const VertexHandleType& vertex_handle) const;
```

### CountInEdge
```c++
size_t CountInEdge() const;
size_t CountInEdge(const EdgeLabelType& edge_label) const;
size_t CountInEdge(const VertexHandleType& vertex_handle) const;
size_t CountInEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle) const;
size_t CountInEdge(const VertexHandleType& vertex_handle) const;
```

### FindOutVertex
```c++
VertexHandleType FindOutVertex(const VertexHandleType& dst_handle);
VertexHandleType FindOutVertex(const VertexIDType& dst_id);
VertexHandleType FindOutVertex(const EdgeLabelType& edge_label, const VertexHandleType& dst_handle);
VertexHandleType FindOutVertex(const EdgeLabelType& edge_label, const VertexIDType& dst_id);

VertexConstHandleType FindOutVertex(const VertexHandleType& dst_handle) const;
VertexConstHandleType FindOutVertex(const VertexIDType& dst_id) const;
VertexConstHandleType FindOutVertex(const EdgeLabelType& edge_label, const VertexHandleType& dst_handle) const;
VertexConstHandleType FindOutVertex(const EdgeLabelType& edge_label, const VertexIDType& dst_id) const;
```

### FindInVertex
```c++
VertexHandleType FindInVertex(const VertexHandleType& dst_handle);
VertexHandleType FindInVertex(const VertexIDType& dst_id);
VertexHandleType FindInVertex(const EdgeLabelType& edge_label, const VertexHandleType& dst_handle);
VertexHandleType FindInVertex(const EdgeLabelType& edge_label, const VertexIDType& dst_id);

VertexConstHandleType FindInVertex(const VertexHandleType& dst_handle) const;
VertexConstHandleType FindInVertex(const VertexIDType& dst_id) const;
VertexConstHandleType FindInVertex(const EdgeLabelType& edge_label, const VertexHandleType& dst_handle) const;
VertexConstHandleType FindInVertex(const EdgeLabelType& edge_label, const VertexIDType& dst_id) const;
```

### FindOutEdge
```c++
EdgeHandleType FindOutEdge(const EdgeIDType& edge_id);
EdgeHandleType FindOutEdge(const VertexHandleType& vertex_handle, const EdgeIDType& edge_id);
EdgeHandleType FindOutEdge(const EdgeLabelType& edge_label, const EdgeIDType& edge_id);
EdgeHandleType FindOutEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle, const EdgeIDType& edge_id);
EdgeHandleType FindOutEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle);

EdgeConstHandleType FindOutEdge(const EdgeIDType& edge_id) const;
EdgeConstHandleType FindOutEdge(const VertexHandleType& vertex_handle, const EdgeIDType& edge_id) const;
EdgeConstHandleType FindOutEdge(const EdgeLabelType& edge_label, const EdgeIDType& edge_id) const;
EdgeConstHandleType FindOutEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle, const EdgeIDType& edge_id) const;
EdgeConstHandleType FindOutEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle) const;
```

### FindInEdge
```c++
EdgeHandleType FindInEdge(const EdgeIDType& edge_id);
EdgeHandleType FindInEdge(const VertexHandleType& vertex_handle, const EdgeIDType& edge_id);
EdgeHandleType FindInEdge(const EdgeLabelType& edge_label, const EdgeIDType& edge_id);
EdgeHandleType FindInEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle, const EdgeIDType& edge_id);
EdgeHandleType FindInEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle);

EdgeConstHandleType FindInEdge(const EdgeIDType& edge_id) const;
EdgeConstHandleType FindInEdge(const VertexHandleType& vertex_handle, const EdgeIDType& edge_id) const;
EdgeConstHandleType FindInEdge(const EdgeLabelType& edge_label, const EdgeIDType& edge_id) const;
EdgeConstHandleType FindInEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle, const EdgeIDType& edge_id) const;
EdgeConstHandleType FindInEdge(const EdgeLabelType& edge_label, const VertexHandleType& vertex_handle) const;
```

# 

**Modifiers**

### AddEdge
```c++
std::pair<EdgeHandleType, bool> AddEdge(const VertexHandleType& dst_handle, const EdgeLabelType& edge_label, const EdgeIDType& edge_id);
```

### EraseEdge
```c++
bool EraseEdge(const EdgeIDType& edge_id);
EdgeIteratorType EraseEdge(const EdgeIteratorType& edge_iterator);
```

#

**Attribute**

### Attribute
Attribute methods see [here](/doc/prog_doc/attribute.md).