#ifndef _GRAPH_H
#define _GRAPH_H

#include "graph_configure.h"
#include "container.h"
#include "iterator.h"
#include "label.h"

#include <map>

namespace GUNDAM {
template <typename... configures>
class Graph {
 private:
  using Configures = GraphConfigures<configures...>;

  using VertexIDType              = typename Configures::VertexIDType;
  using VertexLabelType           = typename Configures::VertexLabelType;
  using VertexAttributeKeyType    = typename Configures::VertexAttributeKeyType;
  using VertexStaticAttributeType = typename Configures::VertexStaticAttributeType;
  using EdgeIDType                = typename Configures::EdgeIDType;
  using EdgeLabelType             = typename Configures::EdgeLabelType;
  using EdgeAttributeKeyType      = typename Configures::EdgeAttributeKeyType;
  using EdgeStaticAttributeType   = typename Configures::EdgeStaticAttributeType;

  static constexpr enum StoreData store_data = Configures::store_data;

  static constexpr bool vertex_label_is_const
          = Configures::vertex_label_is_const;
  static constexpr bool vertex_attribute_is_const
          = Configures::vertex_attribute_is_const;
  static constexpr bool vertex_has_static_attribute
          = Configures::vertex_has_static_attribute;
  static constexpr bool vertex_has_dynamic_attribute
          = Configures::vertex_has_dynamic_attribute;
  static constexpr bool edge_label_is_const
          = Configures::edge_label_is_const;
  static constexpr bool edge_attribute_is_const
          = Configures::edge_attribute_is_const;
  static constexpr bool edge_has_static_attribute
          = Configures::edge_has_static_attribute;
  static constexpr bool edge_has_dynamic_attribute
          = Configures::edge_has_dynamic_attribute;

  static constexpr enum ContainerType vertex_attribute_container_type
                        = Configures::vertex_attribute_container_type;
  static constexpr enum ContainerType   edge_attribute_container_type
                          = Configures::edge_attribute_container_type;
  static constexpr enum ContainerType  decomposed_edge_container_type
                         = Configures::decomposed_edge_container_type;
  static constexpr enum ContainerType       vertex_ptr_container_type
                              = Configures::vertex_ptr_container_type;
  static constexpr enum ContainerType       edge_label_container_type
                              = Configures::edge_label_container_type;
  static constexpr enum ContainerType        vertex_id_container_type
                               = Configures::vertex_id_container_type;
  static constexpr enum ContainerType     vertex_label_container_type
                            = Configures::vertex_label_container_type;

  static constexpr enum SortType vertex_attribute_container_sort_type
                   = Configures::vertex_attribute_container_sort_type;
  static constexpr enum SortType   edge_attribute_container_sort_type
                     = Configures::edge_attribute_container_sort_type;
  static constexpr enum SortType  decomposed_edge_container_sort_type
                    = Configures::decomposed_edge_container_sort_type;
  static constexpr enum SortType       vertex_ptr_container_sort_type
                         = Configures::vertex_ptr_container_sort_type;
  static constexpr enum SortType       edge_label_container_sort_type
                         = Configures::edge_label_container_sort_type;
  static constexpr enum SortType        vertex_id_container_sort_type
                          = Configures::vertex_id_container_sort_type;
  static constexpr enum SortType     vertex_label_container_sort_type
                       = Configures::vertex_label_container_sort_type;

  using TupleIdxType = uint8_t;

  class InnerVertex_;

  /// transparent to programmers
  template<typename  ContainerType_,
           bool           is_const_,
           IteratorDepthType depth_,
           TupleIdxType    ptr_idx_>
  class VertexContentIterator_
    : protected InnerIterator_<ContainerType_,
                                    is_const_,
                                       depth_>{
   private:
    using VertexPtr = typename InnerVertex_::VertexPtr;
    using InnerIteratorType = InnerIterator_<ContainerType_,
                                                  is_const_,
                                                     depth_>;

   protected:
    using InnerIteratorType::ToNext;
    using InnerIteratorType::IsDone;

   public:
    using InnerIteratorType::InnerIteratorType;

    using ContentPtr = VertexPtr;

    inline ContentPtr content_ptr(){
      return std::get<ptr_idx_>(
             InnerIteratorType::template get<depth_-1>());
    }
  };

 public:
  /// the class that provides the basic information about both vertex and edge
  template <typename IDType_,
            typename LabelType_,
            bool has_static_attribtue_,
            typename StaticAttributeType_,
            bool has_dynamic_attribtue_,
            typename AttributeKeyType_>
  class Type_ {
    /// trick compiler, same to static_assert(false);
    static_assert( has_static_attribtue_, "Unsupported configuration");
    static_assert(!has_static_attribtue_, "Unsupported configuration");
  };
  template <typename IDType_,
            typename LabelType_,
            typename StaticAttributeType_,
            typename AttributeKeyType_>
  class Type_<IDType_,
              LabelType_,
              false,
              StaticAttributeType_,
              false,
              AttributeKeyType_> {
   public:
    using    IDType = IDType_;
    using LabelType = LabelType_;
  };
  template <typename IDType_,
            typename LabelType_,
            typename StaticAttributeType_,
            typename AttributeKeyType_>
  class Type_<IDType_,
              LabelType_,
              true,
              StaticAttributeType_,
              false,
              AttributeKeyType_>
    : public Type_<IDType_,
                   LabelType_,
                   false,
                   StaticAttributeType_,
                   false,
                   AttributeKeyType_> {
   public:
    using StaticAttributeType = StaticAttributeType_;
  };
  template <typename IDType_,
            typename LabelType_,
            bool has_static_attribtue_,
            typename StaticAttributeType_,
            typename AttributeKeyType_>
  class Type_<IDType_,
              LabelType_,
              has_static_attribtue_,
              StaticAttributeType_,
              true,
              AttributeKeyType_>
    : public Type_<IDType_,
                   LabelType_,
                   has_static_attribtue_,
                   StaticAttributeType_,
                   false,
                   AttributeKeyType_> {
   public:
    using AttributeKeyType = AttributeKeyType_;
  };

