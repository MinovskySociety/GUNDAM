#ifndef _EDGE_HPP
#define _EDGE_HPP

#include "define.hpp"
#include "configures.hpp"

template<typename... configures>
class EdgeConfigures;

/// declaration of configurable parameters
template<typename tLabelType>
class SetLabelType;

/// default setting of configurations
template<>
class EdgeConfigures<>:public Configures<>{
private:
    using DefaultLabelType = Label<uint8_t>;

protected:
    static constexpr bool SpecifiedLabelType = false;

public:
    using LabelType = DefaultLabelType;
};

/// resolution of the configurations
/// SetLabelType
template<template <typename...> class tLabelType,
         typename tLabelUnderlieConfigures,
         typename...      other_configures>
class EdgeConfigures<
            SetLabelType<tLabelType<tLabelUnderlieConfigures>>,
            other_configures...>
      :public EdgeConfigures<other_configures...>{
private:
    static_assert(!EdgeConfigures<other_configures...>::SpecifiedLabelType,
                    "Redefinition of Edge Label Type");
    static_assert(std::is_base_of<Label    <tLabelUnderlieConfigures>,
                                 tLabelType<tLabelUnderlieConfigures>>::value,
                            "Illegal Edge Label Type");

protected:
    static constexpr bool SpecifiedLabelType = true;

public:
    using LabelType = tLabelType<tLabelUnderlieConfigures>;
};
/// Other configurations are not specified
template<typename    UnSpecifiedConfigure,
         typename... UnSpecifiedConfigures>
class   EdgeConfigures<UnSpecifiedConfigure,
                       UnSpecifiedConfigures...>
:public EdgeConfigures<UnSpecifiedConfigures...>{
public:
    static constexpr bool HasUnSpecifiedConfigure = true;
};

template<typename... configures>
class Edge{
private:
    /// since edge only represents a relation between two
    /// nodes, should not be instantiated
    ///
    /// ToDo by Wenzhi:
    ///     Use some tricks to ensure this class cannot
    ///     be instantiated
    using Configures = EdgeConfigures<configures...>;
    static_assert(!Configures::HasUnSpecifiedConfigure,
                  "Undefined configuration");

public:
    using LabelType = typename Configures::LabelType;

private:
    const LabelType kLabel_;

public:
    Edge(const LabelType& label)
                 :kLabel_(label){
        return;
    }

    template<class... Args>
    Edge(const Args&... args)
               :kLabel_(args...){
        return;
    }

    inline const LabelType& GetLabel() const{
        return this->kLabel_;
    }
    inline bool CanMatchTo(const Edge& e) const{
        return this->kLabel_.CanMatchTo(e.kLabel_);
    }
    inline bool SameEdgeType(const Edge& e) const{
        return this->kLabel_==e.kLabel_;
    }
};

#endif // _EDGE_HPP
