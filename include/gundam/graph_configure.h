#ifndef _GRAPH_CONFIGURE_H
#define _GRAPH_CONFIGURE_H

#include "label.h"
#include "container.h"
#include "attribute.h"

#include <cstdint>
#include <string>

enum class StoreData:bool{
      OutEdge,
    InOutEdge
};

namespace GUNDAM {
template <typename... configures>
class GraphConfigures;

/// declaration of configurable parameters
/// store data type
template <enum StoreData store_data_type>
class SetStoreDataType;
/// node configurations
template <typename LabelType>
class SetVertexLabelType;
template <typename IDType>
class SetVertexIDType;
template <bool HasAttribute>
class SetVertexHasAttribute;
template <typename AttributeType>
class SetVertexStaticAttributeType;
template <bool AttributeIsConst>
class SetVertexAttributeIsConst;
template <bool AttributeIsDynamical>
class SetVertexAttributeIsDynamical;
template <enum ContainerType containerType>
class SetVertexAttributeContainerType;
/// edge configurations
template <typename LabelType>
class SetEdgeLabelType;
template <typename IDType>
class SetEdgeIDType;
template <bool HasAttribute>
class SetEdgeHasAttribute;
template <typename AttributeType>
class SetEdgeStaticAttributeType;
template <bool AttributeIsConst>
class SetEdgeAttributeIsConst;
template <bool AttributeIsDynamical>
class SetEdgeAttributeIsDynamical;
template <enum ContainerType containerType>
class SetEdgeAttributeContainerType;
/// container type configurations
template <enum ContainerType containerType>
class SetEdgeLabelContainerType;
template <enum ContainerType containerType>
class SetVertexesContainerType;
template <enum ContainerType containerType>
class SetEdgeContainerType;
/// container sort type configurations
template <enum SortType sortType>
class SetEdgeLabelContainerSortType;
template <enum SortType sortType>
class SetVertexesContainerSortType;
template <enum SortType sortType>
class SetEdgeContainerSortType;
/// miscellaneous
template <bool AllowMultipleEdge>
class SetAllowMultipleEdge;
template <bool AllowDuplicateEdge>
class SetAllowDuplicateEdge;
template <bool AllowDuplicateNumType>
class SetDuplicateNumType;
// template<typename... AuxiliaryDataTypes>
// class AddAuxiliaryData;

/// default setting of configurations
template <>
class GraphConfigures<> {
 private:
  using DefaultIDType = uint32_t;
  using DefaultKeyType = uint32_t;
  using DefaultLabelType = Label<uint32_t>;
  using DefaultAttributeType = std::string;

  static constexpr enum ContainerType DefaultContainerType =
      ContainerType::Vector;

  static constexpr enum SortType DefaultSortType = SortType::None;

 protected:
  /// mark whether specified store data type
  static constexpr bool specified_store_data_type = false;
  /// mark whether specified node configurations
  static constexpr bool specified_vertex_id_type = false;
  static constexpr bool specified_vertex_label_type = false;
  static constexpr bool specified_vertex_label_is_const = false;
  static constexpr bool specified_vertex_has_static_attribute = false;
  static constexpr bool specified_vertex_attribute_type = false;
  static constexpr bool specified_vertex_attribute_is_const = false;
  static constexpr bool specified_vertex_has_dynamic_attribute = false;
  static constexpr bool specified_vertex_attribute_key_type = false;
  static constexpr bool specified_vertex_attribute_container_type = false;
  static constexpr bool specified_vertex_attribute_container_sort_type = false;
  /// mark whether specified edge configurations
  static constexpr bool specified_edge_id_type = false;
  static constexpr bool specified_edge_label_type = false;
  static constexpr bool specified_edge_label_is_const = false;
  static constexpr bool specified_edge_has_static_attribute = false;
  static constexpr bool specified_edge_attribute_type = false;
  static constexpr bool specified_edge_attribute_is_const = false;
  static constexpr bool specified_edge_has_dynamic_attribute = false;
  static constexpr bool specified_edge_attribute_key_type = false;
  static constexpr bool specified_edge_attribute_container_type = false;
  static constexpr bool specified_edge_attribute_container_sort_type = false;
  /// mark whether specified containers type
  static constexpr bool specified_edge_label_container_type = false;
  static constexpr bool specified_vertex_ptr_container_type = false;
  static constexpr bool specified_edge_container_type = false;
  /// mark whether specified containers sort types
  static constexpr bool specified_edgeLabel_container_sort_type = false;
  static constexpr bool specified_vertex_ptr_container_sort_type = false;
  static constexpr bool specified_edge_container_sort_type = false;
  /// default setting of miscellaneous
  static constexpr bool specified_allow_multiple_edge = false;
  static constexpr bool specified_allow_duplicate_edge = false;
  static constexpr bool specified_duplicate_num_type = false;

