#ifndef _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H
#define _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H

#include <vector>
#include <map>

#include "gundam/tool/topological/star/is_star.h"
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
template <class GraphPatternType>
class DfsCode;

// modify from 
//    template <typename GraphPatternType>
//    class GARDFSCode;
template <class GraphPatternType>
class DfsCodeElement {
 public:
  using VertexLabelType = typename GraphPatternType::VertexType::LabelType;
  using    VertexIDType = typename GraphPatternType::VertexType::   IDType;
  using   EdgeLabelType = typename GraphPatternType::  EdgeType::LabelType;

  template <class>
  friend class DfsCode;

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
    assert(src_vertex_script != -1);
    assert(dst_vertex_script != -1);
    return;
  }

  DfsCodeElement() : src_vertex_script_(-1),
	             dst_vertex_script_(-1),
		              src_label_(),
		             edge_label_(),
		              dst_label_() {
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
  inline DfsCode<GraphPatternType> Concatenate(const DfsCode<GraphPatternType> &b)  const {
    DfsCodeElementType dummy;
    DfsCode<GraphPatternType> ret_vec;
    ret_vec.dfs_code_element_set_.clear();
    ret_vec.dfs_code_element_set_.insert(ret_vec.dfs_code_element_set_.end(), dfs_code_element_set_.begin(), dfs_code_element_set_.end());
    ret_vec.emplace_back(dummy);
    ret_vec.dfs_code_element_set_.insert(ret_vec.dfs_code_element_set_.end(), b.dfs_code_element_set_.begin(), b.dfs_code_element_set_.end());
    return ret_vec;
  }

  inline bool operator<(const DfsCode<GraphPatternType> &b) const {
    if (this->dfs_code_element_set_.size() != b.dfs_code_element_set_.size()) {
      return this->dfs_code_element_set_.size() < b.dfs_code_element_set_.size();
    }
    for (size_t i = 0; i < this->dfs_code_element_set_.size(); i++) {
      if (!(this->dfs_code_element_set_[i] == b.dfs_code_element_set_[i])) {
        return this->dfs_code_element_set_[i] < b.dfs_code_element_set_[i];
      }
    }
    return false;
  }

  inline bool operator==(const DfsCode<GraphPatternType> &b) const {
    if (this->dfs_code_element_set_.size() != b.dfs_code_element_set_.size()) {
      return false;
    }
    for (size_t i = 0; i < this->dfs_code_element_set_.size(); i++) {
      if (!(this->dfs_code_element_set_[i] == b.dfs_code_element_set_[i])) {
        return false;
      }
    }
    return true;
  }

  template <typename... Args>
  inline void emplace_back(Args&&... p1) {
    dfs_code_element_set_.emplace_back(p1...);
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
          class VertexPtr>
void DFS(VertexPtr now_vertex_handle, 
         std::set<VertexPtr> &used_vertex,
         std::set<typename GraphPatternType
                                 ::EdgeType
                                   ::IDType> &used_edge, 
         int32_t &max_script,
         std::map<VertexPtr, int32_t> &vertex_to_script,
         DfsCode<GraphPatternType> &dfs_code_container) {
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
//    DfsCodeElement tmp_element(script, now_script, src_handle->label(),
//		               it.second->label(),
//			       now_vertex_handle->label());
//    dfs_code_container.push_back(tmp_element);
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


template <class GraphPatternType>
inline void GetDFSCode(GraphPatternType& graph_pattern,
 const std::vector<
 typename VertexHandle<GraphPatternType>::type>& src_vertex_handle_set,
                               DfsCode<GraphPatternType>& dfs_code_container) {
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

//template <class GraphPatternType>
//inline std::vector<DfsCode<GraphPatternType>>
//      GetDFSCode(const GraphPatternType& graph_pattern) {
//   std::vector<GraphPatternType> decomposed_patterns 
//                = ConnectedComponent(graph_pattern);
//
//        
//}


template <class GraphPatternType>
inline std::vector<DfsCode<GraphPatternType>> ConcatenateDFSCodeFromOneSide(
             const std::vector<DfsCode<GraphPatternType>>& vec1,
	     const std::vector<DfsCode<GraphPatternType>>& vec2) {
	std::vector<DfsCode<GraphPatternType>> ret_vec;
	
	for (size_t i = 0; i < vec1.size(); i++) {
	  for (size_t j = 0; j < vec2.size(); j++) {
	    DfsCode<GraphPatternType> concatenate_code = vec1[i].Concatenate(vec2[j]);
            ret_vec.push_back(concatenate_code);
	  }
	}
  return ret_vec;
}

template <class GraphPatternType>
inline std::vector<DfsCode<GraphPatternType>> ConcatenateDFSCodeFromBothSides(
             const std::vector<DfsCode<GraphPatternType>>& vec1,
	     const std::vector<DfsCode<GraphPatternType>>& vec2) {
	std::vector<DfsCode<GraphPatternType>> ret_vec;
	
	for (size_t i = 0; i < vec1.size(); i++) {
	  for (size_t j = 0; j < vec2.size(); j++) {
	    DfsCode<GraphPatternType> concatenate_code = vec1[i].Concatenate(vec2[j]);
            ret_vec.push_back(concatenate_code);
	  }
	}

	for (size_t i = 0; i < vec2.size(); i++) {
	  for (size_t j = 0; j < vec1.size(); j++) {
	    DfsCode<GraphPatternType> concatenate_code = vec2[i].Concatenate(vec1[j]);
            ret_vec.push_back(concatenate_code);
	  }
	}
  return ret_vec;
}



// generate all possible DFS code here, call the above method
template <class GraphPatternType>
inline std::vector<DfsCode<GraphPatternType>> GetDFSCode(
		const GraphPatternType& graph_pattern) {

  using GraphPatternVertexHandle = typename VertexHandle<GraphPatternType>::type;

  std::vector<GraphPatternType> decomposed_patterns 
                = ConnectedComponent(graph_pattern);
  std::sort(decomposed_patterns.begin(), decomposed_patterns.end(),
            [](const GraphPatternType& g1, const GraphPatternType& g2)
	    { return g1.CountVertex() < g2.CountVertex()
	             || g1.CountEdge() < g2.CountEdge();});

  // can be optimized: it just supports GCR and GAR only now
  assert(decomposed_patterns.size() <= 2);
  
  std::vector<std::vector<DfsCode<GraphPatternType>>> dfs_code_vec(decomposed_patterns.size());

  
//  for (const auto& pattern : decomposed_patterns) {
    for (size_t i = 0; i < decomposed_patterns.size(); i++) {
    auto& pattern = decomposed_patterns[i];

    if (IsPath<true>(pattern)) {
      auto [link_src_handle, link_dst_handle] = PathEndPoints<true>(pattern);
      
      DfsCode<GraphPatternType> src_dfs_code;
      std::vector<GraphPatternVertexHandle> src_handle_set{link_src_handle};
      GetDFSCode(pattern, src_handle_set, src_dfs_code);
      dfs_code_vec[i].emplace_back(src_dfs_code);
      
      DfsCode<GraphPatternType> dst_dfs_code;
      std::vector<GraphPatternVertexHandle> dst_handle_set{link_dst_handle};
      GetDFSCode(pattern, dst_handle_set, dst_dfs_code);
      dfs_code_vec[i].emplace_back(dst_dfs_code);

      continue;
    }
    if (IsStar<true>(pattern)) {
      // do some thing for star
    }

    for (auto vertex_cit = pattern.VertexBegin();
	      !vertex_cit.IsDone();
	      vertex_cit++) {
      DfsCode<GraphPatternType> v_dfs_code;
      std::vector<GraphPatternVertexHandle> v_handle_set{vertex_cit};
      GetDFSCode(pattern, v_handle_set, v_dfs_code);
      dfs_code_vec[i].emplace_back(v_dfs_code);
    }
  }

  assert (dfs_code_vec.size() <= 2);
  if (decomposed_patterns.size() == 1) {
    return dfs_code_vec[0];
  }

  std::vector<DfsCode<GraphPatternType>> ret_dfs_code_vec;

  if (decomposed_patterns[0].CountVertex() == decomposed_patterns[1].CountVertex()
      && decomposed_patterns[0].CountEdge() == decomposed_patterns[1].CountEdge()) {
    ret_dfs_code_vec = ConcatenateDFSCodeFromBothSides(dfs_code_vec[0], dfs_code_vec[1]);
  } else {
    ret_dfs_code_vec = ConcatenateDFSCodeFromOneSide(dfs_code_vec[0], dfs_code_vec[1]);
  }
  // do something for dis-connected pattern
  return ret_dfs_code_vec;
}

};

#endif // _GUNDAM_TOOL_DEDUPLICATE_PATTERNS_DFS_CODE_H
