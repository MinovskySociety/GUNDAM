#ifndef _GRAPH_CONFIGURE_HPP
#define _GRAPH_CONFIGURE_HPP

//#include "vertex.hpp"
#include "configures.hpp"
#include "label.hpp"

template<typename... configures>
class GraphConfigures;

/// declaration of configurable parameters
/// store data type
template<enum StoreData store_data_type>
class SetStoreDataType;
/// node configurations
template<typename LabelType>
class SetVertexLabelType;
template<typename IDType>
class SetVertexIDType;
template<bool HasAttribute>
class SetVertexHasAttribute;
template<typename AttributeType>
class SetVertexAttributeType;
template<bool AttributeIsConst>
class SetVertexAttributeIsConst;
template<bool AttributeIsDynamical>
class SetVertexAttributeIsDynamical;
template<enum ContainerType containerType>
class SetVertexAttributeContainerType;
/// edge configurations
template<typename LabelType>
class SetEdgeLabelType;
template<typename IDType>
class SetEdgeIDType;
template<bool HasAttribute>
class SetEdgeHasAttribute;
template<typename AttributeType>
class SetEdgeAttributeType;
template<bool AttributeIsConst>
class SetEdgeAttributeIsConst;
template<bool AttributeIsDynamical>
class SetEdgeAttributeIsDynamical;
template<enum ContainerType containerType>
class SetEdgeAttributeContainerType;
/// container type configurations
template<enum ContainerType containerType>
class SetEdgeLabelContainerType;
template<enum ContainerType containerType>
class SetVertexesContainerType;
template<enum ContainerType containerType>
class SetEdgeContainerType;
/// container sort type configurations
template<enum SortType sortType>
class SetEdgeLabelContainerSortType;
template<enum SortType sortType>
class SetVertexesContainerSortType;
template<enum SortType sortType>
class SetEdgeContainerSortType;
/// miscellaneous
template<bool AllowMultipleEdge>
class SetAllowMultipleEdge;
template<bool AllowDuplicateEdge>
class SetAllowDuplicateEdge;
template<bool AllowDuplicateNumType>
class SetDuplicateNumType;
//template<typename... AuxiliaryDataTypes>
//class AddAuxiliaryData;

/// default setting of configurations
template<>
class GraphConfigures<>{
private:
    using DefaultIDType = uint32_t;
    using DefaultKeyType = uint32_t;
    using DefaultLabelType = Label<uint32_t>;
    using DefaultAttributeType = std::string;

    static constexpr enum
            ContainerType
     DefaultContainerType = ContainerType::Vector;

    static constexpr enum
                 SortType
          DefaultSortType =      SortType::None;

protected:
    /// mark whether specified store data type
    static constexpr bool specified_store_data_type                 = false;
    /// mark whether specified node configurations
    static constexpr bool specified_vertex_id_type                  = false;
    static constexpr bool specified_vertex_label_type               = false;
    static constexpr bool specified_vertex_label_is_const           = false;
    static constexpr bool specified_vertex_has_attribute            = false;
    static constexpr bool specified_vertex_attribute_type           = false;
    static constexpr bool specified_vertex_attribute_is_const       = false;
    static constexpr bool specified_vertex_attribute_is_dynamic     = false;
    static constexpr bool specified_vertex_attribute_key_type       = false;
    static constexpr bool specified_vertex_attribute_container_type = false;
    /// mark whether specified edge configurations
    static constexpr bool specified_edge_id_type                    = false;
    static constexpr bool specified_edge_label_type                 = false;
    static constexpr bool specified_edge_label_is_const             = false;
    static constexpr bool specified_edge_has_attribute              = false;
    static constexpr bool specified_edge_attribute_type             = false;
    static constexpr bool specified_edge_attribute_is_const         = false;
    static constexpr bool specified_edge_attribute_is_dynamic       = false;
    static constexpr bool specified_edge_attribute_key_type         = false;
    static constexpr bool specified_edge_attribute_container_type   = false;
    /// mark whether specified containers type
    static constexpr bool specified_edge_label_container_type       = false;
    static constexpr bool specified_vertex_container_type           = false;
    static constexpr bool specified_edge_container_type             = false;
    /// mark whether specified containers sort types
    static constexpr bool specified_edgeLabel_container_sort_type   = false;
    static constexpr bool specified_vertexes_container_sort_type    = false;
    static constexpr bool specified_edge_container_sort_type        = false;
    /// default setting of miscellaneous
    static constexpr bool specified_allow_multiple_edge             = false;
    static constexpr bool specified_allow_duplicate_edge            = false;
    static constexpr bool specified_duplicate_num_type              = false;

public:
    /// default setting of store data type
    static constexpr enum StoreData
             store_data = StoreData::OutEdge;
    /// default setting of node configurations
    using VertexIDType                    = DefaultIDType;
    static constexpr bool
          vertex_id_provided_by_user      = true;
    using VertexLabelType                 = DefaultLabelType;
    static constexpr bool
          vertex_label_is_const           = true;
    static constexpr bool
          vertex_has_attribute            = false;
    using VertexAttributeType             = DefaultAttributeType;
    static constexpr bool
          vertex_attribute_is_const       = true;
    static constexpr bool
          vertex_attribute_is_dynamic     = true;
    using VertexAttributeKeyType          = DefaultKeyType;
    static constexpr enum ContainerType
          vertex_attribute_container_type = DefaultContainerType;
    /// default setting of edge configurations
    using EdgeIDType                      = DefaultIDType;
    static constexpr bool
          edge_id_provided_by_user        = true;
    using EdgeLabelType                   = DefaultLabelType;
    static constexpr bool
          edge_label_is_const             = true;
    static constexpr bool
          edge_has_attribute              = false;
    using EdgeAttributeType               = DefaultAttributeType;
    static constexpr bool
          edge_attribute_is_const         = true;
    static constexpr bool
          edge_attribute_is_dynamic       = true;
    using EdgeAttributeKeyType            = DefaultKeyType;
    static constexpr enum ContainerType
          edge_attribute_container_type   = DefaultContainerType;
    /// default setting of container types
    static constexpr enum ContainerType
               edge_label_container_type = DefaultContainerType;
    static constexpr enum ContainerType
                   vertex_container_type = DefaultContainerType;
    static constexpr enum ContainerType
          decomposed_edge_container_type = DefaultContainerType;
    /// default setting of container sort types
    static constexpr enum SortType
               edge_label_container_sort_type = DefaultSortType;
    static constexpr enum SortType
                   vertex_container_sort_type = DefaultSortType;
    static constexpr enum SortType
          decomposed_edge_container_sort_type = DefaultSortType;
    /// default setting of miscellaneous
    static constexpr bool
                 allow_multiple_edge = false;
    static constexpr bool
                allow_duplicate_edge = true;
    using DuplicateNumType           = uint32_t;
};

#endif // _GRAPH_CONFIGURE_HPP
