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

    public:
        template<typename IDType_,
                 typename LabelType_,
                 typename AttributeType_>
        class Edge{
        public:
            using IDType        = IDType_;
            using LabelType     = LabelType_;
            using AttributeType = AttributeType_;
        };

        using DuplicateNumType  = typename Configures::DuplicateNumType;

        static constexpr enum StoreData
                              store_data = Configures::store_data;

        using EdgeType = Edge<typename Configures::EdgeIDType,
                              typename Configures::EdgeLabelType,
                              typename Configures::EdgeAttributeType>;

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
                                       DuplicateNumType>;

    private:
        using VertexIDType        = typename VertexType::IDType;
        using VertexLabelType     = typename VertexType::LabelType;
        using VertexAttributeType = typename VertexType::AttributeType;

        using EdgeIDType          = typename   EdgeType::IDType;
        using EdgeLabelType       = typename   EdgeType::LabelType;
        using EdgeAttributeType   = typename   EdgeType::AttributeType;

        using VertexesContainer = std::map<const VertexIDType,
                                                 VertexType* const>;
        VertexesContainer vertexes_;

        inline bool HasVertex(const VertexIDType& vertexID) const{
            return this->vertexes_.find(vertexID)
                != this->vertexes_.cend();
        }

    public:
        Graph() = default;

        Graph& operator=(const Graph& ) = delete;
        Graph& operator=(const Graph&&) = delete;

        Graph(const Graph& graph){
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
        }

        inline VertexType* const
               vertex_ptr(const VertexIDType& vertex_id){
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

        template<typename... Args>
        inline VertexType* const AddVertex(const VertexIDType& vertex_id,
                                           const Args&...      args){
            if (this->vertexes_.find(vertex_id)
             != this->vertexes_.cend()){
                /// already exists
                return nullptr;
            }
            /// not exist
            VertexType* const new_vertex = new VertexType(vertex_id,args...);
            this->vertexes_.insert(std::make_pair(vertex_id,new_vertex));
            return new_vertex;
        }

        using single_edge_iterator
            = typename VertexType
            ::iterator
            ::template edge<IteratorType::SingleElement>;

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

        virtual ~Graph(){
            for (const auto& it : this->vertexes_)
                delete(it.second);
            return;
        }
    };
}

#endif // _GRAPH_HPP
