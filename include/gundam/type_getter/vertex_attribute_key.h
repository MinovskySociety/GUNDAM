#ifndef _GUNDAM_TYPE_GETTER_VERTEX_ATTRIBUTE_KEY_H
#define _GUNDAM_TYPE_GETTER_VERTEX_ATTRIBUTE_KEY_H

namespace GUNDAM {

template <typename GraphType>
class VertexAttributeKey;

// non-constant graph type
template <typename GraphType>
class VertexAttributeKey{
 public:
  using type = typename GraphType::VertexAttributePtr;
};

// constant graph type
template <typename GraphType>
class VertexAttributeKey<const GraphType>{
 public:
  using type = typename GraphType::VertexAttributeConstPtr;
};

// remove reference
template <typename GraphType>
class VertexAttributeKey<GraphType&>{
 public:
  using type = typename VertexAttributeKey<GraphType>::type;
};

// template <typename GraphType>
// class VertexConstKey{
//  public:
//   using type = typename GraphType::VertexConstPtr;
// };

// // remove reference
// template <typename GraphType>
// class VertexConstKey<GraphType&>{
//  public:
//   using type = typename VertexConstKey<GraphType>::type;
// };

}; // namespace GUNDAM

#endif // _GUNDAM_TYPE_GETTER_VERTEX_ATTRIBUTE_KEY_H