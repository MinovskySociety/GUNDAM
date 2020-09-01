#ifndef _LARGE_GRAPH2_H
#define _LARGE_GRAPH2_H

#include "attribute.h"
#include "container2.h"
#include "iterator2.h"

namespace GUNDAM {
template <class VertexIDType, class VertexLabelType,
          class VertexAttributeKeyType, class EdgeIDType, class EdgeLabelType,
          class EdgeAttributeKeyType>
class LargeGraph2 {
 public:
  static constexpr bool vertex_has_attribute = true;

  static constexpr bool edge_has_attribute = true;

  static constexpr bool graph_has_vertex_label_index = true;

  static constexpr bool vertex_has_edge_label_index = true;

 private:
  class VertexData;

  class EdgeData;

  using VertexIndexByID = SortedVectorDict<VertexIDType, VertexData *>;

  using VertexIndexByLabel = SortedVectorDict<VertexLabelType, VertexIndexByID>;

  using EdgeIndexByID = SortedVectorDict<EdgeIDType, EdgeData *>;

  class VertexData
      : public WithAttribute_<std::string, false, true, VertexAttributeKeyType,
                              ContainerType::Vector, SortType::Default> {
   private:
    friend class LargeGraph2;

    using EdgePtrSet = SortedVectorSet<EdgeData *>;

    using EdgeIndexByELabel = SortedVectorDict<EdgeLabelType, EdgePtrSet>;

    using EdgeIndexByVertexELabel =
        SortedVectorDict<VertexData *, EdgeIndexByELabel>;

    using EdgeIndexByVertex = SortedVectorDict<VertexData *, EdgePtrSet>;

    using EdgeIndexByELabelVertex =
        SortedVectorDict<EdgeLabelType, EdgeIndexByVertex>;

   public:
    using GraphType = LargeGraph2;

    using IDType = VertexIDType;

    using LabelType = VertexLabelType;

    using AttributeKeyType = VertexAttributeKeyType;

    using EdgeIterator =
        GIterator<typename EdgePtrSet::iterator, EdgeData, PointerCast>;

    using EdgeConstIterator =
        GIterator<typename EdgePtrSet::const_iterator, EdgeData, PointerCast>;

    using EdgeLabelConstIterator =
        GIterator<typename EdgeIndexByELabel::const_iterator,
                  const EdgeLabelType, PairFirstCast>;

    using VertexIterator = GIterator<typename EdgeIndexByVertexELabel::iterator,
                                     VertexData, PairFirstPointerCast>;

    using VertexConstIterator =
        GIterator<typename EdgeIndexByVertexELabel::const_iterator, VertexData,
                  PairFirstPointerCast>;

    using VertexIteratorByELabel =
        GIterator<typename EdgeIndexByVertex::iterator, VertexData,
                  PairFirstPointerCast>;

    using VertexConstIteratorByELabel =
        GIterator<typename EdgeIndexByVertex::const_iterator, VertexData,
                  PairFirstPointerCast>;

    const IDType &id() const { return id_; }

    const VertexLabelType &label() const { return label_; }

    size_t CountOutEdge() const { return out_edges_.Count(); }

    size_t CountOutEdgeLabel() const { return out_edges_by_el_.Count(); }

    size_t CountOutEdge(const EdgeLabelType &edge_label) const {
      auto it = out_edges_by_el_.Find(edge_label);
      if (it == out_edges_by_el_.cend()) return 0;
      return it->second.Count();
    }

    size_t CountOutEdge(const EdgeLabelType &edge_label,
                        const VertexData *vertex_ptr) const {
      auto it1 =
          out_edges_by_dst_el_.Find(const_cast<VertexData *>(vertex_ptr));
      if (it1 == out_edges_by_dst_el_.cend()) return 0;

      auto it2 = it1->second.Find(edge_label);
      if (it2 == it1->second.cend()) return 0;

      return it2->second.Count();
    }

    size_t CountOutVertex() const { return out_edges_by_dst_el_.Count(); }

    size_t CountOutVertex(const EdgeLabelType &edge_label) const {
      auto it = out_edges_by_el_dst_.Find(edge_label);
      if (it == out_edges_by_el_dst_.cend()) return 0;
      return it->second.Count();
    }

    size_t CountInEdge() const { return in_edges_.Count(); }

    size_t CountInEdgeLabel() const { return in_edges_by_el_.Count(); }

    size_t CountInEdge(const EdgeLabelType &edge_label) const {
      auto it = in_edges_by_el_.Find(edge_label);
      if (it == in_edges_by_el_.cend()) return 0;
      return it->second.Count();
    }

    size_t CountInEdge(const EdgeLabelType &edge_label,
                       const VertexData *vertex_ptr) const {
      auto it1 = in_edges_by_src_el_.Find(const_cast<VertexData *>(vertex_ptr));
      if (it1 == in_edges_by_src_el_.cend()) return 0;

      auto it2 = it1->second.Find(edge_label);
      if (it2 == it1->second.cend()) return 0;

      return it2->second.Count();
    }

    size_t CountInVertex() const { return in_edges_by_src_el_.Count(); }

    size_t CountInVertex(const EdgeLabelType &edge_label) const {
      auto it = in_edges_by_el_src_.Find(edge_label);
      if (it == in_edges_by_el_src_.cend()) return 0;
      return it->second.Count();
    }

    EdgeIterator OutEdgeBegin() {
      return {out_edges_.begin(), out_edges_.end()};
    }

    EdgeConstIterator OutEdgeCBegin() const {
      return {out_edges_.cbegin(), out_edges_.cend()};
    }

    EdgeLabelConstIterator OutEdgeLabelBegin() const {
      return {out_edges_by_el_.cbegin(), out_edges_by_el_.cend()};
    }

    EdgeLabelConstIterator OutEdgeLabelCBegin() const {
      return {out_edges_by_el_.cbegin(), out_edges_by_el_.cend()};
    }

    EdgeIterator OutEdgeBegin(const EdgeLabelType &edge_label) {
      auto it = out_edges_by_el_.Find(edge_label);
      if (it == out_edges_by_el_.end()) return {};
      return {it->second.begin(), it->second.end()};
    }

    EdgeConstIterator OutEdgeCBegin(const EdgeLabelType &edge_label) const {
      auto it = out_edges_by_el_.Find(edge_label);
      if (it == out_edges_by_el_.cend()) return {};
      return {it->second.cbegin(), it->second.cend()};
    }

    EdgeIterator OutEdgeBegin(const EdgeLabelType &edge_label,
                              const VertexData *vertex_ptr) {
      auto it1 =
          out_edges_by_dst_el_.Find(const_cast<VertexData *>(vertex_ptr));
      if (it1 == out_edges_by_dst_el_.end()) return {};

      auto it2 = it1->second.Find(edge_label);
      if (it2 == it1->second.end()) return {};

      return {it2->second.begin(), it2->second.end()};
    }

    EdgeConstIterator OutEdgeCBegin(const EdgeLabelType &edge_label,
                                    const VertexData *vertex_ptr) const {
      auto it1 =
          out_edges_by_dst_el_.Find(const_cast<VertexData *>(vertex_ptr));
      if (it1 == out_edges_by_dst_el_.cend()) return {};

      auto it2 = it1->second.Find(edge_label);
      if (it2 == it1->second.cend()) return {};

      return {it2->second.cbegin(), it2->second.cend()};
    }

    VertexIterator OutVertexBegin() {
      return {out_edges_by_dst_el_.begin(), out_edges_by_dst_el_.end()};
    }

    VertexConstIterator OutVertexCBegin() const {
      return {out_edges_by_dst_el_.cbegin(), out_edges_by_dst_el_.cend()};
    }

    VertexIteratorByELabel OutVertexBegin(const EdgeLabelType &edge_label) {
      auto it = out_edges_by_el_dst_.Find(edge_label);
      if (it == out_edges_by_el_dst_.end()) return {};
      return {it->second.begin(), it->second.end()};
    }

    VertexConstIteratorByELabel OutVertexCBegin(
        const EdgeLabelType &edge_label) const {
      auto it = out_edges_by_el_dst_.Find(edge_label);
      if (it == out_edges_by_el_dst_.cend()) return {};
      return {it->second.cbegin(), it->second.cend()};
    }

    EdgeIterator InEdgeBegin() { return {in_edges_.begin(), in_edges_.end()}; }

    EdgeConstIterator InEdgeCBegin() const {
      return {in_edges_.cbegin(), in_edges_.cend()};
    }

    EdgeLabelConstIterator InEdgeLabelBegin() const {
      return {in_edges_by_el_.cbegin(), in_edges_by_el_.cend()};
    }

    EdgeLabelConstIterator InEdgeLabelCBegin() const {
      return {in_edges_by_el_.cbegin(), in_edges_by_el_.cend()};
    }

    EdgeIterator InEdgeBegin(const EdgeLabelType &edge_label) {
      auto it = in_edges_by_el_.Find(edge_label);
      if (it == in_edges_by_el_.end()) return {};
      return {it->second.begin(), it->second.end()};
    }

    EdgeConstIterator InEdgeCBegin(const EdgeLabelType &edge_label) const {
      auto it = in_edges_by_el_.Find(edge_label);
      if (it == in_edges_by_el_.cend()) return {};
      return {it->second.cbegin(), it->second.cend()};
    }

    EdgeIterator InEdgeBegin(const EdgeLabelType &edge_label,
                             const VertexData *vertex_ptr) {
      auto it1 = in_edges_by_src_el_.Find(const_cast<VertexData *>(vertex_ptr));
      if (it1 == in_edges_by_src_el_.end()) return {};

      auto it2 = it1->second.Find(edge_label);
      if (it2 == it1->second.end()) return {};

      return {it2->second.begin(), it2->second.end()};
    }

    EdgeConstIterator InEdgeCBegin(const EdgeLabelType &edge_label,
                                   const VertexData *vertex_ptr) const {
      auto it1 = in_edges_by_src_el_.Find(const_cast<VertexData *>(vertex_ptr));
      if (it1 == in_edges_by_src_el_.cend()) return {};

      auto it2 = it1->second.Find(edge_label);
      if (it2 == it1->second.cend()) return {};

      return {it2->second.cbegin(), it2->second.cend()};
    }

    VertexIterator InVertexBegin() {
      return {in_edges_by_src_el_.begin(), in_edges_by_src_el_.end()};
    }

    VertexConstIterator InVertexCBegin() const {
      return {in_edges_by_src_el_.cbegin(), in_edges_by_src_el_.cend()};
    }

    VertexIteratorByELabel InVertexBegin(const EdgeLabelType &edge_label) {
      auto it = in_edges_by_el_src_.Find(edge_label);
      if (it == in_edges_by_el_src_.end()) return {};
      return {it->second.begin(), it->second.end()};
    }

    VertexConstIteratorByELabel InVertexCBegin(
        const EdgeLabelType &edge_label) const {
      auto it = in_edges_by_el_src_.Find(edge_label);
      if (it == in_edges_by_el_src_.cend()) return {};
      return {it->second.cbegin(), it->second.cend()};
    }

   private:
    VertexData(const IDType &id, const LabelType &label)
        : id_(id), label_(label) {}

    ~VertexData() {}

    void AddOutEdge(EdgeData *e) {
      assert(e->src_ptr() == this);

      auto ret1 = out_edges_.Insert(e);
      assert(ret1.second);

      const auto &edge_label = e->label();
      auto ret2 = out_edges_by_el_.Insert(edge_label);
      assert(ret2.first->first == edge_label);
      auto ret3 = ret2.first->second.Insert(e);
      assert(ret3.second);

      auto dst = e->dst_ptr();
      auto ret4 = out_edges_by_dst_el_.Insert(dst);
      assert(ret4.first->first == dst);
      auto ret5 = ret4.first->second.Insert(edge_label);
      assert(ret5.first->first == edge_label);
      auto ret6 = ret5.first->second.Insert(e);
      assert(ret6.second);

      auto ret7 = out_edges_by_el_dst_.Insert(edge_label);
      assert(ret7.first->first == edge_label);
      auto ret8 = ret7.first->second.Insert(dst);
      assert(ret8.first->first == dst);
      auto ret9 = ret8.first->second.Insert(e);
      assert(ret9.second);
    }

    void AddInEdge(EdgeData *e) {
      assert(e->dst_ptr() == this);

      auto ret1 = in_edges_.Insert(e);
      assert(ret1.second);

      const auto &edge_label = e->label();
      auto ret2 = in_edges_by_el_.Insert(edge_label);
      assert(ret2.first->first == edge_label);
      auto ret3 = ret2.first->second.Insert(e);
      assert(ret3.second);

      auto src = e->src_ptr();
      auto ret4 = in_edges_by_src_el_.Insert(src);
      assert(ret4.first->first == src);
      auto ret5 = ret4.first->second.Insert(edge_label);
      assert(ret5.first->first == edge_label);
      auto ret6 = ret5.first->second.Insert(e);
      assert(ret6.second);

      auto ret7 = in_edges_by_el_src_.Insert(edge_label);
      assert(ret7.first->first == edge_label);
      auto ret8 = ret7.first->second.Insert(src);
      assert(ret8.first->first == src);
      auto ret9 = ret8.first->second.Insert(e);
      assert(ret9.second);
    }

    void RemoveOutEdge(EdgeData *e) {
      assert(e->src_ptr() == this);

      auto it1 = out_edges_.Find(e);
      if (it1 == out_edges_.end()) return;
      out_edges_.Erase(it1);

      const auto &edge_label = e->label();
      auto it2 = out_edges_by_el_.Find(edge_label);
      assert(it2 != out_edges_by_el_.end());
      auto it3 = it2->second.Find(e);
      assert(it3 != it2->second.end());
      it2->second.Erase(it3);
      if (it2->second.Empty()) out_edges_by_el_.Erase(it2);

      auto dst = e->dst_ptr();
      auto it4 = out_edges_by_dst_el_.Find(dst);
      assert(it4 != out_edges_by_dst_el_.end());
      auto it5 = it4->second.Find(edge_label);
      assert(it5 != it4->second.end());
      auto it6 = it5->second.Find(e);
      assert(it6 != it5->second.end());
      it5->second.Erase(it6);
      if (it5->second.Empty()) {
        it4->second.Erase(it5);
        if (it4->second.Empty()) out_edges_by_dst_el_.Erase(it4);
      }

      auto it7 = out_edges_by_el_dst_.Find(edge_label);
      assert(it7 != out_edges_by_el_dst_.end());
      auto it8 = it7->second.Find(dst);
      assert(it8 != it7->second.end());
      auto it9 = it8->second.Find(e);
      assert(it9 != it8->second.end());
      it8->second.Erase(it9);
      if (it8->second.Empty()) {
        it7->second.Erase(it8);
        if (it7->second.Empty()) out_edges_by_el_dst_.Erase(it7);
      }
    }

    void RemoveInEdge(EdgeData *e) {
      assert(e->dst_ptr() == this);

      auto it1 = in_edges_.Find(e);
      if (it1 == in_edges_.end()) return;
      in_edges_.Erase(it1);

      const auto &edge_label = e->label();
      auto it2 = in_edges_by_el_.Find(edge_label);
      assert(it2 != in_edges_by_el_.end());
      auto it3 = it2->second.Find(e);
      assert(it3 != it2->second.end());
      it2->second.Erase(it3);
      if (it2->second.Empty()) in_edges_by_el_.Erase(it2);

      auto src = e->src_ptr();
      auto it4 = in_edges_by_src_el_.Find(src);
      assert(it4 != in_edges_by_src_el_.end());
      auto it5 = it4->second.Find(edge_label);
      assert(it5 != it4->second.end());
      auto it6 = it5->second.Find(e);
      assert(it6 != it5->second.end());
      it5->second.Erase(it6);
      if (it5->second.Empty()) {
        it4->second.Erase(it5);
        if (it4->second.Empty()) in_edges_by_src_el_.Erase(it4);
      }

      auto it7 = in_edges_by_el_src_.Find(edge_label);
      assert(it7 != in_edges_by_el_src_.end());
      auto it8 = it7->second.Find(src);
      assert(it8 != it7->second.end());
      auto it9 = it8->second.Find(e);
      assert(it9 != it8->second.end());
      it8->second.Erase(it9);
      if (it8->second.Empty()) {
        it7->second.Erase(it8);
        if (it7->second.Empty()) in_edges_by_el_src_.Erase(it7);
      }
    }

    VertexIDType id_;
    VertexLabelType label_;

    EdgePtrSet out_edges_;
    EdgePtrSet in_edges_;

    EdgeIndexByELabel out_edges_by_el_;
    EdgeIndexByELabel in_edges_by_el_;

    EdgeIndexByVertexELabel out_edges_by_dst_el_;
    EdgeIndexByVertexELabel in_edges_by_src_el_;

    EdgeIndexByELabelVertex out_edges_by_el_dst_;
    EdgeIndexByELabelVertex in_edges_by_el_src_;
  };

