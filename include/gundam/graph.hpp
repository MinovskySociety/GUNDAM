#ifndef _GRAPH_HPP
#define _GRAPH_HPP

#include <map>

#include "vertex.hpp"
#include "graph_configure.hpp"

namespace GUNDAM{

    template<typename... configures>
    class Graph{
    private:
        using Configures = GraphConfigures<configures...>;

        template<typename IDType_,
                 typename LabelType_,
                 typename AttributeType_>
        class Edge_{
        public:
            using IDType        = IDType_;
            using LabelType     = LabelType_;
            using AttributeType = AttributeType_;
        };

    public:
        using VertexType
            = Vertex<Configures::store_data,
            typename Configures::VertexIDType,
                     Configures::vertex_id_provided_by_user,
            typename Configures::VertexLabelType,
                     Configures::vertex_label_is_const,
                     Configures::vertex_has_attribute,
            typename Configures::VertexAttributeType,
                     Configures::vertex_attribute_is_const,
                     Configures::vertex_attribute_is_dynamic,
            typename Configures::VertexAttributeKeyType,
                     Configures::vertex_attribute_container_type,
            typename Configures::EdgeIDType,
                     Configures::edge_id_provided_by_user,
            typename Configures::EdgeLabelType,
                     Configures::edge_label_is_const,
                     Configures::edge_has_attribute,
            typename Configures::EdgeAttributeType,
                     Configures::edge_attribute_is_const,
                     Configures::edge_attribute_is_dynamic,
            typename Configures::EdgeAttributeKeyType,
                     Configures:: edge_attribute_container_type,
                     Configures::     edge_label_container_type,
                     Configures::         vertex_container_type,
                     Configures::decomposed_edge_container_type,
                     Configures::     edge_label_container_sort_type,
                     Configures::         vertex_container_sort_type,
                     Configures::decomposed_edge_container_sort_type,
                     Configures:: allow_multiple_edge,
                     Configures::allow_duplicate_edge,
            typename Configures::      DuplicateNumType>;

        using EdgeType = Edge_<typename Configures::EdgeIDType,
                               typename Configures::EdgeLabelType,
                               typename Configures::EdgeAttributeType>;

    private:
        using VertexIDType        = typename VertexType::IDType;
        using VertexLabelType     = typename VertexType::LabelType;
        using VertexAttributeType = typename VertexType::AttributeType;

        using EdgeIDType          = typename Configures::EdgeIDType;
        using EdgeLabelType       = typename Configures::EdgeLabelType;
        using EdgeAttributeType   = typename Configures::EdgeAttributeType;

        static constexpr bool allow_duplicate_edge
                = Configures::allow_duplicate_edge;

        template<const enum ContainerType vertex_container_type_,
                 typename   VertexType_>
        class VertexContainer_;

        template<typename VertexType_>
        class VertexContainer_<ContainerType::Map,VertexType_>{
        private:
            using VertexIDType = typename VertexType_::IDType;
            using InnerContainerType = std::map<const VertexIDType,
                                                      VertexType_* const>;
            InnerContainerType vertexes_;

        public:
            using VertexType = VertexType_;
            using const_iterator = typename InnerContainerType::const_iterator;
            using       iterator = typename InnerContainerType::      iterator;
            using      size_type = typename InnerContainerType::     size_type;

            inline const_iterator cbegin() const{
                return this->vertexes_.cbegin();
            }
            inline const_iterator cend() const{
                return this->vertexes_.cend();
            }
            inline iterator begin(){
                return this->vertexes_.begin();
            }
            inline iterator end(){
                return this->vertexes_.end();
            }
            inline size_type size() const{
                return this->vertexes_.size();
            }
            inline bool empty() const{
                return this->vertexes_.empty();
            }
            inline void clear(){
                this->vertexes_.clear();
                return;
            }
            inline void AddVertexPtr(VertexType* const vertex_ptr){
                this->vertexes_.insert(std::make_pair(vertex_ptr->id(),
                                                      vertex_ptr));
                return;
            }

