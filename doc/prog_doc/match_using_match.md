# MatchUsingMatch

## Declaration:

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const Match<QueryGraph,
             TargetGraph>& partial_match,
  const std::map<typename VertexHandle< QueryGraph>::type,
     std::vector<typename VertexHandle<TargetGraph>::type>>& candidate_set,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0);
```

## Description

Graph pattern matching, return the number of matches it has found for query_graph in target_graph. Can specify:

* match semantics
   * isomorphism or homorphism
* which algorithm to use
   * support [VF2](/doc/algo_doc/VF2.pdf) and [DagDp](/doc/algo_doc/daf.pdf) now
* whether to merge neighborhood equivalence class (NEC) in query graph
   * the definition of NEC see [TurboIso](/doc/algo_doc/turbo_iso.pdf)

Input parameters:

* query_graph
   > Data type:
   > ```c++
   > QueryGraph
   > ```
   >
   > Usage:
   >
   > The graph pattern to match.

* target_graph
   > Data type:
   > ```c++
   > TargetGraph
   > ```
   >
   > Usage:
   >
   > Data graph for graph pattern to match.

* partial_match,
   > Data type:
   > ```c++
   > GUNDAM::Match<QueryGraphType, 
   >              TargetGraphType>
   > ```
   > Usage:
   >
   > Allow the users to specify a parital match from a subset of vertexes in query graph to target graph, this method would complete the import partial match.
   > * The algorithm would first check whether the input partial match is legal, the matching process would be ended if not.
   > * Input an empty match if no partial match has been specified.
 
* candidate_set
   > Data type:
   > ```c++
   > std::  map <typename GUNDAM::VertexHandle< QueryGraph>::type,
   > std::vector<typename GUNDAM::VertexHandle<TargetGraph>::type>>
   > ```
   > Usage:
   >
   > To specify the candidate vertexes in data graph for each vertex in query graph.
   > * In Debug mode, the algorithm would check whether all vertex_handle contained in candidate_set are the handle of vertex in graph pattern.

* prune_callback
   > Data type:
   > ```c++
   > std::function<bool(const Match<QueryGraph, TargetGraph>&)>
   > ```
   > Usage:
   >
   > Prune callback for each match & parital-match during the matching process to control whether continue matching based on the input parital match. If: 
   >  * return true, then mean that all match contains the current parital matches are not needed, would stop the following match
   >  * return false, then do nothing and continue matching

* match_callback,
   > Data type:
   > ```c++
   > std::function<bool(const Match<QueryGraph, TargetGraph>&)>
   > ```
   > Usage:
   >
   > Match callback for each match, to decide whether continue the matching process. If:
   >  * return false, then end the matching process
   >  * return true, then do nothing and continue matching

* time_limit
   > Data type:
   > ```c++
   > double
   > ```
   > Usage:
   >
   > Time limit for the pattern matching, set to < 0 as no time limit.
   > * unit: second.

## Example

```c++

GraphPatternType graph_pattern;
   DataGraphType    data_graph;

/* #############################################
 *    initialize graph_pattern and data_graph
 * #############################################*/

using MatchType = GUNDAM::Match<GraphPatternType, 
                                   DataGraphType>;

MatchType partial_match;

bool add_map_ret = partial_match.AddMap(
                   graph_pattern.FindVertex(src_vertex_id),
                      data_graph.FindVertex(dst_vertex_id));
assert(add_map_ret);

using  QueryVertexHandle = typename GUNDAM::VertexHandle< QueryGraph>::type;
using TargetVertexHandle = typename GUNDAM::VertexHandle<TargetGraph>::type;

using CandidateSetType = std::map<QueryVertexHandle, 
                     std::vector<TargetVertexHandle>>;

