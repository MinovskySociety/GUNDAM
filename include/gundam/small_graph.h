#ifndef _SMALL_GRAPH_H
#define _SMALL_GRAPH_H

#include <vector>

#include "gundam/iterator2.h"

namespace GUNDAM {
template <class VertexIDType, class VertexLabelType, class EdgeIDType,
          class EdgeLabelType>
class SmallGraph {
 private:
  class VertexData;

  class EdgeData;

  class VertexData {
   private:
    friend class SmallGraph;

    using VertexContainer = std::set<VertexData *>;

    using EdgeContainer = std::set<EdgeData *>;

   public:
    using GraphType = SmallGraph;

    using IDType = VertexIDType;

    using LabelType = VertexLabelType;

    using VertexIterator =
        GIterator<typename VertexContainer::iterator, VertexData,
                  PointerCast<typename VertexContainer::iterator, VertexData>>;

    using VertexConstIterator = GIterator<
        typename VertexContainer::const_iterator, VertexData,
        PointerCast<typename VertexContainer::const_iterator, VertexData>>;

    using EdgeIterator =
        GIterator<typename EdgeContainer::iterator, EdgeData,
                  PointerCast<typename EdgeContainer::iterator, EdgeData>>;

    using EdgeConstIterator = GIterator<
        typename EdgeContainer::const_iterator, EdgeData,
        PointerCast<typename EdgeContainer::const_iterator, EdgeData>>;

    const IDType &id() const { return id_; }

    const VertexLabelType &label() const { return label_; }

    size_t CountOutEdge() const { return out_edges_.size(); }

    size_t CountOutEdge(const EdgeLabelType &edge_label) const {
      auto it = this->out_edge_labels_.find(edge_label);
      if (it == this->out_edge_labels_.end()) return 0;
      return it->second.size();
    }
    size_t CountInEdge() const { return in_edges_.size(); }
    size_t CountInEdge(const EdgeLabelType &edge_label) const {
      auto it = this->in_edge_labels_.find(edge_label);
      if (it == this->in_edge_labels_.end()) return 0;
      return it->second.size();
    }

    // size_t CountOutVertex() const { return out_vertices_.size(); }

    // size_t CountInVertex() const { return in_vertices_.size(); }

    EdgeIterator OutEdgeBegin() {
      return EdgeIterator(out_edges_.begin(), out_edges_.end());
    }

    EdgeConstIterator OutEdgeCBegin() const {
      return EdgeConstIterator(out_edges_.cbegin(), out_edges_.cend());
    }

    EdgeIterator InEdgeBegin() {
      return EdgeIterator(in_edges_.begin(), in_edges_.end());
    }

    EdgeConstIterator InEdgeCBegin() const {
      return EdgeIterator(in_edges_.cbegin(), in_edges_.cend());
    }

    // VertexIterator OutVertexBegin() {
    //  return VertexIterator(out_vertices_.begin(), out_vertices_.end());
    //}

    // VertexConstIterator OutVertexCBegin() const {
    //  return VertexConstIterator(out_vertices_.cbegin(),
    //  out_vertices_.cend());
    //}

    // VertexIterator InVertexBegin() {
    //  return VertexIterator(in_vertices_.begin(), in_vertices_.end());
    //}

    // VertexConstIterator InVertexCBegin() const {
    //  return VertexIterator(in_vertices_.cbegin(), in_vertices_.cend());
    //}

    EdgeIterator OutEdgeBegin(const EdgeLabelType &edge_label) {
      auto it = out_edge_labels_.find(edge_label);
      if (it == out_edge_labels_.end()) {
        return EdgeIterator();
      }
      return EdgeIterator(it->second.begin(), it->second.end());
    }

    EdgeConstIterator OutEdgeCBegin(const EdgeLabelType &edge_label) const {
      auto it = out_edge_labels_.find(edge_label);
      if (it == out_edge_labels_.cend()) {
        return EdgeConstIterator();
      }
      return EdgeConstIterator(it->second.cbegin(), it->second.cend());
    }

    EdgeIterator InEdgeBegin(const EdgeLabelType &edge_label) {
      auto it = in_edge_labels_.find(edge_label);
      if (it == in_edge_labels_.end()) {
        return EdgeIterator();
      }
      return EdgeIterator(it->second.begin(), it->second.end());
    }

    EdgeConstIterator InEdgeCBegin(const EdgeLabelType &edge_label) const {
      auto it = in_edge_labels_.find(edge_label);
      if (it == in_edge_labels_.cend()) {
        return EdgeConstIterator();
      }
      return EdgeConstIterator(it->second.cbegin(), it->second.cend());
    }

   private:
    VertexData(const IDType &id, const LabelType &label)
        : id_(id), label_(label) {}

    ~VertexData() {}

    void AddOutEdge(EdgeData *e) {
      assert(e->src_ptr() == this);

      auto ret1 = out_edges_.insert(e);
      assert(ret1.second);

      auto ret2 = out_edge_labels_[e->label()].insert(e);
      assert(ret2.second);

      // auto dst = e->dst_ptr();

      // out_vertices_.insert(dst);

      // out_vertex_labels_[dst->label()].insert(dst);
    }

