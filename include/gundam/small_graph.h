#ifndef _SMALL_GRAPH_H
#define _SMALL_GRAPH_H

#include <algorithm>
#include <iterator>
#include <vector>

#include "gundam/container2.h"
#include "gundam/iterator2.h"
#include "gundam/pointer.h"

namespace GUNDAM {

template <class VertexIDType, class VertexLabelType, class EdgeIDType,
          class EdgeLabelType>
class SmallGraph {
 private:
  class VertexData {
   public:
    VertexData(const VertexLabelType &label) : label_(label){};

    VertexLabelType label_;
    SortedVectorSet<EdgeIDType> out_edges_;
    SortedVectorSet<EdgeIDType> in_edges_;
  };

  class EdgeData {
   public:
    EdgeData(const EdgeLabelType &label, const VertexIDType &src,
             const VertexIDType &dst)
        : label_(label), src_(src), dst_(dst){};

    EdgeLabelType label_;
    VertexIDType src_;
    VertexIDType dst_;
  };   
 
  using VertexContainer = SortedVectorDict<VertexIDType, VertexData>;
  using EdgeContainer = SortedVectorDict<EdgeIDType, EdgeData>;

  using VertexContent = std::pair<VertexIDType, VertexData>;
  using EdgeContent = std::pair<EdgeIDType, EdgeData>;

  VertexContainer vertices_;
  EdgeContainer edges_;
  
  template <bool is_const>
  class _Vertex;

  template <bool is_const>
  class _Edge;

  using Vertex = _Vertex<false>;

  using ConstVertex = _Vertex<true>;

  using Edge = _Edge<false>;

  using ConstEdge = _Edge<true>;

 public:
  using VertexPtr = GPointer<Vertex, false>;

  using VertexConstPtr = GPointer<ConstVertex, true>;

  using EdgePtr = GPointer<Edge, false>;

  using EdgeConstPtr = GPointer<ConstEdge, true>;

  using VertexIterator =
      GIterator2<false, SmallGraph, typename VertexContainer::iterator, Vertex,
                VertexPtr>;

  using VertexConstIterator =
      GIterator2<true, SmallGraph, typename VertexContainer::const_iterator,
                ConstVertex, VertexConstPtr>;

  using EdgeIterator =
      GIterator2<false, SmallGraph, typename EdgeContainer::iterator, Edge,
                EdgePtr>;

  using EdgeConstIterator =
      GIterator2<true, SmallGraph, typename EdgeContainer::const_iterator,
                ConstEdge, EdgeConstPtr>;

 private:
  template <bool is_const>
  class _Vertex {
   public:
    using GraphType = SmallGraph;

    using IDType = VertexIDType;

    using LabelType = VertexLabelType;

    using EdgeIterator =
        GIterator2<false, SmallGraph,
                  typename SortedVectorSet<EdgeIDType>::iterator, Edge,
                  EdgePtr>;

    using EdgeConstIterator =
        GIterator2<true, SmallGraph,
                  typename SortedVectorSet<EdgeIDType>::const_iterator,
                  ConstEdge, EdgeConstPtr>;

   private:
    friend GraphType;
    friend typename GraphType::VertexPtr;
    friend typename GraphType::VertexConstPtr;
    friend typename GraphType::VertexIterator;
    friend typename GraphType::VertexConstIterator;
    friend typename GraphType::EdgePtr;
    friend typename GraphType::EdgeConstPtr;
    friend typename GraphType::EdgeIterator;
    friend typename GraphType::EdgeConstIterator;

    _Vertex() = default;

    _Vertex(typename std::conditional<is_const, const GraphType *,
                                      GraphType *>::type graph,
            const VertexContent &content)
        : graph_(graph), id_(content.first), label_(content.second.label_) {}

    _Vertex(typename std::conditional<is_const, const GraphType *,
                                      GraphType *>::type graph,
            const VertexIDType &id)
        : graph_(graph), id_(id) {
      assert(graph_);
      const auto &data = graph_->vertices_.Find(id_)->second;
      label_ = data.label_;
    }

   public:
    const IDType &id() const { return id_; }

    const VertexLabelType &label() const { return label_; }

    size_t CountOutEdge() const {
      assert(graph_);
      const auto &data = graph_->vertices_.Find(id_)->second;
      return data.out_edges_.Count();
    }

    size_t CountInEdge() const {
      assert(graph_);
      const auto &data = graph_->vertices_.Find(id_)->second;
      return data.in_edges_.Count();
    }

    EdgeIterator OutEdgeBegin() {
      assert(graph_);
      auto &data = graph_->vertices_.Find(id_)->second;
      return EdgeIterator(graph_, data.out_edges_.begin(),
                          data.out_edges_.end());
    }