 private:
  template<typename IDType_>
  class WithID_{
   private:
    const IDType_ id_;
   public:
    WithID_(const IDType_& id):id_(id){
      return;
    }
    inline const IDType_& id() const{
      return this->id_;
    }
  };

  template<typename LabelType_,
           bool     is_const_>
  class WithLabel_{
   private:
    typename std::conditional<is_const_,
                       const LabelType_,
                             LabelType_>::type label_;
   public:
    WithLabel_(const LabelType_& label)
                         :label_(label){
      return;
    }
    inline const LabelType_& label() const{
      return this->label_;
    }
    template<bool judge = is_const_,
             typename std::enable_if<!judge,bool>::type = false>
    inline void set_label(const LabelType_& label){
      static_assert(judge == is_const_,
                   "Illegal usage of this method");
      this->label_ = label;
      return;
    }
  };

  template<typename AttributeType_,
           bool     is_const_,
           bool     is_dynamic_,
           typename KeyType_,
           enum ContainerType container_type_,
           enum      SortType      sort_type_>
  class WithAttribute_;

  /// non-dynamic attribute
  template<typename AttributeType_,
           bool     is_const_,
           typename KeyType_,
           enum ContainerType container_type_,
           enum      SortType      sort_type_>
  class WithAttribute_<AttributeType_,
                            is_const_,
                                false,
                             KeyType_,
                      container_type_,
                           sort_type_>{
   private:
    typename std::conditional<is_const_,
                   const AttributeType_,
                         AttributeType_>::type attribute_;
   public:
    WithAttribute_(const AttributeType_& attribute)
                            :attribute_( attribute){
      return;
    }

    template<bool judge_is_const = is_const_,
             typename std::enable_if<!judge_is_const,bool>::type = false>
    WithAttribute_():attribute_(){
      static_assert(judge_is_const == is_const_,
                   "Illegal usage of this method");
      /// the constant non-dynamic attribute should be
      /// initialized in the construct method
      return;
    }

    inline const AttributeType_& const_attribute() const{
      return this->attribute_;
    }

    template<bool judge_is_const = is_const_,
             typename std::enable_if<!judge_is_const,bool>::type = false>
    inline AttributeType_& attribute(){
      static_assert(judge_is_const == is_const_,
                   "Illegal usage of this method");
      /// the constant attribute should not be modified
      return this->attribute_;
    }

    template<bool judge_is_const = is_const_,
             typename std::enable_if<!judge_is_const,bool>::type = false>
    inline void set_attribute(const AttributeType_& attribute){
      static_assert(judge_is_const == is_const_,
                   "Illegal usage of this method");
      /// only the non-constant non-dynamic attribute could be set
      this->attribute_ = attribute;
      return;
    }
  };

  /// dynamic attribute, holds a container for the attributes instead of
  /// a single attribute
  template<typename AttributeType_,
           bool     is_const_,
           typename KeyType_,
           enum ContainerType container_type_,
           enum      SortType      sort_type_>
  class WithAttribute_<AttributeType_,
                            is_const_,
                                 true,
                             KeyType_,
                      container_type_,
                           sort_type_>{
   private:
    static constexpr TupleIdxType kAttributeIdx = 0;
    Container<container_type_,
                   sort_type_,
           Attribute<KeyType_>> attributes_;

   public:
    WithAttribute_():attributes_(){
      return;
    }

    template<typename ConcreteDataType>
    inline const ConcreteDataType&
                 const_attribute(const KeyType_& key) const{
      /// the constant attribute should not be modified
      const Attribute<KeyType_> attribute_to_find(key);
      /// <iterator of attribute container, bool>
      auto ret = this->attributes_.FindConst(attribute_to_find);
      assert(ret.second); /// this key should exist
      return (std::get<kAttributeIdx>(*(ret.first))).const_value();
    }

    template<typename ConcreteDataType>
    inline ConcreteDataType& attribute(const KeyType_& key){
      /// the constant attribute should not be modified
      const Attribute<KeyType_> attribute_to_find(key);
      /// <iterator of attribute container, bool>
      auto ret = this->attributes_.Find(attribute_to_find);
      assert(ret.second); /// this key should exist
      return (std::get<kAttributeIdx>(*(ret.first))).value();
    }

    template<typename ConcreteDataType>
    inline bool add_attribute(const KeyType_& key,
                              const ConcreteDataType& value){
      /// the constant attribute should not be modified
      const Attribute<KeyType_> attribute_to_find(key);
      /// <iterator of attribute container, bool>
      auto ret = this->attributes_.Insert(attribute_to_find);
      if (!ret.second){
        /// has already existed in the Container
        return false;
      }
      ret.first->set_value(value);
      return true;
    }

    template<typename ConcreteDataType>
    inline bool set_attribute(const KeyType_& key,
                              const ConcreteDataType& value){
      /// the constant attribute should not be modified
      const Attribute<KeyType_> attribute_to_find(key);
      /// <iterator of attribute container, bool>
      auto ret = this->attributes_.Find(attribute_to_find);
      if (!ret.second){
        /// does not exist in the Container
        return false;
      }
      ret.first->set_value(value);
      return true;
    }

    /// not be implemented in this beggar version
//    inline bool remove_attribute(const KeyType_& key){
//      static_assert(judge_is_const == is_const_,
//                   "Illegal usage of this method");
//      return this->attributes_.remove_attribute(key);
//    }
  };