    void AddInEdge(EdgeData *e) {
      assert(e->dst_ptr() == this);

      auto ret1 = in_edges_.insert(e);
      assert(ret1.second);

      auto ret2 = in_edge_labels_[e->label()].insert(e);
      assert(ret2.second);

      // auto src = e->src_ptr();

      // in_vertices_.insert(src);

      // in_vertex_labels_[src->label()].insert(src);
    }

    void RemoveOutEdge(EdgeData *e) {
      assert(e->src_ptr() == this);

      auto it1 = out_edges_.find(e);
      if (it1 == out_edges_.end()) return;
      out_edges_.erase(it1);

      auto it2 = out_edge_labels_.find(e->label());
      assert(it2 != out_edge_labels_.end());
      auto it3 = it2->second.find(e);
      assert(it3 != it2->second.end());
      it2->second.erase(it3);
      if (it2->second.empty()) out_edge_labels_.erase(it2);
    }

    void RemoveInEdge(EdgeData *e) {
      assert(e->dst_ptr() == this);

      auto it1 = in_edges_.find(e);
      if (it1 == in_edges_.end()) return;
      in_edges_.erase(it1);

      auto it2 = in_edge_labels_.find(e->label());
      assert(it2 != in_edge_labels_.end());
      auto it3 = it2->second.find(e);
      assert(it3 != it2->second.end());
      it2->second.erase(it3);
      if (it2->second.empty()) in_edge_labels_.erase(it2);
    }

    VertexIDType id_;
    VertexLabelType label_;

    std::set<EdgeData *> out_edges_;
    std::set<EdgeData *> in_edges_;

    // std::map<VertexData *, std::set<EdgeData *>> out_vertices_;
    // std::map<VertexData *, std::set<EdgeData *>> in_vertices_;

    std::map<EdgeLabelType, std::set<EdgeData *>> out_edge_labels_;
    std::map<EdgeLabelType, std::set<EdgeData *>> in_edge_labels_;

    // std::map<VertexLabelType, std::map<VertexData *, std::set<EdgeData *>>>
    //    in_vertex_labels_;
    // std::map<VertexLabelType, std::map<VertexData *, std::set<EdgeData *>>>
    //    out_vertex_labels_;
  };

  class EdgeData {
   private:
    friend class SmallGraph;

   public:
    using GraphType = SmallGraph;

    using IDType = EdgeIDType;

    using LabelType = EdgeLabelType;

    EdgeData(const EdgeIDType &id, const EdgeLabelType &label, VertexData *src,
             VertexData *dst)
        : id_(id), label_(label), src_(src), dst_(dst) {}

    ~EdgeData() {}

    const EdgeIDType &id() const { return id_; }

    const EdgeLabelType &label() const { return label_; }

    VertexData *src_ptr() { return src_; }

    VertexData *dst_ptr() { return dst_; }

    const VertexData *const_src_ptr() const { return src_; }

    const VertexData *const_dst_ptr() const { return dst_; }

   private:
    EdgeIDType id_;
    EdgeLabelType label_;
    VertexData *src_;
    VertexData *dst_;
  };

  using VertexContainer = std::map<VertexIDType, VertexData *>;

  using EdgeContainer = std::map<EdgeIDType, EdgeData *>;

 public:
  using VertexType = VertexData;

  using EdgeType = EdgeData;

  using VertexPtr = VertexData *;

  using VertexConstPtr = const VertexData *;

  using VertexIterator = GIterator<
      typename VertexContainer::iterator, VertexData,
      PairSecondPointerCast<typename VertexContainer::iterator, VertexData>>;

  using VertexConstIterator =
      GIterator<typename VertexContainer::const_iterator, VertexData,
                PairSecondPointerCast<typename VertexContainer::const_iterator,
                                      VertexData>>;

  using EdgePtr = EdgeData *;

  using EdgeConstPtr = const EdgeData *;

  using EdgeIterator = GIterator<
      typename EdgeContainer::iterator, EdgeData,
      PairSecondPointerCast<typename EdgeContainer::iterator, EdgeData>>;

  using EdgeConstIterator = GIterator<
      typename EdgeContainer::const_iterator, EdgeData,
      PairSecondPointerCast<typename EdgeContainer::const_iterator, EdgeData>>;

  static const bool vertex_has_attribute = true;

  static const bool edge_has_attribute = true;

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

  SmallGraph &operator=(const SmallGraph &other) {
    Clear();

    for (auto v_it = other.VertexCBegin(); !v_it.IsDone(); ++v_it) {
      AddVertex(v_it->id(), v_it->label());
    }
    for (auto e_it = other.EdgeCBegin(); !e_it.IsDone(); ++e_it) {
      AddEdge(e_it->const_src_ptr()->id(), e_it->const_dst_ptr()->id(),
              e_it->label(), e_it->id());
    }

    return *this;
  };

  SmallGraph &operator=(SmallGraph &&) = default;

