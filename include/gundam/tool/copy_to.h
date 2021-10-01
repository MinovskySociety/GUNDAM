#ifndef _GUNDAM_TOOL_COPY_TO_H
#define _GUNDAM_TOOL_COPY_TO_H

#include "include/gundam/tool/same_pattern.h"

namespace GUNDAM {

template <typename GraphType0,
          typename GraphType1>
inline bool CopyTo(const GraphType0& graph0,
                         GraphType1& graph1) {
  static_assert(GraphParameter<GraphType1>::vertex_has_attribute
             ||!GraphParameter<GraphType0>::vertex_has_attribute,
               "GraphType0 has vertex attribute but GraphType1 does not have");
  static_assert(GraphParameter<GraphType1>::vertex_has_attribute
             ||!GraphParameter<GraphType0>::vertex_has_attribute,
               "GraphType0 has edge attribute but GraphType1 does not have");

  using Graph1EdgeHandleType = typename EdgeHandle<GraphType1>::type;

  graph1.Clear();
  for (auto vertex_it = graph0.VertexBegin(); 
           !vertex_it.IsDone(); 
            vertex_it++){
    // <VertexPtr, bool>
    auto vertex_ret = graph1.AddVertex(vertex_it->id(), 
                                       vertex_it->label());
    assert(vertex_ret.second);
    if constexpr (GraphParameter<GraphType0>::vertex_has_attribute) {
      for (auto attr_it = vertex_it->AttributeBegin();
               !attr_it.IsDone();
                attr_it++){
        switch (attr_it->value_type()) {
         case BasicDataType::kTypeString:
          vertex_ret.first->AddAttribute(
                  attr_it->key(),
                  attr_it->template const_value<std::string>());
          continue;
         case BasicDataType::kTypeInt:
          vertex_ret.first->AddAttribute(
                  attr_it->key(),
                  attr_it->template const_value<int>());
          continue;
         case BasicDataType::kTypeInt64:
          vertex_ret.first->AddAttribute(
                  attr_it->key(),
                  attr_it->template const_value<int64_t>());
          continue;
         case BasicDataType::kTypeFloat:
          vertex_ret.first->AddAttribute(
                  attr_it->key(),
                  attr_it->template const_value<float>());
          continue;
         case BasicDataType::kTypeDouble:
          vertex_ret.first->AddAttribute(
                  attr_it->key(),
                  attr_it->template const_value<double>());
          continue;
         case BasicDataType::kTypeDateTime:
          vertex_ret.first->AddAttribute(
                  attr_it->key(),
                  attr_it->template const_value<DateTime>());
          continue;
         default:
          /// unknown data type
          assert(false);
        }
      }
    }
  }

  for (auto vertex_it = graph0.VertexBegin(); 
           !vertex_it.IsDone(); 
            vertex_it++){
    auto vertex_ptr = graph1.FindVertex(vertex_it->id());
    for (auto edge_it = vertex_it->OutEdgeBegin(); 
             !edge_it.IsDone(); 
              edge_it++) {
      assert(vertex_ptr->id() == edge_it->const_src_handle()->id());
      Graph1EdgeHandleType edge_handle = Graph1EdgeHandleType();
      assert(!edge_handle);
      if constexpr (GraphParameter<GraphType1>::vertex_level_add_edge) {
        auto dst_ptr = graph1.FindVertex(edge_it->const_dst_handle()->id());
        bool edge_ret = false;
        /// <EdgePtr, bool>
        std::tie(edge_handle,
                 edge_ret) = vertex_ptr->AddEdge(dst_ptr,
                                                 edge_it->label(), 
                                                 edge_it->id());
        assert(edge_handle);
        assert(edge_ret);
      }
      else {
        bool edge_ret = false;
        // does not have vertex level add edge method
        std::tie(edge_handle,
                 edge_ret) = graph1.AddEdge(edge_it->src_handle()->id(),
                                            edge_it->dst_handle()->id(),
                                            edge_it->label(),
                                            edge_it->id());
        assert(edge_handle);
        assert(edge_ret);
      }
      if constexpr (GraphParameter<GraphType0>::edge_has_attribute) {
        for (auto attr_it = edge_it->AttributeBegin();
                 !attr_it.IsDone(); 
                  attr_it++){
          switch(attr_it->value_type()) {
           case BasicDataType::kTypeString:
            edge_handle->AddAttribute(
                attr_it->key(),
                attr_it->template const_value<std::string>());
            continue;
           case BasicDataType::kTypeInt:
            edge_handle->AddAttribute(
                attr_it->key(),
                attr_it->template const_value<int>());
            continue;
           case BasicDataType::kTypeInt64:
            edge_handle->AddAttribute(
                attr_it->key(),
                attr_it->template const_value<int64_t>());
            continue;
           case BasicDataType::kTypeFloat:
            edge_handle->AddAttribute(
                attr_it->key(),
                attr_it->template const_value<float>());
            continue;
           case BasicDataType::kTypeDouble:
            edge_handle->AddAttribute(
                attr_it->key(),
                attr_it->template const_value<double>());
            continue;
           case BasicDataType::kTypeDateTime:
            edge_handle->AddAttribute(
                attr_it->key(),
                attr_it->template const_value<DateTime>());
            continue;
           default:
            /// unknown data type
            assert(false);
          }
        }
      }
    }
  }
  assert(SamePattern(graph0, graph1));
  return true;
}

};

#endif // _GUNDAM_TOOL_COPY_TO_H