  using EdgeAttributeType = WithAttribute_<EdgeStaticAttributeType,
                                           edge_attribute_is_const,
                                           edge_has_dynamic_attribute,
                                           EdgeAttributeKeyType,
                                           edge_attribute_container_type,
                                           edge_attribute_container_sort_type>;

  using VertexWithIDType        = WithID_<
                                      VertexIDType>;
  using VertexWithLabelType     = WithLabel_<
                                      VertexLabelType,
                                      vertex_label_is_const>;
  using VertexWithAttributeType = WithAttribute_<
                                      VertexStaticAttributeType,
                                      vertex_attribute_is_const,
                                      vertex_has_dynamic_attribute,
                                      VertexAttributeKeyType,
                                      vertex_attribute_container_type,
                                      vertex_attribute_container_sort_type>;
  /// this class is transparent to user
  class InnerVertex_ : public VertexWithIDType,
                       public VertexWithLabelType,
                       public VertexWithAttributeType {
   public:
    template <bool is_const_>
    class VertexPtr_ {
     private:
      using VertexPtrType = typename std::conditional<is_const_,
                                             const InnerVertex_*,
                                                   InnerVertex_*>::type;
      VertexPtrType vertex_ptr_;

     public:
      VertexPtr_() : vertex_ptr_(nullptr) {
        return;
      }

      VertexPtr_(VertexPtrType const vertex_ptr)
                       : vertex_ptr_(vertex_ptr) {
        return;
      }

      inline VertexPtr_& operator=(const VertexPtr_& ptr){
        this->vertex_ptr_ = ptr.vertex_ptr_;
        return *this;
      }

      template <const bool judge = is_const_,
                typename std::enable_if<judge, bool>::type = false>
      inline VertexPtrType operator->() const {
        static_assert(judge == is_const_, "Illegal usage of this method");
        return this->vertex_ptr_;
      }

      template <const bool judge = is_const_,
                typename std::enable_if<!judge, bool>::type = false>
      inline VertexPtrType operator->() {
        static_assert(judge == is_const_, "Illegal usage of this method");
        return this->vertex_ptr_;
      }

      inline bool IsNull() const {
        return this->vertex_ptr_ == nullptr;
      }
    };

    using VertexPtr = VertexPtr_<false>;

   private:
    static constexpr TupleIdxType kEdgeIDIdx = 0;
    static constexpr TupleIdxType kEdgeAttributePtrIdx = 1;
    using DecomposedEdgeContainerType =
        Container<decomposed_edge_container_type,
                  decomposed_edge_container_sort_type,
                  EdgeIDType,
                  EdgeAttributeType*>;

    static constexpr TupleIdxType kVertexPtrIdx = 0;
    static constexpr TupleIdxType kDecomposedEdgeContainerIdx = 1;
    using VertexPtrContainerType =
        Container<vertex_ptr_container_type,
                  vertex_ptr_container_sort_type,
                  VertexPtr,
                  DecomposedEdgeContainerType>;

    static constexpr TupleIdxType kEdgeLabelIdx = 0;
    static constexpr TupleIdxType kVertexPtrContainerIdx = 1;
    using EdgeLabelContainerType =
        Container<edge_label_container_type,
                  edge_label_container_sort_type,
                  EdgeLabelType,
                  VertexPtrContainerType>;

//    static constexpr TupleIdxType kEdgeIsOutputIdx = 0;
//    static constexpr TupleIdxType kEdgeLabelContainerIdx = 1;
//    using EdgeContainerType =
//        Container<ContainerType::Vector,
//                  SortType::None,
//                  bool,
//                  EdgeLabelContainerType>;

    template <bool is_const_,
              bool meaning_less_ = true>
    class EdgePtrContent_;

    /// constant content pointer
    template <bool meaning_less_>
    class EdgePtrContent_<true,meaning_less_> {
      static_assert(meaning_less_,"Illegal usage of this method");
     private:
      using EdgeLabelIteratorType
          = typename EdgeLabelContainerType::const_iterator;
      using VertexPtrIteratorType
          = typename VertexPtrContainerType::const_iterator;
      using DecomposedEdgeIteratorType
          = typename DecomposedEdgeContainerType::const_iterator;

     protected:
      const VertexPtr vertex_ptr_;

      const      EdgeLabelIteratorType      edge_label_iterator_;
      const      VertexPtrIteratorType      vertex_ptr_iterator_;
      const DecomposedEdgeIteratorType decomposed_edge_iterator_;

     protected:
      inline bool IsNull() const{
        return this->vertex_ptr_.IsNull();
      }

     public:
      EdgePtrContent_():vertex_ptr_(),
               edge_label_iterator_(),
               vertex_ptr_iterator_(),
          decomposed_edge_iterator_(){
        return;
      }

      EdgePtrContent_(const VertexPtr& vertex_ptr,
          const      EdgeLabelIteratorType&      edge_label_iterator,
          const      VertexPtrIteratorType&      vertex_ptr_iterator,
          const DecomposedEdgeIteratorType& decomposed_edge_iterator)
                        : vertex_ptr_(vertex_ptr),
                 edge_label_iterator_(edge_label_iterator),
                 vertex_ptr_iterator_(vertex_ptr_iterator),
            decomposed_edge_iterator_(decomposed_edge_iterator) {
        return;
      }

      /// public methods same to ContentEdgeIterator_
      inline const EdgeLabelType& label() const {
        return std::get<kEdgeLabelIdx>
                  (*(this->edge_label_iterator_));
      }
      inline const EdgeIDType& id() const {
        return std::get<kEdgeIDIdx>
                  (*(this->decomposed_edge_iterator_));
      }
      inline const VertexPtr& src_ptr() const {
        return this->vertex_ptr_;
      }
      inline const VertexPtr& dst_ptr() const {
        return std::get<kVertexPtrIdx>
                  (*(this->vertex_iterator_));
      }

      template <typename ConcreteDataType>
      inline const ConcreteDataType& const_attribute(
                                 const EdgeAttributeKeyType& key) const {
        return (std::get<kEdgeAttributePtrIdx>
                  (*(this->decomposed_edge_iterator_)))
                              ->const_attribute(key);
      }

      template <typename ConcreteDataType>
      inline bool add_attribute(const EdgeAttributeKeyType& key,
                                const     ConcreteDataType& value) {
        return (std::get<kEdgeAttributePtrIdx>
                  (*(this->decomposed_edge_iterator_)))
                                ->add_attribute(key, value);
      }
    };

    /// non-constant content pointer
    template <bool meaning_less_>
    class EdgePtrContent_<false,meaning_less_>
      : public EdgePtrContent_<true,meaning_less_> {
      static_assert( meaning_less_,"Illegal usage of this method");
     public:
      /// same to ContentEdgeIterator_
      VertexPtr& src_ptr();
      VertexPtr& dst_ptr();

      template <typename ConcreteDataType>
      inline ConcreteDataType& attribute(const EdgeAttributeKeyType& key);

      template <typename ConcreteDataType>
      inline bool set_attribute(const EdgeAttributeKeyType& key,
                                const     ConcreteDataType& value);

      inline bool remove_attribute(const EdgeAttributeKeyType& key);
    };

   public:
    template <bool is_const_>
    class EdgePtr_: protected EdgePtrContent_<is_const_, true> {
     private:
      using EdgeContentType = EdgePtrContent_<is_const_, true>;

     public:
      using EdgeContentType::EdgeContentType;
      using EdgeContentType::IsNull;

      template<bool judge = is_const_,
               typename std::enable_if<judge,bool>::type = false>
      inline EdgeContentType* operator->() const{
        static_assert(judge == is_const_,
                     "Illegal usage of this method");
        EdgePtr_* const temp_ptr = this;
        return temp_ptr;
      }

      template<bool judge = is_const_,
               typename std::enable_if<!judge,bool>::type = false>
      inline EdgeContentType* operator->(){
        static_assert(judge == is_const_,
                     "Illegal usage of this method");
        EdgePtr_* const temp_ptr = this;
        return temp_ptr;
      }
    };

    using EdgePtr = EdgePtr_<false>;

   private:
    template <enum StoreData store_data,
              bool meaning_less = true>
    class Edges;

    template <bool meaning_less>
    class Edges<StoreData::OutEdge,
                meaning_less> {
     private:
      static_assert(meaning_less,"Illegal configuration");
      EdgeLabelContainerType out_edges_;

     public:
      inline const EdgeLabelContainerType& const_out_edges() const {
        return this->out_edges_;
      }
      inline EdgeLabelContainerType& out_edges() {
        return this->out_edges_;
      }
    };

    template <bool meaning_less>
    class Edges<StoreData::InOutEdge,meaning_less>
        : public Edges<StoreData::OutEdge,meaning_less> {
     private:
      static_assert(meaning_less,"Illegal configuration");
      EdgeLabelContainerType in_edges_;

     public:
      inline const EdgeLabelContainerType& const_in_edges() const {
        return this->in_edges_;
      }
      inline EdgeLabelContainerType& in_edges() {
        return this->in_edges_;
      }
    };

    Edges<store_data> edges_;

    void AddInEdge(const VertexPtr& dst_ptr,
                   const EdgeLabelType& edge_label,
                   const EdgeIDType& edge_id,
                   EdgeAttributeType* const edge_attribute_ptr) {
      auto edge_label_it = this->edges_.out_edges()
                                       .Insert(edge_label)
                                       .first;
      auto vertex_ptr_it = std::get<kVertexPtrContainerIdx>(*edge_label_it)
                          .Insert(dst_ptr)
                          .first;
      auto edge_it = std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it)
                    .Insert(edge_id)
                    .first;
      std::get<kEdgeAttributePtrIdx>(*edge_it) = edge_attribute_ptr;
      return;
    }

