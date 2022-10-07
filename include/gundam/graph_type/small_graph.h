#ifndef _GUNDAM_GRAPH_TYPE_SMALL_GRAPH_H
#define _GUNDAM_GRAPH_TYPE_SMALL_GRAPH_H

#include <algorithm>
#include <iterator>
#include <iostream>
#include <string>
#include <vector>

#include "gundam/component/container2.h"
#include "gundam/component/iterator2.h"
#include "gundam/component/pointer.h"
#include "gundam/data_type/datatype.h"

#include "gundam/graph_type/graph_parameter.h"
#include "gundam/graph_type/graph_base.h"

#include "gundam/type_getter/vertex_id.h"
#include "gundam/type_getter/vertex_label.h"
#include "gundam/type_getter/vertex_handle.h"
#include "gundam/type_getter/vertex_attribute_handle.h"

#include "gundam/type_getter/edge_id.h"
#include "gundam/type_getter/edge_label.h"
#include "gundam/type_getter/edge_handle.h"
#include "gundam/type_getter/edge_attribute_handle.h"

#include "gundam/serialize/serialize.h"

namespace GUNDAM {

template <class VertexIDType, class VertexLabelType, 
          class   EdgeIDType, class   EdgeLabelType>
class SmallGraph;

template <class VertexIDType, class VertexLabelType, 
          class   EdgeIDType, class   EdgeLabelType>
class SmallGraph {
 public:
  class _GraphParameter : public GraphParameterBase {
   public:
    static constexpr bool graph_level_vertex_label_index = false;

    static constexpr bool vertex_level_edge_label_index = false;

    static constexpr bool graph_level_edge_iterator = true;

    static constexpr bool graph_level_count_vertex = true;
  };

  // friend class GraphParameter<SmallGraph>;

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

  using InnerGraphContainer = std::tuple<VertexContainer,
                                           EdgeContainer>;

  static VertexContainer& vertices(InnerGraphContainer* inner_graph_ptr) {
    assert( inner_graph_ptr != nullptr );
    return std::get<0>(*inner_graph_ptr);
  }
  static const VertexContainer& vertices(const InnerGraphContainer* inner_graph_ptr) {
    assert( inner_graph_ptr != nullptr );
    return std::get<0>(*inner_graph_ptr);
  }

  static EdgeContainer& edges(InnerGraphContainer* inner_graph_ptr) {
    assert( inner_graph_ptr != nullptr );
    return std::get<1>(*inner_graph_ptr);
  }
  static const EdgeContainer& edges(const InnerGraphContainer* inner_graph_ptr) {
    assert( inner_graph_ptr != nullptr );
    return std::get<1>(*inner_graph_ptr);
  }

  inline VertexContainer& vertices() {
    assert( this->inner_graph_ptr_ != nullptr );
    return std::get<0>(*this->inner_graph_ptr_);
  }
  inline const VertexContainer& vertices() const {
    assert( this->inner_graph_ptr_ != nullptr );
    return std::get<0>(*this->inner_graph_ptr_);
  }

  inline EdgeContainer& edges() {
    assert( this->inner_graph_ptr_ != nullptr );
    return std::get<1>(*this->inner_graph_ptr_);
  }
  inline const EdgeContainer& edges() const {
    assert( this->inner_graph_ptr_ != nullptr );
    return std::get<1>(*this->inner_graph_ptr_);
  }

  using VertexContent = std::pair<VertexIDType, VertexData>;
  using EdgeContent = std::pair<EdgeIDType, EdgeData>;

  template <bool is_const>
  class _Vertex;

  template <bool is_const>
  class _Edge;

  using Vertex = _Vertex<false>;

  using ConstVertex = _Vertex<true>;

  using Edge = _Edge<false>;

  using ConstEdge = _Edge<true>;

 private:
  friend class VertexHandle<SmallGraph>;
  friend class VertexHandle<const SmallGraph>;
  
  friend class EdgeHandle<SmallGraph>;
  friend class EdgeHandle<const SmallGraph>;

  friend class VertexID<SmallGraph>;
  friend class VertexLabel<const SmallGraph>;

  friend class EdgeID<SmallGraph>;
  friend class EdgeLabel<const SmallGraph>;