 public:
  static constexpr bool HasUnSpecifiedConfigure = false;
  /// default setting of store data type
  static constexpr enum StoreData store_data    = StoreData::InOutEdge;
  /// default setting of vertex configurations
                  using VertexIDType                    = DefaultIDType;
  static constexpr bool vertex_id_provided_by_user      = true;
                  using VertexLabelType                 = DefaultLabelType;
  static constexpr bool vertex_label_is_const           = true;
  static constexpr bool vertex_has_static_attribute     = false;
                  using VertexStaticAttributeType       = DefaultAttributeType;
  static constexpr bool vertex_attribute_is_const       = true;
  static constexpr bool vertex_has_dynamic_attribute    = true;
                  using VertexAttributeKeyType          = DefaultKeyType;
  static constexpr enum ContainerType
                        vertex_attribute_container_type = DefaultContainerType;
  static constexpr enum SortType
                        vertex_attribute_container_sort_type = DefaultSortType;
  /// default setting of edge configurations
                  using EdgeIDType = DefaultIDType;
  static constexpr bool edge_id_provided_by_user      = true;
                  using EdgeLabelType                 = DefaultLabelType;
  static constexpr bool edge_label_is_const           = true;
  static constexpr bool edge_has_static_attribute     = false;
                  using EdgeStaticAttributeType       = DefaultAttributeType;
  static constexpr bool edge_attribute_is_const       = true;
  static constexpr bool edge_has_dynamic_attribute    = true;
                  using EdgeAttributeKeyType          = DefaultKeyType;
  static constexpr enum ContainerType
                        edge_attribute_container_type = DefaultContainerType;
  static constexpr enum SortType
                        edge_attribute_container_sort_type = DefaultSortType;
  /// default setting of container types
  static constexpr enum ContainerType
           vertex_label_container_type = DefaultContainerType;
  static constexpr enum ContainerType
              vertex_id_container_type = DefaultContainerType;
  static constexpr enum ContainerType
             edge_label_container_type = DefaultContainerType;
  static constexpr enum ContainerType
             vertex_ptr_container_type = DefaultContainerType;
  static constexpr enum ContainerType
        decomposed_edge_container_type = DefaultContainerType;
  /// default setting of container sort types
  static constexpr enum SortType
           vertex_label_container_sort_type = DefaultSortType;
  static constexpr enum SortType
              vertex_id_container_sort_type = DefaultSortType;
  static constexpr enum SortType
             edge_label_container_sort_type = DefaultSortType;
  static constexpr enum SortType
             vertex_ptr_container_sort_type = DefaultSortType;
  static constexpr enum SortType
        decomposed_edge_container_sort_type = DefaultSortType;
  /// default setting of miscellaneous
  static constexpr bool allow_multiple_edge = false;
  static constexpr bool allow_duplicate_edge = false;
  using DuplicateNumType = uint32_t;
};

// Set Vertex ID
template <typename VIDType, typename... other_configures>
class GraphConfigures<SetVertexIDType<VIDType>, other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(!GraphConfigures<other_configures...>::specified_vertex_id_type,
                "Redefination of vertex id type!\n");
  static_assert(std::is_object<VIDType>::value, "Illegal Edge ID Type");

 protected:
  static constexpr bool specified_vertex_id_type = false;

 public:
  using VertexIDType = VIDType;
};

// Set Node Label Type
template <template <typename...> class VLabelType,
          typename VLabelUnderlieConfigures, typename... other_configures>
class GraphConfigures<SetVertexLabelType<VLabelType<VLabelUnderlieConfigures>>,
                      other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_vertex_label_type,
      "Redefinition of Vertex Label Type");
  static_assert(std::is_base_of<Label<VLabelUnderlieConfigures>,
                                VLabelType<VLabelUnderlieConfigures>>::value,
                "Illegal Vertex Label Type");

 protected:
  static constexpr bool specified_vertex_label_type = true;

 public:
  using VertexLabelType = VLabelType<VLabelUnderlieConfigures>;
};

