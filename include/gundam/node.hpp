#ifndef _NODE_HPP
#define _NODE_HPP

#include "label.hpp"
#include "configures.hpp"

template<typename... configures>
class NodeConfigures;

/// declaration of configurable parameters
template<typename tLabelType>
class SetLabelType;
template<typename tIDType>
class SetIDType;

/// default setting of configurations
template<>
class NodeConfigures<>:public Configures<>{
private:
    using DefaultIDType = uint32_t;
    using DefaultLabelType = Label<uint8_t>;

protected:
    static constexpr bool SpecifiedIDType    = false;
    static constexpr bool SpecifiedLabelType = false;

public:
    using IDType    = DefaultIDType;
    using LabelType = DefaultLabelType;
};

/// resolution of the configurations
/// SetLabelType
template<template <typename...> class tLabelType,
         typename tLabelUnderlieConfigures,
         typename...      other_configures>
class NodeConfigures<
            SetLabelType<tLabelType<tLabelUnderlieConfigures>>,
            other_configures...>
      :public NodeConfigures<other_configures...>{
private:
    static_assert(!NodeConfigures<other_configures...>::SpecifiedLabelType,
                    "Redefinition of Node Label Type");
    static_assert(std::is_base_of<Label    <tLabelUnderlieConfigures>,
                                 tLabelType<tLabelUnderlieConfigures>>::value,
                            "Illegal Node Label Type");

protected:
    static constexpr bool SpecifiedLabelType = true;

public:
    using LabelType = tLabelType<tLabelUnderlieConfigures>;
};
/// SetIDType
template<typename    tIDType,
         typename... other_configures>
class NodeConfigures<
        SetIDType<tIDType>,
        other_configures...>
      :public NodeConfigures<other_configures...>{
private:
    static_assert(!NodeConfigures<other_configures...>::SpecifiedIDType,
                    "Redefinition of Node ID Type");
    static_assert(std::is_object<tIDType>::value,
                            "Illegal Node ID Type");

protected:
    static constexpr bool SpecifiedIDType = true;

public:
    using IDType = tIDType;
};
/// Other configurations are not specified
template<typename    UnSpecifiedConfigure,
         typename... UnSpecifiedConfigures>
class   NodeConfigures<UnSpecifiedConfigure,
                       UnSpecifiedConfigures...>
:public NodeConfigures<UnSpecifiedConfigures...>{
public:
    static constexpr bool HasUnSpecifiedConfigure = true;
};

template<typename... configures>
class Node{
private:
    using Configures = NodeConfigures<configures...>;
    static_assert(!Configures::HasUnSpecifiedConfigure,
                  "Undefined configuration");

public:
    using IDType    = typename Configures::IDType;
    using LabelType = typename Configures::LabelType;

private:
    const    IDType kId_;
    const LabelType kLabel_;

    Node(const Node& ) = delete;
    Node(const Node&&) = delete;
    Node& operator=(const Node& ) = delete;
    Node& operator=(const Node&&) = delete;

public:
    Node(const    IDType& id,
         const LabelType& label)
                        :kId_(id),
                      kLabel_(label){
        return;
    }

    template<class... Args>
    Node(const IDType&    id,
         const   Args&... args)
                        :kId_(id),
                      kLabel_(args...){
        return;
    }

    inline const LabelType& GetLabel() const{
        return this->kLabel_;
    }
    inline const IDType& GetID() const{
        return this->kId_;
    }
    inline bool CanMatchTo(const Node& node) const{
        return this->kLabel_.CanMatchTo(node.kLabel_);
    }
    inline bool SameNode(const Node& node) const{
        return this->kId_ == node.id;
    }
};

#endif // _NODE_HPP