  friend class GraphBase<SmallGraph>;

  using VertexPtr      = GPointer<false, Vertex, ConstVertex>;

  using VertexConstPtr = GPointer< true, Vertex, ConstVertex>;

  using EdgePtr = GPointer<false, Edge, ConstEdge>;

  using EdgeConstPtr = GPointer<true, Edge, ConstEdge>;

 public:
  using VertexIterator =
      GIterator2<false, InnerGraphContainer, typename VertexContainer::iterator, 
                 Vertex,
                 VertexPtr>;

  using VertexConstIterator =
      GIterator2< true, InnerGraphContainer, typename VertexContainer::const_iterator,
                 ConstVertex, 
                 VertexConstPtr>;

  using EdgeIterator =
      GIterator2<false, InnerGraphContainer, typename EdgeContainer::iterator, 
                 Edge, EdgePtr>;

  using EdgeConstIterator =
      GIterator2<true, InnerGraphContainer, typename EdgeContainer::const_iterator,
                 ConstEdge, EdgeConstPtr>;

 private:
  template <bool is_const>
  class _Vertex {
   private:
    friend class _Vertex<!is_const>;

   public:
    using GraphType = SmallGraph;

    using IDType = VertexIDType;

    using LabelType = VertexLabelType;

    using AttributeKeyType = std::string;

    using EdgeIterator =
        GIterator2<false, InnerGraphContainer,
                   typename SortedVectorSet<EdgeIDType>::iterator, Edge,
                   EdgePtr>;

    using EdgeConstIterator =
        GIterator2<true, InnerGraphContainer,
                   typename SortedVectorSet<EdgeIDType>::const_iterator,
                   ConstEdge, EdgeConstPtr>;

   private:
    friend GraphType;
    friend typename GraphType::VertexPtr;
    friend typename GraphType::VertexConstPtr;
    friend typename GraphType::VertexIterator;
    friend typename GraphType::VertexConstIterator;
    friend typename GraphType::Edge;
    friend typename GraphType::ConstEdge;
    friend typename GraphType::EdgePtr;
    friend typename GraphType::EdgeConstPtr;
    friend typename GraphType::EdgeIterator;
    friend typename GraphType::EdgeConstIterator;

    _Vertex() : inner_graph_ptr_(nullptr){};

    _Vertex(std::conditional_t<is_const, 
              const InnerGraphContainer*,
                    InnerGraphContainer*> inner_graph_ptr,
                      const VertexContent &content)
                        : inner_graph_ptr_(inner_graph_ptr), 
                                       id_(content.first), 
                                    label_(content.second.label_) {
      assert(this->inner_graph_ptr_);
      return;
    }

    _Vertex(std::conditional_t<is_const, 
              const InnerGraphContainer*,
                    InnerGraphContainer*> inner_graph_ptr,
                      const VertexIDType &id)
                       : inner_graph_ptr_(inner_graph_ptr), 
                                      id_(id) {
      assert(this->inner_graph_ptr_);
      const auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
      label_ = data.label_;
    }

   public:
    const IDType &id() const {
      assert(HasValue());
      return id_;
    }

    const VertexLabelType &label() const {
      assert(HasValue());
      return label_;
    }

    size_t CountOutEdge() const {
      assert(HasValue());
      const auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
      return data.out_edges_.Count();
    }

    size_t CountOutEdge(const EdgeLabelType &edge_label) const {
      size_t out_edge_count = 0;
      for (auto it = this->OutEdgeBegin(); !it.IsDone(); it++) {
        if (it->label() == edge_label) out_edge_count++;
      }
      return out_edge_count;
    }

    size_t CountInEdge() const {
      assert(HasValue());
      const auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
      return data.in_edges_.Count();
    }

    size_t CountInEdge(const EdgeLabelType &edge_label) const {
      size_t in_edge_count = 0;
      for (auto it = this->InEdgeBegin(); !it.IsDone(); it++) {
        if (it->label() == edge_label) in_edge_count++;
      }
      return in_edge_count;
    }

    size_t CountOutVertex() const {
      std::set<VertexIDType> out_vertex_id_set;
      for (auto it = this->OutEdgeBegin(); !it.IsDone(); it++) {
        out_vertex_id_set.insert(it->const_dst_handle()->id());
      }
      return out_vertex_id_set.size();
    }

