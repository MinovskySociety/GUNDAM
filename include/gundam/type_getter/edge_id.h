#ifndef _GUNDAM_TYPE_GETTER_EDGE_ID_H
#define _GUNDAM_TYPE_GETTER_EDGE_ID_H

namespace GUNDAM{

template <typename GraphType>
class EdgeID{
 public:
  using type = typename GraphType::EdgeType::IDType;
};

} // namespace GUNDAM 

#endif // _GUNDAM_TYPE_GETTER_EDGE_ID_H