#ifndef _GUNDAM_TYPE_GETTER_VERTEX_LABEL_H
#define _GUNDAM_TYPE_GETTER_VERTEX_LABEL_H

namespace GUNDAM{

template <typename GraphType>
class VertexLabel{
 public:
  using type = typename GraphType::VertexType::LabelType;
};

} // namespace GUNDAM 

#endif // _GUNDAM_TYPE_GETTER_VERTEX_LABEL_H