    size_t CountOutVertex(const EdgeLabelType &edge_label) const {
      std::set<VertexIDType> out_vertex_id_set;
      for (auto it = this->OutEdgeBegin(); !it.IsDone(); it++) {
        if (it->label() == edge_label)
          out_vertex_id_set.insert(it->const_dst_handle()->id());
      }
      return out_vertex_id_set.size();
    }

    size_t CountInVertex() const {
      std::set<VertexIDType> in_vertex_id_set;
      for (auto it = this->InEdgeBegin(); !it.IsDone(); it++) {
        in_vertex_id_set.insert(it->const_src_handle()->id());
      }
      return in_vertex_id_set.size();
    }

    size_t CountInVertex(const EdgeLabelType &edge_label) const {
      std::set<VertexIDType> in_vertex_id_set;
      for (auto it = this->InEdgeBegin(); !it.IsDone(); it++) {
        if (it->label() == edge_label)
          in_vertex_id_set.insert(it->const_src_handle()->id());
      }
      return in_vertex_id_set.size();
    }

    size_t CountVertex() const {
      std::set<VertexIDType> vertex_id_set;
      for (auto it = this->OutEdgeBegin(); !it.IsDone(); it++) {
        vertex_id_set.insert(it->dst_handle()->id());
      }
      for (auto it = this->InEdgeBegin(); !it.IsDone(); it++) {
        vertex_id_set.insert(it->src_handle()->id());
      }
      return vertex_id_set.size();
    }

    inline auto OutEdgeBegin() const {
      if constexpr (is_const) {
        assert(HasValue());
        const auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
        return EdgeConstIterator(this->inner_graph_ptr_, 
                                 data.out_edges_.cbegin(),
                                 data.out_edges_.cend());
      }
      else{
        assert(HasValue());
        auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
        return EdgeIterator(this->inner_graph_ptr_, 
                            data.out_edges_.begin(),
                            data.out_edges_.end());
      }
    }

    inline auto InEdgeBegin() const {
      if constexpr (is_const) {
        assert(HasValue());
        const auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
        return EdgeConstIterator(this->inner_graph_ptr_, 
                                 data.in_edges_.cbegin(),
                                 data.in_edges_.cend());
      }
      else {
        assert(HasValue());
        auto &data = SmallGraph::vertices(this->inner_graph_ptr_).Find(id_)->second;
        return EdgeIterator(this->inner_graph_ptr_, data.in_edges_.begin(), data.in_edges_.end());
      }
    }

    template <bool input_is_const>
    bool operator==(const _Vertex<input_is_const> &b) const {
      // std::cout << "this->inner_graph_ptr_: " << this->inner_graph_ptr_ << std::endl;
      // std::cout << "this->id_             : " << this->id_              << std::endl;
      // std::cout << "b.inner_graph_ptr_: " << b.inner_graph_ptr_ << std::endl;
      // std::cout << "b.id_             : " << b.id_              << std::endl;
      if (this->inner_graph_ptr_ != b.inner_graph_ptr_){
        // cannot be the same
        // at least one is not nullptr
        assert(this->inner_graph_ptr_ || b.inner_graph_ptr_);
        // std::cout << "* return 0 *" << std::endl;
        return false;
      }
      // this->inner_graph_ptr_ == b.inner_graph_ptr_
      if (this->inner_graph_ptr_ == nullptr){
        // this->inner_graph_ptr_ is null
        assert(b.inner_graph_ptr_ == nullptr);
        // both ptr are null
        // std::cout << "* return 1 *" << std::endl;
        return true;
      }
      // std::cout << "* return 2 *" << std::endl;
      return id_ == b.id_;
    }

