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

Example

```c++
GrapeType0 graph0;
/* ############################ *
 *    initialize graph0 here    *
 * ############################ */

GrapeType1 graph1;
/* ############################ *
 *    initialize graph1 here    *
 * ############################ */

using MatchType = GUNDAM::Match<GrapeType0,
                                GrapeType1>;

match.AddMap(graph0.FindVertex(0), graph1.FindVertex(1));
match.AddMap(graph0.FindVertex(1), graph1.FindVertex(2));
match.AddMap(graph0.FindVertex(2), graph1.FindVertex(0));

std::vector<GUNDAM::VertexHandle<GrapeType0>::type> src_handles;

src_handles.emplace_back(graph0.FindVertex(0));
src_handles.emplace_back(graph0.FindVertex(1));
src_handles.emplace_back(graph0.FindVertex(2));
src_handles.emplace_back(graph0.FindVertex(3));
src_handles.emplace_back(graph0.FindVertex(4));

for (const auto& src_handle : src_handles) {
  if (!match.HasMap(src_handle)) {
    std::cout << "does not have match for src vertex: " 
              << src_handle->id() 
              << std::endl;
    continue;
  }
  std::cout << "has match for src vertex: " 
            << src_handle->id() 
            << std::endl;
}

for (const auto& src_handle : src_handles) {
  if (!match.HasMap(src_handle->id())) {
    std::cout << "does not have match for src vertex: " 
              << src_handle->id() 
              << std::endl;
    continue;
  }
  std::cout << "has match for src vertex: " 
            << src_handle->id() 
            << std::endl;
}
```

Output:
```
has match for src vertex: 0
has match for src vertex: 1
has match for src vertex: 2
does not have match for src vertex: 3
does not have match for src vertex: 4
has match for src vertex: 0
has match for src vertex: 1
has match for src vertex: 2
does not have match for src vertex: 3
does not have match for src vertex: 4
```

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

Example

```c++
GrapeType0 graph0;
/* ############################ *
 *    initialize graph0 here    *
 * ############################ */

GrapeType1 graph1;
/* ############################ *
 *    initialize graph1 here    *
 * ############################ */

using MatchType = GUNDAM::Match<GrapeType0,
                                GrapeType1>;

match.AddMap(graph0.FindVertex(0), graph1.FindVertex(1));
match.AddMap(graph0.FindVertex(1), graph1.FindVertex(2));
match.AddMap(graph0.FindVertex(2), graph1.FindVertex(0));

std::vector<GUNDAM::VertexHandle<GrapeType0>::type> src_handles;

src_handles.emplace_back(graph0.FindVertex(0));
src_handles.emplace_back(graph0.FindVertex(1));
src_handles.emplace_back(graph0.FindVertex(2));
src_handles.emplace_back(graph0.FindVertex(3));
src_handles.emplace_back(graph0.FindVertex(4));

for (const auto& src_handle : src_handles) {
  if (!match.MapTo(src_handle)) {
    std::cout << "does not have match for src vertex: " 
              << src_handle->id() 
              << std::endl;
    continue;
  }
  std::cout << "map src vertex: " << src_handle->id() 
            << " to vertex: " << match.MapTo(src_handle)->id()
            << std::endl;
}

for (const auto& src_handle : src_handles) {
  if (!match.MapTo(src_handle->id())) {
    std::cout << "does not have match for src vertex: " 
              << src_handle->id() 
              << std::endl;
    continue;
  }
  std::cout << "map src vertex: " << src_handle->id() 
            << " to vertex: " << match.MapTo(src_handle->id())->id()
            << std::endl;
}
```

Output:
```
map src vertex: 0 to vertex: 1
map src vertex: 1 to vertex: 2
map src vertex: 2 to vertex: 0
does not have match for src vertex: 3
does not have match for src vertex: 4
map src vertex: 0 to vertex: 1
map src vertex: 1 to vertex: 2
map src vertex: 2 to vertex: 0
does not have match for src vertex: 3
does not have match for src vertex: 4
```

