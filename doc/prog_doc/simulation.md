# Simulation

## Dual Simulation

### Declaration

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kDualSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t Simulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
     std::map<typename VertexHandle< QueryGraph>::type,
  std::vector<typename VertexHandle<TargetGraph>::type>>& match_set);
```

## Strong Simulation

### Declaration

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kStrongSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline std::vector<TargetGraph> StrongSimulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph);
```