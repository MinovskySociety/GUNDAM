#ifndef _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H
#define _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H

#include "tool/topological/star/is_star.h"
#include "gundam/tool/topological/path/is_path.h"
#include "gundam/tool/connected_component.h"
#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/vertex_handle.h"

namespace GUNDAM {
/* ######################################################################## *
 * ##  wenzhi:                                                           ## *
 * ##    move dfs code from                                              ## *
 * ##    grape-gundam/src/apps/gar_discover/gar_discover/gar_dfs_code.h  ## *
 * ######################################################################## */

// modify from 
//    template <typename GraphPatternType>
//    class GARDFSCode;
template <class GraphPatternType>
class DfsCodeElement {
 public:
  using VertexLabelType = typename GraphPatternType::VertexType::LabelType;
  using    VertexIDType = typename GraphPatternType::VertexType::   IDType;
  using   EdgeLabelType = typename GraphPatternType::  EdgeType::LabelType;

 public:
  DfsCodeElement(int src_vertex_script, 
              int dst_vertex_script,
              VertexLabelType  src_label, 
                EdgeLabelType edge_label,
              VertexLabelType  dst_label)
      : src_vertex_script_(src_vertex_script),
        dst_vertex_script_(dst_vertex_script),
        src_label_( src_label),
       edge_label_(edge_label),
        dst_label_( dst_label) {
    return;
  }

  
  bool operator<(const DfsCodeElement<GraphPatternType> &b) const {
    if (this->src_vertex_script_ != b.src_vertex_script_)
      return this->src_vertex_script_ < b.src_vertex_script_;
    if (this->dst_vertex_script_ != b.dst_vertex_script_)
      return this->dst_vertex_script_ < b.dst_vertex_script_;
    if (this->src_label_ != b.src_label_)
      return this->src_label_ < b.src_label_;
    if (this->edge_label_ != b.edge_label_)
      return this->edge_label_ < b.edge_label_;
    return this->dst_label_ < b.dst_label_;
  }

  bool operator==(const DfsCodeElement<GraphPatternType> &b) const {
    if ((this->src_vertex_script_ == b.src_vertex_script_) &&
        (this->dst_vertex_script_ == b.dst_vertex_script_) &&
        (this->src_label_ == b.src_label_) &&
        (this->edge_label_ == b.edge_label_) &&
        (this->dst_label_ == b.dst_label_))
      return true;
    return false;
  }

 private:
  int src_vertex_script_, 
      dst_vertex_script_;
  VertexLabelType  src_label_;
  VertexLabelType  dst_label_;
    EdgeLabelType edge_label_;
};


template <class GraphPatternType>
class DfsCode {
 private:
  using DfsCodeElementType
      = DfsCodeElement<GraphPatternType>;

 public:
  inline bool operator < () const {
    
  }