            inline bool HasVertex(const VertexIDType& vertexID) const{
                return this->vertexes_.find(vertexID)
                    != this->vertexes_.cend();
            }

            inline VertexType* const
                   vertex_ptr( const VertexIDType& vertex_id){
                auto it = this->vertexes_.find(vertex_id);
                return it != this->vertexes_.cend()?
                       it->second:nullptr;
            }

            inline const VertexType* const
                   const_vertex_ptr(const VertexIDType&  vertex_id) const{
                const auto it = this->vertexes_.find(vertex_id);
                return it != this->vertexes_.cend()?
                       it->second:nullptr;
            }
        };

        template<typename   VertexContainerType_,
                 const bool is_const_>
        class VertexIterator_;

        template<typename   VertexContainerType_,
                 const bool is_const_>
        class VertexIterator_{
        private:
            using InnerVertexContainerType_ =
                typename std::conditional<is_const_,
                         const VertexContainerType_,
                               VertexContainerType_>::type;

            typename std::conditional<is_const_,
                typename VertexContainerType_::const_iterator,
                typename VertexContainerType_::      iterator>::type iterator_;

            const typename VertexContainerType_::const_iterator kEndIter_;

            template<const bool judge = is_const_,
                     typename std::enable_if<judge,bool>::type = false>
            inline typename VertexContainerType_::const_iterator
                                                        iterator_begin(
                      const VertexContainerType_& vertex_container) const{
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                return vertex_container.cbegin();
            }

            template<const bool judge = is_const_,
                     typename std::enable_if<!judge,bool>::type = false>
            inline typename VertexContainerType_::iterator
                                                  iterator_begin(
                            VertexContainerType_& vertex_container) const{
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                return vertex_container.begin();
            }

        public:
            VertexIterator_(
                    InnerVertexContainerType_& vertex_container)
               :iterator_(this->iterator_begin(vertex_container)),
                                     kEndIter_(vertex_container.cend()){
                return;
            }

            inline const VertexType* const const_vertex_ptr() const{
                assert(!this->IsDone());
                return this->iterator_->second;
            }

            template<const bool judge = is_const_,
                     typename std::enable_if<!judge,bool>::type = false>
            inline VertexType* const vertex_ptr(){
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                assert(!this->IsDone());
                return this->iterator_->second;
            }

            template<const bool judge = is_const_,
                     typename std::enable_if<judge,bool>::type = false>
            inline const VertexType* const operator->() const{
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                assert(!this->IsDone());
                return this->iterator_->second;
            }

            template<const bool judge = is_const_,
                     typename std::enable_if<judge,bool>::type = false>
            inline const VertexType& operator*() const{
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                assert(!this->IsDone());
                return this->iterator_->second;
            }

            template<const bool judge = is_const_,
                     typename std::enable_if<!judge,bool>::type = false>
            inline VertexType* const operator->(){
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                assert(!this->IsDone());
                return this->iterator_->second;
            }

            template<const bool judge = is_const_,
                     typename std::enable_if<!judge,bool>::type = false>
            inline VertexType& operator*(){
                static_assert(judge == is_const_,
                             "Illegal usage of this method");
                assert(!this->IsDone());
                return this->iterator_->second;
            }

            inline VertexIterator_ operator++ (){
                ///prefix
                this->iterator_++;
                return *this;
            }

            inline VertexIterator_ operator++ (int){
                ///postfix
                assert(!this->IsDone());
                VertexIterator_ temp(*this);
                ++(*this);
                return temp;
            }

            inline bool IsDone() const{
                return this->iterator_ == this->kEndIter_;
            }
        };

        using VertexContainerType
            = VertexContainer_<ContainerType::Map,VertexType>;

        VertexContainerType vertexes_;

