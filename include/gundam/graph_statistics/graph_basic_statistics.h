#ifndef _GUNDAM_GRAPH_STATISTICS_GRAPH_BASIC_STATISTICS_H
#define _GUNDAM_GRAPH_STATISTICS_GRAPH_BASIC_STATISTICS_H

#include "include/gundam/type_getter/vertex_id.h"
#include "include/gundam/type_getter/vertex_label.h"
#include "include/gundam/type_getter/vertex_handle.h"

#include "include/gundam/type_getter/edge_id.h"
#include "include/gundam/type_getter/edge_label.h"
#include "include/gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/graph_parameter_getter.h"

#include "include/gundam/tool/connected_component.h"

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

  using LegalAttributeValueSetType
      = std::map<std::pair<std::string,  // value_str
                           enum GUNDAM::BasicDataType>, // value_type
                 int>;

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
    // calculate edge type statistic
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
    // connected_edge_type_
    for (auto vertex_it = graph.VertexBegin();
             !vertex_it.IsDone();
              vertex_it++) {
      this->max_vertex_id_ = this->max_vertex_id_ > vertex_it->id() ?
                             this->max_vertex_id_ : vertex_it->id();
      this->vertex_label_counter_[vertex_it->label()]++;
      std::set<EdgeTypeType> out_edge_type_set;
      for (auto out_edge_it = vertex_it->OutEdgeBegin();
               !out_edge_it.IsDone();
                out_edge_it++) {
        out_edge_type_set.emplace(std::tuple(out_edge_it->src_handle()->label(),
                                             out_edge_it->label(),
                                             out_edge_it->dst_handle()->label()));
      }
      std::set<EdgeTypeType> in_edge_type_set;
      for (auto in_edge_it = vertex_it->InEdgeBegin();
               !in_edge_it.IsDone();
                in_edge_it++) {
        in_edge_type_set.emplace(std::tuple(in_edge_it->src_handle()->label(),
                                            in_edge_it->label(),
                                            in_edge_it->dst_handle()->label()));
      }
      for (const auto& out_edge_type : out_edge_type_set) {
        for (const auto& in_edge_type : in_edge_type_set) {
          this->connected_edge_type_[std::pair(out_edge_type, false)]
                                    [std::pair( in_edge_type,  true)]++;
        }
        for (const auto& out_edge_type_2 : out_edge_type_set) {
          this->connected_edge_type_[std::pair(out_edge_type  , false)]
                                    [std::pair(out_edge_type_2, false)]++;
        }
      }
      for (const auto& in_edge_type : in_edge_type_set) {
        for (const auto& in_edge_type_2 : in_edge_type_set) {
          this->connected_edge_type_[std::pair( in_edge_type,    true)]
                                    [std::pair( in_edge_type_2,  true)]++;
        }
        for (const auto& out_edge_type : out_edge_type_set) {
          this->connected_edge_type_[std::pair( in_edge_type,  true)]
                                    [std::pair(out_edge_type, false)]++;
        }
      }
    }
    // connected_edge_type_
    if constexpr (GraphParameter<GraphType>::vertex_has_attribute) {
      this->AddVertexAttr(graph);
    }
    return;
  }

  template <typename GraphPatternType>
  inline size_t SimplePatternHomoCounter(const GraphPatternType& simple_pattern) const {
    using VertexHandleType      = typename VertexHandle<      GraphPatternType>::type;
    using VertexConstHandleType = typename VertexHandle<const GraphPatternType>::type;
    if (simple_pattern.CountEdge() > 2) {
      return 0;
    }
    auto cc_set = ConnectedComponent(simple_pattern);
    size_t homo_counter = 1;
    for (const auto& cc : cc_set) {
      size_t current_cc_counter = 0;
      switch (cc.CountEdge()) {
       case 0:
        assert(cc.CountVertex() == 1);
        current_cc_counter = this->VertexCounter(
                                cc.VertexBegin()->label());
        break;
       case 1:
        assert(cc.CountVertex() == 2);
        for (auto vertex_it = cc.VertexBegin();
                 !vertex_it.IsDone();
                  vertex_it++) {
          for (auto out_edge_it = vertex_it->OutEdgeBegin();
                   !out_edge_it.IsDone();
                    out_edge_it++) {
            current_cc_counter = this->EdgeTypeCounter(out_edge_it->src_handle()->label(),
                                                       out_edge_it->label(),
                                                       out_edge_it->dst_handle()->label());
          }
        }
        break;
       case 2: {
        assert(cc.CountVertex() == 3);
        VertexConstHandleType centrel_vertex = VertexConstHandleType();
        for (auto vertex_it = cc.VertexBegin();
                 !vertex_it.IsDone();
                  vertex_it++) {
          assert((vertex_it->CountInEdge() + vertex_it->CountOutEdge() == 1)
              || (vertex_it->CountInEdge() + vertex_it->CountOutEdge() == 2));
          if (vertex_it->CountInEdge() + vertex_it->CountOutEdge() < 2) {
            continue;
          }
          centrel_vertex = vertex_it;
        }
        assert(centrel_vertex);
        std::vector<std::pair<EdgeTypeType, bool>> edges;
        edges.reserve(2);
        for (auto out_edge_it = centrel_vertex->OutEdgeBegin();
                 !out_edge_it.IsDone();
                  out_edge_it++) {
          edges.emplace_back(EdgeTypeType(out_edge_it->src_handle()->label(),
                                          out_edge_it->label(),
                                          out_edge_it->dst_handle()->label()), false);
        }
        for (auto in_edge_it = centrel_vertex->InEdgeBegin();
                 !in_edge_it.IsDone();
                  in_edge_it++) {
          edges.emplace_back(EdgeTypeType(in_edge_it->src_handle()->label(),
                                          in_edge_it->label(),
                                          in_edge_it->dst_handle()->label()), true);
        }
        current_cc_counter = this->EdgeTypeConnectedTo(edges[0].first, edges[0].second, 
                                                       edges[1].first, edges[1].second);
        break;
       }
       default:
        assert(false);
        return 0;
      }
      homo_counter *= current_cc_counter;
    }
    return homo_counter;
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
    return this->EdgeTypeCounter(edge_handle->src_handle()->label(),
                                 edge_handle->label(),
                                 edge_handle->dst_handle()->label());
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
    return this->AvgOutDegree(edge_handle->src_handle()->label(),
                              edge_handle->label(),
                              edge_handle->dst_handle()->label());
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
    return this->AvgInDegree(edge_handle->src_handle()->label(),
                             edge_handle->label(),
                             edge_handle->dst_handle()->label());
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

  inline size_t legal_attr_value_counter(
         const VertexLabelType& vertex_label,
         const VertexAttributeKeyType& attr_key) const {
    auto legal_attr_set_it  = this->legal_attr_set_.find(vertex_label);
    if ( legal_attr_set_it == this->legal_attr_set_.end()) {
      return 0;
    }
    auto value_set_it  = legal_attr_set_it->second.find(attr_key);
    if ( value_set_it == legal_attr_set_it->second.end()) {
      return 0;
    }
    return value_set_it->second;
  }

  inline bool has_legal_attr_value(
         const VertexLabelType& vertex_label,
         const VertexAttributeKeyType& attr_key) const {
    return this->legal_attr_value_counter(vertex_label, attr_key) > 0;
  }

  inline const LegalAttributeValueSetType& legal_attr_value_set(
         const VertexLabelType& vertex_label,
         const VertexAttributeKeyType& attr_key) const {
     auto  legal_attr_set_it  = this->legal_attr_set_.find(vertex_label);
    assert(legal_attr_set_it != this->legal_attr_set_.end());

     auto  value_set_it  = legal_attr_set_it->second.find(vertex_label);
    assert(value_set_it != legal_attr_set_it->second.end());

    return *value_set_it;
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

  inline std::string ToString() const {
    std::string output_info;

    output_info += "####################################################################\n" ;
    output_info += "# vertex label statistic collection, form: <vertex_label>, counter #\n" ;
    output_info += "####################################################################\n" ;
    for (const auto& [vertex_label, counter] : this->vertex_label_counter()) {
      output_info += "<"  + std::to_string(vertex_label)
                   + "> " + std::to_string(counter)
                   + "\n";
    }

    output_info += "################################################################\n" ;
    output_info += "# edge label statistic collection, form: <edge_label>, counter #\n" ;
    output_info += "################################################################\n" ;
    for (const auto& [edge_label, counter] : this->edge_label_counter()) {
      output_info += "<"  + std::to_string(edge_label)
                   + "> " + std::to_string(counter)
                   + "\n";
    }

    output_info += "######################################################################################\n" ;
    output_info += "# edge types statistic collection, form: <src_label, edge_label, dst_label>, counter #\n" ;
    output_info += "######################################################################################\n" ;
    for (auto edge_type_counter_it  = this->edge_type_counter_begin();
              edge_type_counter_it != this->edge_type_counter_end();
              edge_type_counter_it++) {
      output_info += "<"  + std::to_string(std::get<0>(edge_type_counter_it->first))
                   + ","  + std::to_string(std::get<1>(edge_type_counter_it->first))
                   + ","  + std::to_string(std::get<2>(edge_type_counter_it->first)) 
                   + "> " + std::to_string(edge_type_counter_it->second)
                   + "\n";
    }

    return output_info;
  }

  inline size_t EdgeTypeConnectedTo(
        const EdgeTypeType& edge_type_0, bool edge_0_is_input, // for the central vertex
        const EdgeTypeType& edge_type_1, bool edge_1_is_input  // for the central vertex
      ) const {
    assert( this->connected_edge_type_.find(std::pair(edge_type_0, edge_0_is_input))
         != this->connected_edge_type_.end() );
    #ifndef NDEBUG
    if ( edge_0_is_input &&  edge_1_is_input) {
      assert(std::get<2>(edge_type_0) == std::get<2>(edge_type_1));
    }
    if ( edge_0_is_input && !edge_1_is_input) {
      assert(std::get<2>(edge_type_0) == std::get<0>(edge_type_1));
    }
    if (!edge_0_is_input &&  edge_1_is_input) {
      assert(std::get<0>(edge_type_0) == std::get<2>(edge_type_1));
    }
    if (!edge_0_is_input && !edge_1_is_input) {
      assert(std::get<0>(edge_type_0) == std::get<0>(edge_type_1));
    }
    #endif // NDEBUG
    const auto&  connected_edge_type_set 
         = this->connected_edge_type_.find(std::pair(edge_type_0, edge_0_is_input))->second;
    auto connected_edge_type_set_it
       = connected_edge_type_set.find(std::pair(edge_type_1, edge_1_is_input));
    if ( connected_edge_type_set_it  
      == connected_edge_type_set.end() ) {
      return 0;
    }
    return connected_edge_type_set_it->second;
  }

 private:
  // basic statistics
  std::map<VertexLabelType, size_t> vertex_label_counter_;
  std::map<  EdgeLabelType, size_t>   edge_label_counter_;
  
  std::map<EdgeTypeType, size_t> edge_type_counter_;

  std::map<std::pair<EdgeTypeType, bool /* is input */>, 
  std::map<std::pair<EdgeTypeType, bool /* is input */>,
           size_t>> connected_edge_type_;

  VertexIdType max_vertex_id_;
    EdgeIdType   max_edge_id_;

  std::map<VertexLabelType,
  std::map<VertexAttributeKeyType,
            LegalAttributeValueSetType>> legal_attr_set_;
};

};


#endif // _GUNDAM_GRAPH_STATISTICS_GRAPH_BASIC_STATISTICS_H