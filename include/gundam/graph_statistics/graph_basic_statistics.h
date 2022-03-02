#ifndef _GUNDAM_GRAPH_STATISTICS_GRAPH_BASIC_STATISTICS_H
#define _GUNDAM_GRAPH_STATISTICS_GRAPH_BASIC_STATISTICS_H

#include "include/gundam/type_getter/vertex_id.h"
#include "include/gundam/type_getter/vertex_label.h"
#include "include/gundam/type_getter/vertex_handle.h"

#include "include/gundam/type_getter/edge_id.h"
#include "include/gundam/type_getter/edge_label.h"
#include "include/gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"

namespace GUNDAM {

// the statics of graph
// the basic statics are collected in the 
template <typename GraphType>
class GraphBasicStatistics {
 private:
  using VertexLabelType = typename VertexLabel<GraphType>::type;
  using   EdgeLabelType = typename   EdgeLabel<GraphType>::type;

  using VertexIdType = typename VertexID<GraphType>::type;
  using   EdgeIdType = typename   EdgeID<GraphType>::type;

  using VertexAttributeKeyType = typename GraphType
                                       ::VertexType
                                 ::AttributeKeyType;
  
  using EdgeTypeType = std::tuple<VertexLabelType,
                                    EdgeLabelType,
                                  VertexLabelType>;

 public:
  GraphBasicStatistics() : max_vertex_id_(0),
                             max_edge_id_(0){
    return;

  }

  GraphBasicStatistics(const GraphType& graph)
                        : max_vertex_id_(0),
                            max_edge_id_(0){
    this->AddGraph(graph);
    return;
  }
  
  inline void AddGraph(const GraphType& graph) {
    for (auto vertex_it = graph.VertexBegin();
             !vertex_it.IsDone();
              vertex_it++) {
      this->max_vertex_id_ = this->max_vertex_id_ > vertex_it->id() ?
                             this->max_vertex_id_ : vertex_it->id();
      this->vertex_label_counter_[vertex_it->label()]++;
      for (auto out_edge_it = vertex_it->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        this->max_edge_id_ = this->max_edge_id_ > out_edge_it->id() ?
                             this->max_edge_id_ : out_edge_it->id();
        assert( out_edge_it->src_handle() == vertex_it );
        this->edge_label_counter_[out_edge_it->label() ]++;
        this-> edge_type_counter_[
                         std::tuple(vertex_it->label(),
                                  out_edge_it->label(),
                                  out_edge_it->dst_handle()->label())]++;
      }
    }
    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      this->AddVertexAttr(graph);
    }
    return;
  }

  inline auto VertexCounter(const VertexLabelType& vertex_label) const {
    auto vertex_label_it = this->vertex_label_counter_.find(vertex_label);
    if (vertex_label_it == this->vertex_label_counter_.end()){
      return (size_t)0;
    }
    assert(vertex_label_it->second > 0);
    return vertex_label_it->second;
  }

  inline auto EdgeCounter(const EdgeLabelType& edge_label) const {
    auto edge_label_it = this->edge_label_counter_.find(edge_label);
    if (edge_label_it == this->edge_label_counter_.end()){
      return (size_t)0;
    }
    assert(edge_label_it->second > 0);
    return edge_label_it->second;
  }

  inline auto EdgeTypeCounter(const typename EdgeHandle<GraphType>::type& edge_handle) const {
    return this->EdgeTypeCounter(edge_handle->src_hanlde()->label(),
                                 edge_handle->label(),
                                 edge_handle->dst_hanlde()->label());
  }

  inline auto EdgeTypeCounter(const VertexLabelType&  src_label,
                              const   EdgeLabelType& edge_label,
                              const VertexLabelType&  dst_label) const {
    return this->EdgeTypeCounter(std::tuple(src_label,
                                           edge_label,
                                            dst_label));
  }