    template <bool input_is_const>
    bool operator<(const _Vertex<input_is_const> &b) const {
      // std::cout << "this->inner_graph_ptr_: " << this->inner_graph_ptr_ << std::endl;
      // std::cout << "this->id_             : " << this->id_              << std::endl;
      // std::cout << "b.inner_graph_ptr_: " << b.inner_graph_ptr_ << std::endl;
      // std::cout << "b.id_             : " << b.id_              << std::endl;
      if (this->inner_graph_ptr_ < b.inner_graph_ptr_){
        // one is nullptr and the other is not
        assert(((this->inner_graph_ptr_ == nullptr) && (b.inner_graph_ptr_ != nullptr))
            || ((this->inner_graph_ptr_ != nullptr) && (b.inner_graph_ptr_ == nullptr)));
        // std::cout << "# return 0 #" << std::endl;
        return true;
      }
      if (this->inner_graph_ptr_ > b.inner_graph_ptr_){
        // one is nullptr and the other is not
        assert(((this->inner_graph_ptr_ == nullptr) && (b.inner_graph_ptr_ != nullptr))
            || ((this->inner_graph_ptr_ != nullptr) && (b.inner_graph_ptr_ == nullptr)));
        // std::cout << "# return 1 #" << std::endl;
        return false;
      }
      // this->inner_graph_ptr_
      //  == b.inner_graph_ptr_
      if (this->inner_graph_ptr_ == nullptr) {
        assert(!b.inner_graph_ptr_);
        // both pointers are null, are the same
        // std::cout << "# return 2 #" << std::endl;
        return false;
      }
      // std::cout << "# return 3 #" << std::endl;
      return this->id_ < b.id_;
    }

    void Print() const {
      std::cout << "this->inner_graph_ptr_: " << this->inner_graph_ptr_ << std::endl;
      std::cout << "this->id_             : " << this->id_              << std::endl;
      return;
    }

   private:
    inline void Reset() { 
      this->inner_graph_ptr_ = nullptr; 
      return;
    }

    inline bool HasValue() const { 
      return (this->inner_graph_ptr_ != nullptr); 
    }

    std::conditional_t<is_const, 
    const InnerGraphContainer *, 
          InnerGraphContainer *>  inner_graph_ptr_;
    VertexIDType id_;
    VertexLabelType label_;
  };

  template <bool is_const>
  class _Edge {
   public:
    using GraphType = SmallGraph;

    using IDType = EdgeIDType;

    using LabelType = EdgeLabelType;

    using AttributeKeyType = std::string;

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
    friend typename GraphType::Vertex;
    friend typename GraphType::ConstVertex;
    friend typename GraphType::Vertex::EdgeIterator;
    friend typename GraphType::Vertex::EdgeConstIterator;

    _Edge() : inner_graph_ptr_(nullptr) {
      return;
    }

    _Edge(std::conditional_t<is_const, 
          const InnerGraphContainer *,
                InnerGraphContainer *> inner_graph_ptr,
                    const EdgeContent &content)
                    : inner_graph_ptr_(inner_graph_ptr),
                                   id_(content.first),
                                label_(content.second.label_),
                                  src_(content.second.src_),
                                  dst_(content.second.dst_) {
      assert(this->inner_graph_ptr_);
      return;
    }

    _Edge(std::conditional_t<is_const, 
          const InnerGraphContainer *,
                InnerGraphContainer *> inner_graph_ptr,
                     const EdgeIDType &id)
                    : inner_graph_ptr_(inner_graph_ptr), 
                                   id_(id) {
      assert(this->inner_graph_ptr_);
      const auto &data = SmallGraph::edges(this->inner_graph_ptr_).Find(id_)->second;
      label_ = data.label_;
        src_ = data.src_;
        dst_ = data.dst_;
      return;
    }

   public:
    const EdgeIDType &id() const {
      assert(HasValue());
      return id_;
    }

    const EdgeLabelType &label() const {
      assert(HasValue());
      return label_;
    }

    const VertexIDType &src_id() const {
      assert(HasValue());
      return src_;
    }

    const VertexIDType &dst_id() const {
      assert(HasValue());
      return dst_;
    }

    auto src_handle() const {
      assert(HasValue());
      if constexpr (is_const) {
        return this->const_src_handle();
      }
      else {
        return VertexPtr(Vertex(this->inner_graph_ptr_, src_));
      }
    }

    VertexConstPtr const_src_handle() const {
      assert(HasValue());
      return VertexConstPtr(ConstVertex(this->inner_graph_ptr_, src_));
    }

    auto dst_handle() const {
      assert(HasValue());
      if constexpr (is_const) {
        return this->const_dst_handle();
      }
      else {
        return VertexPtr(Vertex(this->inner_graph_ptr_, dst_));
      }
    }