 private:
  std::vector<DfsCodeElementType> dfs_code_element_set_;
};

template <bool is_out, class EdgePtr>
class EdgeCMP {
 public:
  bool operator()(std::pair<int32_t, EdgePtr> a,
                  std::pair<int32_t, EdgePtr> b) {
    if (a.first != b.first) 
      return a.first < b.first;
    auto a_dst_handle 
       = is_out ? a.second->dst_handle() 
                : a.second->src_handle();
    auto b_dst_handle 
       = is_out ? b.second->dst_handle() 
                : b.second->src_handle();
    if (is_out) {
      if (a.second->label() 
       != b.second->label())
        return a.second->label() 
             < b.second->label();
      if (a_dst_handle->label() != b_dst_handle->label())
        return a_dst_handle->label() < b_dst_handle->label();
    } else {
      if (a_dst_handle->label() != b_dst_handle->label())
        return a_dst_handle->label() < b_dst_handle->label();
      if (a.second->label() != b.second->label())
        return a.second->label() < b.second->label();
    }
    if (a_dst_handle->CountOutVertex() != b_dst_handle->CountOutVertex())
      return a_dst_handle->CountOutVertex() < b_dst_handle->CountOutVertex();
    if (a_dst_handle->CountInVertex() != b_dst_handle->CountInVertex())
      return a_dst_handle->CountInVertex() < b_dst_handle->CountInVertex();
    if (a_dst_handle->CountOutEdge() != b_dst_handle->CountOutEdge())
      return a_dst_handle->CountOutEdge() < b_dst_handle->CountOutEdge();
    if (a_dst_handle->CountInEdge() != b_dst_handle->CountInEdge())
      return a_dst_handle->CountInEdge() < b_dst_handle->CountInEdge();
    return a.second->id() < b.second->id();
  }
};

template <typename GraphPatternType, 
          class VertexPtr, 
          class DFSCodeContainer>
void DFS(VertexPtr now_vertex_handle, 
         std::set<VertexPtr> &used_vertex,
         std::set<typename GraphPatternType
                                 ::EdgeType
                                   ::IDType> &used_edge, 
         int32_t &max_script,
         std::map<VertexPtr, int32_t> &vertex_to_script,
         DFSCodeContainer &dfs_code_container) {
  using   EdgeHandleType = typename GUNDAM::  EdgeHandle<GraphPatternType>::type;
  using VertexHandleType = typename GUNDAM::VertexHandle<GraphPatternType>::type;
  using ComPareEdgeData = std::pair<int, EdgeHandleType>;

  used_vertex.insert(now_vertex_handle);
  constexpr int32_t script_max = INT32_MAX;
  constexpr bool out = true;
  constexpr bool in = false;
  std::vector<ComPareEdgeData> sort_edge_container;
  for (auto out_edge_it = now_vertex_handle->OutEdgeBegin();
           !out_edge_it.IsDone(); 
            out_edge_it++) {
    EdgeHandleType edge_handle = out_edge_it;
    if (used_edge.count(edge_handle->id())) 
      continue;
    auto it = vertex_to_script.find(out_edge_it->dst_handle());
    int32_t dst_script =
        (it == vertex_to_script.end()) ? script_max : it->second;
    sort_edge_container.emplace_back(std::make_pair(dst_script, edge_handle));
  }

  std::stable_sort(sort_edge_container.begin(), 
                   sort_edge_container. end (),
                   EdgeCMP<out, EdgeHandleType>());
  int32_t now_script = vertex_to_script[now_vertex_handle];
  for (const auto &it : sort_edge_container) {
    if (used_edge.count(it.second->id())) {
      continue;
    }
    VertexHandleType dst_handle = it.second->dst_handle();
    auto find_it = vertex_to_script.find(dst_handle);
    int32_t script =
        (find_it == vertex_to_script.end()) ? ++max_script : find_it->second;
    if (find_it == vertex_to_script.end())
      vertex_to_script.emplace(dst_handle, script);
    dfs_code_container.emplace_back(now_script, 
                                        script, 
                                    now_vertex_handle->label(),
                                            it.second->label(), 
                                           dst_handle->label());
    used_edge.insert(it.second->id());
    if (!used_vertex.count(dst_handle))
      DFS<GraphPatternType>(dst_handle, 
                           used_vertex, 
                             used_edge, 
                            max_script, 
                      vertex_to_script,
                    dfs_code_container);
  }
  sort_edge_container.clear();
  for (auto out_edge_it = now_vertex_handle->InEdgeBegin(); 
           !out_edge_it.IsDone();
            out_edge_it++) {
    EdgeHandleType edge_handle = out_edge_it;
    if (used_edge.count(edge_handle->id())) 
      continue;
    auto it = vertex_to_script.find(out_edge_it->src_handle());
    int32_t dst_script =
        (it == vertex_to_script.end()) ? script_max : it->second;
    sort_edge_container.emplace_back(std::make_pair(dst_script, edge_handle));
  }
  std::stable_sort(sort_edge_container.begin(), 
                   sort_edge_container.end(),
                   EdgeCMP<in, EdgeHandleType>());
  for (const auto &it : sort_edge_container) {
    if (used_edge.count(it.second->id())) 
      continue;
    VertexHandleType src_handle = it.second->src_handle();
    auto find_it = vertex_to_script.find(src_handle);
    int32_t script =
       (find_it == vertex_to_script.end()) ? ++max_script : find_it->second;
    if (find_it == vertex_to_script.end())
      vertex_to_script.emplace(src_handle, script);
    dfs_code_container.emplace_back(script, 
                                now_script, 
                                src_handle->label(),
                                 it.second->label(), 
                         now_vertex_handle->label());
    used_edge.insert(it.second->id());
    if (!used_vertex.count(src_handle))
      DFS<GraphPatternType>(src_handle, 
                           used_vertex, 
                           used_edge, 
                            max_script, 
                         vertex_to_script,
                          dfs_code_container);
  }
  return;
}


// modify from 
//    template <class GraphPatternType, 
//              class DFSCodeContainer>
//    inline void GetGARDFSCode(GraphPatternType& graph_pattern,
//           std::vector<typename GUNDAM::VertexHandle<GraphPatternType>::type>& src_vertex_handle_set,
//                               DFSCodeContainer& dfs_code_container);

// new interface:
// template <class GraphPatternType>
// inline void GetDFSCode(GraphPatternType& graph_pattern,
//   const 
//   typename VertexHandle<GraphPatternType>::type>& src_vertex_handle,
//                 DfsCode<GraphPatternType>& dfs_code_container) {
// }


template <class GraphPatternType, 
          class DFSCodeContainer>
inline void GetDFSCode(GraphPatternType& graph_pattern,
 const std::vector<
 typename VertexHandle<GraphPatternType>::type>& src_vertex_handle_set,
                               DFSCodeContainer& dfs_code_container) {
  using EdgeHandleType = typename EdgeHandle<GraphPatternType>::type;
  using VertexHandleType = typename VertexHandle<GraphPatternType>::type;
  using EdgeIDType     = typename GraphPatternType::EdgeType::IDType;

  std::set<VertexHandleType> used_vertex;
  std::set<EdgeIDType>       used_edge;
  std::map<VertexHandleType, int> vertex_to_script;

  for (auto& src_vertex_handle : src_vertex_handle_set){
    assert(src_vertex_handle);

    int32_t max_script = -1;
    vertex_to_script.insert(std::make_pair(src_vertex_handle,
                                        ++max_script));

    DFS<GraphPatternType>(src_vertex_handle, 
                        used_vertex, 
                        used_edge, 
                          max_script,
                    vertex_to_script, 
                          dfs_code_container);
  }
  return;
}

// // new interface
// // return dfs_code_container
// template <class GraphPatternType>
// inline std::vector<DfsCode<GraphPatternType>> // ok to return vector after C++
//     GetDFSCode(const GraphPatternType& graph_pattern) {
// };

// generate all possible DFS code here, call the above method
template <class GraphPatternType, 
          class DFSCodeContainer>
inline void GetDFSCode(const GraphPatternType& graph_pattern,
                             DFSCodeContainer& dfs_code_container) {
  std::vector<GraphPatternType> decomposed_patterns 
                = ConnectedComponent(graph_pattern);
  for (const auto& pattern : decomposed_patterns) {
    if (IsPath<true>(pattern)) {
      // do some thing for link
      continue;
    }
    if (IsStar<true>(pattern)) {
      // do some thing for star
      continue;
    }
    // do some thing for general pattern
  }
  if (decomposed_patterns.size() == 1) {
    return;
  }
  // do something for dis-connected pattern
  return;
}

};

#endif // _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H
