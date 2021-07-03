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

 private:
  template<typename VertexIteratorType>
  class SpecifiedLabelVertexIterator;

  // add a filter on the vertex iterator to travel all
  // vertexes with the specified vertex label
  template<typename VertexIteratorType>
  class SpecifiedLabelVertexIterator{
   private:
    // if the vertex currently points to does not have the
    // specified label, move to the spcifeid 
    inline void MoveToSpecifiedLabel() {
      while (!this->vertex_iterator_.IsDone()) {
        if (this->vertex_iterator_->label() 
         == this->specified_vertex_label_) {
          return;
        }
        // is not point to the specified label
        // move to the next vertex
        this->vertex_iterator_++;
      }
      return;
    }
    
   public:
    SpecifiedLabelVertexIterator(
            const VertexLabelType& specified_vertex_label,
            const VertexIteratorType& vertex_iterator) 
         : specified_vertex_label_(specified_vertex_label),
                     vertex_iterator_(vertex_iterator){
      // the initialized iterator
      // might not point to the vertex with the specified
      this->MoveToSpecifiedLabel();
      return;
    }

    inline SpecifiedLabelVertexIterator operator++() {
      /// prefix
      assert(!this->IsDone());
      this->vertex_iterator_++;
      this->MoveToSpecifiedLabel();
      return *this;
    }
    
    inline SpecifiedLabelVertexIterator operator++(int) {
      /// postfix
      assert(!this->IsDone());
      SpecifiedLabelVertexIterator temp(*this);
      this->vertex_iterator_++;
      this->MoveToSpecifiedLabel();
      return temp;
    }

    inline auto& operator->() {
      assert(!this->IsDone());
      return this->vertex_iterator_;
    }

    inline auto& operator->() const {
      assert(!this->IsDone());
      return this->vertex_iterator_;
    }
        
    inline bool IsDone() const {
      return this->vertex_iterator_.IsDone();
    }

   private:
    VertexLabelType specified_vertex_label_;

    VertexIteratorType vertex_iterator_;
  };

 public:
  // use the construct method for the concrete graph type
  using ConcreteGraphType::ConcreteGraphType;

  inline auto VertexBegin(){
    return ConcreteGraphType::VertexBegin();
  }

  inline auto VertexBegin() const {
    return ConcreteGraphType::VertexBegin();
  }  

  // the concrete graph type has index for vertex label,
  // can provide a more efficiency method
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

  // the concrete graph type does not have index for vertex label,
  // use the encapsulation iterator 
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index,
            std::enable_if_t<!judge, bool> = false>
  inline auto VertexBegin(const VertexLabelType& vertex_label) {
    return SpecifiedLabelVertexIterator<decltype(ConcreteGraphType::VertexBegin())>(
                                   vertex_label, ConcreteGraphType::VertexBegin());
  }
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index,
            std::enable_if_t<!judge, bool> = false>
  inline auto VertexBegin(const VertexLabelType& vertex_label) const {
    return SpecifiedLabelVertexIterator<decltype(ConcreteGraphType::VertexBegin())>(
                                   vertex_label, ConcreteGraphType::VertexBegin());
  }

  // inline std::enable_if_t<ConcreteGraphType::graph_level_count_vertex, bool>
  // CountVertex() const {
  //   return ConcreteGraphType::CountVertex();
  // }

  // inline std::enable_if_t<ConcreteGraphType::graph_level_count_vertex
  //                      && GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, bool>
  // CountVertex(const VertexLabelType& vertex_label) const {
  //   return ConcreteGraphType::CountVertex(vertex_label);
  // }
};

};

#endif // _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H