  inline auto EdgeTypeCounter(const std::tuple<VertexLabelType,
                                                 EdgeLabelType,
                                               VertexLabelType>& edge_type) const {
    auto edge_type_it = this->edge_type_counter_.find(edge_type);
    if (edge_type_it == this->edge_type_counter_.end()){
      return (size_t)0;
    }
    assert(edge_type_it->second > 0);
    return edge_type_it->second;
  }

  inline auto edge_type_counter_begin() const {
    return this->edge_type_counter_.begin();
  }

  inline auto edge_type_counter_end() const {
    return this->edge_type_counter_.end();
  }

  inline auto edge_type_counter_size() const {
    return this->edge_type_counter_.size();
  }

  inline auto vertex_label_counter_begin() const {
    return this->vertex_label_counter_.begin();
  }

  inline auto vertex_label_counter_end() const {
    return this->vertex_label_counter_.end();
  }

  inline auto vertex_label_counter_size() const {
    return this->vertex_label_counter_.size();
  }

  inline auto AvgOutDegree(const typename EdgeHandle<GraphType>::type& edge_handle) const {
    return this->AvgOutDegree(edge_handle->src_hanlde()->label(),
                              edge_handle->label(),
                              edge_handle->dst_hanlde()->label());
  }

  inline auto AvgOutDegree(const VertexLabelType&  src_label,
                           const   EdgeLabelType& edge_label,
                           const VertexLabelType&  dst_label) const {
    return this->AvgOutDegree(std::tuple(src_label,
                                        edge_label,
                                         dst_label));
  }

  inline auto AvgOutDegree(const std::tuple<VertexLabelType,
                                              EdgeLabelType,
                                            VertexLabelType>& edge_type) const {
    auto edge_type_it = this->edge_type_counter_.find(edge_type);
    if (edge_type_it == this->edge_type_counter_.end()){
      return (float)0;
    }
    assert(edge_type_it->second > 0);
    assert(this->VertexCounter(std::get<0>(edge_type)) > 0);
    return (float)(edge_type_it->second)
         / (float)(this->VertexCounter(std::get<0>(edge_type)));
  }

  inline auto AvgInDegree(const typename EdgeHandle<GraphType>::type& edge_handle) const {
    return this->AvgInDegree(edge_handle->src_hanlde()->label(),
                             edge_handle->label(),
                             edge_handle->dst_hanlde()->label());
  }

  inline auto AvgInDegree(const VertexLabelType&  src_label,
                          const   EdgeLabelType& edge_label,
                          const VertexLabelType&  dst_label) const {
    return this->AvgInDegree(std::tuple(src_label,
                                       edge_label,
                                        dst_label));
  }

  inline auto AvgInDegree(const std::tuple<VertexLabelType,
                                             EdgeLabelType,
                                           VertexLabelType>& edge_type) const {
    auto edge_type_it = this->edge_type_counter_.find(edge_type);
    if (edge_type_it == this->edge_type_counter_.end()){
      return (float)0;
    }
    assert(edge_type_it->second > 0);
    assert(this->VertexCounter(std::get<2>(edge_type)) > 0);
    return (float)(edge_type_it->second)
         / (float)(this->VertexCounter(std::get<2>(edge_type)));
  }

  inline const auto& vertex_label_counter() const {
    return this->vertex_label_counter_;
  }

  inline const auto& edge_label_counter() const {
    return this->edge_label_counter_;
  }

  inline const auto& edge_type_counter() const {
    return this->edge_type_counter_;
  }

  inline const auto& legal_attr_set() const {
    return this->legal_attr_set_;
  }

  inline VertexIdType max_vertex_id() const {
    return this->max_vertex_id_;
  }

  inline EdgeIdType max_edge_id() const {
    return this->max_edge_id_;
  }

