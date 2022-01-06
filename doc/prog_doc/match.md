# Match

## Declaration

```c++
template <typename SrcGraphType,
          typename DstGraphType>
class Match;
```

[head file](/include/gundam/match/match.h):

```
/include/gundam/match/match.h
```

## Interface

### HasMap
```c++
bool HasMap(const typename VertexHandle<SrcGraphType>::type& src_handle) const;
bool HasMap(const typename     VertexID<SrcGraphType>::type& src_id) const;
```

### AddMap(src_handle, dst_handle)
```c++
bool AddMap(const typename VertexHandle<SrcGraphType>::type& src_handle,
            const typename VertexHandle<DstGraphType>::type& dst_handle);
```

### EraseMap(src_handle)
```c++
bool EraseMap(const typename VertexID<SrcGraphType>::type& src_handle);
```

### MapTo(src_handle)
```c++
bool MapTo(const typename VertexHandle<SrcGraphType>::type& src_handle) const;
bool MapTo(const typename     VertexID<SrcGraphType>::type& src_id) const;
```

### Reverse()
```c++
Match<DstGraphType, SrcGraphType> Reverse() const;
```

### hash()

### hash(pivot_set)

### MapBegin
```c++
MapConstIterator MapBegin() const;
```

### size()

### empty()

#

### operator<(match)
```c++
bool operator<(const Match& match) const;
```

### operator==(match)
```c++
bool operator==(const Match& match) const;
```

### operator()
```c++
Match<SrcSrcGraphType,
         DstGraphType> operator()(const Match<SrcSrcGraphType,
                                                 SrcGraphType>& match0) const;
```