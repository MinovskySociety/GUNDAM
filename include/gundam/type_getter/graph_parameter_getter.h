#ifndef _GUNDAM_TYPE_GETTER_GRAPH_PARAMETER_GETTER
#define _GUNDAM_TYPE_GETTER_GRAPH_PARAMETER_GETTER

#include "gundam/graph_type/graph.h"
#include "gundam/graph_type/large_graph.h"
#include "gundam/graph_type/large_graph2.h"
#include "gundam/graph_type/small_graph.h"
#include "gundam/graph_type/simple_small_graph.h"

namespace GUNDAM{

template <typename GraphType>
class GraphParameter;

template <typename GraphType>
class GraphParameter : public GraphType::_GraphParameter{
};

// remove constant 
template <typename GraphType>
class GraphParameter<const GraphType> : public GraphParameter<GraphType>{
};

// remove reference
template <typename GraphType>
class GraphParameter<GraphType&> : public GraphParameter<GraphType>{
};

// remove constant reference
template <typename GraphType>
class GraphParameter<const GraphType&> : public GraphParameter<GraphType>{
};

};

#endif // _GUNDAM_TYPE_GETTER_GRAPH_PARAMETER_GETTER