    void AddOutEdge(const VertexPtr& dst_ptr,
                    const EdgeLabelType& edge_label,
                    const EdgeIDType& edge_id,
                    EdgeAttributeType* const edge_attribute_ptr) {
      auto edge_label_it = this->edges_.out_edges()
                                       .Insert(edge_label)
                                       .first;
      auto vertex_ptr_it = std::get<kVertexPtrContainerIdx>(*edge_label_it)
                          .Insert(dst_ptr)
                          .first;
      auto edge_it = std::get<kDecomposedEdgeContainerIdx>(*vertex_ptr_it)
                    .Insert(edge_id)
                    .first;
      std::get<kEdgeAttributePtrIdx>(*edge_it) = edge_attribute_ptr;
      InnerVertex_* temp_this_ptr = this;
      VertexPtr temp_vertex_ptr(temp_this_ptr);
      return EdgePtr(temp_vertex_ptr, /// src_ptr
                       edge_label_it,
                       vertex_ptr_it,
                             edge_it);
    }

   public:
    InnerVertex_(const VertexIDType&    id,
                 const VertexLabelType& label)
                      :VertexWithIDType(id),
                       VertexWithLabelType(label),
                       VertexWithAttributeType(){
      return;
    }

    inline EdgePtr AddEdge(const VertexPtr& dst_ptr,
                           const EdgeLabelType& edge_label,
                           const EdgeIDType& edge_id) {
      EdgeAttributeType* edge_attribute_ptr = new EdgeAttributeType();
      InnerVertex_* const temp_this_ptr = this;
      VertexPtr     const temp_vertex_ptr = VertexPtr(temp_this_ptr);
      dst_ptr->AddInEdge(temp_vertex_ptr,
                         edge_label,
                         edge_id,
                         edge_attribute_ptr);
      return this->AddOutEdge(dst_ptr,
                              edge_label,
                              edge_id,
                              edge_attribute_ptr);
    }
    /// possible extension:
    ///     AddEdge(dst_ptr,edge_label);
    ///         if duplicate edge is not allowed

//   public:
//    template <...>
//    class EdgeLabelIterator_ : protected ContentEdgeLabelIterator_<...> {
//     public:
//      inline EdgeLabelIterator_   operator++();
//      inline EdgeLabelIterator_   operator++(int);
//      inline const EdgeLabelType& operator*() const;
//      inline bool IsDone() const;
//    };
//
//    template <...>
//    class VertexIterator_ : protected ContentVertexIterator_<...> {
//     public:
//      inline VertexIterator_ operator++();
//      inline VertexIterator_ operator++(int);
//      inline VertexPtrType   operator->();
//      inline bool IsDone() const;
//    };
//
//   private:
//    template <bool is_const_, ...>
//    class ContentEdgeIterator_;
//
//    template <...>
//    class ContentEdgeIterator_<true, ...> {
//     public:
//      /// same to EdgePtrContent_
//      const EdgeIDType& id() const;
//      const EdgeLabelType& label() const;
//      const VertexPtr<true>& const_src_ptr() const;
//      const VertexPtr<true>& const_dst_ptr() const;
//
//      template <typename ConcreteDataType>
//      inline const ConcreteDataType& const_attribute(const KeyType_& key) const;
//
//      template <typename ConcreteDataType>
//      inline bool add_attribute(const KeyType_& key,
//                                const ConcreteDataType& value);
//    };
//
//    template <...>
//    class ContentEdgeIterator_<false, ...>
//        : public ContentEdgeIterator_<true, ...> {
//     public:
//      /// same to EdgePtrContent_
//      EdgeAttributeType& attribute();
//      VertexPtr<false>& src_ptr();
//      VertexPtr<false>& dst_ptr();
//
//      template <typename ConcreteDataType>
//      inline ConcreteDataType& attribute(const KeyType_& key);
//
//      template <typename ConcreteDataType>
//      inline bool set_attribute(const KeyType_& key,
//                                const ConcreteDataType& value);
//
//      inline bool remove_attribute(const KeyType_& key);
//    };
//
//   public:
//    template <...>
//    class EdgeIterator_ : protected ContentEdgeIterator_<...> {
//     public:
//      inline EdgeIterator_ operator++();
//      inline EdgeIterator_ operator++(int);
//      inline ContentEdgeIterator_ operator->();
//      inline bool IsDone() const;
//    };


