# Callback method

This document shows how to use the callback method in MatchUsingMatch method.

## Getting start 

Callback method can be function pointer, [function object](https://en.cppreference.com/w/cpp/utility/functional) or [lambda expression](https://en.cppreference.com/w/cpp/language/lambda).

That would be called in the function while running.

## Frequent used callback methods

### collect all match result

```c++
using MatchType = GUNDAM::Match<QueryGraph, 
                               TargetGraph>;
using MatchSetType = GUNDAM::MatchSet<QueryGraph, 
                                     TargetGraph>;
MatchSetType match_result;
std::function<bool(const Match<QueryGraph, TargetGraph>&)> 
  match_callback = [&match_result](const MatchType &match) -> bool {
  // store match into match_result
  match_result.AddMatch(match); 
  // continue matching
  return true; 
};
```

### count the match number

```c++
using MatchType = GUNDAM::Match<QueryGraph, 
                               TargetGraph>;
uint64_t match_counter = 0;
std::function<bool(const Match<QueryGraph, TargetGraph>&)> 
  match_callback = [&match_counter](const MatchType &match) -> bool {
  match_counter++; 
  // continue matching
  return true; 
};
```

### match until reach a specified number

```c++
using MatchType = GUNDAM::Match<QueryGraph, 
                               TargetGraph>;
uint64_t match_counter = 0;
const uint64_t kMatchLimit = 100000;
std::function<bool(const Match<QueryGraph, TargetGraph>&)> 
  match_callback = [&match_counter,
                    &kMatchLimit](const MatchType &match) -> bool {
  match_counter++; 
  if (match_counter >= kMatchLimit) {
    return false; // end matching
  }
  // continue matching
  return true; 
};
```

### collect the set of target vertexes that a specified query vertex maps to

```c++
using MatchType = GUNDAM::Match<QueryGraph, 
                               TargetGraph>;

using  QueryVertexHandleType = typename GUNDAM::VertexHandle< QueryGraph>::type;
using TargetVertexHandleType = typename GUNDAM::VertexHandle<TargetGraph>::type;
                    
QueryVertexHandleType query_vertex_handle 
                    = query_graph.FindVertex(query_vertex_id);

assert(query_vertex_handle);

std::set<TargetVertexHandleType> target_vertex_set;

std::function<bool(const Match<QueryGraph, TargetGraph>&)> 
  match_callback = [&query_vertex_handle,
                   &target_vertex_set](const MatchType &match) -> bool {
  const TargetVertexHandleType target_handle = match.MapTo(query_vertex_handle);
  assert(target_handle);
  target_vertex_set.emplace(target_handle);
  return true; 
};
```

