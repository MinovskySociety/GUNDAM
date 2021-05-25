#ifndef _GUNDAM_TYPE_GETTER_VERTEX_ID_H
#define _GUNDAM_TYPE_GETTER_VERTEX_ID_H

namespace GUNDAM{

template <typename GraphType>
class VertexID{
 public:
  using type = typename GraphType::VertexType::IDType;
};

} // namespace GUNDAM 

#endif // _GUNDAM_TYPE_GETTER_VERTEX_ID_H