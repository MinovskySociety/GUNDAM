# Graph

## Interface

The interface of all graph types are illustrated in [here](/doc/接口统计_分类.pdf). The same method of different graph types have the same semantics and shares this document.

The vertex level interface see [here](/doc/prog_doc/vertex.md) and the edge level interface see [here](/doc/prog_doc/edge.md).


# 

**Iterators**

### VertexBegin
```c++
auto VertexBegin();
auto VertexBegin() const;

auto VertexBegin(const VertexLabelType& vertex_label);
auto VertexBegin(const VertexLabelType& vertex_label) const;
```

### EdgeBegin()
```c++
auto EdgeBegin();
auto EdgeBegin() const;
```

**Lookup**

### CountVertex()
```c++
auto CountVertex() const;
auto CountVertex(const VertexLabelType& vertex_label) const;
```

### CountEdge()
```c++
auto CountEdge() const;
```

### FindVertex
```c++
VertexHandleType FindVertex(const VertexIDType& vertex_id);
VertexConstHandleType FindVertex(const VertexIDType& vertex_id) const;

VertexHandleType FindVertex(const VertexIDType& vertex_id, const VertexLabelType& vertex_label);
VertexConstHandleType FindVertex(const VertexIDType& vertex_id, const VertexLabelType& vertex_label) const;
```

### FindEdge
```c++
EdgeHandleType FindEdge(const EdgeIDType& edge_id);
```

# 

***Modifiers***

### AddVertex
```c++
std::pair<VertexHandleType, bool> AddVertex(const VertexIDType& vertex_id, 
                                         const VertexLabelType& vertex_label);
```


### AddEdge
```c++
std::pair<EdgeHandleType, bool> AddEdge(const VertexIDType& src_id, 
                                        const VertexIDType& dst_id, 
                                        const EdgeLabelType& edge_label, 
                                        const EdgeIDType& edge_id);
```

### Clear
```c++
void Clear();
```

### EraseVertex
```c++
bool EraseVertex(const VertexIDType& vertex_id);
bool EraseVertex(const VertexHandleType& vertex_handle);
VertexIterator EraseVertex(VertexIteratorType vertex_iterator);
```

### EraseEdge
```c++
bool EraseEdge(const EdgeIDType& edge_id);
```