  inline void PreserveEdgeTypeSet(
       const std::set<EdgeTypeType>& specified_edge_type_set) {

    for (auto edge_type_set_it  = this->edge_type_counter_.begin();
              edge_type_set_it != this->edge_type_counter_.end();){
      if (specified_edge_type_set.find(edge_type_set_it->first)
       == specified_edge_type_set.end()){
        // this edge_type is not contained in the specified_edge_type_set
        // remove from specified_edge_type_set
        edge_type_set_it = this->edge_type_counter_.erase(edge_type_set_it);
        continue;
      }
      edge_type_set_it++;
    }
    
    std::map<VertexLabelType, size_t> temp_vertex_label_counter;
    std::map<  EdgeLabelType, size_t>   temp_edge_label_counter;

    this->vertex_label_counter_.swap(temp_vertex_label_counter);
    this->  edge_label_counter_.swap(  temp_edge_label_counter);

    for (const auto& [edge_type, edge_type_counter] : this->edge_type_counter_) {
      assert(temp_vertex_label_counter.find(std::get<0>(edge_type))
          != temp_vertex_label_counter.end());
      assert(temp_vertex_label_counter.find(std::get<2>(edge_type))
          != temp_vertex_label_counter.end());
      assert(  temp_edge_label_counter.find(std::get<1>(edge_type))
          !=   temp_edge_label_counter.end());
      this->vertex_label_counter_.emplace(*temp_vertex_label_counter.find(std::get<0>(edge_type)));
      this->vertex_label_counter_.emplace(*temp_vertex_label_counter.find(std::get<2>(edge_type)));
      this->  edge_label_counter_.emplace(*  temp_edge_label_counter.find(std::get<1>(edge_type)));
    }

    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      for (auto legal_attr_set_it  = this->legal_attr_set_.begin();
                legal_attr_set_it != this->legal_attr_set_.end();){
        if (this->vertex_label_counter_.find(legal_attr_set_it->first)
        != this->vertex_label_counter_.end()) {
          legal_attr_set_it++;
          continue;
        }
        legal_attr_set_it = this->legal_attr_set_.erase(legal_attr_set_it);
      }
    }
    return;
  }

  inline void PreserveEdgeLabelSet(
       const std::set<EdgeLabelType>& specified_edge_label_set) {

    for (auto edge_type_set_it  = this->edge_type_counter_.begin();
              edge_type_set_it != this->edge_type_counter_.end();){
      if (specified_edge_label_set.find(std::get<1>(edge_type_set_it->first))
       == specified_edge_label_set.end()) {
        edge_type_set_it = this->edge_type_counter_.erase(edge_type_set_it);
        continue;
      }
      edge_type_set_it++;
    }
    
    std::map<VertexLabelType, size_t> temp_vertex_label_counter;
    std::map<  EdgeLabelType, size_t>   temp_edge_label_counter;

    this->vertex_label_counter_.swap(temp_vertex_label_counter);
    this->  edge_label_counter_.swap(  temp_edge_label_counter);

    for (const auto& [edge_type, edge_type_counter] : this->edge_type_counter_) {
      assert(temp_vertex_label_counter.find(std::get<0>(edge_type))
          != temp_vertex_label_counter.end());
      assert(temp_vertex_label_counter.find(std::get<2>(edge_type))
          != temp_vertex_label_counter.end());
      assert(  temp_edge_label_counter.find(std::get<1>(edge_type))
          !=   temp_edge_label_counter.end());
      this->vertex_label_counter_.emplace(*temp_vertex_label_counter.find(std::get<0>(edge_type)));
      this->vertex_label_counter_.emplace(*temp_vertex_label_counter.find(std::get<2>(edge_type)));
      this->  edge_label_counter_.emplace(*  temp_edge_label_counter.find(std::get<1>(edge_type)));
    }

    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      for (auto legal_attr_set_it  = this->legal_attr_set_.begin();
                legal_attr_set_it != this->legal_attr_set_.end();){
        if (this->vertex_label_counter_.find(legal_attr_set_it->first)
        != this->vertex_label_counter_.end()) {
          legal_attr_set_it++;
          continue;
        }
        legal_attr_set_it = this->legal_attr_set_.erase(legal_attr_set_it);
      }
    }
    return;
  }

  inline void AddVertexAttr(const GraphType& graph,
                            double constant_freq_bound = 0.0) {

    static_assert(GraphParameter<GraphType>::vertex_has_attribute,
                 "GraphType does not support attribute on vertex");
  
    assert(constant_freq_bound >= 0.0);
                  
    using AttrValueContainerType
        = std::map<std::pair<std::string,  // value_str
                             enum GUNDAM::BasicDataType>,  // value_type
                   int>;

    using AttrKeyContainerType
        = std::map<VertexAttributeKeyType,
                   AttrValueContainerType>;

    for (auto vertex_it = graph.VertexBegin();
             !vertex_it.IsDone();
              vertex_it++) {
      auto [legal_attr_it,
            legal_attr_ret] = this->legal_attr_set_.emplace(vertex_it->label(),
                                                            AttrKeyContainerType());
      assert(legal_attr_it != this->legal_attr_set_.end());
      for (auto attr_it = vertex_it->AttributeBegin();
               !attr_it.IsDone();
                attr_it++) {
        auto [vertex_label_it,
              vertex_label_ret]
            = legal_attr_it->second.emplace(attr_it->key(),
                                            AttrValueContainerType());
        assert(vertex_label_it 
              != legal_attr_it->second.end());
        auto [vertex_attr_key_it,
              vertex_attr_key_ret]
            = vertex_label_it->second.emplace(std::pair(attr_it->value_str(), 
                                                        attr_it->value_type()), 1);
        if (!vertex_attr_key_ret){
          // added not added successful, maintain the counter
          vertex_attr_key_it->second++;
        }
      }
    }
    // preserve high-frequence constant value based on constant_freq_bound
    for (auto& vertex_label_legal_attr : this->legal_attr_set_) {
      for (auto& attr_key_legal_attr_set 
           : vertex_label_legal_attr.second){
        int total_attr_number = 0;
        for (const auto& [value_str_type_pair, 
                          value_counter] : attr_key_legal_attr_set.second) {
          total_attr_number += value_counter;
        }
        for (auto attr_it  = attr_key_legal_attr_set.second.begin();
                  attr_it != attr_key_legal_attr_set.second.end();) {
          if ((((double)attr_it->second)
             / ((double)total_attr_number)) >= constant_freq_bound){
            // the frequence of this attribute has reached the constant_freq_bound
            // need to be preserved
            attr_it++;
            continue;
          }
          attr_it = attr_key_legal_attr_set.second.erase(attr_it);
        }
      }
    }
    return;
  }
  
  inline void ResetVertexAttr(const GraphType& graph,
                              double constant_freq_bound = 0.0) {

    static_assert(GraphParameter<GraphType>::vertex_has_attribute,
                 "GraphType does not support attribute on vertex");
    this->legal_attr_set_.clear();
    this->AddVertexAttr(graph, constant_freq_bound);
    return;
  }

 private:
  // basic statistics
  std::map<VertexLabelType, size_t> vertex_label_counter_;
  std::map<  EdgeLabelType, size_t>   edge_label_counter_;
  
  std::map<std::tuple<VertexLabelType,
                        EdgeLabelType,
                      VertexLabelType>, size_t> edge_type_counter_;

  VertexIdType max_vertex_id_;
    EdgeIdType   max_edge_id_;

  std::map<VertexLabelType,
  std::map<VertexAttributeKeyType,
  std::map<std::pair<std::string,  // value_str
                     enum GUNDAM::BasicDataType>, // value_type
           int>>> legal_attr_set_;
};

};


#endif // _GUNDAM_GRAPH_STATISTICS_GRAPH_BASIC_STATISTICS_H