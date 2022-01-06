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

The following example shows how to use this method:

```c++
GraphPatternType graph_pattern;
   DataGraphType    data_graph;

/* ############################################# *
 *    initialize graph_pattern and data_graph    *
 * ############################################# */

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

/* ############################## *
 *    initialize candidate set    *
 * ############################## */
if (!_dp_iso_using_match::InitCandidateSet<MatchSemantics::kIsomorphism>(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // initialize failed, there is no legal matching 
  return 0;
}

/* ########################################################## *
 *    refine candidate set, equivalent to dual simulation     *
 * ########################################################## */
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

This method also also the user to import constant graph query_graph/target_graph.
Take a [helper method](#helper-method5) as a simple example here:

```c++
QueryGraph  query;
/* ###################### *
 *    initialize query    *
 * ###################### */

TargetGraph target;
/* ####################### *
 *    initialize target    *
 * ####################### */

auto&  query_ref =  query; // reference
auto& target_ref = target; // reference

const auto&  query_const_ref =  query; // constant reference
const auto& target_const_ref = target; // constant reference

// the following methods works the same:
size_t count0 = GUNDAM::MatchUsingMatch(query, target);
size_t count1 = GUNDAM::MatchUsingMatch(query, target_ref);
size_t count2 = GUNDAM::MatchUsingMatch(query, target_const_ref);
assert(count0 == count1);
assert(count0 == count2);

size_t count3 = GUNDAM::MatchUsingMatch(query_ref, target);
size_t count4 = GUNDAM::MatchUsingMatch(query_ref, target_ref);
size_t count5 = GUNDAM::MatchUsingMatch(query_ref, target_const_ref);
assert(count0 == count3);
assert(count0 == count4);
assert(count0 == count5);

size_t count6 = GUNDAM::MatchUsingMatch(query_const_ref, target);
size_t count7 = GUNDAM::MatchUsingMatch(query_const_ref, target_ref);
size_t count8 = GUNDAM::MatchUsingMatch(query_const_ref, target_const_ref);
assert(count0 == count6);
assert(count0 == count7);
assert(count0 == count8);
```

With the same idea, it can be more complicated for the [method](#helper-method7) with more parameters:

```c++
QueryGraph  query;
/* ###################### *
 *    initialize query    *
 * ###################### */

TargetGraph target;
/* ####################### *
 *    initialize target    *
 * ####################### */

auto&  query_ref =  query; // reference
auto& target_ref = target; // reference

const auto&  query_const_ref =  query; // constant reference
const auto& target_const_ref = target; // constant reference

// the following methods works the same:
GUNDAM::MatchSet<QueryGraph, TargetGraph> match_result0;
size_t count0 = GUNDAM::MatchUsingMatch(query, target, match_result0);
assert(count0 == match_result0.size());

GUNDAM::MatchSet<QueryGraph, TargetGraph> match_result1;
size_t count1 = GUNDAM::MatchUsingMatch(query, target_ref, match_result1);
assert(count1 == match_result1.size());

GUNDAM::MatchSet<QueryGraph, const TargetGraph> match_result2;
size_t count2 = GUNDAM::MatchUsingMatch(query, target_const_ref, match_result2);
assert(count2 == match_result2.size());

GUNDAM::MatchSet<QueryGraph, TargetGraph> match_result3;
size_t count3 = GUNDAM::MatchUsingMatch(query_ref, target, match_result3);
assert(count3 == match_result3.size());

GUNDAM::MatchSet<QueryGraph, TargetGraph> match_result4;
size_t count4 = GUNDAM::MatchUsingMatch(query_ref, target_ref, match_result4);
assert(count4 == match_result4.size());

GUNDAM::MatchSet<QueryGraph, const TargetGraph> match_result5;
size_t count5 = GUNDAM::MatchUsingMatch(query_ref, target_const_ref, match_result5);
assert(count5 == match_result5.size());

GUNDAM::MatchSet<const QueryGraph, TargetGraph> match_result6;
size_t count6 = GUNDAM::MatchUsingMatch(query_const_ref, target, match_result6);
assert(count6 == match_result6.size());

GUNDAM::MatchSet<const QueryGraph, TargetGraph> match_result7;
size_t count7 = GUNDAM::MatchUsingMatch(query_const_ref, target_ref, match_result7);
assert(count7 == match_result7.size());