   public:
    /// access auxiliary data, will not be implement in this beggar version
//    const VertexAuxiliaryType& const_auxiliary() const;
//    VertexAuxiliaryType& auxiliary();

    /// access element, will not be implement in this beggar version
    //            EdgeLabelConstPtr FindConstOutEdgeLabel(const EdgeLabelType&
    //            edge_label) const; EdgeLabelConstPtr
    //            FindConstInEdgeLabel(const EdgeLabelType& edge_label) const;

   private:
    VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                         const VertexPtr& dst_ptr) {
      for (auto& edge_label_it : edge_label_container) {
        /// <iterator, bool>
        auto ret = std::get<kVertexPtrContainerIdx>
                      (edge_label_it).Find(dst_ptr);
        if (ret.second) {
          /// found it
          return *(ret.first);
        }
      }
      return VertexPtr();
    }

    VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                         const VertexIDType& dst_id) {
      for (auto& edge_label_it : edge_label_container) {
        for (auto& vertex_it : std::get<kVertexPtrContainerIdx>
                                                (edge_label_it)) {
          VertexPtr vertex_ptr = std::get<kVertexPtrIdx>(vertex_it);
          if (vertex_ptr->id() == dst_id)
            return vertex_ptr; /// found it
        }
      }
      /// not found
      return VertexPtr();
    }

    inline VertexPtr FindVertex(
              EdgeLabelContainerType& edge_label_container,
                 const EdgeLabelType& edge_label,
                 const VertexPtr& dst_ptr) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second)  /// does not have edge_label
        return VertexPtr();
      /// <iterator of VertexContainer, bool>
      auto vertex_ret = edge_label_ret.first->Find(dst_ptr);
      if (!vertex_ret.second)
        return VertexPtr();
      /// found it
      return *(vertex_ret.first);
    }

    VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                         const EdgeLabelType& edge_label,
                         const VertexIDType& dst_id) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second)  /// does not have edge_label
        return VertexPtr();
      for (auto& vertex_it : std::get<kVertexPtrContainerIdx>
                                     (*edge_label_ret.first)) {
        VertexPtr vertex_ptr = std::get<kVertexPtrIdx>(vertex_it);
        if (vertex_ptr->id() == dst_id)
          return vertex_ptr; /// found it
      }
      /// not found
      return VertexPtr();
    }

   public:
    inline VertexPtr FindOutVertex(const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.out_edges(), dst_ptr);
    }
    inline VertexPtr FindOutVertex(const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.out_edges(), dst_id);
    }
    inline VertexPtr FindOutVertex(const EdgeLabelType& edge_label,
                                   const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.out_edges(), edge_label, dst_ptr);
    }
    inline VertexPtr FindOutVertex(const EdgeLabelType& edge_label,
                                   const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.out_edges(), edge_label, dst_id);
    }
    inline VertexPtr FindInVertex(const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.in_edges(), dst_ptr);
    }
    inline VertexPtr FindInVertex(const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.in_edges(), dst_id);
    }
    inline VertexPtr FindInVertex(const EdgeLabelType& edge_label,
                                  const VertexPtr& dst_ptr) {
      return this->FindVertex(this->edges_.in_edges(), edge_label, dst_ptr);
    }
    inline VertexPtr FindInVertex(const EdgeLabelType& edge_label,
                                  const VertexIDType& dst_id) {
      return this->FindVertex(this->edges_.in_edges(), edge_label, dst_id);
    }

    /// will not be implemented in the beggar version
    //            VertexConstPtr FindConstOutVertex(const VertexPtr& dst_ptr)
    //            const; VertexConstPtr FindConstOutVertex(const VertexIDType&
    //            dst_id ) const; VertexConstPtr FindConstOutVertex(const
    //            EdgeLabelType& edge_label,
    //                                              const VertexPtr& dst_ptr)
    //                                              const;
    //            VertexConstPtr FindConstOutVertex(const EdgeLabelType&
    //            edge_label,
    //                                              const VertexIDType& dst_id
    //                                              ) const;
    //            VertexConstPtr FindConstInVertex(const VertexPtr& dst_ptr)
    //            const; VertexConstPtr FindConstInVertex(const VertexIDType&
    //            dst_id ) const; VertexConstPtr FindConstInVertex(const
    //            EdgeLabelType& edge_label,
    //                                             const VertexPtr& dst_ptr)
    //                                             const;
    //            VertexConstPtr FindConstInVertex(const EdgeLabelType&
    //            edge_label,
    //                                             const VertexIDType&  dst_id
    //                                             ) const;

   private:
    EdgePtr FindEdge(EdgeLabelContainerType & edge_label_container,
                     const EdgeLabelType& edge_label,
                     const VertexPtr& dst_ptr,
                     const EdgeIDType& edge_id) {
      /// <iterator of EdgeLabelContainer, bool>
      auto edge_label_ret = edge_label_container.Find(edge_label);
      if (!edge_label_ret.second)  /// does not have this edge label
        return EdgePtr();
      /// <iterator of VertexContainer, bool>
      auto vertex_ret = edge_label_ret.first->Find(dst_ptr);
      if (!vertex_ret.second)  /// does not have this VertexPtr
        return EdgePtr();
      /// <iterator of DecomposedEdgeContainer, bool>
      auto decomposed_edge_ret = vertex_ret.first->Find(edge_id);
      if (!decomposed_edge_ret)  /// does not find it
        return EdgePtr();
      return EdgePtr(edge_label_ret.first, vertex_ret.first,
                     decomposed_edge_ret.first);
    }

   public:
    inline EdgePtr FindOutEdge(const EdgeLabelType& edge_label,
                               const VertexPtr& dst_ptr,
                               const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.out_edges(),
                            edge_label,
                            dst_ptr,
                            edge_id);
    }
    /// possible extension:
    ///     FindOutEdge(edge_label,dst_ptr)
    ///         when there are not duplicate edge
    ///     FindOutEdge(dst_ptr)
    ///         when there are not duplicate edge
    ///         and only has one edge label type

    inline EdgePtr FindInEdge(const EdgeLabelType& edge_label,
                              const VertexPtr& dst_ptr,
                              const EdgeIDType& edge_id) {
      return this->FindEdge(this->edges_.in_edges(),
                            edge_label,
                            dst_ptr,
                            edge_id);
    }
    /// possible extension:
    ///     FindInEdge(edge_label,dst_ptr)
    ///         when there are not duplicate edge
    ///     FindInEdge(dst_ptr)
    ///         when there are not duplicate edge
    ///         and only has one edge label type

    /// will not be implemented in the beggar version
    //            EdgeConstPtr FindConstOutEdge(const VertexPtr& dst_ptr);
    //            EdgeConstPtr FindConstInEdge(const VertexPtr& dst_ptr);

      /// iterate element:
      /// output iterators:

    using VertexIterator
              = Iterator_<VertexContentIterator_<
                          EdgeLabelContainerType,false,2,
                         kVertexPtrIdx>>;
    using VertexIteratorSpecifiedEdgeLabel
              = Iterator_<VertexContentIterator_<
                          VertexPtrContainerType,false,1,
                         kVertexPtrIdx>>;