    VertexConstPtr const_dst_handle() const {
      assert(HasValue());
      return VertexConstPtr(ConstVertex(this->inner_graph_ptr_, dst_));
    }

    bool operator==(const _Edge &b) const {
      if (!this->inner_graph_ptr_) {
        return !b.inner_graph_ptr_;
      } 
      if (!b.inner_graph_ptr_) {
        return false;
      } 
      return id_ == b.id_;
    }

    bool operator<(const _Edge &b) const {
      if (!this->inner_graph_ptr_ 
           || !b.inner_graph_ptr_) {
        return false;
      }
      return this->id_ < b.id_;
    }

   private:
    void Reset() { 
      this->inner_graph_ptr_ = nullptr; 
    }

    bool HasValue() const { 
      return (this->inner_graph_ptr_ != nullptr); 
    }

    std::conditional_t<is_const, 
    const InnerGraphContainer *, 
          InnerGraphContainer *> inner_graph_ptr_;
        
    EdgeIDType id_;
    EdgeLabelType label_;
    VertexIDType src_;
    VertexIDType dst_;
  };

 public:
  using VertexType = Vertex;

  using VertexCounterType = size_t;

  using EdgeType = Edge;

  static constexpr bool vertex_has_attribute = false;

  static constexpr bool edge_has_attribute = false;

  static constexpr bool is_mutable = true;

  SmallGraph() : inner_graph_ptr_(nullptr) {
    assert(this->inner_graph_ptr_ == nullptr);
    this->inner_graph_ptr_ = new InnerGraphContainer();
    return;
  }

  explicit SmallGraph(const SmallGraph &other) : inner_graph_ptr_(nullptr) {
    assert( this->inner_graph_ptr_ == nullptr );
    this->inner_graph_ptr_ = new InnerGraphContainer(*other.inner_graph_ptr_);
    return;
  }

  SmallGraph(SmallGraph &&other) : inner_graph_ptr_(nullptr) {
    assert( this->inner_graph_ptr_ == nullptr );
    this->inner_graph_ptr_ = other.inner_graph_ptr_;
    other.inner_graph_ptr_ = new InnerGraphContainer();
    return;
  }

  SmallGraph &operator=(const SmallGraph &other) {
    // safe to delete nullptr
    delete this->inner_graph_ptr_;
    (*this->inner_graph_ptr_) = (*other.inner_graph_ptr_);
    return *this;
  }

  SmallGraph &operator=(SmallGraph &&other) {
    // safe to delete nullptr
    delete this->inner_graph_ptr_;
    this->inner_graph_ptr_ = other.inner_graph_ptr_;
    other.inner_graph_ptr_ = new InnerGraphContainer();
    return *this;
  }

  ~SmallGraph() {
    // safe to delete nullptr
    delete this->inner_graph_ptr_;
    return;
  }

  size_t CountVertex() const { 
    return this->vertices().Count(); 
  }

  std::pair<VertexPtr, bool> AddVertex(const VertexIDType &id,
                                       const VertexLabelType &label) {
    typename VertexContainer::iterator it;
    bool res;
    std::tie(it, res) = this->vertices().Insert(id, label);

    return std::make_pair(VertexPtr(Vertex(this->inner_graph_ptr_, *it)), res);
  }

  VertexPtr FindVertex(const VertexIDType &id) {
    auto it = this->vertices().Find(id);
    if (it == this->vertices().end()) return VertexPtr();
    return VertexPtr(Vertex(this->inner_graph_ptr_, *it));
  }
  
  VertexConstPtr FindVertex(const VertexIDType &id) const {
    auto it = this->vertices().Find(id);
    if (it == this->vertices().end()) return VertexConstPtr();
    return VertexConstPtr(ConstVertex(this->inner_graph_ptr_, *it));
  }

  VertexIterator VertexBegin() {
    return VertexIterator(this->inner_graph_ptr_, 
                          this->vertices().begin(), 
                          this->vertices().end());
  }

  VertexConstIterator VertexBegin() const {
    return VertexConstIterator(this->inner_graph_ptr_, 
                               this->vertices().cbegin(),
                               this->vertices().cend());
  }

