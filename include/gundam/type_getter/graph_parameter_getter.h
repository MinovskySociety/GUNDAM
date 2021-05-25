#ifndef _GUNDAM_TYPE_GETTER_GRAPH_PARAMETER_GETTER
#define _GUNDAM_TYPE_GETTER_GRAPH_PARAMETER_GETTER

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