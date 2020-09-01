#ifdef _ATTRIBUTE2_H
#define _ATTRIBUTE2_H

#include <sstream>
#include <string>

#include "gundam/container.h"
#include "gundam/datatype.h"
#include "gundam/iterator.h"

enum class AttributeType : bool {
  kSeparated = 0, /// each vertex/edge has its own vertex container 
  kGrouped   = 1  /// a group of vertexes/edges shares a same attribute container
};

template<enum AttributeType attr_type,
         bool is_const_,
         typename KeyType_, 
         enum ContainerType container_type_,
         enum SortType sort_type_>
class Attribute_;

/// seperated attribute container
template<bool is_const_,
         typename KeyType_, 
         enum ContainerType container_type_,
         enum SortType sort_type_>
class Attribute_<kSeparated,
                 is_const_,
                 KeyType_,
                 container_type_,
                 sort_type_>{
 private:


 public:
  Attribute(const GroupIDType& group_id){
    /// since the each vertex/edge holds a seperated container, 
    /// the group_id would not be used
    return;
  }
}

/// grouped attribute container
/// the attribute containers in a same graph share the same key & value type
template<bool is_const_,
         typename KeyType_, 
         enum ContainerType container_type_,
         enum SortType sort_type_>
class Attribute_<kGrouped,
                 is_const_,
                 KeyType_,
                 container_type_,
                 sort_type_>{
 private:
  using GroupContainerType = std::map<GroupIDType, Cont>;
  static GroupContainerType group_container_;

  /// automatically alloc id for the containers in each group
  using ContainerIDCounterContainer = std::vector<ContainerIDType>;
  static ContainerIDCounterContainer container_id_counter_container_;

  Container<GroupKeyType,
            Container<AbstractAttributeList*>
            std::vector<>
            AttributeList<>>;

  Container<GroupKeyType,
            Container<AbstractAttributeList*>
            AttributeList<>>;

  std::map<GroupKeyType,
           std::vector<>>

  std::vector<GroupKeyType>

  class AttributeContainerGroup_{
   private:
    template<typename  AttributeKeyType_,
            typename   ContainerIDType_>
    class AbstractAttributeList_;

    template<typename  AttributeKeyType_,
            typename   ContainerIDType_,
            typename AttributeDataType_>
    class ConcreteAttributeList_;

    template<typename AttributeKeyType_,
            typename  ContainerIDType_>
    class AbstractAttributeList_{
     private:
      template<typename AttributeDataType_>
      using ConcreteAttributeListType 
          = ConcreteAttributeList_<
                    AttributeKeyType_,
                     ContainerIDType_,
                   AttributeDataType_>;
     public:
      AbstractAttributeList(const AttributeKeyType_& key,
                            const    BasicDataType&  value_type)
                                              :key_(key),
                                        value_type_(value_type){
        return;
      }

      virtual ~AbstractAttributeList(){
        return;
      }

     private:
      AttributeKeyType   key_;
      enum BasicDataType value_type_;
    };

    template<typename   AttributeKeyType,
             typename    ContainerIDType,
             typename AttributeValueType>
    class ConcreteAttributeList : public AbstractAttributeList<
                                                 AttributeKeyType,
                                                  ContainerIDType>{
     private:
      bool check() const{
        ContainerIDTyep temp_value_counter = 0;
        for (const auto& value : this->value_list_){
          if (value.second){
            /// has value here
            temp_value_counter++;
          }
        }
        return temp_value_counter == this->value_counter_;
      }

      inline void extend_to(const ContainerIDType& container_id){
        if (this->value_list_.size() > container_id){
          /// the value_list is big enough
          return;
        }
        this->value_list_.resize(container_id + 1,
            /// the new elements were set to not have value
        std::pair<AttributeValueType, bool>(AttributeValueType(), false));
        return;
      }

      inline bool& has_value(const ContainerIDType& container_id) const{
        /// the value_list_ should be big enough
        assert(this->value_list_.size() > container_id);
        return this->value_list_.at(container_id).second;
      }

      inline AttributeValueType& value(const ContainerIDType& container_id) {
        /// the value_list_ should be big enough
        assert(this->value_list_.size() > container_id);
        return this->value_list_.at(container_id).fist;
      }

     public:
      ConcreteAttributeList():value_counter_(0){
        return;
      }
      
      ~ConcreteAttributeList(){
        return;
      }

      inline std::pair<AttributeDataType, bool>& 
        attribute(const ContainerIDType& container_id){
        if (this->value_list_.size() <= container_id){
          /// does not has this value
          reutrn std::make_pair(AttributeDataType(), false);
        }
        return this->value_list_.at(container_id);
      }

      inline bool AddAttribute(const ContainerIDType& container_id,
                            const AttributeDataType& attribute_value){
        /// ensure the size of vector is bigger than container_id
        this->extend_to(container_id);
        assert(this->value_list_.size() > container_id);
        if (this->has_value(container_id)){
          /// already has value at position, fail to add attribute
          return false;
        }
        /// does not have value for this container_id yet
        this->value(container_id) = attribute_value;
        this->value_counter_++;
        return true; /// added the value successfully
      }

      bool EraseAttribute(const ContainerIDType& container_id) override {
        if (this->value_list_.size() <= container_id){
          /// the value list is not big enough at the beginning,
          /// e.g. this value does not exist
          return false;
        }
        if (!this->has_value(container_id)){
          /// this value list does not have value for this container
          /// does not successfully erase it
          return false;
        }
        this->has_value(container_id) = false;
        this->value_counter_--;
        /// successfully erased
        return true;
      }
      
     private:
      /// this container is denoted as vector here,
      /// since the random access operation is required
      std::vector<std::pair<AttributeDataType, bool>> value_list_;

      /// how many value is actually stored in the value_list
      ContainerIDType value_counter_;
    };

   public:
    using AttributeIteratorType = ;
    using AttributeConstIteratorType = ;

    AttributeContainerGroup_(){
      return;
    }

    ~AttributeContainerGroup_(){
      for (auto it  = this->attribute_list_ptr_container_.begin();
                it != this->attribute_list_ptr_container_.end();it++){
        delete it.template get<kAttributeListPtrIdx>();
      }
      return;
    }

    template<typename ValueType>
    ValueType& attribute(const ContainerIDType& container_id){

    }

    template<typename ValueType>
    inline bool AddAttribute(const ContainerIDType& container_id,
                                   const ValueType& attribute_value){
    }

    BasicDataType attribute_value_type(
          const ContainerIDType& container_id,
          const KeyType_& key) const {
    }

    const char* attribute_value_type_name(
          const ContainerIDType& container_id,
          const KeyType_& key) const {
      assert(key_to_value_type_map.find(key) != key_to_value_type_map.end());
      return EnumToString(key_to_value_type_map.find(key)->second);
    }

    inline AttributeIteratorType AttributeBegin(
           const ContainerIDType& container_id){
      return AttributeIteratorType(container_id,
          this->attribute_list_ptr_container_.begin(),
          this->attribute_list_ptr_container_.end());
    }

    inline AttributeConstIteratorType AttributeCBegin(
                const ContainerIDType& container_id) const{
      return AttributeConstIteratorType(container_id,
               this->attribute_list_ptr_container_.cbegin(),
               this->attribute_list_ptr_container_.cend());
    }

    inline AttributePtr FindAttributePtr(
                      const ContainerIDType&  container_id,
                              const KeyType_& key) {
    }

    AttributeConstPtr FindConstAttributePtr(
                      const ContainerIDType&  container_id,
                              const KeyType_& key) const {
    }

    template <typename ConcreteDataType>
    ConcreteDataType& attribute(
                      const ContainerIDType& container_id,
                      const KeyType_& key) {
    }

    template <typename ConcreteDataType>
    const ConcreteDataType& const_attribute(
                      const ContainerIDType& container_id,
                      const KeyType_& key) const {
    }

    template <typename ConcreteDataType>
    std::pair<AttributePtr, bool> AddAttribute(
                      const ContainerIDType& container_id,
                      const KeyType_& key,
                      const ConcreteDataType& value) {
    }

    std::pair<AttributePtr, bool> AddAttribute(
                      const ContainerIDType& container_id,
                      const KeyType_& key, 
                      const enum BasicDataType& data_type,
                      const std::string& value_str) {
    }

    template <typename ConcreteDataType>
    std::pair<AttributePtr, bool> SetAttribute(
                      const ContainerIDType& container_id,
                      const KeyType_& key,
                      const ConcreteDataType& value) {
    }

    AttributeIterator EraseAttribute(
                      const ContainerIDType& container_id,
                      const AttributeIterator& attribute_iterator) {
      assert(container_id == attribute_iterator.container_id_);
    }

    inline size_t EraseAttribute(
                      const ContainerIDType& container_id,
                      const KeyType_& key) {
      // return this->attributes_.Erase(key) ? 1 : 0;
    }

   private:
    static constexpr TupleIdxType kAttributeListPtrIdx = 1;
    using AttributeListPtrContainerType 
                        = Container<AttributeKeyType,
                            AbstractAttributeList*>;
    
    AttributeListPtrContainerType attribute_list_ptr_container_;
  }

  static Container<GroupKeyType,
                   GroupPtrType> group_ptr_container_;

 public:
  using AttributeIteratorType = ;
  using AttributeConstIteratorType = ;

  Attribute_(const GroupKeyType& group_key) 
      : container_id_(Attribute_::container_id_counter_container_(group_id)++){
    /// automatically alloc a container id in this group
    return;
  }

  inline BasicDataType attribute_value_type(const KeyType_& key) const {
    return Attribute_::group_ptr_container_.attribute_value_type(
                           this->container_id_, key);
  }

  inline const char* attribute_value_type_name(const KeyType_& key) const {
    return Attribute_::group_ptr_container_.attribute_value_type_name(
                           this->container_id_, key);
  }

  inline AttributeIterator AttributeBegin() {
    return Attribute_::group_ptr_container_.AttributeBegin(
                           this->container_id_);
  }

  inline AttributeConstIterator AttributeCBegin() const {
    return Attribute_::group_ptr_container_.AttributeCBegin(
                           this->container_id_);
  }

  inline AttributePtr FindAttributePtr(const KeyType_& key) {
    return Attribute_::group_ptr_container_.FindAttributePtr(
                           this->container_id_, key);
  }

  inline AttributeConstPtr FindConstAttributePtr(const KeyType_& key) const {
    return Attribute_::group_ptr_container_.FindConstAttributePtr(
                           this->container_id_, key);
  }

  template <typename ConcreteDataType>
  inline ConcreteDataType& attribute(const KeyType_& key) {
    return Attribute_::group_ptr_container_.attribute<ConcreteDataType>(
                           this->container_id_, key);
  }

  template <typename ConcreteDataType>
  inline const ConcreteDataType& const_attribute(const KeyType_& key) const {
    return Attribute_::group_ptr_container_.attribute<ConcreteDataType>(
                           this->container_id_, key);
  }

  template <typename ConcreteDataType>
  inline std::pair<AttributePtr, bool> 
                AddAttribute(const KeyType_& key,
                             const ConcreteDataType& value) {
    return Attribute_::group_ptr_container_.AddAttribute(
                           this->container_id_, key, value);
  }

  inline std::pair<AttributePtr, bool> AddAttribute(
      const KeyType_& key, 
      const enum BasicDataType& data_type,
      const std::string& value_str) {
    return Attribute_::group_ptr_container_.AddAttribute(
                           this->container_id_, data_type, value_str);
  }

  template <typename ConcreteDataType>
  inline std::pair<AttributePtr, bool> SetAttribute(
                        const KeyType_& key,
                const ConcreteDataType& value) {
    return Attribute_::group_ptr_container_.SetAttribute(
                           this->container_id_, key, value);
  }

  inline AttributeIterator EraseAttribute(
   const AttributeIterator& attribute_iterator) {
    /// although the container_id is contained in the attribute_iterator
    /// pass this->container_id_ into group_ptr_container_ to check whether
    /// it is consist with teh container_id in attribute_iterator
    return Attribute_::group_ptr_container_.EraseAttribute(
                           this->container_id_,  attribute_iterator);
  }

  inline size_t EraseAttribute(const KeyType_& key) {
    return Attribute_::group_ptr_container_.EraseAttribute(
                           this->container_id_, key);
  }

 private:
  using AttributeContainerType = ;
  static AttributeContainerType attr_container_;
  
  /// the id of this container in group
  ContainerIDType container_id_;
};


#endif ///_ATTRIBUTE2_H