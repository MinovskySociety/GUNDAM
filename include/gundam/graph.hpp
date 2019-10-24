#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include <map>

#include "graph_configure.hpp"

template<typename   AttributeType_,
         const bool is_const_,
         const bool is_dynamic_,
         typename   KeyType_,
         const enum ContainerType container_type_>
class Attribute_;

template<typename   AttributeType_,
         typename   KeyType_,
         const enum ContainerType container_type_>
class Attribute_<AttributeType_,
                 true,
                 true,
                 KeyType_,
                 container_type_>{
public:
    template<typename ConcreteDataType>
    inline const ConcreteDataType&
           const_attribute(const KeyType_& key) const;

    template<typename ConcreteDataType>
    inline bool add_attribute(const KeyType_& key,
                              const ConcreteDataType& value);
};

/// dynamic attribute, holds a container for the attributes instead of
/// a single attribute
template<typename   AttributeType_,
         typename   KeyType_,
         const enum ContainerType container_type_>
class Attribute_<AttributeType_,
                 false,
                 true,
                 KeyType_,
                 container_type_>
    :public Attribute_<AttributeType_,
                       true,
                       true,
                       KeyType_,
                       container_type_>{
public:
    template<typename ConcreteDataType>
    inline ConcreteDataType& attribute(const KeyType_& key);

    template<typename ConcreteDataType>
    inline bool set_attribute(const KeyType_& key,
                              const ConcreteDataType& value);

    inline bool remove_attribute(const KeyType_& key);
};

template<enum    ContainerType container_type,
         enum         SortType      sort_type,
         typename      KeyType,
         typename... ValueType>
class Container{
    /// trick the compiler, same to static_assert(false)
    static_assert( std::is_object<KeyType>::value,
                 "Unsupported configuration");
    static_assert(!std::is_object<KeyType>::value,
                 "Unsupported configuration");
}

template<enum SortType sort_type,
         typename        KeyType,
         typename...   ValueType>
class Container<ContainerType::Vector,
                    sort_type,
                      KeyType,
                    ValueType...>{
public:
    using     KeyType = KeyType;
    using ElementType = std::tuple<KeyType,ValueType...>;

private:
    using InnerContainerType = std::vector<ElementType>;
    InnerContainerType container_;

    static_assert(sort_type == SortType::None,
                 "other sorting type are not supported yet");

public:
    using const_iterator = typename InnerContainerType::const_iterator;
    using       iterator = typename InnerContainerType::      iterator;
    using      size_type = typename InnerContainerType::     size_type;

    inline const_iterator cbegin() const{
        return this->container_.cbegin();
    }
    inline const_iterator cend() const{
        return this->container_.cend();
    }
    inline iterator begin(){
        return this->container_.begin();
    }
    inline iterator end(){
        return this->container_.end();
    }
    inline size_type size() const{
        return this->container_.size();
    }
    inline bool empty() const{
        return this->container_.empty();
    }
    inline void shrink_to_fit() const{
        this->container_.shrink_to_fit();
        return;
    }

    inline std::pair<iterator,bool> Insert(const KeyType& key){
        if (sort_type == SortType::None){
            for (iterator it  = this->container_.begin();
                          it != this->container_.end();++it)
                if (it->get<0> == key)
                    return std::pair<iterator,bool>(it,false);
            this->container_.emplace_back(std::tuple_cat(
                                          std::tuple<  KeyType   >(key),
                                          std::tuple<ValueType...>()));
            return std::pair<iterator,bool>
                  (this->container_.end() - 1,true);
        }
        /// other sorting type are not implemented
        assert(false);
        return std::pair<iterator,bool>
              (this->container_.end() - 1,true);
    }

    inline std::pair<iterator,bool>
        Find(const KeyType& key){
        if (sort_type == SortType::None){
            for (iterator it  = this->container_.begin();
                          it != this->container_. end (); ++it)
                if (it->get<0> == key)
                    return std::pair<iterator,bool>(it,true);
            return std::pair<iterator,bool>
              (this->container_.end(),false);
        }
        /// other sorting type are not implemented
        assert(false);
        return std::pair<iterator,bool>
          (this->container_.end(),false);
    }

    inline std::pair<const_iterator,bool>
        FindConst(const KeyType& key) const{
        if (sort_type == SortType::None){
            for (const_iterator it  = this->container_.cbegin();
                                it != this->container_. cend (); ++it)
                if (it->get<0> == key)
                    return std::pair<const_iterator,bool>(it,true);
            return std::pair<const_iterator,bool>
                   (this->container_.cend(),false);
        }
        /// other sorting type are not implemented
        assert(false);
        return std::pair<const_iterator,bool>
               (this->container_.cend(),false);
    }
};

