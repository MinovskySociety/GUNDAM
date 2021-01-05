#ifndef _LARGE_GRAPH_H
#define _LARGE_GRAPH_H

#include <map>
#include <set>
#include <unordered_set>
#include <vector>

#include "attribute.h"
#include "container2.h"
#include "iterator2.h"

namespace GUNDAM {
template <class VertexIDType, class VertexLabelType,
          class VertexAttributeKeyType, class EdgeIDType, class EdgeLabelType,
          class EdgeAttributeKeyType>
class LargeGraph {
 public:
  static constexpr bool vertex_has_attribute = true;

  static constexpr bool edge_has_attribute = true;

  static constexpr bool graph_has_vertex_label_index = true;

  static constexpr bool vertex_has_edge_label_index = true;

 public:
  class VertexData;

  class EdgeData;

  class VertexData
      : public WithAttribute_<std::string, false, true, VertexAttributeKeyType,
                              ContainerType::Vector, SortType::Default> {
   private:
    friend class LargeGraph;

    using VertexContainer = std::set<VertexData *>;

    using EdgeContainer = std::set<EdgeData *>;

    using EdgeLabelContainer = std::map<EdgeLabelType, std::set<EdgeData *>>;

   public:
    using GraphType = LargeGraph;

    using IDType = VertexIDType;

    using LabelType = VertexLabelType;

    using AttributeKeyType = VertexAttributeKeyType;

    using VertexIterator =
        GIterator<typename VertexContainer::iterator, VertexData, PointerCast>;

    using VertexConstIterator =
        GIterator<typename VertexContainer::const_iterator, VertexData,
                  PointerCast>;

    using EdgeIterator =
        GIterator<typename EdgeContainer::iterator, EdgeData, PointerCast>;

    using EdgeConstIterator = GIterator<typename EdgeContainer::const_iterator,
                                        EdgeData, PointerCast>;

    using EdgeLabelConstIterator =
        GIterator<typename EdgeLabelContainer::const_iterator,
                  const EdgeLabelType, PairFirstCast>;

    const IDType &id() const { return id_; }

    const VertexLabelType &label() const { return label_; }

    size_t CountOutEdge() const { return out_edges_.size(); }

    size_t CountOutEdge(const EdgeLabelType &edge_label) const {
      auto it = this->out_edge_labels_.find(edge_label);
      if (it == this->out_edge_labels_.end()) return 0;
      return it->second.size();
    }
    size_t CountOutEdge(const EdgeLabelType &edge_label,
                        const VertexData *vertex_ptr) const {
      auto it =
          out_edge_build_on_vertex_.find(const_cast<VertexData *>(vertex_ptr));
      if (it == out_edge_build_on_vertex_.end()) {
        return 0;
      }
      auto it1 = it->second.find(edge_label);
      if (it1 == it->second.end()) {
        return 0;
      }
      return it1->second.size();
    }
    size_t CountInEdge() const { return in_edges_.size(); }
    size_t CountInEdge(const EdgeLabelType &edge_label) const {
      auto it = this->in_edge_labels_.find(edge_label);
      if (it == this->in_edge_labels_.end()) return 0;
      return it->second.size();
    }
    size_t CountInEdge(const EdgeLabelType &edge_label,
                       const VertexData *vertex_ptr) const {
      auto it =
          in_edge_build_on_vertex_.find(const_cast<VertexData *>(vertex_ptr));
      if (it == in_edge_build_on_vertex_.end()) {
        return 0;
      }
      auto it1 = it->second.find(edge_label);
      if (it1 == it->second.end()) {
        return 0;
      }
      return it1->second.size();
    }
    size_t CountInVertex() const {
      return this->in_edge_build_on_vertex_.size();
    }
    size_t CountInVertex(const EdgeLabelType &edge_label) const {
      auto it = this->in_vertices_.find(edge_label);
      if (it == this->in_vertices_.end()) return 0;
      return it->second.size();
    }
    size_t CountOutVertex() const {
      return this->out_edge_build_on_vertex_.size();
    }
    size_t CountOutVertex(const EdgeLabelType &edge_label) const {
      auto it = this->out_vertices_.find(edge_label);
      if (it == this->out_vertices_.end()) return 0;
      return it->second.size();
    }

    // size_t CountVertex() const { return this->adj_vertex_.size(); }
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

    // OutEdgeBegin(edge_label,dst_ptr),InEdgeBegin(edge_label,src_ptr)
    EdgeIterator OutEdgeBegin(const EdgeLabelType &edge_label,
                              const VertexData *vertex_ptr) {
      auto it =
          out_edge_build_on_vertex_.find(const_cast<VertexData *>(vertex_ptr));
      if (it == out_edge_build_on_vertex_.end()) {
        return EdgeIterator();
      }
      auto it1 = it->second.find(edge_label);
      if (it1 == it->second.end()) {
        return EdgeIterator();
      }
      return EdgeIterator(it1->second.begin(), it1->second.end());
    }

    EdgeConstIterator OutEdgeCBegin(const EdgeLabelType &edge_label,
                                    const VertexData *vertex_ptr) const {
      auto it =
          out_edge_build_on_vertex_.find(const_cast<VertexData *>(vertex_ptr));
      if (it == out_edge_build_on_vertex_.cend()) {
        return EdgeConstIterator();
      }
      auto it1 = it->second.find(edge_label);
      if (it1 == it->second.end()) {
        return EdgeConstIterator();
      }
      return EdgeConstIterator(it1->second.cbegin(), it1->second.cend());
    }

    EdgeIterator InEdgeBegin(const EdgeLabelType &edge_label,
                             const VertexData *vertex_ptr) {
      auto it =
          in_edge_build_on_vertex_.find(const_cast<VertexData *>(vertex_ptr));
      if (it == in_edge_build_on_vertex_.end()) {
        return EdgeIterator();
      }
      auto it1 = it->second.find(edge_label);
      if (it1 == it->second.end()) {
        return EdgeIterator();
      }
      return EdgeIterator(it1->second.begin(), it1->second.end());
    }

    EdgeConstIterator InEdgeCBegin(const EdgeLabelType &edge_label,
                                   const VertexData *vertex_ptr) const {
      auto it =
          in_edge_build_on_vertex_.find(const_cast<VertexData *>(vertex_ptr));
      if (it == in_edge_build_on_vertex_.cend()) {
        return EdgeConstIterator();
      }
      auto it1 = it->second.find(edge_label);
      if (it1 == it->second.end()) {
        return EdgeConstIterator();
      }
      return EdgeConstIterator(it1->second.cbegin(), it1->second.cend());
    }

    // OutVertexBegin(edge_label),InVertexBegin(edge_label)
    VertexIterator InVertexBegin(const EdgeLabelType &edge_label) {
      auto it = this->in_vertices_.find(edge_label);
      if (it == this->in_vertices_.end()) {
        return VertexIterator();
      }
      return VertexIterator(it->second.begin(), it->second.end());
    }

    VertexConstIterator InVertexCBegin(const EdgeLabelType &edge_label) const {
      auto it = this->in_vertices_.find(edge_label);
      if (it == this->in_vertices_.cend()) {
        return VertexIterator();
      }
      return VertexConstIterator(it->second.cbegin(), it->second.cend());
    }

    VertexIterator OutVertexBegin(const EdgeLabelType &edge_label) {
      auto it = this->out_vertices_.find(edge_label);
      if (it == this->out_vertices_.end()) {
        return VertexIterator();
      }
      return VertexIterator(it->second.begin(), it->second.end());
    }

    VertexConstIterator OutVertexCBegin(const EdgeLabelType &edge_label) const {
      auto it = this->out_vertices_.find(edge_label);
      if (it == this->out_vertices_.cend()) {
        return VertexIterator();
      }
      return VertexConstIterator(it->second.cbegin(), it->second.cend());
    }

    EdgeLabelConstIterator OutEdgeLabelBegin() const {
      return EdgeLabelConstIterator(out_edge_labels_.cbegin(),
                                    out_edge_labels_.cend());
    }

    EdgeLabelConstIterator OutEdgeLabelCBegin() const {
      return EdgeLabelConstIterator(out_edge_labels_.cbegin(),
                                    out_edge_labels_.cend());
    }

    EdgeLabelConstIterator InEdgeLabelBegin() const {
      return EdgeLabelConstIterator(in_edge_labels_.cbegin(),
                                    in_edge_labels_.cend());
    }

    EdgeLabelConstIterator InEdgeLabelCBegin() const {
      return EdgeLabelConstIterator(in_edge_labels_.cbegin(),
                                    in_edge_labels_.cend());
    }

   private:
    VertexData(const IDType &id, const LabelType &label)
        : id_(id), label_(label) {}

    ~VertexData() {}

    void AddOutEdge(EdgeData *e) {
      assert(e->src_ptr() == this);

      auto edge_label = e->label();

      auto ret1 = out_edges_.insert(e);
      assert(ret1.second);

      auto ret2 = out_edge_labels_[edge_label].insert(e);
      assert(ret2.second);

      auto dst = e->dst_ptr();

      auto ret3 = out_vertices_[edge_label].insert(dst);
      // assert(ret3.second);

      auto ret4 = out_edge_build_on_vertex_[dst][edge_label].insert(e);
      assert(ret4.second);

      // auto dst = e->dst_ptr();

      // out_vertices_.insert(dst);

      // out_vertex_labels_[dst->label()].insert(dst);
      // this->adj_vertex_.insert(dst);
    }

    void AddInEdge(EdgeData *e) {
      assert(e->dst_ptr() == this);
      auto edge_label = e->label();

      auto ret1 = in_edges_.insert(e);
      assert(ret1.second);

      auto ret2 = in_edge_labels_[edge_label].insert(e);
      assert(ret2.second);

      auto src = e->src_ptr();

      auto ret3 = in_vertices_[edge_label].insert(src);
      // assert(ret3.second);

      auto ret4 = in_edge_build_on_vertex_[src][edge_label].insert(e);
      assert(ret4.second);
      // auto src = e->src_ptr();

      // in_vertices_.insert(src);

      // in_vertex_labels_[src->label()].insert(src);
      // this->adj_vertex_.insert(src);
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

      // find VertexPtr
      auto it4 = out_edge_build_on_vertex_.find(e->dst_ptr());
      assert(it4 != out_edge_build_on_vertex_.end());
      // find (VertexPtr,label)
      auto it5 = it4->second.find(e->label());
      assert(it5 != it4->second.end());
      // find (VertexPtr,label,edge)
      auto it6 = it5->second.find(e);
      assert(it6 != it5->second.end());
      it5->second.erase(it6);
      //(VertexPtr,label) empty
      if (it5->second.empty()) {
        it4->second.erase(it5);
        out_vertices_.find(e->label())->second.erase(e->dst_ptr());
        //(VertexPtr) empty
        if (it4->second.empty()) {
          out_edge_build_on_vertex_.erase(e->dst_ptr());
          // if (this->in_edge_build_on_vertex_.count(e->dst_ptr()) == 0) {
          //  this->adj_vertex_.erase(e->dst_ptr());
          //}
        }
      }
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

      auto it4 = in_edge_build_on_vertex_.find(e->src_ptr());
      assert(it4 != in_edge_build_on_vertex_.end());
      auto it5 = it4->second.find(e->label());
      assert(it5 != it4->second.end());
      auto it6 = it5->second.find(e);
      assert(it6 != it5->second.end());
      it5->second.erase(it6);
      if (it5->second.empty()) {
        it4->second.erase(it5);
        in_vertices_.find(e->label())->second.erase(e->src_ptr());
        //(VertexPtr) empty
        if (it4->second.empty()) {
          in_edge_build_on_vertex_.erase(e->src_ptr());
          // if (this->in_edge_build_on_vertex_.count(e->src_ptr()) == 0) {
          //  this->adj_vertex_.erase(e->src_ptr);
          //}
        }
      }
    }

    VertexIDType id_;
    VertexLabelType label_;
    // VertexAttributeListType attributes_;

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

    // InVertexBegin,OutVertexBegin,InVertexBegin(label),OutVertexBegin(label)
    std::map<EdgeLabelType, std::set<VertexData *>> out_vertices_;
    std::map<EdgeLabelType, std::set<VertexData *>> in_vertices_;

    // CountOutVertex,CountInVertex
    // OutEdgeBegin(edge_label,dst_ptr),InEdgeBegin(edge_label,src_ptr)
    std::map<VertexData *, std::map<EdgeLabelType, std::set<EdgeData *>>>
        out_edge_build_on_vertex_;
    std::map<VertexData *, std::map<EdgeLabelType, std::set<EdgeData *>>>
        in_edge_build_on_vertex_;
    std::set<VertexData *> adj_vertex_;
  };