  class EdgeData
      : public WithAttribute_<std::string, false, true, EdgeAttributeKeyType,
                              ContainerType::Vector, SortType::Default> {
   private:
    friend class LargeGraph2;

   public:
    using GraphType = LargeGraph2;

    using IDType = EdgeIDType;

    using LabelType = EdgeLabelType;

    using AttributeKeyType = EdgeAttributeKeyType;

    EdgeData(const EdgeIDType &id, const EdgeLabelType &label, VertexData *src,
             VertexData *dst)
        : id_(id), label_(label), src_(src), dst_(dst) {}

    ~EdgeData() {}

    const EdgeIDType &id() const { return id_; }

    const EdgeLabelType &label() const { return label_; }

    const VertexIDType &src_id() const { return src_->id(); }

    const VertexIDType &dst_id() const { return dst_->id(); }

    VertexData *src_ptr() { return src_; }

    VertexData *dst_ptr() { return dst_; }

    const VertexData *const_src_ptr() const { return src_; }

    const VertexData *const_dst_ptr() const { return dst_; }

   private:
    EdgeIDType id_;
    EdgeLabelType label_;
    VertexData *src_;
    VertexData *dst_;
    // EdgeAttributeListType attributes_;
  };

 public:
  using VertexType = VertexData;

  using EdgeType = EdgeData;