//    inline EdgeLabelIterator OutEdgeLabelBegin() const;

    inline VertexIterator OutVertexBegin(){
      return VertexIterator(this->edges_.out_edges().begin(),
                            this->edges_.out_edges().end());
    }
    inline VertexIteratorSpecifiedEdgeLabel
        OutVertexBegin(const EdgeLabelType& edge_label){
      /// <iterator of EdgeLabelContainer, bool>
      auto ret = this->edges_.out_edges().Find(edge_label);
      if (!ret.second){
        /// not find
        return VertexIteratorSpecifiedEdgeLabel();
      }
      return VertexIteratorSpecifiedEdgeLabel(
          (std::get<kVertexPtrContainerIdx>(*(ret.first))).begin(),
          (std::get<kVertexPtrContainerIdx>(*(ret.first))).end());
    }
    inline VertexIterator InVertexBegin(){
      return VertexIterator(this->edges_.in_edges().begin(),
                            this->edges_.in_edges().end());
    }
//    inline VertexIterator InVertexBegin(){
//      return VertexIterator(this->edges_.in_edges().begin(),
//                            this->edges_.in_edges().end());
//    }

//    VertexConstIterator OutVertexConstBegin() const;
//    /// possible extension:
//    ///     OutVertexBegin(edge_label)
//    ///     OutVertexBegin(edge_label,vertex_label)
//    ///     ...
//
//    EdgeIterator OutEdgeBegin();
//    EdgeConstIterator OutEdgeConstBegin() const;
//    /// possible extension:
//    ///     OutEdgeBegin(edge_label)
//    ///     OutEdgeBegin(edge_label,vertex_label)
//    ///     ...
//
//    /// input iterators:
//    EdgeLabelIterator InEdgeLabelBegin() const;
//
//    VertexConstIterator InVertexConstBegin() const;
//    /// possible extension:
//    ///     InVertexBegin(edge_label)
//    ///     InVertexBegin(edge_label,vertex_label)
//    ///     ...
//
//    EdgeIterator InEdgeBegin();
//    EdgeConstIterator InEdgeConstBegin() const;
//    /// possible extension:
//    ///     InEdgeBegin(edge_label)
//    ///     InEdgeBegin(edge_label,vertex_label)
//    ///     ...
    };

    using InnerVertexType = InnerVertex_;

   public:
    using EdgeType = Type_<EdgeIDType,
                           EdgeLabelType,
                           edge_has_static_attribute,
                           EdgeStaticAttributeType,
                           edge_has_dynamic_attribute,
                           EdgeAttributeKeyType>;
    using VertexType = Type_<VertexIDType,
                             VertexLabelType,
                             vertex_has_static_attribute,
                             VertexStaticAttributeType,
                             vertex_has_dynamic_attribute,
                             VertexAttributeKeyType>;
    /// non-constant pointer
    using   EdgePtr = typename InnerVertexType::  EdgePtr;
    using VertexPtr = typename InnerVertexType::VertexPtr;
    /// constant pointer
    using   EdgeConstPtr = typename InnerVertexType::  EdgePtr_<true>;
    using VertexConstPtr = typename InnerVertexType::VertexPtr_<true>;

   private:
    static constexpr TupleIdxType kVertexIDIdx  = 0;
    static constexpr TupleIdxType kVertexPtrIdx = 1;
    using VertexIDContainerType
                = Container<vertex_id_container_type,
                            vertex_id_container_sort_type,
                            VertexIDType,
                            VertexPtr>;

    static constexpr TupleIdxType kVertexLabelIdx       = 0;
    static constexpr TupleIdxType kVertexIDContainerIdx = 1;
    using VertexLabelContainerType
                   = Container<vertex_label_container_type,
                               vertex_label_container_sort_type,
                               VertexLabelType,
                               VertexIDContainerType>;
   public:
    using VertexSizeType = typename VertexIDContainerType::size_type;

   private:
    VertexLabelContainerType vertexes_;

   public:
    VertexPtr AddVertex(const typename VertexType::IDType&    id,
                        const typename VertexType::LabelType& label){
      /// should not have already exist
      assert(this->FindVertex(id).IsNull());
      InnerVertex_* temp_inner_vertex_ptr = new InnerVertex_(id,label);
      VertexPtr temp_vertex_ptr(temp_inner_vertex_ptr);
      /// vertex label iterator
      auto vertex_label_it = this->vertexes_.Insert(label)
                                            .first;
      /// vertex ID iterator
      auto vertex_id_it = std::get<kVertexIDContainerIdx>
                                  (*vertex_label_it).Insert(id)
                                                    .first;
      std::get<kVertexPtrIdx>(*vertex_id_it) = temp_vertex_ptr;
      return std::get<kVertexPtrIdx>(*vertex_id_it);
    }
    /// possible variant:
    ///     AddVertex(id)
    ///     AddVertex(id,label)
    ///     AddVertex(id,label,attribute)
    ///     AddVertex(id,label,attribute0,attribute1,...)
    ///     AddVertex(id,attribute)
    ///     AddVertex(id,attribute0,attribute1,...)
    ///     ...

    EdgePtr AddEdge(const typename VertexType::   IDType&  src_id,
                    const typename VertexType::   IDType&  dst_id,
                    const typename   EdgeType::LabelType& edge_label,
                    const typename   EdgeType::   IDType& edge_id){
      VertexPtr src_ptr = this->FindVertex(src_id);
      VertexPtr dst_ptr = this->FindVertex(dst_id);
      assert(!src_ptr.IsNull());
      assert(!dst_ptr.IsNull());
      return src_ptr->AddEdge(dst_ptr,edge_label,
                                      edge_id);
    }
    /// possible variant:
    ///     AddEdge(src_id,dst_id)
    ///     AddEdge(src_id,dst_id,edge_label)
    ///     AddEdge(src_id,dst_id,edge_label,edge_id)
    ///     AddEdge(src_id,dst_id,edge_label,edge_id,edge_attribute)
    ///     ...

    VertexPtr FindVertex(const typename VertexType::IDType& id){
      for (auto& vertex_label_it : this->vertexes_){
        /// <iterator of ID container, bool>
        auto ret = std::get<kVertexIDContainerIdx>(vertex_label_it).Find(id);
        if (ret.second){
          /// found it
          return std::get<kVertexPtrIdx>(*(ret.first));
        }
      }
      /// not found
      return VertexPtr();
    }

    inline VertexPtr FindVertex(const typename VertexType::IDType&    id,
                                const typename VertexType::LabelType& label){
      /// <iterator of VertexLabelContainer, bool>
      auto vertex_label_ret = this->vertexes_.Find(label);
      if (!vertex_label_ret.second){
        /// not have this vertex label
        return VertexPtr();
      }
      auto vertex_id_ret = std::get<kVertexIDContainerIdx>
                               (*(vertex_label_ret.first)).Find(id);
      if (!vertex_id_ret.second){
        /// not have this vertex id
        return VertexPtr();
      }
      return std::get<kVertexPtrIdx>(*(vertex_id_ret.first));
    }

    /// will not be implemented in this beggar version