// Has Node Attribute
template <bool HasAttribute, typename... other_configures>
class GraphConfigures<SetVertexHasAttribute<HasAttribute>, other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_vertex_has_static_attribute,
      "Redefinition of Has Vertex Attr");

 protected:
  static constexpr bool specified_vertex_has_static_attribute = true;
  static constexpr bool vertex_has_static_attribute = HasAttribute;

 public:
};

// set Node attr
template <template <typename...> class VAttrType,
          typename VAttrUnderlieConfigures, typename... other_configures>
class GraphConfigures<
    SetVertexStaticAttributeType<VAttrType<VAttrUnderlieConfigures>>,
    other_configures...> : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_vertex_attribute_type,
      "Redefinition of Vertex Attr Type");
  static_assert(std::is_base_of<Attribute<VAttrUnderlieConfigures>,
                                VAttrType<VAttrUnderlieConfigures>>::value,
                "Illegal Vertex Attr Type");

 protected:
  static constexpr bool specified_vertex_attribute_type = false;

 public:
  using VertexStaticAttributeType = VAttrType<VAttrUnderlieConfigures>;
};

// Set Edge ID
template <typename EIDType, typename... other_configures>
class GraphConfigures<SetEdgeIDType<EIDType>, other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(!GraphConfigures<other_configures...>::specified_edge_id_type,
                "Redefinition of Edge ID Type");
  static_assert(std::is_object<EIDType>::value, "Illegal Edge ID Type");

 protected:
  static constexpr bool specified_edge_id_type = true;

 public:
  using EdgeIDType = EIDType;
};

// set edge label
template <template <typename...> class ELabelType,
          typename ELabelUnderlieConfigures, typename... other_configures>
class GraphConfigures<SetEdgeLabelType<ELabelType<ELabelUnderlieConfigures>>,
                      other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_edge_label_type,
      "Redefinition of Edge Label Type");
  static_assert(std::is_base_of<Label<ELabelUnderlieConfigures>,
                                ELabelType<ELabelUnderlieConfigures>>::value,
                "Illegal Edge Label Type");

 protected:
  static constexpr bool specified_edge_label_type = true;

 public:
  using EdgeLabelType = ELabelType<ELabelUnderlieConfigures>;
};

// Has Edge Attribute
template <bool HasAttribute, typename... other_configures>
class GraphConfigures<SetEdgeHasAttribute<HasAttribute>, other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_edge_has_static_attribute,
      "Redefinition of Has Edge Attr");

 protected:
  static constexpr bool specified_edge_has_static_attribute = true;

 public:
  static constexpr bool edge_has_static_attribute = HasAttribute;
};

// set edge attr
template <template <typename...> class EAttrType,
          typename EAttrUnderlieConfigures, typename... other_configures>
class GraphConfigures<SetEdgeStaticAttributeType<EAttrType<EAttrUnderlieConfigures>>,
                      other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_edge_attribute_type,
      "Redefinition of Edge Attr Type");
  static_assert(std::is_base_of<Attribute<EAttrUnderlieConfigures>,
                                EAttrType<EAttrUnderlieConfigures>>::value,
                "Illegal Edge Attr Type");

 protected:
  static constexpr bool specified_edge_attribute_type = false;

 public:
  using EdgeStaticAttributeType = EAttrType<EAttrUnderlieConfigures>;
};

// set multi edge
template <bool AllowMultiEdge, typename... other_configures>
class GraphConfigures<SetAllowMultipleEdge<AllowMultiEdge>, other_configures...>
    : public GraphConfigures<other_configures...> {
 private:
  static_assert(
      !GraphConfigures<other_configures...>::specified_allow_multiple_edge,
      "Redefinition of Allow Multi Edge");

 protected:
  static constexpr bool specified_allow_multiple_edge = true;

 public:
  static constexpr bool allow_multiple_edge = AllowMultiEdge;
};

/// Other configurations are not specified
template <typename UnSpecifiedConfigure, typename... other_configures>
class GraphConfigures<UnSpecifiedConfigure, other_configures...>
    : public GraphConfigures<other_configures...> {
 public:
  static constexpr bool HasUnSpecifiedConfigure = true;
};
}  // namespace GUNDAM

#endif  // _GRAPH_CONFIGURE_HPP