  using VertexPtr = VertexData *;

  using VertexConstPtr = const VertexData *;

  using VertexIterator = GIterator<typename VertexIndexByID::iterator,
                                   VertexData, PairSecondPointerCast>;

  using VertexConstIterator =
      GIterator<typename VertexIndexByID::const_iterator, VertexData,
                PairSecondPointerCast>;

  using EdgePtr = EdgeData *;

  using EdgeConstPtr = const EdgeData *;

  using EdgeIterator = GIterator<typename EdgeIndexByID::iterator, EdgeData,
                                 PairSecondPointerCast>;

  using EdgeConstIterator = GIterator<typename EdgeIndexByID::const_iterator,
                                      EdgeData, PairSecondPointerCast>;

  LargeGraph2() = default;

  LargeGraph2(const LargeGraph2 &other) {
    for (auto v_it = other.VertexCBegin(); !v_it.IsDone(); ++v_it) {
      AddVertex(v_it->id(), v_it->label());
    }
    for (auto e_it = other.EdgeCBegin(); !e_it.IsDone(); ++e_it) {
      AddEdge(e_it->const_src_ptr()->id(), e_it->const_dst_ptr()->id(),
              e_it->label(), e_it->id());
    }
  }

  LargeGraph2(LargeGraph2 &&) = default;