        template<template <typename...> class GraphType_,
                 typename... configures_>
        inline void CanConvertTo(const GraphType_<configures_...>& graph)const{
            static_assert(std::is_base_of<Graph     <configures_...>,
                                          GraphType_<configures_...>>::value,
                         "Illegal input parameter, should be a graph");
            using ConvertToGraphType = GraphType_<configures_...>;
            static_assert(Graph::allow_duplicate_edge
                        ||!ConvertToGraphType::allow_duplicate_edge,
                        "Cannot convert from a graph allows duplicate edge\
                         to a graph does not allow duplicate edge");
            /// wenzhi ToDo:
            ///     complete the checking process of the parameters,
            ///     such as the above statement, if this graph type does
            ///     not allow duplicate edges but the InputGraphType allows
            ///     that, then this convert would be illegal
            return;
        }

        template<template <typename...> class GraphType_,
                 typename... configures_>
        inline void Construct(const GraphType_<configures_...>& graph){
            for (const auto& vit : graph.vertexes_)
                this->AddVertex(vit.second->id(),
                                vit.second->label());
            for (const auto& vit : graph.vertexes_){
                for (auto eit = vit.second->edge_cbegin();
                         !eit.IsDone();eit++){
                    this->AddEdge(eit->src_id(),
                                  eit->dst_id(),
                                  eit->label());
                }
            }
            return;
        }

        inline void Deconstruct(){
            for (const auto& it : this->vertexes_)
                delete(it.second);
            this->vertexes_.clear();
            return;
        }

    public:
        using const_vertex_iterator
                  = VertexIterator_<VertexContainerType,true>;

        using vertex_iterator
            = VertexIterator_<VertexContainerType,false>;

        using single_edge_iterator
            = typename VertexType::iterator
            ::template edge<IteratorType::SingleElement>;

        Graph() = default;

        template<template <typename...> class GraphType_,
                 typename... configures_>
        Graph(const GraphType_<configures_...>& graph){
            this->CanConvertTo(graph);
            this->Construct(graph);
            return;
        }

        template<template <typename...> class GraphType_,
                 typename... configures_>
        Graph& operator=(const GraphType_<configures_...>& graph){
            this->CanConvertTo(graph);
            this->Deconstruct();
            this->Construct(graph);
            return;
        }

        virtual ~Graph(){
            this->Deconstruct();
            return;
        }

        inline VertexType* const
               vertex_ptr(const VertexIDType& vertex_id){
            return this->vertexes_.vertex_ptr(vertex_id);
        }

        inline const VertexType* const
               const_vertex_ptr(const VertexIDType& vertex_id) const{
            return this->vertexes_.const_vertex_ptr(vertex_id);
        }

        inline bool HasVertex(const VertexIDType& vertex_id) const{
            return this->vertexes_.HasVertex(vertex_id);
        }

        template<typename... Args>
        inline VertexType* const AddVertex(const VertexIDType& vertex_id,
                                           const Args&...      args){
            if (this->HasVertex(vertex_id))
                return nullptr; /// already exists
            /// not exist
            VertexType* const new_vertex = new VertexType(vertex_id,
                                                          args...);
            this->vertexes_.AddVertexPtr(new_vertex);
            return new_vertex;
        }

        template<typename... EdgeArgs>
        inline single_edge_iterator
                    AddEdge(const  VertexIDType&  src_id,
                            const  VertexIDType&  dst_id,
                            const EdgeLabelType& edge_label,
                            const EdgeArgs&...   edge_args){
            assert(this->HasVertex(src_id));
            assert(this->HasVertex(dst_id));
            VertexType* const src_ptr = this->vertex_ptr(src_id);
            VertexType* const dst_ptr = this->vertex_ptr(dst_id);
            return src_ptr->AddEdge(dst_ptr,edge_label,
                                            edge_args...);
        }

        inline const_vertex_iterator vertex_cbegin() const{
            return const_vertex_iterator(this->vertexes_);
        }

        inline vertex_iterator vertex_begin(){
            return vertex_iterator(this->vertexes_);
        }
    };
}

#endif // _GRAPH_HPP
