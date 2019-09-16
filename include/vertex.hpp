#ifndef _VERTEX_HPP
#define _VERTEX_HPP

#include "configures.hpp"
#include "attribute.hpp"
#include "label.hpp"

#include <memory>
#include <sstream>
#include <vector>
#include <list>
#include <map>
#include <set>

template<const enum StoreData store_type,
         /// vertex
         typename   VertexIDType,
         const bool vertex_id_provided_by_user,
         typename   VertexLabelType,
         const bool vertex_label_is_const,
         const bool vertex_has_attribute,
         typename   VertexAttributeType,
         const bool vertex_attribute_is_const,
         const bool vertex_attribute_is_dynamic,
         typename   VertexAttributeKeyType,
         enum ContainerType
                    vertex_attribute_container_type,
         /// edge
         typename   EdgeIDType,
         const bool edge_id_provided_by_user,
         typename   EdgeLabelType,
         const bool edge_label_is_const,
         const bool edge_has_attribute,
         typename   EdgeAttributeType,
         const bool edge_attribute_is_const,
         const bool edge_attribute_is_dynamic,
         typename   EdgeAttributeKeyType,
         const enum ContainerType
                    edge_attribute_container_type,
         /// container
         const enum ContainerType ///  first level container type
                    edge_label_container_type,
         const enum ContainerType /// second level container type
                        vertex_container_type,
         const enum ContainerType ///  third level container type
               decomposed_edge_container_type,
         /// container sort type
         const enum SortType /// the manner to sort edge label
                             /// in the edge label container
                             /// (first level container)
               edge_label_container_sort_type,
         const enum SortType /// the manner to sort vertex in
                             /// vertex container (second
                             /// level container)
                   vertex_container_sort_type,
         const enum SortType /// the manner to sort edge
                             /// in the edge container
                             /// (third level container)
          decomposed_edge_container_sort_type,
         /// miscellaneous, graph characteristics
         const bool  allow_multiple_edge,/// whether allows multiple edges
                                         /// with different label between two
                                         /// vertexes
         const bool allow_duplicate_edge,/// whether allows multiple edges
                                         /// with same label between two
                                         /// vertexes
         typename DuplicateNumType,/// the data type to store duplicate number
         typename... AuxiliaryData>
class Vertex{
private:
    static_assert(edge_label_is_const,
                 "Not support variable edge label yet");
    static_assert(vertex_id_provided_by_user,
                 "id of vertex should provided by the user");
    static_assert(edge_id_provided_by_user,
                 "id of edge should provided by the user");

    /// if allow_duplicate_edge is true then allow_multiple_edge must be true
    static_assert(allow_duplicate_edge || !allow_multiple_edge,
                 "Illegal configuration, allows repeated edges\
                              must first allows multiple edges");

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

    template<const bool is_const_>
    class WithVertexPtr_{
    public:
        using VertexPtr = typename std::conditional<is_const_,
                                          const Vertex* const,
                                                Vertex* const>::type;
    private:
        VertexPtr vertex_ptr_;

    public:
        WithVertexPtr_(VertexPtr vertex_ptr)
                    :vertex_ptr_(vertex_ptr){
            return;
        }

        inline const Vertex* const const_vertex_ptr() const{
            return this->vertex_ptr_;
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const vertex_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return this->vertex_ptr_;
        }
    };

