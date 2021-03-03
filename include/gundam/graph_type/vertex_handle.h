#ifndef _TYPE_H
#define _TYPE_H

namespace GUNDAM {

template <typename GraphType>
class VertexHandle;

// non-constant graph type
template <typename GraphType>
class VertexHandle{
 public:
  using type = typename GraphType::VertexPtr;
};

// non-constant graph type
// remove reference
template <typename GraphType>
class VertexHandle<GraphType&>{
 public:
  using type = typename VertexHandle<GraphType>::type;
};

// constant graph type
template <typename GraphType>
class VertexHandle<const GraphType>{
 public:
  using type = typename GraphType::VertexConstPtr;
};

// constant graph type
// remove reference
template <typename GraphType>
class VertexHandle<const GraphType&>{
 public:
  using type = typename VertexHandle<const GraphType>::type;
};

}; // namespace GUNDAM

#endif // _TYPE_H