#include "../../include/GUNDAM/include/gundam/graph.h"
#include "../../include/stardand/errcode.h"
#include "../../include/stardand/paratype.h"
#include <iostream>
#include <sstream>
#include <string> 
using ReturnType= int;

template<template <typename...> class GraphType,typename... configures>
ReturnType OutputGraph(const GraphType<configures...>& graph){
    using VertexType = typename GraphType<configures...>::VertexType;
    using   EdgeType = typename GraphType<configures...>::EdgeType;
    using VertexIDType            = typename VertexType::IDType;
    using VertexAttributeType     = typename VertexType::AttributeType;
    using VertexLabelType         = typename VertexType::LabelType;
    using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
    using EdgeLabelType         = typename EdgeType::LabelType;
    using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
    using VertexPtr  = const VertexType*;
    std::cout<<"node label:\n";
    for (auto it=graph.VertexBegin;!it.IsDone();it++){
        std::cout<<it->id()<<" "
        <<it->label().to_string()<<std::endl;
    }
    std::cout<<"src_id dst_id edge_label:\n";
    for (auto it=graph.VertexBegin();!it.IsDone();it++){
        for (auto edge_it=graph.FindVertex(it->id())->edge_cbegin();
             !edge_it.IsDone();edge_it++){
            std::cout<<edge_it->src_id()<<" "<<edge_it->dst_id()
            <<" "<<edge_it->label().to_string()<<std::endl;
        }
    }
    return E_OK;
}