  LargeGraph2 &operator=(const LargeGraph2 &other) {
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

  LargeGraph2 &operator=(LargeGraph2 &&) = default;

  ~LargeGraph2() { Clear(); }

  size_t CountVertex() const { return vertices_.Count(); }

  size_t CountVertex(const typename VertexType::LabelType &label) const {
    auto it = vertex_labels_.Find(label);
    if (it == vertex_labels_.cend()) {
      return 0;
    }
    return it->second.Count();
  }

  std::pair<VertexPtr, bool> AddVertex(
      const typename VertexType::IDType &id,
      const typename VertexType::LabelType &label) {
    VertexData *v = new VertexData(id, label);

    auto ret1 = vertices_.Insert(id, v);
    if (!ret1.second) {
      delete v;
      return std::make_pair(ret1.first->second, false);
    }

    auto ret2 = vertex_labels_.Insert(label);
    assert(ret2.first != vertex_labels_.end());
    assert(ret2.first->first == label);
    auto &label_vertices = ret2.first->second;

    auto ret3 = label_vertices.Insert(id, v);
    assert(ret3.second);

    return std::make_pair(v, true);
  }

  VertexPtr FindVertex(const typename VertexType::IDType &id) {
    auto it = vertices_.Find(id);
    if (it == vertices_.end()) return nullptr;
    return it->second;
  }

  VertexConstPtr FindConstVertex(const typename VertexType::IDType &id) const {
    auto it = vertices_.Find(id);
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
    auto it = vertex_labels_.Find(label);
    if (it == vertex_labels_.end()) {
      return VertexIterator();
    }
    return VertexIterator(it->second.begin(), it->second.end());
  };

  VertexConstIterator VertexCBegin(
      const typename VertexType::LabelType &label) const {
    auto it = vertex_labels_.Find(label);
    if (it == vertex_labels_.cend()) {
      return VertexConstIterator();
    }
    return VertexConstIterator(it->second.cbegin(), it->second.cend());
  }

  size_t EraseVertex(const typename VertexType::IDType &id) {
    auto v_it = vertices_.Find(id);
    if (v_it == vertices_.end()) return 0;

    size_t count = 0;

    VertexData *v = v_it->second;
    while (!v->out_edges_.Empty()) {
      count += EraseEdge((*(v->out_edges_.begin()))->id());
    }
    while (!v->in_edges_.Empty()) {
      count += EraseEdge((*(v->in_edges_.begin()))->id());
    }
    assert(v->out_edges_.Empty());
    assert(v->in_edges_.Empty());
    assert(v->out_edges_by_el_.Empty());
    assert(v->in_edges_by_el_.Empty());

    const auto &label = v->label();
    auto l_it = vertex_labels_.Find(label);
    assert(l_it != vertex_labels_.end());
    l_it->second.Erase(id);
    if (l_it->second.Empty()) vertex_labels_.Erase(l_it);

    delete v;
    vertices_.Erase(v_it);
    ++count;

    return count;
  }

  size_t CountEdge() const { return edges_.Count(); }

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

    auto ret1 = edges_.Insert(id, e);
    if (!ret1.second) {
      return std::make_pair(ret1.first->second, false);
    }
    src_ptr->AddOutEdge(e);
    dst_ptr->AddInEdge(e);

    return std::make_pair(e, true);
  }

