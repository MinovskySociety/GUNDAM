#ifndef _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H
#define _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H

#include <typeinfo> 
#include "gundam/type_getter/graph_parameter_getter.h"

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/edge_handle.h"

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

  using VertexCounterType = typename ConcreteGraphType::VertexCounterType;

 private:
  using VertexLabelType = typename VertexType::LabelType;
  using   EdgeLabelType = typename   EdgeType::LabelType;

  using VertexIDType = typename VertexType::IDType;
  using   EdgeIDType = typename   EdgeType::IDType;

  using VertexPtr = typename ConcreteGraphType::VertexPtr;
  using   EdgePtr = typename ConcreteGraphType::  EdgePtr;

  using VertexConstPtr = typename ConcreteGraphType::VertexConstPtr;
  using   EdgeConstPtr = typename ConcreteGraphType::  EdgeConstPtr;

  friend class VertexHandle<GraphBase<ConcreteGraphType>>;
  friend class VertexHandle<const GraphBase<ConcreteGraphType>>;

  friend class EdgeHandle<GraphBase<ConcreteGraphType>>;
  friend class EdgeHandle<const GraphBase<ConcreteGraphType>>;

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

  template<typename VertexIteratorType,
           typename   EdgeIteratorType>
  class GraphLevelEdgeIterator;

  template<typename VertexIteratorType,
           typename   EdgeIteratorType>
  class GraphLevelEdgeIterator{
   private:
    // if the current edge is done, move to the 
    // out edge iterator of next vertex
    inline void MoveToNonIsDoneEdgeIterator() {
      assert(!this->vertex_iterator_.IsDone());
      while (this->edge_iterator_.IsDone()) {
        assert(!this->vertex_iterator_.IsDone());
        this->vertex_iterator_++;
        if (this->vertex_iterator_.IsDone()){
          // has visited all edges in this graph
          assert(this->IsDone());
          return;
        }
        assert(!this->vertex_iterator_.IsDone());
        this->edge_iterator_ = this->vertex_iterator_->OutEdgeBegin();
      }
      assert(!this->IsDone());
      assert(!this->edge_iterator_.IsDone());
      return;
    }
    
   public:
    GraphLevelEdgeIterator(
      const VertexIteratorType& vertex_iterator)
             : vertex_iterator_(vertex_iterator),
                 edge_iterator_(){
      assert(this->edge_iterator_.IsDone());
      assert(this->IsDone());
      if (this->vertex_iterator_.IsDone()) {
        assert(this->IsDone());
        return;
      }
      assert(!this->vertex_iterator_.IsDone());
      this->edge_iterator_ = this->vertex_iterator_->OutEdgeBegin();
      this->MoveToNonIsDoneEdgeIterator();
      return;
    }

    inline GraphLevelEdgeIterator operator++() {
      /// prefix
      assert(!this->IsDone());
      assert(!this->vertex_iterator_.IsDone());
      this->edge_iterator_++;
      this->MoveToNonIsDoneEdgeIterator();
      return *this;
    }
    
    inline GraphLevelEdgeIterator operator++(int) {
      /// postfix
      assert(!this->IsDone());
      assert(!this->vertex_iterator_.IsDone());
      GraphLevelEdgeIterator temp(*this);
      this->edge_iterator_++;
      this->MoveToNonIsDoneEdgeIterator();
      return temp;
    }

    inline auto& operator->() {
      assert(!this->IsDone());
      return this->edge_iterator_;
    }

    inline auto& operator->() const {
      assert(!this->IsDone());
      return this->edge_iterator_;
    }
        
    inline bool IsDone() const {
      return this->edge_iterator_.IsDone();
    }

   private:
    VertexIteratorType vertex_iterator_;
      EdgeIteratorType   edge_iterator_;
  };

 public:
  // use the construct method for the concrete graph type
  using ConcreteGraphType::ConcreteGraphType;

  // those methods must be provided by concrete graph type
  inline auto VertexBegin(){
    return ConcreteGraphType::VertexBegin();
  }
  inline auto VertexBegin() const {
    return ConcreteGraphType::VertexBegin();
  }  
  inline auto AddVertex(
      const VertexIDType& id,
      const VertexLabelType& label) {
    return ConcreteGraphType::AddVertex(id, label);
  }
  inline auto AddEdge(const  VertexIDType &src_vertex_id,
                      const  VertexIDType &dst_vertex_id,
                      const EdgeLabelType &edge_label,
                      const EdgeIDType    &edge_id)  {
    return ConcreteGraphType::AddEdge(src_vertex_id, 
                                      dst_vertex_id, 
                                      edge_label, 
                                      edge_id);
  }
  inline auto FindVertex(const VertexIDType &id) {
    return ConcreteGraphType::FindVertex(id);
  }
  inline auto FindVertex(const VertexIDType &id) const {
    return ConcreteGraphType::FindVertex(id);
  }
  inline void Clear() {
    ConcreteGraphType::Clear();
    return;
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
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
                 "illegal usage of this method");
    return SpecifiedLabelVertexIterator<decltype(ConcreteGraphType::VertexBegin())>(
                                   vertex_label, ConcreteGraphType::VertexBegin());
  }
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index,
            std::enable_if_t<!judge, bool> = false>
  inline auto VertexBegin(const VertexLabelType& vertex_label) const {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, 
                 "illegal usage of this method");
    return SpecifiedLabelVertexIterator<decltype(ConcreteGraphType::VertexBegin())>(
                                   vertex_label, ConcreteGraphType::VertexBegin());
  }

  // graph provides the graph level edge iterator
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_edge_iterator,
            std::enable_if_t<judge, bool> = false>
  inline auto EdgeBegin() {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_edge_iterator, 
                 "illegal usage of this method");
    return ConcreteGraphType::EdgeBegin();
  }
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_edge_iterator,
            std::enable_if_t<judge, bool> = false>
  inline auto EdgeBegin() const  {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_edge_iterator, 
                 "illegal usage of this method");
    return ConcreteGraphType::EdgeBegin();
  }

  // graph does not provides the graph level edge iterator
  // use the encapsulation iterator 
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_edge_iterator,
            std::enable_if_t<!judge, bool> = false>
  inline auto EdgeBegin() {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_edge_iterator, 
                 "illegal usage of this method");
    auto vertex_iterator = ConcreteGraphType::VertexBegin();
    return GraphLevelEdgeIterator<decltype(ConcreteGraphType::VertexBegin()),
                                  decltype( vertex_iterator->OutEdgeBegin())>(
                                           ConcreteGraphType::VertexBegin());
  }
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_edge_iterator,
            std::enable_if_t<!judge, bool> = false>
  inline auto EdgeBegin() const  {
    static_assert(judge == GraphParameter<ConcreteGraphType>::graph_level_edge_iterator, 
                 "illegal usage of this method");
    auto vertex_iterator = ConcreteGraphType::VertexBegin();
    return GraphLevelEdgeIterator<decltype(ConcreteGraphType::VertexBegin()),
                                  decltype( vertex_iterator->OutEdgeBegin())>(
                                           ConcreteGraphType::VertexBegin());
  }

  // the concrete graph type provides CountVertex method,
  // can provide a more efficiency method
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_count_vertex,
            std::enable_if_t<judge, bool> = false>
  inline auto CountVertex() const {
    return ConcreteGraphType::CountVertex();
  }

  // the concrete graph type provides CountEdge method,
  // can provide a more efficiency method
  template <bool judge = GraphParameter<ConcreteGraphType>::graph_level_count_edge,
            std::enable_if_t<judge, bool> = false>
  inline auto CountEdge() const {
    return ConcreteGraphType::CountEdge();
  }

  // inline std::enable_if_t<ConcreteGraphType::graph_level_count_vertex
  //                      && GraphParameter<ConcreteGraphType>::graph_level_vertex_label_index, bool>
  // CountVertex(const VertexLabelType& vertex_label) const {
  //   return ConcreteGraphType::CountVertex(vertex_label);
  // }
};

};

#endif // _GUNDAM_GRAPH_TYPE_GRAPH_BASE_H