    EdgeConstIterator OutEdgeCBegin() const {
      assert(graph_);
      const auto &data = graph_->vertices_.Find(id_)->second;
      return EdgeConstIterator(graph_, data.out_edges_.cbegin(),
                               data.out_edges_.cend());
    }

    EdgeIterator InEdgeBegin() {
      assert(graph_);
      auto &data = graph_->vertices_.Find(id_)->second;
      return EdgeIterator(graph_, data.in_edges_.begin(), data.in_edges_.end());
    }

    EdgeConstIterator InEdgeCBegin() const {
      assert(graph_);
      const auto &data = graph_->vertices_.Find(id_)->second;
      return EdgeConstIterator(graph_, data.in_edges_.cbegin(),
                               data.in_edges_.cend());
    }

    bool operator==(const _Vertex &b) const {
      if (!graph_) {
        return !b.graph_;
      } else if (!b.graph_) {
        return false;
      } else {
        return id_ == b.id_;
      }
    }

    bool operator<(const _Vertex &b) const {
      if (!graph_ || !b.graph_) {
        return false;
      } else {
        return id_ < b.id_;
      }
    }

   private:
    typename std::conditional<is_const, const GraphType *, GraphType *>::type
        graph_;
    VertexIDType id_;
    VertexLabelType label_;
  };

  template <bool is_const>
  class _Edge {
   public:
    using GraphType = SmallGraph;

    using IDType = EdgeIDType;

    using LabelType = EdgeLabelType;

   private:
    friend GraphType;
    friend typename GraphType::VertexPtr;
    friend typename GraphType::VertexConstPtr;
    friend typename GraphType::VertexIterator;
    friend typename GraphType::VertexConstIterator;
    friend typename GraphType::EdgePtr;
    friend typename GraphType::EdgeConstPtr;
    friend typename GraphType::EdgeIterator;
    friend typename GraphType::EdgeConstIterator;
    friend typename GraphType::Vertex::EdgeIterator;
    friend typename GraphType::Vertex::EdgeConstIterator;

    _Edge() = default;

    _Edge(typename std::conditional<is_const, const GraphType *,
                                    GraphType *>::type graph,
          const EdgeContent &content)
        : graph_(graph),
          id_(content.first),
          label_(content.second.label_),
          src_(content.second.src_),
          dst_(content.second.dst_) {}

    _Edge(typename std::conditional<is_const, const GraphType *,
                                    GraphType *>::type graph,
          const EdgeIDType &id)
        : graph_(graph), id_(id) {
      assert(graph_);
      const auto &data = graph_->edges_.Find(id_)->second;
      label_ = data.label_;
      src_ = data.src_;
      dst_ = data.dst_;
    }

   public:
    const EdgeIDType &id() const { return id_; }

    const EdgeLabelType &label() const { return label_; }

    const VertexIDType &src_id() const { return src_; }

    const VertexIDType &dst_id() const { return dst_; }

    VertexPtr src_ptr() { return VertexPtr(Vertex(graph_, src_)); }

    VertexConstPtr src_ptr() const {
      return VertexConstPtr(ConstVertex(graph_, src_));
    }

    VertexConstPtr const_src_ptr() const {
      return VertexConstPtr(ConstVertex(graph_, src_));
    }

    VertexPtr dst_ptr() { return VertexPtr(Vertex(graph_, dst_)); }

    VertexConstPtr dst_ptr() const {
      return VertexConstPtr(ConstVertex(graph_, dst_));
    }

    VertexConstPtr const_dst_ptr() const {
      return VertexConstPtr(ConstVertex(graph_, dst_));
    }

    bool operator==(const _Edge &b) const {
      if (graph_) {
        if (b.graph_) {
          return id_ == b.id_;
        } else {
          return false;
        }
      } else {
        if (b.graph_) {
          return false;
        } else {
          return true;
        }
      }
    }

   private:
    typename std::conditional<is_const, const GraphType *, GraphType *>::type
        graph_;
    EdgeIDType id_;
    EdgeLabelType label_;
    VertexIDType src_;
    VertexIDType dst_;
  };

 public:
  using VertexType = Vertex;

  using EdgeType = Edge;

  static constexpr bool vertex_has_attribute = false;

  static constexpr bool edge_has_attribute = false;

  static constexpr bool is_mutable = true;

  SmallGraph() = default;

  SmallGraph(const SmallGraph &other) {
    for (auto v_it = other.VertexCBegin(); !v_it.IsDone(); ++v_it) {
      AddVertex(v_it->id(), v_it->label());
    }
    for (auto e_it = other.EdgeCBegin(); !e_it.IsDone(); ++e_it) {
      AddEdge(e_it->const_src_ptr()->id(), e_it->const_dst_ptr()->id(),
              e_it->label(), e_it->id());
    }
  }

  SmallGraph(SmallGraph &&) = default;

  SmallGraph &operator=(const SmallGraph &other) = default;