  EdgePtr FindEdge(const typename EdgeType::IDType &id) {
    auto it = edges_.Find(id);
    if (it == edges_.end()) return nullptr;
    return it->second;
  }

  EdgeConstPtr FindConstEdge(const typename EdgeType::IDType &id) const {
    auto it = edges_.Find(id);
    if (it == edges_.end()) return nullptr;
    return it->second;
  }

  EdgeIterator EdgeBegin() {
    return EdgeIterator(edges_.begin(), edges_.end());
  }

  EdgeConstIterator EdgeCBegin() const {
    return EdgeConstIterator(edges_.cbegin(), edges_.cend());
  }

  size_t EraseEdge(const typename EdgeType::IDType &id) {
    auto it = edges_.Find(id);
    if (it == edges_.end()) return 0;

    EdgeData *e = it->second;
    e->src_ptr()->RemoveOutEdge(e);
    e->dst_ptr()->RemoveInEdge(e);
    delete e;

    edges_.Erase(it);
    return 1;
  }

  void Clear() {
    for (auto &p : vertices_) {
      delete p.second;
    }
    for (auto &p : edges_) {
      delete p.second;
    }

    vertices_.Clear();
    vertex_labels_.Clear();
    edges_.Clear();
  }

 private:
  VertexIndexByID vertices_;

  VertexIndexByLabel vertex_labels_;

  EdgeIndexByID edges_;
};

}  // namespace GUNDAM

#endif