  // VertexIterator VertexBegin(const VertexLabelType &label);

  // VertexConstIterator VertexCBegin(const VertexLabelType &label) const;

  size_t EraseVertex(const VertexIDType &id) {
    auto it = this->vertices().Find(id);
    if (it == this->vertices().end()) return 0;

    size_t count = 0;

    auto &v_data = it->second;

    std::vector<VertexIDType> edges;
    std::merge(v_data.out_edges_.begin(), 
               v_data.out_edges_. end (),
               v_data. in_edges_.begin(), 
               v_data. in_edges_. end (),
               std::back_inserter(edges));

    for (auto it  = v_data.out_edges_.begin();
              it != v_data.out_edges_.end(); it++){
      auto ret = this->vertices().Find(this->edges().Find(*it)->second.dst_);
      ret->second.in_edges_.Erase(*it);
    }

    for (auto it  = v_data.in_edges_.begin();
              it != v_data.in_edges_.end(); it++){
      auto ret = this->vertices().Find(this->edges().Find(*it)->second.src_);
      ret->second.out_edges_.Erase(*it);
    }

    for (auto it_r  = edges.rbegin(); 
              it_r != edges.rend(); 
            ++it_r) {
      if (it_r != edges.rbegin() && *it_r == *(it_r - 1)) 
        continue;
      count += this->edges().Erase(*it_r);
    }

    this->vertices().Erase(it);
    ++count;

    return count;
  }

  size_t CountEdge() const { 
    return this->edges().Count(); 
  }

  std::pair<EdgePtr, bool> AddEdge(const VertexIDType &src,
                                   const VertexIDType &dst,
                                   const EdgeLabelType &label,
                                   const EdgeIDType &id) {
    auto it_src = this->vertices().Find(src);
    if (it_src == this->vertices().end()) {
      return std::make_pair(EdgePtr(), false);
    }

    auto it_dst = this->vertices().Find(dst);
    if (it_dst == this->vertices().end()) {
      return std::make_pair(EdgePtr(), false);
    }

    auto ret1 = this->edges().Insert(id, label, src, dst);
    if (!ret1.second) {
      return std::make_pair(EdgePtr(Edge(this->inner_graph_ptr_, *ret1.first)), false);
    }

    it_src->second.out_edges_.Insert(id);

    it_dst->second.in_edges_.Insert(id);

    return std::make_pair(EdgePtr(Edge(this->inner_graph_ptr_, *ret1.first)), true);
  }

  EdgePtr FindEdge(const EdgeIDType &id) {
    auto it = this->edges().Find(id);
    if (it == this->edges().end()) return EdgePtr();
    return EdgePtr(Edge(this->inner_graph_ptr_, *it));
  }

  EdgeConstPtr FindEdge(const EdgeIDType &id) const {
    auto it = this->edges().Find(id);
    if (it == this->edges().end()) return EdgeConstPtr();
    return EdgeConstPtr(ConstEdge(this->inner_graph_ptr_, *it));
  }

  EdgeIterator EdgeBegin() {
    return EdgeIterator(this->inner_graph_ptr_, 
                        this->edges().begin(), 
                        this->edges().end());
  }

  EdgeConstIterator EdgeBegin() const {
    return EdgeConstIterator(this->inner_graph_ptr_, 
                             this->edges().cbegin(), 
                             this->edges().cend());
  }

  size_t EraseEdge(const EdgeIDType &id) {
    auto it1 = this->edges().Find(id);
    if (it1 == this->edges().end()) return 0;

    auto &e_data = it1->second;

    auto it2 = this->vertices().Find(e_data.src_);
    auto &src_data = it2->second;
    bool res = src_data.out_edges_.Erase(id);
    assert(res);

    auto it3 = this->vertices().Find(e_data.dst_);
    auto &dst_data = it3->second;
    res = dst_data.in_edges_.Erase(id);
    assert(res);

    this->edges().Erase(it1);
    return 1;
  }

  void Clear() {
    this->vertices().Clear();
    this->edges().Clear();
  }

 private:
  // VertexContainer vertices_;
  // EdgeContainer edges_;

  InnerGraphContainer* inner_graph_ptr_;
};

}  // namespace GUNDAM

#endif