GUNDAM::MatchSet<const QueryGraph, const TargetGraph> match_result8;
size_t count8 = GUNDAM::MatchUsingMatch(query_const_ref, target_const_ref, match_result8);
assert(count8 == match_result8.size());
```

More examples see [here](/test/test_match_using_match.cc).

## Helper methods

Several helper methods are provided to simplify the usage of MatchUsingMatch.

### Helper method0

#### Parameter list
* partial_match
* candidate_set
* max_match (optional, < 0 as no limit) 
* time_limit (optional, < 0 as no limit) 

#### Declaration
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

#### Description

Match begins at parital match in the imported candidate set, can limit match count and match time.

#### Example

```c++
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

/* ############################## *
 *    initialize candidate set    *
 * ############################## */
if (!_dp_iso_using_match::InitCandidateSet<MatchSemantics::kIsomorphism>(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // initialize failed, there is no legal matching 
  return 0;
}

/* ########################################################## *
 *    refine candidate set, equivalent to dual simulation     *
 * ########################################################## */
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

### Helper method1

#### Parameter list
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

#### Description

Match begins in the imported candidate set, support prune callback and match callback.

#### Example

```c++
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

/* ############################## *
 *    initialize candidate set    *
 * ############################## */
if (!_dp_iso_using_match::InitCandidateSet<MatchSemantics::kIsomorphism>(
              graph_pattern,
                 data_graph,
              candidate_set)) {
  // initialize failed, there is no legal matching 
  return 0;
}

/* ########################################################## *
 *    refine candidate set, equivalent to dual simulation     *
 * ########################################################## */
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

auto ret = GUNDAM::MatchUsingMatch(graph_pattern,
                                      data_graph,
                                   candidate_set, 
                          prune_nothing_callback, 
                          pattern_match_callback,
                                      time_limit);
assert(ret >= 0 && ret <= kMatchLimit);
```

### Helper method2

#### Parameter list
* partial_match
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
          enum MergeNecConfig merge_target_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
              QueryGraph&  query_graph,
             TargetGraph& target_graph,
  const Match<QueryGraph, 
             TargetGraph>& partial_match,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0);
```

#### Description

Match begins at the input partial_match, would check whether the input partial is legal. Support prune callback and match callback.

#### Example

```c++
GUNDAM::MatchUsingMatch(query_graph,
                       target_graph,
                      partial_match,
                     prune_callback,
                     match_callback,
                         time_limit);
```

### Helper method3

#### Parameter list
* partial_match
* time_limit (optional, < 0 as no limit)

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          enum MergeNecConfig merge_target_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const Match<QueryGraph, TargetGraph>& partial_match,
   int64_t max_match = -1,
   double time_limit = -1.0);
```

#### Description

Match begins at the input partial_match, would check whether the input partial is legal.

### Helper method4

#### Parameter list
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
          enum MergeNecConfig merge_target_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& prune_callback,
  const std::function<bool(const Match<QueryGraph, TargetGraph>&)>& match_callback,
   double time_limit = -1.0);
```

#### Description

Match support prune callback and match callback.

### Helper method5

#### Parameter list
* max_match  (optional, < 0 as no limit)
* time_limit (optional, < 0 as no limit)

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          enum MergeNecConfig merge_target_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
   int64_t max_match = -1,
   double time_limit = -1.0);
```

#### Description

Match returns only match count.

### Helper method6

#### Parameter list
* partial_match
* match_result
* max_match  (optional, < 0 as no limit)
* time_limit (optional, < 0 as no limit)

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          enum MergeNecConfig merge_target_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
  const Match<QueryGraph, TargetGraph>& partial_match,
     MatchSet<QueryGraph, TargetGraph>& match_result,
   int64_t max_match = -1,
   double time_limit = -1.0);
```

#### Description

Match begins at input partial_match and store all match result into match_result. Would check whether the input partial_match is legal.

### Helper method7

#### Parameter list
* match_result
* max_match  (optional, < 0 as no limit)
* time_limit (optional, < 0 as no limit)

```c++
template <enum MatchSemantics match_semantics 
             = MatchSemantics::kIsomorphism,
          enum MatchAlgorithm match_algorithm
             = MatchAlgorithm::kDagDp,
          enum MergeNecConfig merge_query_nec_config 
             = MergeNecConfig::kNotMerge,
          enum MergeNecConfig merge_target_nec_config 
             = MergeNecConfig::kNotMerge,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t MatchUsingMatch(
   QueryGraph&  query_graph, 
  TargetGraph& target_graph,
     MatchSet<QueryGraph, TargetGraph>& match_result,
   int64_t max_match = -1,
   double time_limit = -1.0);
```

#### Description

Match that stores all match result into match_result.

