#ifndef _GRAPH_ITEM_H
#define _GRAPH_ITEM_H

#include <type_traits>

//#include "gundam/graph.h"

namespace GUNDAM {
enum class ItemType { Vertex, VertexAttr };

template <class GraphType>
class GraphItem {
 private:
  static constexpr bool kIsConst_ = std::is_const<GraphType>::value;
  using VertexPtrType =
      typename std::conditional<kIsConst_, typename GraphType ::VertexConstPtr,
                                typename GraphType ::VertexPtr>::type;
  using VertexConstPtrType = typename GraphType::VertexConstPtr;
  using VertexAttributeKeyType =
      typename GraphType::VertexType::AttributeKeyType;

  enum ItemType type_;

  VertexPtrType vertex_ptr_;
  VertexAttributeKeyType vertex_attribute_key_;

 public:
  /// vertex
  GraphItem(const VertexPtrType& vertex_ptr)
      : type_(ItemType::Vertex),
        vertex_ptr_(vertex_ptr),
        vertex_attribute_key_() {
    return;
  }

  /// vertex attribute
  GraphItem(const VertexPtrType& vertex_ptr,
            const VertexAttributeKeyType& attribute_key)
      : type_(ItemType::VertexAttr),
        vertex_ptr_(vertex_ptr),
        vertex_attribute_key_(attribute_key) {
    return;
  }

  inline bool operator<(const GraphItem& graph_item) const {
    return this->vertex_ptr_ < graph_item.vertex_ptr_;
  }

  inline enum ItemType type() const { return this->type_; }

  inline VertexPtrType vertex_ptr() {
    assert(this->type_ == ItemType::Vertex ||
           this->type_ == ItemType::VertexAttr);
    return this->vertex_ptr_;
  }

  inline VertexConstPtrType vertex_const_ptr() const {
    assert(this->type_ == ItemType::Vertex ||
           this->type_ == ItemType::VertexAttr);
    return this->vertex_ptr_;
  }

  inline const VertexAttributeKeyType& vertex_attribute_key() const {
    assert(this->type_ == ItemType::VertexAttr);
    return this->vertex_attribute_key_;
  }
};

}  // namespace GUNDAM

#endif  // _GRAPH_ITEM_H
