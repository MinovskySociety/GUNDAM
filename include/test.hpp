#ifndef _TEST_HPP
#define _TEST_HPP

namespace test{
    template<template <typename...> class GraphType, typename... configures>
    void LoadGraph(GraphType<configures...>& graph,
                   std::istream& vertex_file,
                   std::istream&   edge_file){
        /// wenzhi:
        ///     not a complete method, just for test
        ///     only support Vertex with:
        ///         LabelType: Label<uint32_t>
        ///         IDType: uint32_t
        ///         AttributeType: std::string
        ///     Edge Label:
        ///         LabelType: Label<uint32_t>
        ///         no id
        ///         no attribute

        static_assert(std::is_base_of<GUNDAM::Graph    <configures...>,
                                              GraphType<configures...>>::value,
                     "Illegal input parameter, should be a graph");
        using VertexType = typename GraphType<configures...>::VertexType;
        using   EdgeType = typename GraphType<configures...>::EdgeType;

        using VertexIDType            = typename VertexType::IDType;
        using VertexAttributeType     = typename VertexType::AttributeType;
        using VertexLabelType         = typename VertexType::LabelType;
        using VertexLabelUnderlieType = typename VertexType::LabelType
                                                           ::UnderlieType;

        using EdgeLabelType         = typename EdgeType::LabelType;
        using EdgeLabelUnderlieType = typename EdgeType::LabelType
                                                       ::UnderlieType;

        static_assert(std::is_same<VertexIDType,             uint32_t >::value
                   && std::is_same<VertexLabelType,    Label<uint32_t>>::value
                   && std::is_same<VertexAttributeType,std::string    >::value,
                     "Illegal vertex type");
        static_assert(std::is_same<EdgeLabelType,Label<uint32_t>>::value,
                     "Illegal edge type");
        /// wenzhi: the static_assert statement is far from complete

//        constexpr uint32_t default_key = 1;

        while (!vertex_file.eof()){
            VertexIDType            vertex_id;
            VertexLabelUnderlieType vertex_underlie_label;
            VertexAttributeType     vertex_attribute;

            vertex_file>>vertex_id;
            vertex_file>>vertex_underlie_label;
            vertex_file>>vertex_attribute;

            graph.AddVertex(vertex_id,
                            VertexLabelType(vertex_underlie_label));
//                            ->add_attribute(default_key,
//                                            vertex_attribute);

            if (vertex_file.peek() == EOF)
                break;
        }

//        uint32_t edge_counter = 0;

//        const std::string test_value = "test";
//        const uint32_t    test_key   = 1;

        while (!edge_file.eof()){
            VertexIDType src_id;
            VertexIDType dst_id;
            EdgeLabelUnderlieType edge_underlie_label;

            edge_file>>src_id;
            edge_file>>dst_id;
            edge_file>>edge_underlie_label;

            graph.AddEdge(src_id,dst_id,
                          EdgeLabelType(edge_underlie_label));
//                          edge_counter++)
//                          ->add_attribute(test_key,test_value);

            if (edge_file.peek() == EOF)
                break;
        }
        return;
    }
}

#endif // _TEST_HPP
