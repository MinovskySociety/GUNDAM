#ifndef _GUNDAM_ALGORITHM_SIMULATION_H
#define _GUNDAM_ALGORITHM_SIMULATION_H

#include "gundam/type_getter/vertex_handle.h"

#include "gundam/algorithm/dp_iso_using_match.h"
#include "gundam/algorithm/match_semantics.h"

#include "gundam/tool/connected_component.h"
#include "gundam/tool/diameter.h"
#include "gundam/tool/operator/preserve.h"

#include "gundam/match/match.h"

#include <unordered_map>
#include <unordered_set>
#include <functional>

namespace GUNDAM {

template <enum MatchSemantics match_semantics 
             = MatchSemantics::kDualSimulation,
          typename  QueryGraph,
          typename TargetGraph>
inline size_t Bisimulation(
        QueryGraph&  query_graph,
       TargetGraph& target_graph,
     std::map<typename VertexHandle< QueryGraph>::type,
  std::vector<typename VertexHandle<TargetGraph>::type>>& match_set) {


}; // GUNDAM

#endif // _GUNDAM_ALGORITHM_SIMULATION_H
