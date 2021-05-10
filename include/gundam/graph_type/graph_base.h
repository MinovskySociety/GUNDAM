#ifndef _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H
#define _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H

#include <typeinfo> 
#include "gundam/type_getter/graph_parameter_getter.h"

namespace GUNDAM {

template<typename ConcreteGraphType>
class GraphBase;

template <typename ConcreteGraphType>
class GraphParameter<GraphBase<ConcreteGraphType>> 
       : public GraphParameter<ConcreteGraphType>{
};

template<typename ConcreteGraphType>
class GraphBase : protected ConcreteGraphType {
 public:
  using VertexType = typename ConcreteGraphType::VertexType;
  using   EdgeType = typename ConcreteGraphType::  EdgeType;

 private:
  using VertexLabelType = typename VertexType::LabelType;
  using   EdgeLabelType = typename   EdgeType::LabelType;

 public:
  // use the construct method for the concrete graph type
  using ConcreteGraphType::ConcreteGraphType;

  inline auto VertexBegin(){
    return ConcreteGraphType::VertexBegin();
  }

  inline auto VertexBegin() const {
    return ConcreteGraphType::VertexBegin();
  }  

  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index,
            std::enable_if_t<judge, bool> = false>
  inline auto VertexBegin(const VertexLabelType& vertex_label) {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
                 "illegal usage of this method");
    return ConcreteGraphType::VertexBegin(vertex_label);
  }

  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index,
            std::enable_if_t<judge, bool> = false>
  inline auto VertexBegin(const VertexLabelType& vertex_label) const {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
                 "illegal usage of this method");
    return ConcreteGraphType::VertexBegin(vertex_label);
  }

  // inline std::enable_if_t<ConcreteGraphType::graph_level_count_vertex, auto>
  // CountVertex() const {
  //   return ConcreteGraphType::CountVertex();
  // }

  // inline std::enable_if_t<ConcreteGraphType::graph_level_count_vertex
  //                      && GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, auto>
  // CountVertex(const VertexLabelType& vertex_label) const {
  //   return ConcreteGraphType::CountVertex(vertex_label);
  // }

  // inline std::enable_if_t<!GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, auto>
  // VertexBegin(const VertexLabelType& vertex_label){
  //   // trick the compiler, equal to static_assert(false)
  //   static_assert( GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
  //                                    "graph_level_vertex_label_index is false for graph type: "
  //                                   + std::typeid(ConcreteGraphType).name()
  //                                   + " does not have graph.VertexBegin(vertex_label) method.");
  //   static_assert(!GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
  //                                    "graph_level_vertex_label_index is false for graph type: "
  //                                   + std::typeid(ConcreteGraphType).name()
  //                                   + " does not have graph.VertexBegin(vertex_label) method.");
  //   return;
  // }

  // inline std::enable_if_t<!GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, auto>
  // VertexBegin(const VertexLabelType& vertex_label) const {
  //   // trick the compiler, equal to static_assert(false)
  //   static_assert( GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
  //                                    "graph_level_vertex_label_index is false for graph type: "
  //                                   + std::typeid(ConcreteGraphType).name()
  //                                   + " does not have graph.VertexBegin(vertex_label) const method.");
  //   static_assert(!GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
  //                                    "graph_level_vertex_label_index is false for graph type: "
  //                                   + std::typeid(ConcreteGraphType).name()
  //                                   + " does not have graph.VertexBegin(vertex_label) const method.");
  //   return;
  // }
};

};

#endif // _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H
