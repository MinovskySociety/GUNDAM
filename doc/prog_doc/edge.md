# Edge

## Types

### EdgeHandle 
```c++
using EdgeHandleType = typename GUNDAM::EdgeHandle<GraphType>::type
```
### EdgeID
```c++
using EdgeIDType = typename GUNDAM::EdgeID<GraphType>::type
```
### EdgeLabel
```c++
using EdgeLabelType = typename GUNDAM::EdgeLabel<GraphType>::type
```
### EdgeAttributeHandle
```c++
using EdgeAttributeHandleType = typename GUNDAM::EdgeAttributeHandle<GraphType>::type
```

## Interface

### id
```c++
EdgeIDType id() const;
```

### label
```c++
EdgeIDType label() const;
```

### src_id
```c++
VertexIDType src_id() const;
```

### dst_id
```c++
VertexIDType dst_id() const;
```

### src_handle
```c++
VertexHandleType src_handle() const;
```

### dst_handle
```c++
VertexHandleType dst_handle() const;
```

**Attribute**

### Attribute
Attribute methods see [here](/doc/prog_doc/attribute.md).
