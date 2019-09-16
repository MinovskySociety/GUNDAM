#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include <assert.h>

#include <vector>

#include "define.hpp"
#include "configures.hpp"

template<typename KeyType>
class Attribute{
private:
    static_assert(std::is_object<KeyType>::value,
                  "Illegal Attribute Key Type");

    const KeyType kKey_;

public:
    Attribute(const KeyType& key):kKey_(key){
        return;
    }

    virtual ~Attribute(){
        return;
    }

    inline const KeyType& key() const{
        return this->kKey_;
    }
};

template<typename   KeyType,
         typename   ValueType,
         const bool attribute_is_const>
class ConcreteAttribute:public Attribute<KeyType>{
private:
    static_assert(std::is_object<ValueType>::value,
                  "Illegal Attribute Value Type");

    typename std::conditional<attribute_is_const,
                                 const ValueType,
                                       ValueType>::type attribute_;

public:
    ConcreteAttribute(const   KeyType& key,
                      const ValueType& attribute)
                   :Attribute<KeyType>(key),
                            attribute_(attribute){
        return;
    }

    virtual ~ConcreteAttribute() override{
        return;
    }

    inline const ValueType& const_attribute() const{
        return this->attribute_;
    }

    inline ValueType& attribute(){
        return this->attribute_;
    }

    inline void set_attribute(const ValueType& attribute){
        this->attribute_ = attribute;
        return;
    }
};

/// definition of attribute containers
template<const enum ContainerType container_type,
         typename   KeyType,
         const bool attribute_is_const>
class AttributeContainer{
    /// trick the compiler, equivalent to static_assert(false)
    static_assert( attribute_is_const,"Unsupported container type");
    static_assert(!attribute_is_const,"Unsupported container type");
};

template<typename   KeyType,
         const bool attribute_is_const>
class AttributeContainer<ContainerType::Vector,
                         KeyType,attribute_is_const>{
private:
    typename std::conditional<attribute_is_const,
                  std::vector<const Attribute<KeyType>*>,
                  std::vector<      Attribute<KeyType>*>
                             >::type attributes_;

    template<typename ConcreteDataType>
    using ConcreteAttributeType =
        typename std::conditional<attribute_is_const,
          const ConcreteAttribute<KeyType,
                                  ConcreteDataType,
                                  attribute_is_const>,
                ConcreteAttribute<KeyType,
                                  ConcreteDataType,
                                  attribute_is_const>>::type;

public:
    AttributeContainer(){
        return;
    }

    ~AttributeContainer(){
        for (const auto& it : this->attributes_)
            delete(it);
        return;
    }

    inline bool HasAttribute(const KeyType& key_type) const{
        for (const auto& it : this->attributes_)
            if (it->key() == key_type)
                return true;
        return false;
    }

    template<typename ConcreteDataType>
    inline const ConcreteDataType& const_attribute
                   (const KeyType& key_type) const{
        assert(this->HasAttribute(key_type));
        for (const auto& it : this->attributes_)
            if (it->key() == key_type){
                return (static_cast<ConcreteAttributeType<
                                    ConcreteDataType>*>(it)
                       )->const_attribute();
            }
        assert(false);
        return (static_cast<ConcreteAttributeType<
                            ConcreteDataType>*>(*this->attributes_
                                                       .cbegin())
               )->const_attribute();
    }

    template<typename   ConcreteDataType,
             const bool judge = attribute_is_const,
             typename   std::enable_if<!judge,bool>::type = false>
    inline ConcreteDataType& attribute
            (const KeyType& key_type){
        static_assert(judge == attribute_is_const,
                     "Illegal usage of this method");
        assert(this->HasAttribute(key_type));
        for (auto& it : this->attributes_)
            if (it->key() == key_type){
                return (static_cast<ConcreteAttributeType<
                                    ConcreteDataType>* >(it)
                       )->attribute();
            }
        assert(false);
        return (static_cast<ConcreteAttributeType<
                            ConcreteDataType>* >(*this->attributes_
                                                       .begin())
               )->attribute();
    }

    template<typename ConcreteDataType>
    inline bool add_attribute(const KeyType& key_type,
                              const ConcreteDataType& concrete_data){
        if (this->HasAttribute(key_type))
            return false;
        auto temp = new ConcreteAttributeType<
                        ConcreteDataType>(key_type,concrete_data);
        this->attributes_.push_back(temp);
        return true;
    }

    template<typename   ConcreteDataType,
             const bool judge = attribute_is_const,
             typename   std::enable_if<!judge,bool>::type = false>
    inline bool remove_attribute(const KeyType& key_type){
        static_assert(judge == attribute_is_const,
                     "Illegal usage of this method");
        for (const auto it  = this->attributes_.cbegin();
                        it != this->attributes_.cend();it++){
            if ((*it)->key() == key_type)
                return true;
        }
        return false;
    }

    template<typename   ConcreteDataType,
             const bool judge = attribute_is_const,
             typename   std::enable_if<!judge,bool>::type = false>
    inline bool set_attribute(const KeyType& key_type,
                              const ConcreteDataType& concrete_data){
        static_assert(judge == attribute_is_const,
                     "Illegal usage of this method");
        for (auto& it : this->attributes_)
            if (it->key() == key_type){
                it->set_attribute(concrete_data);
                return true;
            }
        return false;
    }
};
#endif // _ATTRIBUTE_HPP
