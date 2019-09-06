#ifndef _ATTRIBUTE_NODE_HPP
#define _ATTRIBUTE_NODE_HPP

#include <type_traits>
#include <map>
#include <memory>
#include <list>

#include "node.hpp"
#include "attribute.hpp"

template<typename... configures>
class AttributeNodeConfigures;

/// declaration of configurable parameters
template<bool IsConst>
class SetAttributeIsConst;
template<typename KeyType>
class SetAttributeKeyType;

/// default setting of configurations
template<>
class AttributeNodeConfigures<>:public Configures<>{
private:
    static constexpr
    bool  DefaultIsConst = false;
    using DefaultKeyType = uint8_t;

protected:
    static constexpr bool SpecifiedIsConst = false;
    static constexpr bool SpecifiedKeyType = false;

public:
    static constexpr
    bool  IsConst = DefaultIsConst;
    using KeyType = DefaultKeyType;
};

/// resolution of the configurations
/// SetAttributeIsConst
template<bool tIsConst,
         typename... other_configures>
class AttributeNodeConfigures<
         SetAttributeIsConst<tIsConst>,other_configures...
      >:public AttributeNodeConfigures<other_configures...>{
private:
    static_assert(!AttributeNodeConfigures<other_configures...>
                    ::SpecifiedIsConst,
                    "Redefinition of IsConst");

protected:
    static constexpr bool SpecifiedIsConst = true;

public:
    static constexpr bool IsConst = tIsConst;
};
/// SetAttributeKeyType
template<typename tKeyType,
         typename... other_configures>
class AttributeNodeConfigures<
         SetAttributeKeyType<tKeyType>,other_configures...
      >:public AttributeNodeConfigures<other_configures...>{
private:
    static_assert(!AttributeNodeConfigures<other_configures...>
                    ::SpecifiedKeyType,
                    "Redefinition of Attribute Key Type");
    static_assert(std::is_object<tKeyType>::value,
                            "Illegal Attribute Key Type");

protected:
    static constexpr bool SpecifiedKeyType = true;

public:
    using KeyType = tKeyType;
};
/// Other configurations are not specified
/// if they are not specified in NodeConfigures
template<typename configure,
         typename... other_configures>
class AttributeNodeConfigures<configure,other_configures...>
        :public AttributeNodeConfigures<other_configures...>{
public:
    static constexpr bool HasUnSpecifiedConfigure
        = NodeConfigures<configure>::HasUnSpecifiedConfigure;
};

template<typename... configures>
class AttributeNode:
        public ConfigureFilter<
                    NodeConfigures,
                    true,/// find the parameter that are
                         /// contained in NodeConfigures
                    Node,
                    configures...,
                    EndOfConfigure<>>::type{
private:
    using NodeType =
        typename ConfigureFilter<
                    NodeConfigures,
                    true,/// find the parameter that are
                         /// contained in NodeConfigures
                    Node,
                    configures...,
                    EndOfConfigure<>>::type;
    using Configures =
        typename ConfigureFilter<
                    NodeConfigures,
                    false,/// find the parameters that are
                          /// not contained in NodeConfigure
                    AttributeNodeConfigures,
                    configures...,
                    EndOfConfigure<>>::type;
    static_assert(!Configures::HasUnSpecifiedConfigure,
                  "Undefined configuration");

public:
    /// configuration of nodes
    using IDType    = typename NodeType::IDType;
    using LabelType = typename NodeType::LabelType;
    /// configuration of AttributeNode
    static constexpr bool
          AttributeIsConst =          Configures::IsConst;
    using AttributeKeyType = typename Configures::KeyType;
    using    AttributeType = Attribute<AttributeKeyType>;
    template<typename AttributeValueType>
    using ConcreteAttributeType ///alias template, for polymorphic
        = typename std::conditional<AttributeIsConst,
            const ConcreteAttribute<AttributeKeyType,
                                    AttributeValueType,
                                    AttributeIsConst>,
                  ConcreteAttribute<AttributeKeyType,
                                    AttributeValueType,
                                    AttributeIsConst>>::type;

private:
    #ifndef NDEBUG
    /// to check whether the
    using HashCodeType = size_t;
    /// type of the return value of hash_code()
    static std::map<AttributeKeyType,HashCodeType> type_dictionary_;
    #endif // NDEBUG

    typename std::conditional<AttributeIsConst,
        typename std::list<const AttributeType*>,
        typename std::list<      AttributeType*>>::type attributes_;

public:
    using NodeType::Node;

    virtual ~AttributeNode(){
        for(const auto& it : this->attributes_)
            delete(it);
        return;
    }

    inline bool HasAttr(const AttributeKeyType& key) const{
        for (const auto& it : this->attributes_)
            if (it->SameKey(key))
                return true;
        return false;
    }

    template <typename AttributeValueType>
    inline void AddAttribute(const AttributeKeyType&   key,
                             const AttributeValueType& value){
        assert(!this->HasAttr(key));
        this->attributes_.emplace_back(
            new ConcreteAttributeType<AttributeValueType>>(key,value)
        );
        return;
    }

    template <typename ValueType>
    inline ConcreteAttributeType<ValueType>* const
        GetAttribute(const AttributeKeyType& key){
        assert(this->HasAttr(key));
        for (const auto& it : this->attributes_)
            if (it->SameKey(key))
                return std::static_pointer_cast<
                        ConcreteAttributeType<ValueType>>(it);
        assert(false);
        return nullptr;
    }

    template <typename ValueType>
    inline const ValueType& GetValue(const AttributeKeyType& key) const{
        assert(this->HasAttr(key));
        for (const auto& it : this->attributes_)
            if (it->SameKey(key)){
                return std::static_pointer_cast<
                            ConcreteAttributeType<ValueType>>
                       (it)->GetValue();
            }
        assert(false);
        return std::static_pointer_cast<
                   ConcreteAttributeType<ValueType>>
               (*(this->attributes_.begin()))->GetValue();
    }
};

#endif // _ATTRIBUTE_NODE_HPP