  SmallGraph &operator=(SmallGraph &&) = default;

  ~SmallGraph() = default;

  size_t CountVertex() const { return vertices_.Count(); }

  std::pair<VertexPtr, bool> AddVertex(const VertexIDType &id,
                                       const VertexLabelType &label) {
    typename VertexContainer::iterator it;
    bool res;
    std::tie(it, res) = vertices_.Insert(id, label);

    return std::make_pair(VertexPtr(Vertex(this, *it)), res);
  }

  VertexPtr FindVertex(const VertexIDType &id) {
    auto it = vertices_.Find(id);
    if (it == vertices_.end()) return nullptr;
    return VertexPtr(Vertex(this, *it));
  }

  VertexConstPtr FindConstVertex(const VertexIDType &id) const {
    auto it = vertices_.Find(id);
    if (it == vertices_.end()) return nullptr;
    return VertexConstPtr(ConstVertex(this, *it));
  }

  VertexIterator VertexBegin() {
    return VertexIterator(this, vertices_.begin(), vertices_.end());
  }

  VertexConstIterator VertexCBegin() const {
    return VertexConstIterator(this, vertices_.cbegin(), vertices_.cend());
  }

  // VertexIterator VertexBegin(const VertexLabelType &label) {
  //  auto it = vertex_labels_.find(label);
  //  if (it == vertex_labels_.end()) {
  //    return VertexIterator();
  //  }
  //  return VertexIterator(it->second.begin(), it->second.end());
  //};

  // VertexConstIterator VertexCBegin(
  //    const VertexLabelType &label) const {
  //  auto it = vertex_labels_.find(label);
  //  if (it == vertex_labels_.cend()) {
  //    return VertexConstIterator();
  //  }
  //  return VertexConstIterator(it->second.cbegin(), it->second.cend());
  //}

  bool EraseVertex(const VertexIDType &id) {
    auto it = vertices_.Find(id);
    if (it == vertices_.end()) return false;

    auto &v_data = it->second;

    std::vector<VertexIDType> edges;
    std::merge(v_data.out_edges_.begin(), v_data.out_edges_.end(),
               v_data.in_edges_.begin(), v_data.in_edges_.end(),
               std::back_inserter(edges));

    for (auto it_r = edges.rbegin(); it_r != edges.rend(); ++it_r) {
      if (it_r != edges.rbegin() && *it_r == *(it_r - 1)) continue;
      edges_.Erase(*it_r);
    }

    vertices_.Erase(it);

    return true;
  }

  size_t CountEdge() const { return edges_.Count(); }

  std::pair<EdgePtr, bool> AddEdge(const VertexIDType &src,
                                   const VertexIDType &dst,
                                   const EdgeLabelType &label,
                                   const EdgeIDType &id) {
    auto it_src = vertices_.Find(src);
    if (it_src == vertices_.end()) return std::make_pair(EdgePtr(), false);

    auto it_dst = vertices_.Find(dst);
    if (it_dst == vertices_.end()) {
      return std::make_pair(EdgePtr(), false);
    }

    auto ret1 = edges_.Insert(id, label, src, dst);
    if (!ret1.second) {
      return std::make_pair(EdgePtr(Edge(this, *ret1.first)), false);
    }

    it_src->second.out_edges_.Insert(id);

    it_dst->second.in_edges_.Insert(id);

    return std::make_pair(EdgePtr(Edge(this, *ret1.first)), true);
  }

  EdgePtr FindEdge(const EdgeIDType &id) {
    auto it = edges_.find(id);
    if (it == edges_.end()) return EdgePtr();
    return EdgePtr(this, *it);
  }

  EdgeConstPtr FindConstEdge(const EdgeIDType &id) const {
    auto it = edges_.Find(id);
    if (it == edges_.end()) return EdgeConstPtr();
    return EdgeConstPtr(this, *it);
  }

  EdgeIterator EdgeBegin() {
    return EdgeIterator(this, edges_.begin(), edges_.end());
  }

  EdgeConstIterator EdgeCBegin() const {
    return EdgeConstIterator(this, edges_.cbegin(), edges_.cend());
  }

  bool EraseEdge(const EdgeIDType &id) {
    auto it1 = edges_.Find(id);
    if (it1 == edges_.end()) return false;

    auto &e_data = it1->second;

    auto it2 = vertices_.Find(e_data.src_);
    auto &src_data = it2->second;
    bool res = src_data.out_edges_.Erase(id);
    assert(res);

    auto it3 = vertices_.Find(e_data.dst_);
    auto &dst_data = it3->second;
    res = dst_data.in_edges_.Erase(id);
    assert(res);

    edges_.Erase(it1);

    return true;
  }

  void Clear() {
    vertices_.Clear();
    edges_.Clear();
  }
};

}  // namespace GUNDAM

#endif