//    VertexConstPtr FindConstVertex(
//        typename const VertexType::IDType& vertex_id) const;
//    /// possible extension:
//    ///     FindVertex(vertex_label,vertex_id)
//    ///     ...
//    ///     FindConstVertex(vertex_label,vertex_id)
//    ///     ...

//    VertexIterator VertexBegin();
//    VertexConstIterator VertexConstBegin() const;
//    /// possible extension:
//    ///     VertexBegin(vertex_label)
//    ///     ...
//    ///     VertexConstBegin(vertex_label)
//    ///     ...

    VertexSizeType CountVertex() const{
      VertexSizeType vertex_num = 0;
      for (auto& vertex_label : this->vertexes_)
        vertex_num += std::get<kVertexIDContainerIdx>
                               (vertex_label).size();
      return vertex_num;
    }
    VertexSizeType CountVertex(
         const typename VertexType::LabelType& label) const{
      /// <iterator of VertexLabelContainer, bool>
      auto ret = this->vertexes_.FindConst(label);
      if (!ret.second){
        /// does not have this vertex label
        return 0;
      }
      return std::get<kVertexIDContainerIdx>(*(ret.first)).size();
    }

    using VertexIterator
              = Iterator_<VertexContentIterator_<
                          VertexLabelContainerType,false,2,
                         kVertexPtrIdx>>;
    using VertexIteratorSpecifiedLabel
              = Iterator_<VertexContentIterator_<
                          VertexIDContainerType,false,1,
                         kVertexPtrIdx>>;

    inline VertexIterator VertexBegin(){
      return VertexIterator(this->vertexes_.begin(),
                            this->vertexes_.end());
    }

    inline VertexIteratorSpecifiedLabel VertexBegin(
                        typename VertexType::LabelType label){
      /// <iterator of VertexLabelContainer, bool>
      auto ret = this->vertexes_.Find(label);
      if (!ret.second) /// does not have this vertex label
        return VertexIteratorSpecifiedLabel();
      return VertexIteratorSpecifiedLabel(
              std::get<kVertexIDContainerIdx>(*(ret.first)).begin(),
              std::get<kVertexIDContainerIdx>(*(ret.first)).end());
    }
  };
}  // namespace GUNDAM

#endif  // _GRAPH_HPP