CandidateSetType candidate_set;
if (!_dp_iso_using_match::InitCandidateSet<MatchSemantics::kIsomorphism>(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // initialize failed, there is no legal matching 
  return 0;
}
if (!_dp_iso_using_match::RefineCandidateSet(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // refine failed, there is no legal matching 
  return 0;
}

double time_limit = 1000.0; // time limit set to 1000.0s

std::function<bool(const GUNDAM::Match<GraphPatternType, DataGraphType>&)> 
prune_nothing_callback
= [](const MatchType& partial_match) {
   // prune nothing, continue the matching
   return false;
};

std::function<bool(const GUNDAM::Match<GraphPatternType, DataGraphType>&)> 
prune_nothing_callback
= [](const MatchType& partial_match) {
   // prune nothing, continue the matching
   return false;
};

const size_t kMatchLimit = 100; // consider only 100 matches
size_t match_counter = 0; // count how many matches have been considered so far

std::function<bool(const GUNDAM::Match<GraphPatternType, DataGraphType>&)> 
pattern_match_callback
= [&kMatchLimit,
   &match_counter](const MatchType& partial_match) {
   match_counter++;
   if (match_counter >= kMatchLimit) {
      // has reached the match limit, stop matching
      return false;
   }
   // continue matching
   return true;
};

auto ret = GUNDAM::MatchUsingMatch<
           GUNDAM::MatchSemantics::kIsomorphism, // semantics is graph isomorphism
           GUNDAM::MatchAlgorithm::kDagDp, // using dag-dp algorithm
           GUNDAM::MergeNecConfig::kMerge>(// merge NEC in data graph for acceleration
                  pattern, // query graph set as pattern
                  data_graph, // target graph set as data_graph
                  partial_match, // the partial match to begin with
                  candidate_set, 
                  prune_nothing_callback,
                  pattern_match_callback, 
                  time_limit);

assert(ret == match_counter);
```

## Helper method0

### Parameter list
* partial_match
* candidate_set
* max_match (optional, < 0 as no limit) 
* time_limit (optional, < 0 as no limit) 

### Declaration
```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const Match<QueryGraph,
             TargetGraph>& partial_match,
  const std::map<typename VertexHandle< QueryGraph>::type,
     std::vector<typename VertexHandle<TargetGraph>::type>>& candidate_set,
   int64_t max_match = -1,
   double time_limit = -1.0);
```

### Description

Match begins at parital match and 

### Example

```c++

const size_t kMaxMatch = 1000;

GraphPatternType graph_pattern;
   DataGraphType    data_graph;

/* #############################################
 *    initialize graph_pattern and data_graph
 * #############################################*/

using  QueryVertexHandle = typename GUNDAM::VertexHandle< QueryGraph>::type;
using TargetVertexHandle = typename GUNDAM::VertexHandle<TargetGraph>::type;

using CandidateSetType = std::map<QueryVertexHandle, 
                     std::vector<TargetVertexHandle>>;

CandidateSetType candidate_set;
if (!_dp_iso_using_match::InitCandidateSet<MatchSemantics::kIsomorphism>(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // initialize failed, there is no legal matching 
  return 0;
}
if (!_dp_iso_using_match::RefineCandidateSet(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // refine failed, there is no legal matching 
  return 0;
}

const size_t kMatchLimit = 100; // consider only 100 matches

double time_limit = 1000.0; // time limit set to 1000.0s

using MatchType = GUNDAM::Match<GraphPatternType, 
                                   DataGraphType>;

// empty parital match, no partial_match specified
MatchType partial_match;

auto ret = GUNDAM::MatchUsingMatch(graph_pattern,
                                      data_graph,
                                   partial_match, 
                                   candidate_set, 
                                     kMatchLimit,
                                      time_limit);
assert(ret >= 0 && ret <= kMatchLimit);
```

## Helper method1

### Parameter list
* candidate_set
* prune_callback
* match_callback
* time_limit (optional, < 0 as no limit)

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  const std::map<typename VertexHandle< QueryGraph>::type, 
     std::vector<typename VertexHandle<TargetGraph>::type>>& candidate_set,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0);
```

### Description



### Example

```c++

```