  ~SmallGraph() { Clear(); }

  size_t CountVertex() const { return vertices_.size(); }

  size_t CountVertex(const typename VertexType::LabelType &label) const {
    auto it = vertex_labels_.find(label);
    if (it == vertex_labels_.cend()) {
      return 0;
    }
    return it->second.size();
  }

  std::pair<VertexPtr, bool> AddVertex(
      const typename VertexType::IDType &id,
      const typename VertexType::LabelType &label) {
    VertexData *v = new VertexData(id, label);

    auto p = std::make_pair(id, v);

    auto ret1 = vertices_.insert(p);
    if (!ret1.second) {
      delete v;
      return std::make_pair(ret1.first->second, false);
    }

    auto ret2 = vertex_labels_[label].insert(p);
    assert(ret2.second);

    return std::make_pair(v, true);
  }

  VertexPtr FindVertex(const typename VertexType::IDType &id) {
    auto it = vertices_.find(id);
    if (it == vertices_.end()) return nullptr;
    return it->second;
  }

  VertexConstPtr FindConstVertex(const typename VertexType::IDType &id) const {
    auto it = vertices_.find(id);
    if (it == vertices_.end()) return nullptr;
    return it->second;
  }

  VertexIterator VertexBegin() {
    return VertexIterator(vertices_.begin(), vertices_.end());
  }

  VertexConstIterator VertexCBegin() const {
    return VertexConstIterator(vertices_.cbegin(), vertices_.cend());
  }

  VertexIterator VertexBegin(const typename VertexType::LabelType &label) {
    auto it = vertex_labels_.find(label);
    if (it == vertex_labels_.end()) {
      return VertexIterator();
    }
    return VertexIterator(it->second.begin(), it->second.end());
  };

  VertexConstIterator VertexCBegin(
      const typename VertexType::LabelType &label) const {
    auto it = vertex_labels_.find(label);
    if (it == vertex_labels_.cend()) {
      return VertexConstIterator();
    }
    return VertexConstIterator(it->second.cbegin(), it->second.cend());
  }

  bool EraseVertex(const typename VertexType::IDType &id) {
    auto it = vertices_.find(id);
    if (it == vertices_.end()) return false;

    VertexData *v = it->second;
    while (!v->out_edges_.empty()) {
      EraseEdge((*(v->out_edges_.begin()))->id());
    }
    while (!v->in_edges_.empty()) {
      EraseEdge((*(v->in_edges_.begin()))->id());
    }
    assert(v->out_edges_.empty());
    assert(v->in_edges_.empty());
    assert(v->out_edge_labels_.empty());
    assert(v->in_edge_labels_.empty());
    delete v;

    vertices_.erase(it);
    return true;
  }

  size_t CountEdge() const { return edges_.size(); }

  std::pair<EdgePtr, bool> AddEdge(const typename VertexType::IDType &src,
                                   const typename VertexType::IDType &dst,
                                   const typename EdgeType::LabelType &label,
                                   const typename EdgeType::IDType &id) {
    VertexData *src_ptr = FindVertex(src);
    VertexData *dst_ptr = FindVertex(dst);
    if (!src_ptr || !dst_ptr) {
      return std::make_pair(nullptr, false);
    }

    EdgeData *e = new EdgeData(id, label, src_ptr, dst_ptr);
    assert(e);

    auto ret1 = edges_.insert(std::make_pair(id, e));
    if (!ret1.second) {
      return std::make_pair(ret1.first->second, false);
    }

    src_ptr->AddOutEdge(e);
    dst_ptr->AddInEdge(e);

    return std::make_pair(e, true);
  }

  EdgePtr FindEdge(const typename EdgeType::IDType &id) {
    auto it = edges_.find(id);
    if (it == edges_.end()) return nullptr;
    return it->second;
  }

  EdgeConstPtr FindConstEdge(const typename EdgeType::IDType &id) const {
    auto it = edges_.find(id);
    if (it == edges_.end()) return nullptr;
    return it->second;
  }

  EdgeIterator EdgeBegin() {
    return EdgeIterator(edges_.begin(), edges_.end());
  }

  EdgeConstIterator EdgeCBegin() const {
    return EdgeConstIterator(edges_.cbegin(), edges_.cend());
  }

  bool EraseEdge(const typename EdgeType::IDType &id) {
    auto it = edges_.find(id);
    if (it == edges_.end()) return false;

    EdgeData *e = it->second;
    e->src_ptr()->RemoveOutEdge(e);
    e->dst_ptr()->RemoveInEdge(e);
    delete e;

    edges_.erase(it);
    return true;
  }

  void Clear() {
    for (auto &p : vertices_) {
      delete p.second;
    }
    for (auto &p : edges_) {
      delete p.second;
    }

    vertices_.clear();
    edges_.clear();
    vertex_labels_.clear();
  }

 private:
  VertexContainer vertices_;

  std::map<VertexLabelType, VertexContainer> vertex_labels_;

  EdgeContainer edges_;
};

}  // namespace GUNDAM

#endif