namespace GUNDAM{
    template<typename... configures>
    class Graph{
    private:
        using Configures = GraphConfigures<configures...>;

        using VertexIDType           = typename Configures::VertexIDType;
        using VertexLabelType        = typename Configures::VertexLabelType;
        using VertexAttributeKeyType = typename Configures::VertexAttributeKeyType;
        using EdgeIDType             = typename Configures::EdgeIDType;
        using EdgeLabelType          = typename Configures::EdgeLabelType;
        using EdgeAttributeKeyType   = typename Configures::EdgeAttributeKeyType;

        /// the class that provides the basic information about data type
        template<bool has_attribtue_,
                 bool has_dynamic_attribtue_>
        class EdgeType_{
            static_assert( has_attribtue_,"Unsupported configuration");
            static_assert(!has_attribtue_,"Unsupported configuration");
        };
        template<>
        class EdgeType_<false,false>{
        public:
            using IDType    = EdgeIDType;
            using LabelType = EdgeLabelType;
        };
        template<>
        class EdgeType_<true,true>
            :public EdgeType_<false,false>{
        public:
            using AttributeKeyType = EdgeAttributeKeyType;
        };

        /// the class that provides the basic information about data type
        template<bool has_attribtue_,
                 bool has_dynamic_attribtue_>
        class VertexType_{
            static_assert( has_attribtue_,"Unsupported configuration");
            static_assert(!has_attribtue_,"Unsupported configuration");
        };
        template<>
        class VertexType_<false,false>{
        public:
            using IDType    = VertexIDType;
            using LabelType = VertexLabelType;
        };
        template<>
        class VertexType_<true,true>
            :public VertexType_<false,false>{
        public:
            using AttributeKeyType = VertexAttributeKeyType;
        };