### Reverse
```c++
Match<DstGraphType, SrcGraphType> Reverse() const;
```

Description

Reverse this match.

Example:
```c++
GrapeType0 graph0;
/* ############################ *
 *    initialize graph0 here    *
 * ############################ */

GrapeType1 graph1;
/* ############################ *
 *    initialize graph1 here    *
 * ############################ */

using Match0To1Type = GUNDAM::Match<GrapeType0,
                                    GrapeType1>;

Match0To1Type match0to1;

match0to1.AddMap(graph0.FindVertex(0), graph1.FindVertex(1));
match0to1.AddMap(graph0.FindVertex(1), graph1.FindVertex(2));
match0to1.AddMap(graph0.FindVertex(2), graph1.FindVertex(0));

std::cout << "Map before reverse: " << std::endl;
for (auto map_it = match0to1.MapBegin();
         !map_it.IsDone();
          map_it++) {
  std::cout << " src_id: " << map_it->src_handle()->id()
            << " dst_id: " << map_it->dst_handle()->id()
            << std::endl;
}

using Match1To0Type = GUNDAM::Match<GrapeType1,
                                    GrapeType0>;

Match1To0Type match1to0 = match0to1.Reverse();

std::cout << "Map after reverse: " << std::endl;
for (auto map_it = match1to0.MapBegin();
         !map_it.IsDone();
          map_it++) {
  std::cout << " src_id: " << map_it->src_handle()->id()
            << " dst_id: " << map_it->dst_handle()->id()
            << std::endl;
}
```

Output:
```
Map before reverse: 
 src_id: 0 dst_id: 1
 src_id: 1 dst_id: 2
 src_id: 2 dst_id: 0
Map after reverse: 
 src_id: 0 dst_id: 2
 src_id: 1 dst_id: 0
 src_id: 2 dst_id: 1
```

See also [MapBegin](#mapbegin)

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

Description

Return how many vertex-vertex pairs are contained in this match.

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

Example

```c++
GrapeType0 graph0;
/* ############################ *
 *    initialize graph0 here    *
 * ############################ */

GrapeType1 graph1;
/* ############################ *
 *    initialize graph1 here    *
 * ############################ */

GrapeType2 graph2;
/* ############################ *
 *    initialize graph2 here    *
 * ############################ */

using Match0To1Type = GUNDAM::Match<GrapeType0,
                                    GrapeType1>;

using Match1To2Type = GUNDAM::Match<GrapeType1,
                                    GrapeType2>;

using Match0To2Type = GUNDAM::Match<GrapeType0,
                                    GrapeType2>;

Match0To1Type match0to1;

match0to1.AddMap(graph0.FindVertex(0), graph1.FindVertex(1));
match0to1.AddMap(graph0.FindVertex(1), graph1.FindVertex(2));
match0to1.AddMap(graph0.FindVertex(2), graph1.FindVertex(0));

match1to2.AddMap(graph1.FindVertex(0), graph2.FindVertex(1));
match1to2.AddMap(graph1.FindVertex(1), graph2.FindVertex(2));
match1to2.AddMap(graph1.FindVertex(2), graph2.FindVertex(0));

match0to2 = match1to2(match0to1);

for (auto map0to2_it = match0to2.MapBegin();
         !map0to2_it.IsDone();
          map0to2_it++) {
  std::cout << "map0to2_it maps vertex: " << map0to2_it->src_handle()->id()
                        << " to vertex: " << map0to2_it->dst_handle()->id()
                        << std::endl;
}
```

Output

```
map0to2_it maps vertex: 0 to vertex 2
map0to2_it maps vertex: 1 to vertex 0
map0to2_it maps vertex: 2 to vertex 1
```

# MatchSet

```c++
template <typename SrcGraphType,
          typename DstGraphType>
class MatchSet;
```

Description

A set of match from src_graph to dst_graph.

See also [MatchUsingMatch](/doc/prog_doc/match_using_match.md#helper-method7).
