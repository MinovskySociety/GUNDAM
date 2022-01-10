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

Description

Return whether there is a map for the input src_vertex.

### AddMap
```c++
bool AddMap(const typename VertexHandle<SrcGraphType>::type& src_handle,
            const typename VertexHandle<DstGraphType>::type& dst_handle);
```

Description

Add a vertex-vertex map into this match, return whether it is added successfully.

### EraseMap
```c++
bool EraseMap(const typename VertexID<SrcGraphType>::type& src_handle);
```

Description

Remove a vertex-vertex map from this match, return whether it is remove successfully.

### MapTo
```c++
bool MapTo(const typename VertexHandle<SrcGraphType>::type& src_handle) const;
bool MapTo(const typename     VertexID<SrcGraphType>::type& src_id) const;
```

Description

Return the dst_vertex that the input src_vertex maps to, return nullptr if there is no map for the input src_vertex.

### Reverse
```c++
Match<DstGraphType, SrcGraphType> Reverse() const;
```

Description

Reverse this match.

### hash
```c++
HashType hash() const;
```

### MapBegin
```c++
MapConstIterator MapBegin() const;
```

Description

To iterate over all maps contained in this match.

Example
```c++
GraphPatternType graph_pattern;
   DataGraphType    data_graph;

/* ############################################# *
 *    initialize graph_pattern and data_graph    *
 * ############################################# */

using MatchSetType = GUNDAM::MatchSet<GraphPatternType,
                                         DataGraphType>;

MatchSetType match_result;

GUNDAM::MatchUsingMatch(graph_pattern,
                         data_graph,
                        match_result);

for (auto match_it = match_result.MatchBegin();
         !match_it.IsDone();
          match_it++) {
  for (auto map_it = match_it->MapBegin();
           !map_it.IsDone();
            map_it++) {
    std::cout << "###############" << std::endl
              << "src_vertex id: " << map_it->src_handle()->id() << std::endl
              << "dst_vertex id: " << map_it->dst_handle()->id() << std::endl;
  }
}
```

See also [MatchUsingMatch](/doc/prog_doc/match_using_match.md#helper-method7) and [MatchSet](#MatchSet).

### size
```c++
size_t size() const;
```

### empty
```c++
bool empty() const;
```

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
                                                 SrcGraphType>& match) const;
```

# MatchSet