        /// this class is transparent to user
        class InnerVertex_:public WithID_<...>,
                           public WithLabel_<...>,
                           public WithAttribute_<...>{
        private:
            static constexpr uint8_t kEdgeIDIdx           = 0;
            static constexpr uint8_t kEdgeAttributePtrIdx = 1;
            using DecomposedEdgeContainerType
                              = Container<decomposed_edge_container_type,
                                          decomposed_edge_container_sort_type,
                                          EdgeIDType,
                                         *EdgeAttributeType>;

            static constexpr uint8_t kVertexPtrIdx               = 0;
            static constexpr uint8_t kDecomposedEdgeContainerIdx = 1;
            using VertexContainerType
                      = Container<vertex_container_type,
                                  vertex_container_sort_type,
                                  VertexPtr,
                                  DecomposedEdgeContainerType>;

            static constexpr uint8_t kEdgeLabelIdx       = 0;
            static constexpr uint8_t kVertexContainerIdx = 1;
            using EdgeLabelContainerType
                         = Container<edge_label_container_type,
                                     edge_label_container_sort_type,
                                     EdgeLabelType,
                                     VertexContainerType>;

            template<enum StoreData>
            class Edges;

            template<>
            class Edges<StoreData::OutEdge>{
            private:
                EdgeLabelContainerType out_edges_;

            public:
                inline const EdgeLabelContainerType& const_out_edges() const{
                    return this->out_edges_;
                }
                inline EdgeLabelContainerType& out_edges(){
                    return this->out_edges_;
                }
            };

            template<>
            class Edges<StoreData::InOutEdge>
                :public Edges<StoreData::OutEdge>{
            private:
                EdgeLabelContainerType in_edges_;

            public:
                inline const EdgeLabelContainerType& const_in_edges() const{
                    return this->in_edges_;
                }
                inline EdgeLabelContainerType& in_edges(){
                    return this->in_edges_;
                }
            };

            Edges edges_;

            void AddInEdge(const VertexPtr&          dst_ptr,
                           const EdgeLabelType&     edge_label,
                           const EdgeIDType&        edge_id,
                           EdgeAttributeType* const edge_attribute_ptr){
                this->edges_.in_edges()
                            .Insert(edge_label)
                            .first->get<kVertexContainerIdx>
                            .Insert(dst_ptr)
                            .first->get<kDecomposedEdgeContainerIdx>
                            .Insert(edge_id)
                            .first->get<kEdgeAttributePtrIdx>
                          = edge_attribute_ptr;
                return;
            }

            void AddOutEdge(const VertexPtr&          dst_ptr,
                            const EdgeLabelType&     edge_label,
                            const EdgeIDType&        edge_id,
                            EdgeAttributeType* const edge_attribute_ptr){
                auto edge_label_it = this->edges_.out_edges()
                                                 .Insert(edge_label)
                                                 .first;
                auto     vertex_it = edge_label_it->get<kVertexContainerIdx>
                                                   .Insert(dst_ptr)
                                                   .first;
                auto       edge_it = vertex_it->get<kDecomposedEdgeContainerIdx>
                                               .Insert(edge_id)
                                               .first;
                edge_it->get<kEdgeAttributePtrIdx> = edge_attribute_ptr;
                return EdgePtr(edge_label_it,
                                   vertex_it,
                                     edge_it);
            }

        public:
            EdgePtr AddEdge(const VertexPtr&      dst_ptr,
                            const EdgeLabelType& edge_label,
                            const EdgeIDType&    edge_id){
                EdgeAttributeType* edge_attribute_ptr
                              = new EdgeAttributeType();
                InnerVertex* const temp_this_ptr = this;
                VertexPtr const temp_this_vertex_ptr
                           = VertexPtr(temp_this_ptr);
                dst_ptr->AddInEdge(temp_this_vertex_ptr,
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

        public:
            template<...>
            class EdgeLabelIterator_
                    :protected ContentEdgeLabelIterator_<...>{
            public:
                inline EdgeLabelIterator_   operator++();
                inline EdgeLabelIterator_   operator++(int);
                inline const EdgeLabelType& operator*() const;
                inline bool IsDone() const;
            };

            template<...>
            class VertexIterator_
                    :protected ContentVertexIterator_<...>{
            public:
                inline VertexIterator_ operator++();
                inline VertexIterator_ operator++(int);
                inline VertexPtrType   operator->();
                inline bool IsDone() const;
            };

        private:
            template<bool is_const_,...>
            class ContentEdgeIterator_;

            template<...>
            class ContentEdgeIterator_<true,...>{
            public:
                /// same to EdgePtrContent_
                const EdgeIDType&       id() const;
                const EdgeLabelType& label() const;
                const VertexPtr<true>& const_src_ptr() const;
                const VertexPtr<true>& const_dst_ptr() const;

                template<typename ConcreteDataType>
                inline const ConcreteDataType&
                       const_attribute(const KeyType_& key) const;

                template<typename ConcreteDataType>
                inline bool add_attribute(const KeyType_& key,
                                          const ConcreteDataType& value);
            };

            template<...>
            class ContentEdgeIterator_<false,...>
                :public ContentEdgeIterator_<true,...>{
            public:
                /// same to EdgePtrContent_
                EdgeAttributeType& attribute();
                VertexPtr<false>&    src_ptr();
                VertexPtr<false>&    dst_ptr();

                template<typename ConcreteDataType>
                inline ConcreteDataType& attribute(const KeyType_& key);

                template<typename ConcreteDataType>
                inline bool set_attribute(const KeyType_& key,
                                          const ConcreteDataType& value);

                inline bool remove_attribute(const KeyType_& key);
            };

        public:
            template<...>
            class EdgeIterator_
                :protected ContentEdgeIterator_<...>{
            public:
                inline        EdgeIterator_ operator++();
                inline        EdgeIterator_ operator++(int);
                inline ContentEdgeIterator_ operator->();
                inline bool IsDone() const;
            };

        public:
            template<bool is_const_>
            class VertexPtr_{
            private:
                using VertexPtrType
                    = std::conditional<is_const_,
                              const InnerVertex_*,
                                    InnerVertex_*>::type;

                VertexPtrType const vertex_ptr_;

            public:
                VertexPtr_():vertex_ptr_(nullptr){
                    return;
                }

                VertexPtr_(VertexPtrType const vertex_ptr)
                                  :vertex_ptr_(vertex_ptr){
                    return;
                }

                template<const bool judge = is_const_,
                         typename std::enable_if<judge,bool>::type = false>
                inline VertexPtrType operator->() const{
                    static_assert(judge == is_const_,
                                 "Illegal usage of this method");
                    return this->vertex_ptr_;
                }

                template<const bool judge = is_const_,
                         typename std::enable_if<!judge,bool>::type = false>
                inline VertexPtrType operator->(){
                    static_assert(judge == is_const_,
                                 "Illegal usage of this method");
                    return this->vertex_ptr_;
                }

                inline bool IsNull() const{
                    return this->vertex_ptr_ == nullptr;
                }
            };

        private:
            template<bool is_const>
            class EdgePtrContent_;

            /// constant content pointer
            template<>
            class EdgePtrContent_<true>{
            public:
                /// same to ContentEdgeIterator_
                const EdgeIDType&       id() const;
                const EdgeLabelType& label() const;
                const VertexPtr<true>& const_src_ptr() const;
                const VertexPtr<true>& const_dst_ptr() const;

                template<typename ConcreteDataType>
                inline const ConcreteDataType&
                       const_attribute(const KeyType_& key) const;

                template<typename ConcreteDataType>
                inline bool add_attribute(const KeyType_& key,
                                          const ConcreteDataType& value);
            };

            /// non-constant content pointer
            template<>
            class EdgePtrContent_<false>
                :public EdgePtrContent_<true>{
            public:
                /// same to ContentEdgeIterator_
                EdgeAttributeType& attribute();
                VertexPtr<false>&    src_ptr();
                VertexPtr<false>&    dst_ptr();

                template<typename ConcreteDataType>
                inline ConcreteDataType& attribute(const KeyType_& key);

                template<typename ConcreteDataType>
                inline bool set_attribute(const KeyType_& key,
                                          const ConcreteDataType& value);

                inline bool remove_attribute(const KeyType_& key);
            };

        public:
            template<bool is_const_>
            class EdgePtr_: protected EdgePtrContent_<is_const_>{
            private:
                using InnerEdgeType = EdgePtrContent_<is_const_>;

            public:
                inline EdgePtrContent* operator->(){
                    EdgePtr_* const temp_ptr = this;
                    return temp_ptr;
                }
                inline bool IsNull() const;
            };


        public:
            const VertexAuxiliaryType& const_auxiliary() const;
                  VertexAuxiliaryType&       auxiliary();

        /// access element, will not be implement in this beggar version
//            EdgeLabelConstPtr FindConstOutEdgeLabel(const EdgeLabelType& edge_label) const;
//            EdgeLabelConstPtr  FindConstInEdgeLabel(const EdgeLabelType& edge_label) const;

        /// vertex
        private:
            VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                                 const VertexPtr& dst_ptr){
                /// wenzhi: test whether it works
                for (auto edge_label_it : edge_label_container){
                    /// <iterator, bool>
                    auto ret = edge_label_it.get<kVertexContainerIdx>
                                            .Find(dst_ptr);
                    if (ret.second){
                        /// found it
                        return *(ret.first);
                    }
                }
                return VertexPtr();
            }

            VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                                 const VertexIDType& dst_id){
                /// wenzhi: test whether it works
                for (auto edge_label_it : edge_label_container){
                    for (auto vertex_it : edge_label_it
                                         .get<kVertexContainerIdx>){
                        VertexPtr vertex_ptr = vertex_it.get<kVertexPtrIdx>;
                        if (vertex_ptr->id() == dst_id){
                            /// found it
                            return vertex_ptr;
                        }
                    }
                /// not found
                return VertexPtr();
            }

            VertexPtr FindVertex(EdgeLabelContainerType& edge_label_container,
                                 const EdgeLabelType& edge_label,
                                 const VertexPtr&     dst_ptr){
                /// <iterator of EdgeLabelContainer, bool>
                auto edge_label_ret = edge_label_container.Find(edge_label);
                if (!edge_label_ret.second) /// does not have edge_label
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
                                 const VertexIDType&  dst_id){
                /// <iterator of EdgeLabelContainer, bool>
                auto edge_label_ret = edge_label_container.Find(edge_label);
                if (!edge_label_ret.second) /// does not have edge_label
                    return VertexPtr();
                for (auto vertex_it : edge_label_ret.first
                                                   ->get<kVertexContainerIdx>){
                    VertexPtr vertex_ptr = vertex_it.get<kVertexPtrIdx>;
                    if (vertex_ptr->id() == dst_id){
                        /// found it
                        return vertex_ptr;
                    }
                }
                /// not found
                return VertexPtr();
            }

        public:
            inline VertexPtr FindOutVertex(const VertexPtr& dst_ptr){
                return this->FindVertex(this->edges_.out_edges(),dst_ptr);
            }
            inline VertexPtr FindOutVertex(const VertexIDType& dst_id){
                return this->FindVertex(this->edges_.out_edges(),dst_id);
            }
            inline VertexPtr FindOutVertex(const EdgeLabelType& edge_label,
                                           const VertexPtr&     dst_ptr){
                return this->FindVertex(this->edges_.out_edges(),edge_label,
                                                                 dst_ptr);
            }
            inline VertexPtr FindOutVertex(const EdgeLabelType& edge_label,
                                           const VertexIDType&  dst_id){
                return this->FindVertex(this->edges_.out_edges(),edge_label,
                                                                 dst_id);
            }
            inline VertexPtr FindInVertex(const VertexPtr& dst_ptr){
                return this->FindVertex(this->edges_.in_edges(),dst_ptr);
            }
            inline VertexPtr FindInVertex(const VertexIDType& dst_id){
                return this->FindVertex(this->edges_.in_edges(),dst_id);
            }
            inline VertexPtr FindInVertex(const EdgeLabelType& edge_label,
                                           const VertexPtr&     dst_ptr){
                return this->FindVertex(this->edges_.in_edges(),edge_label,
                                                                dst_ptr);
            }
            inline VertexPtr FindInVertex(const EdgeLabelType& edge_label,
                                           const VertexIDType&  dst_id){
                return this->FindVertex(this->edges_.in_edges(),edge_label,
                                                                dst_id);
            }

            /// will not be implemented in the beggar version
//            VertexConstPtr FindConstOutVertex(const VertexPtr&     dst_ptr) const;
//            VertexConstPtr FindConstOutVertex(const VertexIDType&  dst_id ) const;
//            VertexConstPtr FindConstOutVertex(const EdgeLabelType& edge_label,
//                                              const VertexPtr&     dst_ptr) const;
//            VertexConstPtr FindConstOutVertex(const EdgeLabelType& edge_label,
//                                              const VertexIDType&  dst_id ) const;
//            VertexConstPtr FindConstInVertex(const VertexPtr&     dst_ptr) const;
//            VertexConstPtr FindConstInVertex(const VertexIDType&  dst_id ) const;
//            VertexConstPtr FindConstInVertex(const EdgeLabelType& edge_label,
//                                             const VertexPtr&     dst_ptr) const;
//            VertexConstPtr FindConstInVertex(const EdgeLabelType& edge_label,
//                                             const VertexIDType&  dst_id ) const;

        private:
            EdgePtr FindEdge(EdgeLabelContainerType& edge_label_container,
                             const EdgeLabelType& edge_label,
                             const VertexPtr&  dst_ptr,
                             const EdgeIDType& edge_id){
                /// <iterator of EdgeLabelContainer, bool>
                auto edge_label_ret = edge_label_container.Find(edge_label);
                if (!edge_label_ret.second) /// does not have this edge label
                    return EdgePtr();
                /// <iterator of VertexContainer, bool>
                auto vertex_ret = edge_label_ret.first->Find(dst_ptr);
                if (!vertex_ret.second) /// does not have this VertexPtr
                    return EdgePtr();
                /// <iterator of DecomposedEdgeContainer, bool>
                auto decomposed_edge_ret = vertex_ret.first->Find(edge_id);
                if (!decomposed_edge_ret) /// does not find it
                    return EdgePtr();
                return EdgePtr(edge_label_ret.first,
                                   vertex_ret.first,
                          decomposed_edge_ret.first);
            }

        public:
            inline EdgePtr FindOutEdge(const EdgeLabelType& edge_label,
                                       const VertexPtr&  dst_ptr,
                                       const EdgeIDType& edge_id){
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

            EdgePtr FindInEdge(const VertexPtr& dst_ptr){
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
            EdgeLabelIterator OutEdgeLabelBegin() const;

                 VertexIterator OutVertexBegin();
            VertexConstIterator OutVertexConstBegin() const;
            /// possible extension:
            ///     OutVertexBegin(edge_label)
            ///     OutVertexBegin(edge_label,vertex_label)
            ///     ...

                 EdgeIterator OutEdgeBegin();
            EdgeConstIterator OutEdgeConstBegin() const;
            /// possible extension:
            ///     OutEdgeBegin(edge_label)
            ///     OutEdgeBegin(edge_label,vertex_label)
            ///     ...

            /// input iterators:
            EdgeLabelIterator InEdgeLabelBegin() const;

                 VertexIterator InVertexBegin();
            VertexConstIterator InVertexConstBegin() const;
            /// possible extension:
            ///     InVertexBegin(edge_label)
            ///     InVertexBegin(edge_label,vertex_label)
            ///     ...

                 EdgeIterator InEdgeBegin();
            EdgeConstIterator InEdgeConstBegin() const;
            /// possible extension:
            ///     InEdgeBegin(edge_label)
            ///     InEdgeBegin(edge_label,vertex_label)
            ///     ...
        };

        using InnerVertexType = InnerVertex_<...>;


    public:
        using   EdgeType =   EdgeType_<  edge_has_attribtue,
                                         edge_has_dynamic_attribtue>;
        using VertexType = VertexType_<vertex_has_attribtue,
                                       vertex_has_dynamic_attribtue>;
        /// non-constant pointer
        using   EdgePtr  = typename InnerVertexType::  EdgePtr_<false,...>;
        using VertexPtr  = typename InnerVertexType::VertexPtr_<false,...>;
        /// constant pointer
        using   EdgeConstPtr = typename InnerVertexType::  EdgePtr_<true,...>;
        using VertexConstPtr = typename InnerVertexType::VertexPtr_<true,...>;

        using VertexSizeType = ...;

        VertexPtr AddVertex(const typename VertexType::IDType&    vertex_id,
                            const typename VertexType::LabelType& vertex_label);
        /// possible variant:
        ///     AddVertex(id)
        ///     AddVertex(id,label)
        ///     AddVertex(id,label,attribute)
        ///     AddVertex(id,label,attribute0,attribute1,...)
        ///     AddVertex(id,attribute)
        ///     AddVertex(id,attribute0,attribute1,...)
        ///     ...

        EdgePtr AddEdge(const typename VertexType::IDType&  src_id,
                        const typename VertexType::IDType&  dst_id,
                        const typename   EdgeType::IDType& edge_id);
        /// possible variant:
        ///     AddEdge(src_id,dst_id)
        ///     AddEdge(src_id,dst_id,edge_label)
        ///     AddEdge(src_id,dst_id,edge_label,edge_id)
        ///     AddEdge(src_id,dst_id,edge_label,edge_id,edge_attribute)
        ///     ...

             VertexPtr FindVertex     (typename const VertexType::IDType& vertex_id);
        VertexConstPtr FindConstVertex(typename const VertexType::IDType& vertex_id) const;
        /// possible extension:
        ///     FindVertex(vertex_label,vertex_id)
        ///     ...
        ///     FindConstVertex(vertex_label,vertex_id)
        ///     ...

             VertexIterator VertexBegin();
        VertexConstIterator VertexConstBegin() const;
        /// possible extension:
        ///     VertexBegin(vertex_label)
        ///     ...
        ///     VertexConstBegin(vertex_label)
        ///     ...

        VertexSizeType VertexSize() const;
        /// possible extension:
        ///     VertexSize(vertex_label)
        ///     ...
    };
}
#endif // _GRAPH_HPP