    template<typename LabelType_,
             const bool is_const_>
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
        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline void set_label(const LabelType_& label){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            this->label_ = label;
            return;
        }
    };

    template<typename   AttributeType_,
             const bool is_const_,
             const bool is_dynamic_,
             typename   KeyType_,
             const enum ContainerType container_type_>
    class WithAttribute_;

    /// non-dynamic attribute
    template<typename   AttributeType_,
             const bool is_const_,
             typename   KeyType_,
             const enum ContainerType container_type_>
    class WithAttribute_<AttributeType_,
                         is_const_,
                         false,
                         KeyType_,
                         container_type_>{
    private:
        typename std::conditional<is_const_,const AttributeType_,
                                                  AttributeType_
                                 >::type attribute_;
    public:
        WithAttribute_(const AttributeType_& attribute)
                                :attribute_( attribute){
            return;
        }

        template<const bool judge_is_const = is_const_,
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

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline AttributeType_& attribute(){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            /// the constant attribute should not be modified
            return this->attribute_;
        }

        template<const bool judge_is_const = is_const_,
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
    template<typename   AttributeType_,
             const bool is_const_,
             typename   KeyType_,
             const enum ContainerType container_type_>
    class WithAttribute_<AttributeType_,
                         is_const_,
                         true,
                         KeyType_,
                         container_type_>{
    private:
        AttributeContainer<container_type_,
                           KeyType_,
                           is_const_> attributes_;

    public:
        WithAttribute_():attributes_(){
            return;
        }

        template<typename ConcreteDataType>
        inline const ConcreteDataType&
               const_attribute(const KeyType_& key) const{
            /// the constant attribute should not be modified
            return this->attributes_.const_attribute<ConcreteDataType>(key);
        }

        template<typename ConcreteDataType,
                 const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline ConcreteDataType& attribute(const KeyType_& key){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            /// the constant attribute should not be modified
            return this->attributes_.attribute<ConcreteDataType>(key);
        }

        template<typename ConcreteDataType>
        inline bool add_attribute(const KeyType_& key,
                                  const ConcreteDataType& value){
            return this->attributes_.add_attribute(key,value);
        }

        template<typename   ConcreteDataType,
                 const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline bool set_attribute(const KeyType_& key,
                                  const ConcreteDataType& value){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this->attributes_.set_attribute(key,value);
        }

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline bool remove_attribute(const KeyType_& key){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this->attributes_.remove_attribute(key);
        }
    };

    template<const enum ContainerType decomposed_edge_container_type_,
             typename DecomposedEdgeType_>
    class DecomposedEdgeContainer_{
        /// trick the compiler, equivalent to static_assert(false)
        static_assert(decomposed_edge_container_type_ == ContainerType::Set,
                     "unsupported vertex container type");
        static_assert(decomposed_edge_container_type_ != ContainerType::Set,
                     "unsupported vertex container type");
    };

    template<typename DecomposedEdgeType_>
    class DecomposedEdgeContainer_<ContainerType::Set,
                                   DecomposedEdgeType_>{
    private:
        std::set<DecomposedEdgeType_> decomposed_edges_;

    public:
        using DecomposedEdgeType = DecomposedEdgeType_;
        using const_iterator
            = typename std::set<DecomposedEdgeType_>::const_iterator;
        using       iterator
            = typename std::set<DecomposedEdgeType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->decomposed_edges_.cbegin();
        }
        inline const_iterator cend() const{
            return this->decomposed_edges_.cend();
        }
        inline iterator begin(){
            return this->decomposed_edges_.begin();
        }
        inline iterator end(){
            return this->decomposed_edges_.end();
        }
        inline DuplicateNumType size() const{
            return this->decomposed_edges_.size();
        }

        template<typename... Args>
        inline std::pair<DecomposedEdgeType_&,bool> InsertDecomposedEdge(
                   const EdgeIDType& edge_id,
                   const Args&...    args){
            auto temp = this->decomposed_edges_.emplace(edge_id,args...);
            return std::make_pair(*(temp.first),temp.second);
        }
    };

    template<typename DecomposedEdgeType_>
    class DecomposedEdgeContainer_<ContainerType::List,
                                   DecomposedEdgeType_>{
    private:
        std::list<DecomposedEdgeType_> decomposed_edges_;

        static_assert(decomposed_edge_container_sort_type == SortType::None,
                     "other sorting type are not implemented");

    public:
        using DecomposedEdgeType = DecomposedEdgeType_;
        using const_iterator
            = typename std::list<DecomposedEdgeType_>::const_iterator;
        using       iterator
            = typename std::list<DecomposedEdgeType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->decomposed_edges_.cbegin();
        }
        inline const_iterator cend() const{
            return this->decomposed_edges_.cend();
        }
        inline iterator begin(){
            return this->decomposed_edges_.begin();
        }
        inline iterator end(){
            return this->decomposed_edges_.end();
        }
        inline DuplicateNumType size() const{
            return this->decomposed_edges_.size();
        }

        template<typename... Args>
        inline std::pair<DecomposedEdgeType_&,bool> InsertDecomposedEdge(
                   const EdgeIDType& edge_id,
                   const Args&...    args){
            if (decomposed_edge_container_sort_type == SortType::None){
                for (auto& it : this->decomposed_edges_)
                    if (it.id() == edge_id)
                        return std::make_pair(it,false);
                this->decomposed_edges_.emplace_back(edge_id,args...);
                return std::make_pair(this->decomposed_edges_.back(),true);
            }
            /// other sorting type are not implemented
            assert(false);
            return std::make_pair(this->decomposed_edges_.back(),true);
        }
    };

    template<typename DecomposedEdgeType_>
    class DecomposedEdgeContainer_<ContainerType::Vector,
                                   DecomposedEdgeType_>{
    private:
        std::vector<DecomposedEdgeType_> decomposed_edges_;

        static_assert(decomposed_edge_container_sort_type == SortType::None,
                     "other sorting type are not implemented");

    public:
        using DecomposedEdgeType = DecomposedEdgeType_;
        using const_iterator
            = typename std::vector<DecomposedEdgeType_>::const_iterator;
        using       iterator
            = typename std::vector<DecomposedEdgeType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->decomposed_edges_.cbegin();
        }
        inline const_iterator cend() const{
            return this->decomposed_edges_.cend();
        }
        inline iterator begin(){
            return this->decomposed_edges_.begin();
        }
        inline iterator end(){
            return this->decomposed_edges_.end();
        }
        inline DuplicateNumType size() const{
            return this->decomposed_edges_.size();
        }
        inline void shrink_to_fit() const{
            this->decomposed_edges_.shrink_to_fit();
            return;
        }

        template<typename... Args>
        inline std::pair<DecomposedEdgeType_&,bool> InsertDecomposedEdge(
                   const EdgeIDType& edge_id,
                   const Args&...    args){
            if (decomposed_edge_container_sort_type == SortType::None){
                for (auto& it : this->decomposed_edges_)
                    if (it.id() == edge_id)
                        return std::pair<DecomposedEdgeType_&,bool>(it,false);
                this->decomposed_edges_.emplace_back(edge_id,args...);
                return std::pair<DecomposedEdgeType_&,bool>
                          (this->decomposed_edges_.back(),true);
            }
            /// other sorting type are not implemented
            assert(false);
            return std::pair<DecomposedEdgeType_&,bool>
                      (this->decomposed_edges_.back(),true);
        }
    };

    template<const enum ContainerType vertex_container_type_,
             typename CompoundVertexType_>
    class VertexContainer_{
        static_assert(vertex_container_type != ContainerType::Map,
                     "Not support std::map as Vertex container type,\
                      use std::set instead");
        /// trick the compiler, equivalent to static_assert(false)
        static_assert(vertex_container_type_ == ContainerType::Set,
                     "unsupported vertex container type");
        static_assert(vertex_container_type_ != ContainerType::Set,
                     "unsupported vertex container type");
    };

    template<typename CompoundVertexType_>
    class VertexContainer_<ContainerType::Set,
                           CompoundVertexType_>{
    private:
        std::set<CompoundVertexType_> compound_vertexes_;

    public:
        using CompoundVertexType = CompoundVertexType_;
        using const_iterator
            = typename std::set<CompoundVertexType_>::const_iterator;
        using       iterator
            = typename std::set<CompoundVertexType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->compound_vertexes_.cbegin();
        }
        inline const_iterator cend() const{
            return this->compound_vertexes_.cend();
        }
        inline iterator begin(){
            return this->compound_vertexes_.begin();
        }
        inline iterator end(){
            return this->compound_vertexes_.end();
        }

        template<typename... Args>
        inline std::pair<CompoundVertexType_&,bool> InsertCompoundVertex(
                   const Vertex* const vertex,
                   const Args&... arg){
            auto temp = this->compound_vertexes_.emplace(vertex,arg...);
            return std::make_pair(*(temp.first),temp.second);
        }
    };

    template<typename CompoundVertexType_>
    class VertexContainer_<ContainerType::List,
                           CompoundVertexType_>{
    private:
        std::list<CompoundVertexType_> compound_vertexes_;

        static_assert(vertex_container_sort_type == SortType::None,
                     "other sorting type are not implemented");

    public:
        using CompoundVertexType = CompoundVertexType_;
        using const_iterator
            = typename std::list<CompoundVertexType_>::const_iterator;
        using       iterator
            = typename std::list<CompoundVertexType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->compound_vertexes_.cbegin();
        }
        inline const_iterator cend() const{
            return this->compound_vertexes_.cend();
        }
        inline iterator begin(){
            return this->compound_vertexes_.begin();
        }
        inline iterator end(){
            return this->compound_vertexes_.end();
        }

        template<typename... Args>
        inline std::pair<CompoundVertexType_&,bool> InsertCompoundVertex(
                           const Vertex* const vertex,
                           const Args&... args){
            if (vertex_container_sort_type == SortType::None){
                for (auto& it : this->compound_vertexes_)
                    if (it.id() == vertex->id())
                        return std::make_pair(it,false);/// already exist
                this->compound_vertexes_.emplace_back(vertex,args...);
                return  std::make_pair(this->compound_vertexes_.back(),true);
            }
            /// other sorting type are not implemented
            assert(false);
            return std::make_pair(this->compound_vertexes_.back(),true);
        }
    };

    template<typename CompoundVertexType_>
    class VertexContainer_<ContainerType::Vector,
                           CompoundVertexType_>{
    private:
        std::vector<CompoundVertexType_> compound_vertexes_;

        static_assert(vertex_container_sort_type == SortType::None,
                     "other sorting type are not implemented");

    public:
        using CompoundVertexType = CompoundVertexType_;
        using const_iterator
            = typename std::vector<CompoundVertexType_>::const_iterator;
        using       iterator
            = typename std::vector<CompoundVertexType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->compound_vertexes_.cbegin();
        }
        inline const_iterator cend() const{
            return this->compound_vertexes_.cend();
        }
        inline iterator begin(){
            return this->compound_vertexes_.begin();
        }
        inline iterator end(){
            return this->compound_vertexes_.end();
        }
        inline void shrink_to_fit() const{
            this->compound_vertexes_.shrink_to_fit();
            return;
        }

        template<typename... Args>
        inline std::pair<CompoundVertexType_&,bool> InsertCompoundVertex(
                                 Vertex* const vertex,
                           const Args&... args){
            if (vertex_container_sort_type == SortType::None){
                for (auto& it : this->compound_vertexes_)
                    if (it.const_vertex_ptr()->id() == vertex->id())
                        return std::pair<CompoundVertexType_&,bool>(it,false);
                this->compound_vertexes_.emplace_back(vertex,args...);
                return std::pair<CompoundVertexType_&,bool>(
                                       this->compound_vertexes_.back(),true);
            }
            /// other sorting type are not implemented
            assert(false);
            return std::pair<CompoundVertexType_&,bool>(
                                       this->compound_vertexes_.back(),true);
        }
    };

    template<const enum ContainerType edge_label_container_type_,
             typename EdgeLabelType_,
             typename VertexContainerType_>
    class EdgeLabelContainer_{
        static_assert(edge_label_container_type != ContainerType::Set,
                     "Not support std::set as Edge Label container type,\
                      use std::map instead");
        /// trick the compiler, equivalent to static_assert(false)
        static_assert(edge_label_container_type == ContainerType::Set,
                     "unsupported edge label container type");
        static_assert(edge_label_container_type != ContainerType::Set,
                     "unsupported edge label container type");
    };

    template<typename EdgeLabelType_,
             typename VertexContainerType_>
    class EdgeLabelContainer_<ContainerType::Map,
                              EdgeLabelType_,
                              VertexContainerType_>{
    private:
        std::map<const EdgeLabelType_,VertexContainerType_> edge_labels_;

    public:
        using VertexContainerType = VertexContainerType_;
        using const_iterator
            = typename std::map<const EdgeLabelType_,
                                      VertexContainerType_>::const_iterator;
        using       iterator
            = typename std::map<const EdgeLabelType_,
                                      VertexContainerType_>::      iterator;

        inline const_iterator cbegin() const{
            return this->edge_labels_.cbegin();
        }
        inline const_iterator cend() const{
            return this->edge_labels_.cend();
        }
        inline iterator begin(){
            return this->edge_labels_.begin();
        }
        inline iterator end(){
            return this->edge_labels_.end();
        }

        inline bool HasEdgeLabel(const EdgeLabelType_& edge_label) const{
            const auto it = this->edge_labels_.find(edge_label);
            return it != this->edge_labels_.cend();
        }

        inline const VertexContainerType_&
               const_vertex_container(const EdgeLabelType_& edge_label) const{
            assert(this->HasEdgeLabel(edge_label));
            return this->edge_labels_.find(edge_label)->second;
        }

        inline VertexContainerType_&
               vertex_container(const EdgeLabelType_& edge_label){
            assert(this->HasEdgeLabel(edge_label));
            return this->edge_labels_.find(edge_label)->second;
        }

        inline std::pair<VertexContainerType_&,bool>
                   InsertVertexContainer(const EdgeLabelType& edge_label){
            auto temp = this->edge_labels_.emplace(edge_label,
                                                   VertexContainerType_());
            return std::make_pair(*(temp.first),temp.second);
        }
    };

    template<typename EdgeLabelType_,
             typename VertexContainerType_>
    class EdgeLabelContainer_<ContainerType::List,
                              EdgeLabelType_,VertexContainerType_>{
    private:
        using EdgeLabelVertexContainerPair
         = std::pair<const EdgeLabelType_,
                           VertexContainerType_>;

        std::list<EdgeLabelVertexContainerPair> edge_labels_;

        static_assert(edge_label_container_sort_type == SortType::None,
                     "other sorting type are not implemented");

    public:
        using VertexContainerType = VertexContainerType_;
        using const_iterator
          = typename std::list<EdgeLabelVertexContainerPair>::const_iterator;
        using       iterator
          = typename std::list<EdgeLabelVertexContainerPair>::      iterator;

        inline const_iterator cbegin() const{
            return this->edge_labels_.cbegin();
        }
        inline const_iterator cend() const{
            return this->edge_labels_.cend();
        }
        inline iterator begin(){
            return this->edge_labels_.begin();
        }
        inline iterator end(){
            return this->edge_labels_.end();
        }

        inline bool HasEdgeLabel(const EdgeLabelType_& edge_label) const{
            if (edge_label_container_sort_type == SortType::None){
                for (const auto& it : this->edge_labels_)
                    if (it.first == edge_label)
                        return true;
                return false;
            }
            /// other sorting type are not implemented
            assert(false);
            return false;
        }

        inline const VertexContainerType_&
               const_vertex_container(const EdgeLabelType_& edge_label) const{
            assert(this->HasEdgeLabel(edge_label));
            if (edge_label_container_sort_type == SortType::None){
                for (const auto& it : this->edge_labels_)
                    if (it.first == edge_label)
                        return it.second;
                assert(false);
            }
            assert(false);
            return this->edge_labels_.front().second;
        }

        inline VertexContainerType_&
               vertex_container(const EdgeLabelType_& edge_label){
            assert(this->HasEdgeLabel(edge_label));
            if (edge_label_container_sort_type == SortType::None){
                for (auto& it : this->edge_labels_)
                    if (it.first == edge_label)
                        return it.second;
                assert(false);
            }
            assert(false);
            return this->edge_labels_.front().second;
        }

        inline std::pair<VertexContainerType_&,bool>
                   InsertVertexContainer(const EdgeLabelType_& edge_label){
            if (edge_label_container_sort_type == SortType::None){
                for (auto& it : this->edge_labels_)
                    if (it.first == edge_label){
                        /// already exist
                        return std::make_pair(it.second,false);
                    }
                /// not exist, has inserted
                this->edge_labels_.emplace_back(edge_label,
                                                VertexContainerType_());
                return std::make_pair(this->edge_labels_.back().second,
                                      true);
            }
            /// other sorting type are not implemented
            assert(false);
            return std::make_pair(this->edge_labels_.back().second,
                                  true);
        }
    };

    template<typename EdgeLabelType_,
             typename VertexContainerType_>
    class EdgeLabelContainer_<ContainerType::Vector,
                              EdgeLabelType_,VertexContainerType_>{
    private:
        using EdgeLabelVertexContainerPair
           = std::pair<EdgeLabelType_,
                       VertexContainerType_>;

        std::vector<EdgeLabelVertexContainerPair> edge_labels_;

        static_assert(edge_label_container_sort_type == SortType::None,
                     "other sorting type are not implemented");

    public:
        using VertexContainerType = VertexContainerType_;
        using const_iterator
          = typename std::vector<EdgeLabelVertexContainerPair>::const_iterator;
        using       iterator
          = typename std::vector<EdgeLabelVertexContainerPair>::      iterator;

        inline const_iterator cbegin() const{
            return this->edge_labels_.cbegin();
        }
        inline const_iterator cend() const{
            return this->edge_labels_.cend();
        }
        inline iterator begin(){
            return this->edge_labels_.begin();
        }
        inline iterator end(){
            return this->edge_labels_.end();
        }

        inline std::pair<VertexContainerType_&,bool>
                   InsertVertexContainer(const EdgeLabelType_& edge_label){
            if (edge_label_container_sort_type == SortType::None){
                for (auto& it : this->edge_labels_){
                    if (it.first == edge_label){
                        /// already exist
                        return std::pair<VertexContainerType_&,bool>
                                                    (it.second,false);
                    }
                }
                /// not exist, has inserted
                this->edge_labels_.emplace_back(edge_label,
                                                VertexContainerType_());
                return std::pair<VertexContainerType_&,bool>
                     (this->edge_labels_.back().second,true);
            }
            /// other sorting type are not implemented
            assert(false);
            return std::pair<VertexContainerType_&,bool>
                 (this->edge_labels_.back().second,true);
        }

        inline bool HasEdgeLabel(const EdgeLabelType_& edge_label) const{
            if (edge_label_container_sort_type == SortType::None){
                for (const auto& it : this->edge_labels_)
                    if (it.first == edge_label)
                        return true;
                return false;
            }
            /// other sorting type are not implemented
            assert(false);
            return false;
        }

        inline VertexContainerType_&
               vertex_container(const EdgeLabelType_& edge_label){
            assert(this->HasEdgeLabel(edge_label));
            if (edge_label_container_sort_type == SortType::None){
                for (auto& it : this->edge_labels_)
                    if (it.first == edge_label)
                        return it.second;
                assert(false);
            }
            assert(false);
            return this->edge_labels_.front().second;
        }

        inline const VertexContainerType_&
               const_vertex_container(const EdgeLabelType_& edge_label) const{
            assert(this->HasEdgeLabel(edge_label));
            if (edge_label_container_sort_type == SortType::None){
                for (const auto& it : this->edge_labels_)
                    if (it.first == edge_label)
                        return it.second;
                assert(false);
            }
            assert(false);
            return this->edge_labels_.front().second;
        }

        inline void shrink_to_fit() const{
            this->edge_labels_.shrink_to_fit();
            return;
        }
    };

    /// a inner class that is transparent to programmers,
    /// contains partial information about the edge
    /// used in the CompoundVertex
    template<bool allow_duplicate_edge_,
             bool   edge_has_attribute_,
             bool         meaning_less_ = false>/// trick the  compiler
    class DecomposedEdge_{
        /// the unspecified configuration should not be instantiated
        /// trick the compiler, equivalent to static_assert(false)
        static_assert( std::is_object<EdgeAttributeType>::value,
                     "this class should not be instantiated,\
                      do not have edge container in this case");
        static_assert(!std::is_object<EdgeAttributeType>::value,
                     "this class should not be instantiated,\
                      do not have edge container in this case");
    };

    /// not allow duplicate edge but has attribute
    /// therefore, has no ID
    template<bool meaning_less_>
    class DecomposedEdge_<false,true,meaning_less_>
        :public WithAttribute_<EdgeAttributeType,
                               edge_attribute_is_const,
                               edge_attribute_is_dynamic,
                               EdgeAttributeKeyType,
                               edge_attribute_container_type>{
    private:
        static_assert(!meaning_less_,"Illegal configuration");

        using WithAttributeType
            = WithAttribute_<EdgeAttributeType,
                             edge_attribute_is_const,
                             edge_attribute_is_dynamic,
                             EdgeAttributeKeyType,
                             edge_attribute_container_type>;

    public:
        template<typename... Args>
        DecomposedEdge_(const Args&... args)
                    :WithAttributeType(args...){
            return;
        }
    };

    /// allow duplicate edge and has attribute
    /// has both ID and attribute
    /// would be used in the EdgeContainer
    template<bool meaning_less_>
    class DecomposedEdge_< true,true,meaning_less_>
        :public DecomposedEdge_<false,true,meaning_less_>,
         public WithID_        <EdgeIDType>{
    private:
        static_assert(!meaning_less_,"Illegal configuration");

    public:
        template<typename... Args>
        DecomposedEdge_(const EdgeIDType& id,
                        const Args&...    args)
          :DecomposedEdge_<false,true,
                           meaning_less_>(args...),
                      WithID_<EdgeIDType>(id){
            return;
        }
    };

    template<bool     edge_has_attribute_,
             bool   allow_duplicate_edge_,
             typename DecomposedEdgeType_ = void,
             typename DecomposedEdgeContainerType_ = void>
    class CompoundVertex_;

    /// edge does not have attribute and not allows duplicate edges
    /// in this case, CompoundVertex is just a trivial structure that
    /// only contains the pointer to the vertex
    template<typename DecomposedEdgeType_,
             typename DecomposedEdgeContainerType_>
    class CompoundVertex_<false,false,
                          DecomposedEdgeType_,
                          DecomposedEdgeContainerType_>
                :public WithVertexPtr_<false>{/// holds a non-constant pointer
    protected:
        static_assert(std::is_void<DecomposedEdgeType_>::value
                   && std::is_void<DecomposedEdgeContainerType_>::value,
                     "Illegal configuration, should be void");
        using VertexPtr = typename WithVertexPtr_<false>::VertexPtr;

    public:
        CompoundVertex_(VertexPtr vertex)
           :WithVertexPtr_<false>(vertex){
            return;
        }

        inline constexpr DuplicateNumType duplicate_cardinality() const{
            return 1;
        }
    };

    /// edge does not have attributes but allows duplicate edges
    /// in this case, besides the pointer to the vertex, CompoundVertex
    /// also contains a counter of duplicate cardinality
    template<typename DecomposedEdgeType_,
             typename DecomposedEdgeContainerType_>
    class CompoundVertex_<false,true,
                          DecomposedEdgeType_,
                          DecomposedEdgeContainerType_>
        :public CompoundVertex_<false,false>{
    private:
        static_assert(std::is_void<DecomposedEdgeType_>::value
                   && std::is_void<DecomposedEdgeContainerType_>::value,
                     "Illegal configuration, should be void");

        DuplicateNumType duplicate_cardinality_;
        /// duplicate cardinality

    public:
        CompoundVertex_(Vertex* const vertex)
        :CompoundVertex_<false,false>(vertex){
            /// initialized to zero since the "AddDecomposedEdge()" would be
            /// called in the adding edge process
            this->duplicate_cardinality_ = 0;
            return;
        }
        inline void AddDecomposedEdge(){
            /// add an decomposed edge
            this->duplicate_cardinality_++;
            return;
        }
        inline const DuplicateNumType&
                     duplicate_cardinality() const{
            return this->duplicate_cardinality_;
        }
    };

    /// edge has attribute
    /// holds decomposed_edge instead of container of it
    template<typename DecomposedEdgeType_,
             typename DecomposedEdgeContainerType_>
    class CompoundVertex_<true,false,
                          DecomposedEdgeType_,
                          DecomposedEdgeContainerType_>
               :public CompoundVertex_<false,false>{
    private:
        static_assert(!std::is_void<DecomposedEdgeType_>::value
                    && std::is_void<DecomposedEdgeContainerType_>::value,
                      "Illegal configuration");

        DecomposedEdgeType_ decomposed_edge_;

    public:
        using DecomposedEdgeType = DecomposedEdgeType_;

        template<typename... Args>
        CompoundVertex_(Vertex* const  vertex,
                        const Args&... args)
         :CompoundVertex_<false,false>(vertex),
                      decomposed_edge_(args...){
            return;
        }

        inline const DecomposedEdgeType_& const_decomposed_edge() const{
            return this->decomposed_edge_;
        }

        inline DecomposedEdgeType_& decomposed_edge(){
            return this->decomposed_edge_;
        }

        inline constexpr DuplicateNumType duplicate_cardinality() const{
            return 1;/// since duplicate edge is not allowed
        }
    };

    /// edge has attribute
    /// holds the container of decomposed_edge
    template<typename DecomposedEdgeType_,
             typename DecomposedEdgeContainerType_>
    class CompoundVertex_<true,true,
                          DecomposedEdgeType_,
                          DecomposedEdgeContainerType_>
               :public CompoundVertex_<false,false>{
    private:
        static_assert(!std::is_void<DecomposedEdgeType_>::value
                    &&!std::is_void<DecomposedEdgeContainerType_>::value,
                      "Illegal configuration");

        DecomposedEdgeContainerType_ decomposed_edges_;

    public:
        using DecomposedEdgeContainerType = DecomposedEdgeContainerType_;
        template<typename... Args>
        CompoundVertex_(Vertex* const  vertex,
                        const Args&... args)
         :CompoundVertex_<false,false>(vertex),
                     decomposed_edges_(args...){
            return;
        }

        inline const DecomposedEdgeContainerType_&
               const_decomposed_edge_container() const{
            return this->decomposed_edges_;
        }

        inline DecomposedEdgeContainerType_&
               decomposed_edge_container(){
            return this->decomposed_edges_;
        }

        template<const bool judge_edge_attribute_is_dynamic
                                = edge_attribute_is_dynamic,
                 typename std::enable_if<!judge_edge_attribute_is_dynamic,
                                         bool>::type = false>
        inline std::pair<DecomposedEdgeType_&,bool>
                      AddDecomposedEdge(const EdgeIDType& id,
                                        const EdgeAttributeType& attribute){
            static_assert(judge_edge_attribute_is_dynamic
                             == edge_attribute_is_dynamic,
                         "Illegal usage of this method");
            return this->decomposed_edges_.InsertDecomposedEdge(id,attribute);
        }

        template<const bool judge_edge_attribute_is_dynamic
                                = edge_attribute_is_dynamic,
                 typename std::enable_if<judge_edge_attribute_is_dynamic,
                                         bool>::type = false>
        inline std::pair<DecomposedEdgeType_&,bool>
                      AddDecomposedEdge(const EdgeIDType& id){
            static_assert(judge_edge_attribute_is_dynamic
                             == edge_attribute_is_dynamic,
                         "Illegal usage of this method");
            return this->decomposed_edges_.InsertDecomposedEdge(id);
        }

        inline DuplicateNumType duplicate_cardinality() const{
            return this->decomposed_edges_.size();
        }
    };

    /// iterator of the first level container
    template<typename   EdgeLabelContainerType_,
             const bool is_const_>/// trick the compiler
    class EdgeLabelIterator_;
    /// iterator of the second level container
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool is_const_>
    class InnerVertexIterator_;
    /// package of VertexIterator_, for better data reuse
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool is_const_>
    class VertexIterator_;
    /// iterator of the third level container
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool has_decomposed_edge_container_,
             const bool is_const_>
    class InnerEdgeIterator_;/// a inner class, transparent
                             /// to the programmer
    /// a package of the EdgeIterator_
    /// to make it looks like an iterator on a single container
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool has_decomposed_edge_container_,
             const bool is_const_>
    class EdgeIterator_;

    /// iterator of the first level container
    template<typename   EdgeLabelContainerType_,
             const bool is_const_>
    class EdgeLabelIterator_{
    /// return all the edge labels
    /// iterator on the first level storage
    private:
        using VertexContainerType_ = typename EdgeLabelContainerType_
                                               ::VertexContainerType;

        using InnerEdgeLabelContainerType_
            = typename std::conditional<is_const_,
                    const EdgeLabelContainerType_,
                          EdgeLabelContainerType_>::type;

        typename std::conditional<is_const_,
              typename EdgeLabelContainerType_::const_iterator,
              typename EdgeLabelContainerType_::      iterator
                                                      >::type iterator_;

        const typename EdgeLabelContainerType_::const_iterator kEndIter_;

        template<const bool judge = is_const_,
                 typename std::enable_if<judge,bool>::type = false>
        inline typename EdgeLabelContainerType_::const_iterator
                                                       iterator_begin(
                  const EdgeLabelContainerType_& edge_label_container) const{
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return edge_label_container.cbegin();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline typename EdgeLabelContainerType_::iterator
                                                 iterator_begin(
                        EdgeLabelContainerType_& edge_label_container) const{
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return edge_label_container.begin();
        }

    protected:
        inline const VertexContainerType_& const_vertex_container() const{
            assert(!this->IsDone());
            return this->iterator_->second;
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline VertexContainerType_& vertex_container(){
            static_assert(judge == is_const_,
                         "illegal usage of this method");
            assert(!this->IsDone());
            return this->iterator_->second;
        }

        inline void ToNext(){
            this->iterator_++;
            return;
        }

        inline const EdgeLabelType& label() const{
            assert(!this->IsDone());
            return this->iterator_->first;
        }

    public:
        EdgeLabelIterator_(InnerEdgeLabelContainerType_& edge_label_container)
                         :iterator_(this->iterator_begin(edge_label_container)),
                                               kEndIter_(edge_label_container
                                                                      .cend()){
            assert(!this->IsDone());/// should not be empty
            return;
        }

        inline const EdgeLabelType& operator*() const{
            assert(!this->IsDone());
            return this->label();
        }

        inline const EdgeLabelType* const operator->() const{
            assert(!this->IsDone());
            return &(this->label());
        }

        inline EdgeLabelIterator_ operator++ (){
            ///prefix
            assert(!this->IsDone());
            this->ToNext();
            return *this;
        }

        inline EdgeLabelIterator_ operator++ (int){
            ///postfix
            assert(!this->IsDone());
            EdgeLabelIterator_ temp(*this);
            ++(*this);
            return temp;
        }

        inline bool IsDone() const{
            return this->iterator_ == this->kEndIter_;
        }
    };

    template<typename   EdgeLabelContainerType_,
             const bool is_const_>
    class InnerVertexIterator_<EdgeLabelContainerType_,
                      IteratorType::SpecifiedEdgeLabel,is_const_>{
    /// return all the vertexes connected to this vertex
    /// that satisfies some certain restrictions
    ///
    /// the iterator on the second level storage
    /// it can use the first level iterator to reuse the code
    private:
        using VertexContainerType_ = typename EdgeLabelContainerType_
                                               ::VertexContainerType;

        using CompoundVertexType_ = typename VertexContainerType_
                                            ::CompoundVertexType;

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

    protected:
        inline const CompoundVertexType_& const_compound_vertex() const{
            assert(!this->IsDone());
            return *(this->iterator_);
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline CompoundVertexType_& compound_vertex(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return *(this->iterator_);
        }

        inline void ToNext(){
            this->iterator_++;
            return;
        }

    public:
        InnerVertexIterator_(InnerVertexContainerType_& vertex_container)
                        :iterator_(this->iterator_begin(vertex_container)),
                                              kEndIter_(vertex_container
                                                                  .cend()){
            return;
        }

        inline const Vertex* const const_vertex_ptr() const{
            assert(!this->IsDone());
            return this->iterator_->const_vertex_ptr();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const vertex_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return this->iterator_->vertex_ptr();
        }

        inline bool IsDone() const{
            return this->iterator_ == this->kEndIter_;
        }
    };

    template<typename   EdgeLabelContainerType_,
             const bool is_const_>
    class InnerVertexIterator_<EdgeLabelContainerType_,
                    IteratorType::UnspecifiedEdgeLabel,is_const_>
        :protected EdgeLabelIterator_<EdgeLabelContainerType_,is_const_>{
    /// return all the vertexes connected to this vertex
    /// that satisfies some certain restrictions
    ///
    /// the iterator on the second level storage
    /// it can use the first level iterator to reuse the code
    private:
        using VertexContainerType_ = typename EdgeLabelContainerType_
                                               ::VertexContainerType;

        using InnerEdgeLabelContainerType_ =
            typename std::conditional<is_const_,
                  const EdgeLabelContainerType_,
                        EdgeLabelContainerType_>::type;

        using EdgeLabelIteratorType_
            = EdgeLabelIterator_<EdgeLabelContainerType_,is_const_>;

        using CompoundVertexType_
           = typename VertexContainerType_::CompoundVertexType;

        typename std::conditional<is_const_,
              typename VertexContainerType_::const_iterator,
              typename VertexContainerType_::      iterator>::type iterator_;

        template<const bool judge = is_const_,
                 typename std::enable_if<judge,bool>::type = false>
        inline typename VertexContainerType_::const_iterator
                                                    iterator_begin() const{
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return EdgeLabelIteratorType_::const_vertex_container()
                                                          .cbegin();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline typename VertexContainerType_::iterator
                                              iterator_begin(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return EdgeLabelIteratorType_::vertex_container()
                                                     .begin();
        }

    protected:
        inline const CompoundVertexType_& const_compound_vertex() const{
            assert(!this->IsDone());
            return *(this->iterator_);
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline CompoundVertexType_& compound_vertex(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return *(this->iterator_);
        }

        inline void ToNext(){
            assert(!this->IsDone());
            this->iterator_++;
            if (this->iterator_
             != EdgeLabelIteratorType_::const_vertex_container().cend()){
                /// has not visited all edges with this edge label
                return;
            }
            /// has already visited all edges with this edge label
            EdgeLabelIteratorType_::ToNext();/// to next edge label
            if (EdgeLabelIteratorType_::IsDone()){
                /// has visited all edge label
                assert(this->IsDone());
                return;
            }
            /// has not visited all edge label
            this->iterator_ = this->iterator_begin();
            return;
        }

    public:
        using EdgeLabelIteratorType_::IsDone;

        InnerVertexIterator_(
            InnerEdgeLabelContainerType_& edge_label_container)
                 :EdgeLabelIteratorType_( edge_label_container),
                               iterator_(this->iterator_begin()){
            assert(!this->IsDone());/// should not be empty
            return;
        }

        inline const Vertex* const const_vertex_ptr() const{
            assert(!this->IsDone());
            return this->iterator_->const_vertex_ptr();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const vertex_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return this->iterator_->vertex_ptr();
        }
    };

    template<typename EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool is_const_>
    class VertexIterator_
        :public InnerVertexIterator_<EdgeLabelContainerType_,
                                              iterator_type_,is_const_>{
    private:
        using InnerVertexIteratorType_
            = InnerVertexIterator_<EdgeLabelContainerType_,
                                            iterator_type_,is_const_>;

    public:
        using InnerVertexIteratorType_::InnerVertexIteratorType_;

        template<const bool judge = is_const_,
                 typename std::enable_if<judge,bool>::type = false>
        inline const Vertex* const operator->() const{
            static_assert(judge == is_const_,"Illegal usage of this method");
            assert(!this->IsDone());
            return InnerVertexIteratorType_::const_compound_vertex()
                                                     .const_vertex_ptr();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<judge,bool>::type = false>
        inline const Vertex& operator*() const{
            static_assert(judge == is_const_,"Illegal usage of this method");
            assert(!this->IsDone());
            return InnerVertexIteratorType_::const_compound_vertex()
                                                     .const_vertex_ptr();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const operator->(){
            static_assert(judge == is_const_,"Illegal usage of this method");
            assert(!this->IsDone());
            return InnerVertexIteratorType_::compound_vertex()
                                                     .vertex_ptr();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex& operator*(){
            static_assert(judge == is_const_,"Illegal usage of this method");
            assert(!this->IsDone());
            return InnerVertexIteratorType_::compound_vertex()
                                                     .vertex_ptr();
        }

        inline VertexIterator_ operator++ (){
            ///prefix
            InnerVertexIteratorType_::ToNext();
            return *this;
        }

        inline VertexIterator_ operator++ (int){
            ///postfix
            assert(!this->IsDone());
            VertexIterator_ temp(*this);
            ++(*this);
            return temp;
        }
    };

    template<const bool holds_edge_label_,
             const bool holds_attribute_ptr_,
             const bool holds_edge_id_,
             const bool is_const_>
    class EdgeInfo_{
        /// trick the compiler
        static_assert(!is_const_,"Unsupported configuration");
        static_assert( is_const_,"Unsupported configuration");
    };

    /// all other configurations has been resolved
    template<const bool is_const_>
    class EdgeInfo_<false,false,false,is_const_>
               :public WithVertexPtr_<is_const_>{
    private:
        using WithVertexPtrType = WithVertexPtr_<is_const_>;
    public:
        using WithVertexPtrType::WithVertexPtrType;
        using VertexPtr=typename WithVertexPtrType::VertexPtr;
    };

    /// whether holds edge label, the first configure to be resolved
    template<const bool holds_attribute_ptr_,
             const bool holds_edge_id_,
             const bool is_const_>
    class EdgeInfo_<true,holds_attribute_ptr_,
                         holds_edge_id_,is_const_>
         :public EdgeInfo_<false,holds_attribute_ptr_,
                                 holds_edge_id_,is_const_>,
          public WithLabel_<EdgeLabelType,edge_label_is_const || is_const_>{
    private:
        using WithLabelType = WithLabel_<EdgeLabelType,
                                         edge_label_is_const || is_const_>;

    public:
        template<typename... Args>
        EdgeInfo_(const EdgeLabelType& edge_label,
                  Args&...             args)
            :EdgeInfo_<false,holds_attribute_ptr_,holds_edge_id_,
                            is_const_>(args...),
                         WithLabelType(edge_label){
            return;
        }
    };

    /// whether holds attribute pointer, the second configure to be resolved
    template<const bool holds_edge_id_,
             const bool is_const_>
    class EdgeInfo_<false,true,holds_edge_id_,is_const_>
         :public EdgeInfo_<false,false,holds_edge_id_,is_const_>{
    private:
        using AttributeType
        = WithAttribute_<EdgeAttributeType,
                         edge_attribute_is_const,
                         edge_attribute_is_dynamic,
                         EdgeAttributeKeyType,
                         edge_attribute_container_type>;

        using AttributePtrType =
            typename std::conditional<is_const_,
                                const AttributeType* const,
                                      AttributeType* const
                                     >::type;

        AttributePtrType attribute_ptr_;

    public:
        template<typename... Args>
        EdgeInfo_(AttributePtrType attribute_ptr,
                  Args&...         args)
            :EdgeInfo_<false,false,holds_edge_id_,
                        is_const_>(args...),
                    attribute_ptr_(attribute_ptr){
            return;
        }

        inline const AttributeType* const const_attribute_ptr() const{
            return this->attribute_ptr_;
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline AttributeType* const attribute_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return this->attribute_ptr_;
        }
    };

    /// whether holds edge id, the third configure to be resolved
    template<const bool is_const_>
    class EdgeInfo_<false,false,true,is_const_>
         :public EdgeInfo_<false,false,false,is_const_>,
          public WithID_<EdgeIDType>{
    public:
        template<typename... Args>
        EdgeInfo_(const EdgeIDType& id,
                  Args&...          args)
            :EdgeInfo_<false,false,false,
                         is_const_>(args...),
                WithID_<EdgeIDType>(id){
            return;
        }
    };

    /// iterator of the third level container
    /// does not have container for decomposed edges
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool is_const_>
    class InnerEdgeIterator_<EdgeLabelContainerType_,
                             iterator_type_,false,is_const_>
    :protected VertexIterator_<EdgeLabelContainerType_,
                               iterator_type_,is_const_>{
    private:
        static_assert(!allow_duplicate_edge || !edge_has_attribute,
                     "Illegal configuration, should have decomposed edge\
                      container in this case");

        using VertexContainerType_ = typename EdgeLabelContainerType_
                                               ::VertexContainerType;

        using InnerEdgeLabelContainerType_ =
            typename std::conditional<is_const_,
                  const EdgeLabelContainerType_,
                        EdgeLabelContainerType_>::type;

        using InnerVertexContainerType_ =
            typename std::conditional<is_const_,
                     const VertexContainerType_,
                           VertexContainerType_>::type;

        using EdgeLabelIteratorType_
            = EdgeLabelIterator_<EdgeLabelContainerType_,is_const_>;

        using VertexIteratorType_
            = VertexIterator_<EdgeLabelContainerType_,
                              iterator_type_,is_const_>;

        using InnerEdgeIteratorType_
            = InnerEdgeIterator_<EdgeLabelContainerType_,
                                 iterator_type_,
                                 false,is_const_>;

        using EdgeInfoType_ = EdgeInfo_<iterator_type_
                                     == IteratorType::SpecifiedEdgeLabel,
                                        false,/// not holds attribute pointer
                                        false,/// not holds edge id
                                        is_const_>;

        using VertexPtr = typename EdgeInfoType_::VertexPtr;

        EdgeInfoType_ edge_info_;

    protected:
        using VertexIteratorType_::IsDone;
        using VertexIteratorType_::ToNext;

    public:
        template<const enum IteratorType judge = iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::SpecifiedEdgeLabel,
                            bool>::type = false>
        InnerEdgeIterator_(InnerVertexContainerType_& vertex_container,
                           VertexPtr                  src_ptr,
                           const EdgeLabelType&       edge_label)
                                 :VertexIteratorType_(vertex_container),
                                           edge_info_(src_ptr,edge_label){
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            return;
        }

        template<const enum IteratorType judge = iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::UnspecifiedEdgeLabel,
                            bool>::type = false>
        InnerEdgeIterator_(InnerEdgeLabelContainerType_& edge_label_container,
                           VertexPtr                     src_ptr)
                                    :VertexIteratorType_(edge_label_container),
                                              edge_info_(src_ptr){
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            return;
        }

        inline const Vertex* const const_src_ptr() const{
            assert(!this->IsDone());
            return this->edge_info_.const_vertex_ptr();
        }

        inline const VertexIDType& src_id() const{
            assert(!this->IsDone());
            return this->const_src_ptr()->id();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const src_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return this->edge_info_.vertex_ptr();
        }

        inline const Vertex* const const_dst_ptr() const{
            assert(!this->IsDone());
            return VertexIteratorType_::const_compound_vertex()
                                                .const_vertex_ptr();
        }

        inline const VertexIDType& dst_id() const{
            assert(!this->IsDone());
            return this->const_dst_ptr()->id();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const dst_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return VertexIteratorType_::compound_vertex()
                                                .vertex_ptr();
        }

        template<const enum IteratorType judge =  iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::SpecifiedEdgeLabel,
                            bool>::type = false>
        inline const EdgeLabelType& label() const{
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            /// edge_info_ holds the label of the edge in this case
            return this->edge_info_.label();
        }

        template<const enum IteratorType judge = iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::UnspecifiedEdgeLabel,
                            bool>::type = false>
        inline const EdgeLabelType& label() const{
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            static_assert(std::is_base_of<EdgeLabelIteratorType_,
                                          InnerEdgeIteratorType_>::value,
                         "EdgeIterator should be the derived class \
                          of EdgeLabelIterator in this case");
            assert(!this->IsDone());
            return EdgeLabelIteratorType_::label();
        }

        template<const bool judge = !allow_duplicate_edge
                                  && edge_has_attribute,
                 typename std::enable_if<judge>::type>
        inline const EdgeAttributeType& const_attribute() const{
            static_assert(judge ==!allow_duplicate_edge
                                && edge_has_attribute,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return  VertexIteratorType_::const_compound_vertex()
                                        .const_decomposed_edge()
                                        .const_attribute();
        }

        template<bool judge = !allow_duplicate_edge
                            && edge_has_attribute
                            &&!is_const_,
                 typename std::enable_if<judge>::type>
        inline EdgeAttributeType& attribute(){
            static_assert(judge ==!allow_duplicate_edge
                                && edge_has_attribute
                                &&!is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return  VertexIteratorType_::compound_vertex()
                                        .decomposed_edge()
                                        .attribute();
        }

        inline const DuplicateNumType& duplicate_cardinality() const{
            assert(!this->IsDone());
            return VertexIteratorType_::const_compound_vertex()
                                       .duplicate_cardinality();
        }

        inline std::string to_string() const{
            static_assert(!edge_has_attribute,
                         "to_string method does not supports \
                          edge that has attribute");
            return "src id: " + std::to_string(this->const_src_ptr()->id())
                 + "dst id: " + std::to_string(this->const_dst_ptr()->id())
                 + " label: " +                this->label().to_string();
        }
    };

    /// have container for decomposed edges
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool is_const_>
    class InnerEdgeIterator_<EdgeLabelContainerType_,
                             iterator_type_,true,is_const_>
        :protected VertexIterator_<EdgeLabelContainerType_,
                                   iterator_type_,is_const_>{
    private:
        using VertexContainerType_ = typename EdgeLabelContainerType_
                                               ::VertexContainerType;

        using DecomposedEdgeContainerType_ = typename VertexContainerType_
                                            ::CompoundVertexType
                                            ::DecomposedEdgeContainerType;

        using InnerEdgeLabelContainerType_ =
            typename std::conditional<is_const_,
                  const EdgeLabelContainerType_,
                        EdgeLabelContainerType_>::type;

        using InnerVertexContainerType_ =
            typename std::conditional<is_const_,
                     const VertexContainerType_,
                           VertexContainerType_>::type;

        using EdgeLabelIteratorType_
            = EdgeLabelIterator_<EdgeLabelContainerType_,
                                 is_const_>;

        using VertexIteratorType_
            = VertexIterator_<EdgeLabelContainerType_,
                              iterator_type_,is_const_>;

        using InnerEdgeIteratorType_
            = InnerEdgeIterator_<EdgeLabelContainerType_,
                                 iterator_type_,
                                 true,is_const_>;

        using EdgeInfoType_ = EdgeInfo_<iterator_type_
                                     == IteratorType::SpecifiedEdgeLabel,
                                        false,/// not holds attribute pointer
                                        false,/// not holds edge id
                                        is_const_>;

        using VertexPtr = typename EdgeInfoType_::VertexPtr;

        EdgeInfoType_ edge_info_;

        typename std::conditional<is_const_,
             typename DecomposedEdgeContainerType_::const_iterator,
             typename DecomposedEdgeContainerType_::      iterator
                                 >::type decomposed_edge_iterator_;

        template<const bool judge = is_const_,
                 typename std::enable_if<judge,bool>::type = false>
        inline typename DecomposedEdgeContainerType_::const_iterator
                                                      iterator_begin() const{
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return VertexIteratorType_::const_compound_vertex()
                       .const_decomposed_edge_container().cbegin();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline typename DecomposedEdgeContainerType_::iterator
                                                      iterator_begin() const{
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return VertexIteratorType_::compound_vertex()
                             .decomposed_edge_container().begin();
        }

    protected:
        using VertexIteratorType_::IsDone;

        inline void ToNext(){
            assert(!this->IsDone());
            this->decomposed_edge_iterator_++;
            if (this->decomposed_edge_iterator_
             != VertexIteratorType_::const_compound_vertex()
                          .const_decomposed_edge_container().cend()){
                /// has not visited all elements
                /// in this container of decomposed edge
                return;
            }
            /// has visited all elements in this
            /// decomposed_edge_container
            VertexIteratorType_::ToNext();
            if (this->IsDone()){
                /// has already visited all elements
                /// in this container of decomposed edge
                return;
            }
            /// visits the next decomposed_edge_container
            /// the next decomposed_edge_container should not be empty
            assert(VertexIteratorType_::const_compound_vertex()
                             .const_decomposed_edge_container().cbegin()
                != VertexIteratorType_::const_compound_vertex()
                             .const_decomposed_edge_container().cend  ());
            this->decomposed_edge_iterator_ = this->iterator_begin();
            return;
        }

    public:
        template<const enum IteratorType judge = iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::SpecifiedEdgeLabel,
                            bool>::type = false>
        InnerEdgeIterator_(InnerVertexContainerType_& vertex_container,
                           VertexPtr                  src_ptr,
                           const EdgeLabelType&       edge_label)
                                 :VertexIteratorType_(vertex_container),
                                           edge_info_(src_ptr,edge_label),
                            decomposed_edge_iterator_(this->iterator_begin()){
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return;
        }

        template<const enum IteratorType judge = iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::UnspecifiedEdgeLabel,
                            bool>::type = false>
        InnerEdgeIterator_(InnerEdgeLabelContainerType_& edge_label_container,
                           VertexPtr                     src_ptr)
                                  :VertexIteratorType_(edge_label_container),
                                            edge_info_(src_ptr),
                             decomposed_edge_iterator_(this->iterator_begin()){
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return;
        }

        inline const Vertex* const const_src_ptr() const{
            assert(!this->IsDone());
            return this->edge_info_.const_vertex_ptr();
        }

        inline const VertexIDType& src_id() const{
            assert(!this->IsDone());
            return this->const_src_ptr()->id();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const src_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return this->edge_info_.vertex_ptr();
        }

        inline const Vertex* const const_dst_ptr() const{
            assert(!this->IsDone());
            return VertexIteratorType_::const_compound_vertex()
                                            .const_vertex_ptr();
        }

        inline const VertexIDType& dst_id() const{
            assert(!this->IsDone());
            return this->const_dst_ptr()->id();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const dst_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return VertexIteratorType_::compound_vertex()
                                                .vertex_ptr();
        }

        template<const enum IteratorType judge =  iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::SpecifiedEdgeLabel,
                            bool>::type = false>
        inline const EdgeLabelType& label() const{
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            /// edge_info_ holds the label of the edge in this case
            return this->edge_info_.label();
        }

        template<const enum IteratorType judge =  iterator_type_,
                 typename std::enable_if<
                            judge == IteratorType::UnspecifiedEdgeLabel,
                            bool>::type = false>
        inline const EdgeLabelType& label() const{
            static_assert(judge == iterator_type_,
                         "Illegal usage of this method");
            static_assert(std::is_base_of<EdgeLabelIteratorType_,
                                          InnerEdgeIteratorType_>::value,
                         "EdgeIterator should be the derived class \
                          of EdgeLabelIterator in this case");
            assert(!this->IsDone());
            return EdgeLabelIteratorType_::label();
        }

        inline const EdgeIDType& id() const{
            assert(!this->IsDone());
            return this->decomposed_edge_iterator_->id();
        }

        inline const EdgeAttributeType& const_attribute() const{
            assert(!this->IsDone());
            return this->decomposed_edge_iterator_->const_attribute();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline EdgeAttributeType& attribute(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            assert(!this->IsDone());
            return this->decomposed_edge_iterator_->attribute();
        }

        inline const DuplicateNumType& duplicate_cardinality() const{
            assert(!this->IsDone());
            return this->decomposed_edge_iterator_
                       ->duplicate_cardinality();
        }

        inline std::string to_string() const{
            return "src id: " + std::to_string(this->const_src_ptr()->id())
                 + "dst id: " + std::to_string(this->const_dst_ptr()->id())
                 + " label: " + this->label().to_string();
        }
    };

    /// single element iterator, not has decomposed edge container
    template<typename   EdgeLabelContainerType_,
             const bool is_const_>
    class InnerEdgeIterator_<EdgeLabelContainerType_,
                             IteratorType::SingleElement,
                             false,is_const_>{
    private:
        using EdgeInfoType_ = EdgeInfo_<true,edge_has_attribute,
                                        false,is_const_>;

    protected:
        using VertexPtr = typename EdgeInfoType_::VertexPtr;

    private:
        EdgeInfoType_ edge_info_;

        VertexPtr dst_vertex_ptr_;

        const DuplicateNumType duplicate_cardinality_;

    public:
        template<typename... Args>
        InnerEdgeIterator_(VertexPtr         dst_ptr,
                     const DuplicateNumType& duplicate_cardinality,
                           Args&...          args)
                                 :edge_info_(args...),
                             dst_vertex_ptr_(dst_ptr),
                      duplicate_cardinality_(duplicate_cardinality){
            return;
        }

        inline const Vertex* const const_src_ptr() const{
            return this->edge_info_.const_vertex_ptr();
        }

        inline const Vertex* const const_dst_ptr() const{
            return this->dst_vertex_ptr_;
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const src_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return this->edge_info_.vertex_ptr();
        }

        template<const bool judge = is_const_,
                 typename std::enable_if<!judge,bool>::type = false>
        inline Vertex* const dst_ptr(){
            static_assert(judge == is_const_,
                         "Illegal usage of this method");
            return this->dst_vertex_ptr_;
        }

        inline const EdgeLabelType& label() const{
            /// edge_info_ holds the label of the edge in this case
            return this->edge_info_.label();
        }

        template<const bool judge_edge_attribute_is_dynamic
                                = edge_attribute_is_dynamic,
                 typename std::enable_if<!judge_edge_attribute_is_dynamic,
                                         bool>::type = false>
        inline const EdgeAttributeType& const_attribute() const{
            static_assert(judge_edge_attribute_is_dynamic
                             == edge_attribute_is_dynamic,
                         "Illegal usage of this method");
            return this->edge_info_.const_attribute_ptr();
        }

        template<const bool judge_is_const = is_const_,
                 const bool judge_edge_attribute_is_dynamic
                                = edge_attribute_is_dynamic,
                 typename std::enable_if<!judge_is_const
                                       &&!judge_edge_attribute_is_dynamic,
                                         bool>::type = false>
        inline EdgeAttributeType& attribute(){
            static_assert(judge_is_const == is_const_
                       && judge_edge_attribute_is_dynamic
                             == edge_attribute_is_dynamic,
                         "Illegal usage of this method");
            return this->edge_info_.attribute_ptr();
        }

        template<typename ConcreteDataType>
        inline const ConcreteDataType&
               const_attribute(const EdgeAttributeKeyType& key) const{
            return this->edge_info_.const_attribute_ptr()
                                  ->const_attribute<ConcreteDataType>(key);
        }

        template<typename ConcreteDataType,
                 const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline ConcreteDataType& attribute(const EdgeAttributeKeyType& key){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            /// the constant attribute should not be modified
            return this->edge_info_.attribute_ptr()
                                  ->attribute<ConcreteDataType>(key);
        }

        template<typename ConcreteDataType>
        inline bool add_attribute(const EdgeAttributeKeyType& key,
                                  const ConcreteDataType&     value){
            return this->edge_info_.attribute_ptr()
                                  ->add_attribute(key,value);
        }

        template<typename   ConcreteDataType,
                 const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline bool set_attribute(const EdgeAttributeKeyType& key,
                                  const ConcreteDataType&     value){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this->edge_info_.attribute_ptr()
                                  ->set_attribute(key,value);
        }

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<!judge_is_const,bool>::type = false>
        inline bool remove_attribute(const EdgeAttributeKeyType& key){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this->edge_info_.attribute_ptr()
                                  ->remove_attribute(key);
        }

        inline const DuplicateNumType& duplicate_cardinality() const{
            return this->duplicate_cardinality_;
        }
    };

    /// single element iterator, has decomposed edge container
    /// needs to hold the ID of edge
    template<typename   EdgeLabelContainerType_,
             const bool is_const_>
    class InnerEdgeIterator_<EdgeLabelContainerType_,
                             IteratorType::SingleElement,
                             true,is_const_>
        :public InnerEdgeIterator_<EdgeLabelContainerType_,
                                   IteratorType::SingleElement,
                                   false,is_const_>,
         public WithID_<EdgeIDType>{
    private:
        using InnerEdgeIteratorType
            = InnerEdgeIterator_<EdgeLabelContainerType_,
                                 IteratorType::SingleElement,
                                 false,is_const_>;
        using WithIDType = WithID_<EdgeIDType>;
        using typename InnerEdgeIteratorType::VertexPtr;

    public:
        template<typename... Args>
        InnerEdgeIterator_(VertexPtr         dst_ptr,
                     const DuplicateNumType& duplicate_cardinality,
                     const EdgeIDType&       id,
                           Args&...          args)
                      :InnerEdgeIteratorType(dst_ptr,
                                             duplicate_cardinality,
                                             args...),
                                  WithIDType(id){
            return;
        }
    };

    /// non-single element iterator
    template<typename   EdgeLabelContainerType_,
             const enum IteratorType iterator_type_,
             const bool has_decomposed_edge_container_,
             const bool is_const_>
    class EdgeIterator_
        :protected InnerEdgeIterator_<EdgeLabelContainerType_,
                                      iterator_type_,
                                      has_decomposed_edge_container_,
                                      is_const_>{
    private:
        using InnerEdgeIteratorType_
            = InnerEdgeIterator_<EdgeLabelContainerType_,
                                 iterator_type_,
                                 has_decomposed_edge_container_,
                                 is_const_>;

    public:
        using InnerEdgeIteratorType_::InnerEdgeIteratorType_;

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<
                    judge_is_const,bool>::type = false>
        inline const InnerEdgeIteratorType_* const operator->() const{
            assert(!this->IsDone());
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this;
        }

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<
                   !judge_is_const,bool>::type = false>
        inline InnerEdgeIteratorType_* const operator->(){
            assert(!this->IsDone());
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this;
        }

        inline bool IsDone() const{
            return InnerEdgeIteratorType_::IsDone();
        }

        inline EdgeIterator_ operator++(){
            ///prefix
            InnerEdgeIteratorType_::ToNext();
            return (*this);
        }

        inline EdgeIterator_ operator++(int){
            ///postfix
            EdgeIterator_ temp(*this);
            ++(*this);
            return temp;
        }
    };

    /// single element iterator
    template<typename   EdgeLabelContainerType_,
             const bool has_decomposed_edge_container_,
             const bool is_const_>
    class EdgeIterator_<EdgeLabelContainerType_,
                        IteratorType::SingleElement,
                        has_decomposed_edge_container_,
                        is_const_>
        :protected InnerEdgeIterator_<EdgeLabelContainerType_,
                                      IteratorType::SingleElement,
                                      has_decomposed_edge_container_,
                                      is_const_>{
    private:
        using InnerEdgeIteratorType_
            = InnerEdgeIterator_<EdgeLabelContainerType_,
                                 IteratorType::SingleElement,
                                 has_decomposed_edge_container_,
                                 is_const_>;

    public:
        using InnerEdgeIteratorType_::InnerEdgeIteratorType_;

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<
                    judge_is_const,bool>::type = false>
        inline const InnerEdgeIteratorType_* const operator->() const{
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this;
        }

        template<const bool judge_is_const = is_const_,
                 typename std::enable_if<
                   !judge_is_const,bool>::type = false>
        inline InnerEdgeIteratorType_* const operator->(){
            static_assert(judge_is_const == is_const_,
                         "Illegal usage of this method");
            return this;
        }
    };

private:
    /// only this class should be defined as an inner class
    /// a inner class that is transparent to programmers,
    /// stores the information of the vertex itself,
    /// contains id, label, attribute
    template<const bool vertex_has_attribute_,
             const bool meaning_less_ = false>
    class Vertex_;

    /// vertex that only has id and label
    template<const bool meaning_less_>
    class Vertex_<false,meaning_less_>
        :public WithID_   <VertexIDType>,
         public WithLabel_<VertexLabelType,vertex_label_is_const>{
    private:
        static_assert(!meaning_less_,"Illegal configuration");

        using WithIDType    = WithID_   <VertexIDType>;
        using WithLabelType = WithLabel_<VertexLabelType,
                                         vertex_label_is_const>;

    public:
        Vertex_(const    VertexIDType& id,
                const VertexLabelType& label)
                           :WithIDType(id),
                         WithLabelType(label){
            return;
        }
    };

    /// vertex has attribute in addition to id and label
    template<const bool meaning_less_>
    class Vertex_< true,meaning_less_>
        :public Vertex_<false,meaning_less_>,
         public WithAttribute_<VertexAttributeType,
                               vertex_attribute_is_const,
                               vertex_attribute_is_dynamic,
                               VertexAttributeKeyType,
                               vertex_attribute_container_type>{
    private:
        static_assert(!meaning_less_,"Illegal configuration");

        using WithAttributeType
            = WithAttribute_<VertexAttributeType,
                             vertex_attribute_is_const,
                             vertex_attribute_is_dynamic,
                             VertexAttributeKeyType,
                             vertex_attribute_container_type>;

    public:
        template<const bool judge_is_dynamic = vertex_attribute_is_dynamic,
                 typename std::enable_if<!judge_is_dynamic,bool>::type = false>
        Vertex_(const VertexIDType&        id,
                const VertexLabelType&     label,
                const VertexAttributeType& attribute)
                           :Vertex_<false>(id,label),
                         WithAttributeType(attribute){
            static_assert(judge_is_dynamic == vertex_attribute_is_dynamic,
                          "Illegal usage of this method");
            /// the dynamic attribute should not be initialized
            /// in the construct method
            return;
        }

        template<const bool judge_is_const   = vertex_attribute_is_const,
                 const bool judge_is_dynamic = vertex_attribute_is_dynamic,
                 typename std::enable_if<!judge_is_const
                                       || judge_is_dynamic,bool>::type = false>
        Vertex_(const VertexIDType&    id,
                const VertexLabelType& label)
                       :Vertex_<false>(id,label),
                     WithAttributeType(){
            static_assert((judge_is_const   == vertex_attribute_is_const)
                        &&(judge_is_dynamic == vertex_attribute_is_dynamic),
                         "Illegal usage of this method");
            /// the constant non-dynamic attribute should be
            /// initialized in the construct method
            return;
        }
    };

    static constexpr bool
        kHasDecomposedEdgeContainer_
                     = edge_has_attribute && allow_duplicate_edge;

    /// stores the information of the vertex itself
    Vertex_<vertex_has_attribute> vertex_;

    /// forbidden the copy construction
    Vertex(const Vertex& ) = delete;
    Vertex(const Vertex&&) = delete;
    Vertex& operator=(const Vertex& ) = delete;
    Vertex& operator=(const Vertex&&) = delete;

    using DecomposedEdgeType
        = DecomposedEdge_<allow_duplicate_edge,
                          edge_has_attribute>;

    using DecomposedEdgeContainerType
        = DecomposedEdgeContainer_<decomposed_edge_container_type,
                                   DecomposedEdgeType>;

    using CompoundVertexType
        = typename std::conditional<edge_has_attribute
                                 && allow_duplicate_edge,
                    CompoundVertex_<edge_has_attribute,
                                    allow_duplicate_edge,
                                    DecomposedEdgeType,
                                    DecomposedEdgeContainerType>,
          typename std::conditional<edge_has_attribute,/// not allow duplicate
                    CompoundVertex_<edge_has_attribute,/// edge in this case
                                    allow_duplicate_edge,
                                    DecomposedEdgeType>,
                    CompoundVertex_<edge_has_attribute,
                                    allow_duplicate_edge>
                                   >::type
                               >::type;

    using VertexContainerType
        = VertexContainer_<vertex_container_type,
                           CompoundVertexType>;

    using EdgeLabelContainerType
        = EdgeLabelContainer_<edge_label_container_type,
                              EdgeLabelType,
                              VertexContainerType>;
    /// input edge
    using InputCompoundVertexType
             = CompoundVertex_<false,/// input edges not store attribute
                               allow_duplicate_edge>;

    using InputVertexContainerType
             = VertexContainer_<vertex_container_type,
                                InputCompoundVertexType>;

    using InputEdgeLabelContainerType
             = EdgeLabelContainer_<edge_label_container_type,
                                   EdgeLabelType,
                                   InputVertexContainerType>;

    template<typename OutputEdgeLabelContainerType_,
             typename  InputEdgeLabelContainerType_ = void>
    class Edges_;

    /// only holds output edges
    template<typename OutputEdgeLabelContainerType_>
    class Edges_<OutputEdgeLabelContainerType_,void>{
    private:
        static_assert(!std::is_void<OutputEdgeLabelContainerType_>::value,
                     "Illegal configuration");
        OutputEdgeLabelContainerType_ out_edges_;

    public:
        inline const OutputEdgeLabelContainerType_& const_out_edges() const{
            return this->out_edges_;
        }
        inline OutputEdgeLabelContainerType_& out_edges(){
            return this->out_edges_;
        }
    };

    /// holds both input and output edges
    template<typename OutputEdgeLabelContainerType_,
             typename  InputEdgeLabelContainerType_>
    class Edges_
        :public Edges_<OutputEdgeLabelContainerType_>{
    private:
        static_assert(!std::is_void<OutputEdgeLabelContainerType_>::value
                    &&!std::is_void< InputEdgeLabelContainerType_>::value,
                     "Illegal configuration");
        InputEdgeLabelContainerType_ in_edges_;

    public:
        inline const InputEdgeLabelContainerType_& const_in_edges() const{
            return this->in_edges_;
        }
        inline InputEdgeLabelContainerType_& in_edges(){
            return this->in_edges_;
        }
    };

    typename std::conditional<store_type == StoreData::OutEdge,
                          Edges_<EdgeLabelContainerType>,
                          Edges_<EdgeLabelContainerType,
                            InputEdgeLabelContainerType>>::type edges_;

public:
    using IDType        = VertexIDType;
    using LabelType     = VertexLabelType;
    using AttributeType = VertexAttributeType;

    template<const enum StoreData store_data_,
             const bool meaning_less_ = false> /// trick the compiler
    class IteratorTypes_;

    template<const bool meaning_less_>
    class IteratorTypes_<StoreData::OutEdge,meaning_less_>{
    private:
        static_assert(!meaning_less_,"Illegal configuration");
    public:
        /// another name of EdgeLabelIterator
        using const_edge_label
                    = EdgeLabelIterator_<EdgeLabelContainerType,true>;
        /// alias template of VertexIterator
        template<enum IteratorType iterator_type_>
        using vertex
            = VertexIterator_<EdgeLabelContainerType,
                              iterator_type_,false>;
        template<enum IteratorType iterator_type_>
        using const_vertex
            = VertexIterator_<EdgeLabelContainerType,
                              iterator_type_, true>;
        /// alias template of EdgeIterator_
        template<enum IteratorType iterator_type_>
        using edge
            = EdgeIterator_<EdgeLabelContainerType,
                            iterator_type_,
                            kHasDecomposedEdgeContainer_,
                            false>;
        template<enum IteratorType iterator_type_>
        using const_edge
            = EdgeIterator_<EdgeLabelContainerType,
                            iterator_type_,
                            kHasDecomposedEdgeContainer_,
                            true>;
    };

    template<const bool meaning_less_>
    class IteratorTypes_<StoreData::InOutEdge,meaning_less_>
                 :public IteratorTypes_<StoreData::OutEdge>{
    private:
        static_assert(!meaning_less_,"Illegal configuration");
    public:
        /// another name of EdgeLabelIterator
        using const_input_edge_label
                    = EdgeLabelIterator_<InputEdgeLabelContainerType,true>;
        /// alias template of VertexIterator
        template<enum IteratorType iterator_type_>
        using input_vertex
            = VertexIterator_<InputEdgeLabelContainerType,
                              iterator_type_,false>;
        template<enum IteratorType iterator_type_>
        using const_input_vertex
            = VertexIterator_<InputEdgeLabelContainerType,
                              iterator_type_,true>;
        /// alias template of EdgeIterator_
        template<enum IteratorType iterator_type_>
        using input_edge
            = EdgeIterator_<InputEdgeLabelContainerType,
                            iterator_type_,
                            kHasDecomposedEdgeContainer_,
                            false>;
        template<enum IteratorType iterator_type_>
        using const_input_edge
            = EdgeIterator_<InputEdgeLabelContainerType,
                            iterator_type_,
                            kHasDecomposedEdgeContainer_,
                            true>;
    };

    using iterator = IteratorTypes_<StoreData::InOutEdge>;

    template<class... Args>
    Vertex(const Args&... args):vertex_(args...){
        return;
    }

    inline const VertexIDType& id() const{
        return this->vertex_.id();
    }

    inline const VertexLabelType& label() const{
        return this->vertex_.label();
    }

    template<const bool judge = vertex_label_is_const,
             typename std::enable_if<!judge,bool>::type=true>
    inline void set_label(const VertexLabelType& label){
        static_assert(judge == vertex_label_is_const,
                     "Illegal usage of this method");
        return this->vertex_.set_label(label);
    }

    template<const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             typename std::enable_if<judge_vertex_has_attribute
                                  &&!judge_vertex_attribute_is_dynamic,
                                     bool>::type=true>
    inline const VertexAttributeType& const_attribute() const{
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic,
                     "Illegal usage of this method");
        return this->vertex_.const_attribute();
    }

    template<typename   ConcreteDataType,
             const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             typename std::enable_if<judge_vertex_has_attribute
                                  && judge_vertex_attribute_is_dynamic,
                                     bool>::type=true>
    inline const ConcreteDataType& const_attribute
          (const VertexAttributeKeyType& key) const{
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic,
                     "Illegal usage of this method");
        return this->vertex_.const_attribute<ConcreteDataType>(key);
    }

    template<const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             const bool judge_vertex_attribute_is_const
                            = vertex_attribute_is_const,
             typename std::enable_if<judge_vertex_has_attribute
                                  &&!judge_vertex_attribute_is_dynamic
                                  &&!judge_vertex_attribute_is_const,
                                     bool>::type=true>
    inline VertexAttributeType& attribute(){
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic
                   && judge_vertex_attribute_is_const
                         == vertex_attribute_is_const,
                     "Illegal usage of this method");
        return this->vertex_.attribute();
    }

    template<typename   ConcreteDataType,
             const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             const bool judge_vertex_attribute_is_const
                            = vertex_attribute_is_const,
             typename std::enable_if<judge_vertex_has_attribute
                                  && judge_vertex_attribute_is_dynamic
                                  &&!judge_vertex_attribute_is_const,
                                     bool>::type=true>
    inline ConcreteDataType& attribute
          (const VertexAttributeKeyType& key){
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic
                   && judge_vertex_attribute_is_const
                         == vertex_attribute_is_const,
                     "Illegal usage of this method");
        return this->vertex_.attribute<ConcreteDataType>(key);
    }

    template<typename   ConcreteDataType,
             const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             typename std::enable_if<judge_vertex_has_attribute
                                  && judge_vertex_attribute_is_dynamic,
                                     bool>::type=true>
    inline bool add_attribute(const VertexAttributeKeyType& key,
                              const       ConcreteDataType& value){
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic,
                     "Illegal usage of this method");
        return this->vertex_.add_attribute(key,value);
    }

    template<typename   ConcreteDataType,
             const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             const bool judge_vertex_attribute_is_const
                            = vertex_attribute_is_const,
             typename std::enable_if<judge_vertex_has_attribute
                                  && judge_vertex_attribute_is_dynamic
                                  &&!judge_vertex_attribute_is_const,
                                     bool>::type=true>
    inline bool set_attribute(const VertexAttributeKeyType& key,
                              const       ConcreteDataType& value){
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic
                   && judge_vertex_attribute_is_const
                         == vertex_attribute_is_const,
                     "Illegal usage of this method");
        return this->vertex_.set_attribute(key,value);
    }

    template<const bool judge_vertex_has_attribute
                            = vertex_has_attribute,
             const bool judge_vertex_attribute_is_dynamic
                            = vertex_attribute_is_dynamic,
             const bool judge_vertex_attribute_is_const
                            = vertex_attribute_is_const,
             typename std::enable_if<judge_vertex_has_attribute
                                  && judge_vertex_attribute_is_dynamic
                                  &&!judge_vertex_attribute_is_const,
                                     bool>::type=true>
    inline bool remove_attribute(const VertexAttributeKeyType& key){
        static_assert(judge_vertex_has_attribute
                         == vertex_has_attribute
                   && judge_vertex_attribute_is_dynamic
                         == vertex_attribute_is_dynamic
                   && judge_vertex_attribute_is_const
                         == vertex_attribute_is_const,
                     "Illegal usage of this method");
        return this->vertex_.remove_attribute(key);
    }

    /// return the iterator of the first level container
    /// only have const iterator
    inline typename iterator::const_edge_label
        edge_label_cbegin() const{
        /// visit all edge labels
        return
           typename iterator::const_edge_label(this->edges_.const_out_edges());
    }

    /// return the iterator of the vertex container with all edge label
    /// constant iterator
    inline typename iterator
         ::template const_vertex<IteratorType::UnspecifiedEdgeLabel>
        vertex_cbegin() const{
        /// visit the vertexes connected with all edge labels
        return
           typename iterator
         ::template const_vertex<IteratorType::UnspecifiedEdgeLabel>
                                    (this->edges_.const_out_edges());
    }
    /// non-constant iterator
    inline typename iterator
         ::template vertex<IteratorType::UnspecifiedEdgeLabel>
        vertex_begin(){
        /// visit the vertexes connected with all edge labels
        return
           typename iterator
         ::template vertex<IteratorType::UnspecifiedEdgeLabel>
                                    (this->edges_.out_edges());
    }

    /// return the iterator of the vertex container with specified edge label
    /// constant iterator
    inline typename iterator
         ::template const_vertex<IteratorType::SpecifiedEdgeLabel>
        vertex_cbegin(const EdgeLabelType& edge_label) const{
        /// visit the vertexes connected with specified edge label
        return
           typename iterator
         ::template const_vertex<IteratorType::SpecifiedEdgeLabel>
                   (this->edges_
                   .const_out_edges()
                   .const_vertex_container(edge_label));
    }
    /// non-constant iterator
    inline typename iterator
         ::template vertex<IteratorType::SpecifiedEdgeLabel>
        vertex_begin(const EdgeLabelType& edge_label){
        /// visit the vertexes connected with specified edge label
        return
           typename iterator
         ::template vertex<IteratorType::SpecifiedEdgeLabel>
           (this->edges_.out_edges().vertex_container(edge_label));
    }

    /// return the iterator of the third level container
    /// with unspecified edge label
    /// constant iterator
    inline typename iterator
         ::template const_edge<IteratorType::UnspecifiedEdgeLabel>
        edge_cbegin() const{
        /// visit the vertexes connected with specified edge label
        return
           typename iterator
         ::template const_edge<IteratorType::UnspecifiedEdgeLabel>
                             (this->edges_.const_out_edges(),this);
    }
    /// non-constant iterator
    inline typename iterator
         ::template edge<IteratorType::UnspecifiedEdgeLabel>
        edge_begin(){
        /// visit the vertexes connected with specified edge label
        return
           typename iterator
         ::template edge<IteratorType::UnspecifiedEdgeLabel>
                             (this->edges_.out_edges(),this);
    }

    /// return the iterator of the third level container
    /// with specified edge label
    /// constant iterator
    inline typename iterator
         ::template const_edge<IteratorType::SpecifiedEdgeLabel>
        edge_cbegin(const EdgeLabelType& edge_label) const{
        /// visit the vertexes connected with specified edge label
        return
           typename iterator
         ::template const_edge<IteratorType::SpecifiedEdgeLabel>
                   (this->edges_.const_out_edges()
                                .const_vertex_container(edge_label),
                    this,edge_label);
    }
    /// non-constant iterator
    inline typename iterator
         ::template edge<IteratorType::SpecifiedEdgeLabel>
        edge_begin(const EdgeLabelType& edge_label){
        /// visit the vertexes connected with specified edge label
        return
           typename iterator
         ::template edge<IteratorType::SpecifiedEdgeLabel>
             (this->edges_.out_edges().vertex_container(edge_label),
              this, edge_label);
    }

    /// input edge iterators
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        != StoreData::OutEdge>::type = false>
    inline typename iterator::const_input_edge_label
        input_edge_label_cbegin() const{
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::const_input_edge_label(
                                            this->edges_.const_in_edges());
    }
    /// constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template const_input_vertex<
                                    IteratorType::UnspecifiedEdgeLabel>
        input_vertex_cbegin() const{
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template const_input_vertex<
                                    IteratorType::UnspecifiedEdgeLabel>
                                        (this->edges_.const_in_edges());
    }
    /// non-constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template input_vertex<
                                    IteratorType::UnspecifiedEdgeLabel>
        input_vertex_begin(){
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template input_vertex<
                                    IteratorType::UnspecifiedEdgeLabel>
                                              (this->edges_.in_edges());
    }
    /// constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template const_input_vertex<
                                    IteratorType::SpecifiedEdgeLabel>
        input_vertex_cbegin(const EdgeLabelType& edge_label) const{
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template const_input_vertex<
                                    IteratorType::SpecifiedEdgeLabel>
                     (this->edges_.const_in_edges()
                                  .const_vertex_container(edge_label));
    }
    /// non-constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template input_vertex<
                                    IteratorType::SpecifiedEdgeLabel>
        input_vertex_begin(const EdgeLabelType& edge_label){
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template input_vertex<
                                    IteratorType::SpecifiedEdgeLabel>
               (this->edges_.in_edges().vertex_container(edge_label));
    }
    /// constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template const_input_edge<
                                    IteratorType::UnspecifiedEdgeLabel>
        input_edge_cbegin() const{
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template const_input_edge<
                                    IteratorType::UnspecifiedEdgeLabel>
                                   (this->edges_.const_in_edges(),this);
    }
    /// non-constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template input_edge<
                                    IteratorType::UnspecifiedEdgeLabel>
        input_edge_begin(){
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template input_edge<
                                    IteratorType::UnspecifiedEdgeLabel>
                                         (this->edges_.in_edges(),this);
    }
    /// constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template const_input_edge<
                                    IteratorType::SpecifiedEdgeLabel>
        input_edge_cbegin(const EdgeLabelType& edge_label) const{
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template const_input_edge<
                                    IteratorType::SpecifiedEdgeLabel>
                (this->edges_.const_in_edges()
                             .const_vertex_container(edge_label),
                 this,edge_label);
    }
    /// non-constant iterator
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline typename iterator::template input_edge<
                                    IteratorType::SpecifiedEdgeLabel>
        input_edge_begin(const EdgeLabelType& edge_label){
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return
           typename iterator::template input_edge<
                                    IteratorType::SpecifiedEdgeLabel>
           (this->edges_.in_edges().vertex_container(edge_label),
            this,edge_label);
    }

    inline bool HasEdgeLabel(const EdgeLabelType& edge_label) const{
        return this->edges_.const_out_edges().HasEdgeLabel(edge_label);
    }

    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type
                                        == StoreData::InOutEdge>::type = false>
    inline bool HasInputEdgeLabel(const EdgeLabelType& edge_label) const{
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return this->edges_.const_in_edges().HasEdgeLabel(edge_label);
    }

private:
    /// operate on the third level container
    template<const bool judge_edge_has_attribute   = edge_has_attribute,
             const bool judge_allow_duplicate_edge = allow_duplicate_edge,
             typename std::enable_if<!judge_edge_has_attribute
                                   &&!judge_allow_duplicate_edge,bool
                                    >::type = false>
    inline typename iterator::template edge<IteratorType::SingleElement>
        AddOutEdge(Vertex* const vertex,
                   const EdgeLabelType& edge_label){
        static_assert((judge_edge_has_attribute   == edge_has_attribute)
                    &&(judge_allow_duplicate_edge == allow_duplicate_edge),
                     "Illegal usage of this method");
        /// add an directed edge, from "this" to "vertex"
        static_assert(!kHasDecomposedEdgeContainer_,
                     "Illegal configuration, should not have \
                      decomposed edge container in this case");
        this->edges_.out_edges().InsertVertexContainer(edge_label).first
                                .InsertCompoundVertex (vertex);
        constexpr DuplicateNumType kDefaultDuplicateNum = 1;
        return typename iterator::template edge<IteratorType::SingleElement>(
                    vertex,              /// dst_vertex_ptr
                    kDefaultDuplicateNum,/// duplicate_cardinality
                    edge_label,          /// edge label
                    this);               /// src_vertex_ptr
    }

    /// operate on the third level container
    template<const bool judge_edge_has_attribute   = edge_has_attribute,
             const bool judge_allow_duplicate_edge = allow_duplicate_edge,
             typename std::enable_if<!judge_edge_has_attribute
                                   && judge_allow_duplicate_edge,bool
                                    >::type = true>
    inline typename iterator::template edge<IteratorType::SingleElement>
        AddOutEdge(Vertex* const        vertex,
                   const EdgeLabelType& edge_label){
        static_assert((judge_edge_has_attribute   == edge_has_attribute)
                    &&(judge_allow_duplicate_edge == allow_duplicate_edge),
                     "Illegal usage of this method");
        /// add an directed edge, from "this" to "vertex"
        static_assert(!kHasDecomposedEdgeContainer_,
                     "Illegal configuration, should not have \
                      decomposed edge container in this case");
        CompoundVertexType&
        ret = this->edges_.out_edges().InsertVertexContainer(edge_label).first
                                      .InsertCompoundVertex (  vertex  ).first;
        ret.AddDecomposedEdge();
		auto tmp = this;	// *** ÷±Ω”–¥this±‡“Î¥ÌŒÛ
        return typename iterator::template edge<IteratorType::SingleElement>(
                    vertex,                      /// dst_vertex_ptr
                    ret.duplicate_cardinality(), /// duplicate_cardinality
                    edge_label,                  /// edge label
					tmp);                   /// src_vertex_ptr
    }

    /// operate on the third level container
    template<const bool judge_edge_has_attribute   = edge_has_attribute,
             const bool judge_allow_duplicate_edge = allow_duplicate_edge,
             typename std::enable_if<judge_edge_has_attribute
                                  &&!judge_allow_duplicate_edge,bool
                                    >::type = true>
    inline typename iterator::template edge<IteratorType::SingleElement>
        AddOutEdge(Vertex* const            vertex,
                   const EdgeLabelType&     edge_label,
                   const EdgeAttributeType& edge_attribute){
        static_assert((judge_edge_has_attribute   == edge_has_attribute)
                    &&(judge_allow_duplicate_edge == allow_duplicate_edge),
                     "Illegal usage of this method");
        /// add an directed edge, from "this" to "vertex"
        static_assert(!kHasDecomposedEdgeContainer_,
                     "Illegal configuration, should not have \
                      decomposed edge container in this case");
        CompoundVertexType* const ret_compound_vertex_ptr
            = this->edges_.out_edges().InsertVertexContainer(edge_label).first
                                      .InsertCompoundVertex (vertex,
                                                             edge_attribute
                                                                       ).first;
        constexpr DuplicateNumType kDefaultDuplicateNum = 1;
        return typename iterator::template edge<IteratorType::SingleElement>(
                    vertex,                 /// dst_vertex_ptr
                    kDefaultDuplicateNum,   /// duplicate_cardinality
                    edge_label,             /// edge label
                    ret_compound_vertex_ptr
                        ->decomposed_edge(),/// edge attribute,
                                            /// implicit convert to base class:
                                            /// WithAttribute_<...>
                    this);                  /// src_vertex_ptr
    }

    /// operate on the third level container
    template<const bool judge_edge_has_attribute
                            = edge_has_attribute,
             const bool judge_allow_duplicate_edge
                            = allow_duplicate_edge,
             const bool judge_edge_attribute_is_dynamic
                            = edge_attribute_is_dynamic,
             typename std::enable_if<judge_edge_has_attribute
                                  && judge_allow_duplicate_edge
                                  &&!judge_edge_attribute_is_dynamic,bool
                                    >::type = true>
    inline typename iterator::template edge<IteratorType::SingleElement>
        AddOutEdge(Vertex* const            vertex,
                   const EdgeLabelType&     edge_label,
                   const EdgeIDType&        edge_id,
                   const EdgeAttributeType& edge_attribute){
        static_assert(judge_edge_has_attribute
                         == edge_has_attribute
                    &&judge_allow_duplicate_edge
                         == allow_duplicate_edge
                    &&judge_edge_attribute_is_dynamic
                         == edge_attribute_is_dynamic,
                     "Illegal usage of this method");
        /// add an directed edge, from "this" to "vertex"
        static_assert(!kHasDecomposedEdgeContainer_,
                     "Illegal configuration, should not have \
                      decomposed edge container in this case");
        CompoundVertexType& ret_compound_vertex
            = this->edges_.out_edges().InsertVertexContainer(edge_label).first
                                      .InsertCompoundVertex (  vertex  ).first;
        DecomposedEdgeType* const ret_decomposed_edge_ptr
            = &(ret_compound_vertex.AddDecomposedEdge(edge_id,
                                                      edge_attribute).first);
        return typename iterator::template edge<IteratorType::SingleElement>(
                    vertex,                 /// dst_vertex_ptr
                    ret_compound_vertex.duplicate_cardinality(),
                    ret_decomposed_edge_ptr
                                     ->id(),/// edge id
                                            /// duplicate_cardinality
                    edge_label,             /// edge label
                    ret_decomposed_edge_ptr,/// edge attribute
                                            /// implicit convert to base class:
                                            /// WithAttribute_<...>
                    this);                  /// src_vertex_ptr
    }

    /// operate on the third level container
    template<const bool judge_edge_has_attribute
                            = edge_has_attribute,
             const bool judge_allow_duplicate_edge
                            = allow_duplicate_edge,
             const bool judge_edge_attribute_is_dynamic
                            = edge_attribute_is_dynamic,
             typename std::enable_if<judge_edge_has_attribute
                                  && judge_allow_duplicate_edge
                                  && judge_edge_attribute_is_dynamic,bool
                                    >::type = true>
    inline typename iterator::template edge<IteratorType::SingleElement>
        AddOutEdge(Vertex* const        vertex,
                   const EdgeLabelType& edge_label,
                   const EdgeIDType&    edge_id){
        static_assert((judge_edge_has_attribute
                          == edge_has_attribute)
                    &&(judge_allow_duplicate_edge
                          == allow_duplicate_edge)
                    &&(judge_edge_attribute_is_dynamic
                          == edge_attribute_is_dynamic),
                     "Illegal usage of this method");
        /// add an directed edge, from "this" to "vertex"
        static_assert(kHasDecomposedEdgeContainer_,
                     "Illegal configuration, should have \
                      decomposed edge container in this case");
        CompoundVertexType& ret_compound_vertex
            = this->edges_.out_edges().InsertVertexContainer(edge_label).first
                                      .InsertCompoundVertex (  vertex  ).first;
        DecomposedEdgeType* const ret_decomposed_edge_ptr
            = &(ret_compound_vertex.AddDecomposedEdge(edge_id).first);
        return typename iterator::template edge<IteratorType::SingleElement>(
                    vertex,                 /// dst_vertex_ptr
                    ret_compound_vertex.duplicate_cardinality(),
                                            /// duplicate_cardinality
                    ret_decomposed_edge_ptr
                                     ->id(),/// edge id
                    edge_label,             /// edge label
                    ret_decomposed_edge_ptr,/// edge attribute
                                            /// implicit convert to base class:
                                            /// WithAttribute_<...>
                    this);                  /// src_vertex_ptr
    }

    template<const enum StoreData judge_store_type
                                      = store_type,
             const bool judge_allow_duplicate_edge
                            = allow_duplicate_edge,
             typename std::enable_if<judge_store_type == StoreData::InOutEdge
                                  &&!judge_allow_duplicate_edge,bool
                                    >::type = false>
    inline bool AddInEdge(Vertex* const        vertex,
                          const EdgeLabelType& edge_label){
        static_assert(judge_store_type == store_type
                   && judge_allow_duplicate_edge
                         == allow_duplicate_edge,
                     "Illegal usage of this method");
        return
            this->edges_.in_edges().InsertVertexContainer(edge_label).first
                                   .InsertCompoundVertex (  vertex  ).second;
    }

    template<const enum StoreData judge_store_type
                                      = store_type,
             const bool judge_allow_duplicate_edge
                            = allow_duplicate_edge,
             typename std::enable_if<judge_store_type == StoreData::InOutEdge
                                  && judge_allow_duplicate_edge,bool
                                    >::type = false>
    inline bool AddInEdge(Vertex* const        vertex,
                          const EdgeLabelType& edge_label){
        static_assert(judge_store_type == store_type
                   && judge_allow_duplicate_edge
                         == allow_duplicate_edge,
                     "Illegal usage of this method");
        auto temp =
            this->edges_.in_edges().InsertVertexContainer(edge_label).first
                                   .InsertCompoundVertex (  vertex  );
        temp.first.AddDecomposedEdge();
        return temp.second;
    }

    /// only store output edge, do not need to hold input edge
    template<const enum StoreData judge_store_type = store_type,
             typename std::enable_if<judge_store_type == StoreData::OutEdge,
                                     bool>::type = false>
    inline constexpr bool AddInEdge(Vertex* const        vertex,
                                    const EdgeLabelType& edge_label){
        static_assert(judge_store_type == store_type,
                     "Illegal usage of this method");
        return false;
    }

public:
    template<typename... Args>
    inline typename iterator::template edge<IteratorType::SingleElement>
        AddEdge(Vertex* const        vertex,
                const EdgeLabelType& edge_label,
                Args&...             args){
        vertex->AddInEdge(this,edge_label);
        return this->AddOutEdge(vertex,edge_label,args...);
    }

    inline std::string to_string() const{
        return " vertex id: "        + std::to_string(this->id()) + "\n"
             + " vertex label: "     + this->label().to_string()  + "\n";
    }
};
#endif // _VERTEX_HPP