  class EdgeData
      : public WithAttribute_<std::string, false, true, EdgeAttributeKeyType,
                              ContainerType::Vector, SortType::Default> {
   private:
    friend class LargeGraph;

   public:
    using GraphType = LargeGraph;

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

 private:
  using VertexContainer = TreeDict<VertexIDType, VertexData *>;

  using EdgeContainer = TreeDict<EdgeIDType, EdgeData *>;

 public:
  using VertexType = VertexData;

  using EdgeType = EdgeData;

  using VertexPtr = VertexData *;

  using VertexConstPtr = const VertexData *;

  using VertexIterator = GIterator<typename VertexContainer::iterator,
                                   VertexData, PairSecondPointerCast>;

  using VertexConstIterator =
      GIterator<typename VertexContainer::const_iterator, VertexData,
                PairSecondPointerCast>;

  using EdgePtr = EdgeData *;

  using EdgeConstPtr = const EdgeData *;

  using EdgeIterator = GIterator<typename EdgeContainer::iterator, EdgeData,
                                 PairSecondPointerCast>;

  using EdgeConstIterator = GIterator<typename EdgeContainer::const_iterator,
                                      EdgeData, PairSecondPointerCast>;

  LargeGraph() = default;

  LargeGraph(const LargeGraph &other) {
    for (auto it_v = other.VertexCBegin(); !it_v.IsDone(); ++it_v) {
      auto [v, r] = AddVertex(it_v->id(), it_v->label());
      CopyAllAttributes(it_v, v);
    }
    for (auto it_e = other.EdgeCBegin(); !it_e.IsDone(); ++it_e) {
      auto [e, r] = AddEdge(it_e->src_id(), it_e->dst_id(),
              it_e->label(), it_e->id());
      CopyAllAttributes(it_e, e);
    }
  }

  LargeGraph(LargeGraph &&) = default;

  LargeGraph &operator=(const LargeGraph &other) {
    Clear();

    for (auto it_v = other.VertexCBegin(); !it_v.IsDone(); ++it_v) {
      auto [v, r] = AddVertex(it_v->id(), it_v->label());
      CopyAllAttributes(it_v, v);
    }
    for (auto it_e = other.EdgeCBegin(); !it_e.IsDone(); ++it_e) {
      auto [e, r] =
          AddEdge(it_e->src_id(), it_e->dst_id(), it_e->label(), it_e->id());
      CopyAllAttributes(it_e, e);
    }

    return *this;
  };

  LargeGraph &operator=(LargeGraph &&) = default;

  ~LargeGraph() { Clear(); }

  size_t CountVertex() const { return vertices_.Count(); }

  size_t CountVertex(const typename VertexType::LabelType &label) const {
    auto it = vertex_labels_.find(label);
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

    auto ret2 = vertex_labels_[label].Insert(id, v);
    assert(ret2.second);

    return std::make_pair(v, true);
  }

  VertexPtr FindVertex(const typename VertexType::IDType &id) {
    auto it = vertices_.Find(id);
    if (it == vertices_.end()) return nullptr;
    return it->second;
  }
  
  inline VertexConstPtr FindVertex(const typename VertexType
                                                    ::IDType& id) const{
    return this->FindConstVertex(id);
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

  size_t EraseVertex(const typename VertexType::IDType &id) {   
    auto it_v = vertices_.Find(id);
    if (it_v == vertices_.end()) return 0;

    size_t count = 0;

    VertexData *v = it_v->second;
    while (!v->out_edges_.empty()) {
      count += EraseEdge((*(v->out_edges_.begin()))->id());
    }
    while (!v->in_edges_.empty()) {
      count += EraseEdge((*(v->in_edges_.begin()))->id());
    }
    assert(v->out_edges_.empty());
    assert(v->in_edges_.empty());
    assert(v->out_edge_labels_.empty());
    assert(v->in_edge_labels_.empty());

    auto label = v->label();
    auto l_it = vertex_labels_.find(label);
    assert(l_it != vertex_labels_.end());
    l_it->second.Erase(id);
    if (l_it->second.Empty()) {
      vertex_labels_.erase(label);
    }

    delete v;
    vertices_.Erase(it_v);
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
    assert(e);

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
    edges_.Clear();
    vertex_labels_.clear();
  }

 private:
  VertexContainer vertices_;

  std::map<VertexLabelType, VertexContainer> vertex_labels_;

  EdgeContainer edges_;
};

// class SmallGraph {
//  class VertexData {
//    VertexLabelType label_;
//  };

//  class EdgeData {
//    EdgeLabelType label_;
//  };
//  std::vector<VertexData> vertices_;
//  std::vector<EdgeDta> edges_;
//};

}  // namespace GUNDAM

#endif
