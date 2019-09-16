#ifdef _WEIGHTED_EDGE_HPP
#define _WEIGHTED_EDGE_HPP

#include "define.hpp"
#include "configures.hpp"

template<typename... configures>
class WeightedEdgeConfigures;

/// declaration of configurable parameters
template<typename tWeightType>
class SetWeightType;

/// default setting of configurations
template<>
class WeightedEdgeConfigures<>:public Configures<>{
private:
    using DefaultWeightType = uint8_t;

protected:
    static constexpr bool SpecifiedWeightType = false;

public:
    using WeightType = DefaultWeightType;
};

/// resolution of the configurations
/// SetWeightType
template<typename    tWeightType,
         typename... other_configures>
class WeightedEdgeConfigures<
            SetWeightType<tWeightType>,
            other_configures...>
      :public WeightedEdgeConfigures<other_configures...>{
private:
    static_assert(!WeightedEdgeConfigures<other_configures...>::SpecifiedWeightType,
                    "Redefinition of Edge Weight Type");
    static_assert(std::is_object<tWeightType>::value,
                            "Illegal Edge Weight Type");

protected:
    static constexpr bool SpecifiedWeightType = true;

public:
    using WeightType = tWeightType;
};
/// Other configurations are not specified
/// if they are not specified in EdgeConfigures
template<typename configure,
         typename... other_configures>
class WeightedEdgeConfigures<configure,other_configures...>
        :public WeightedEdgeConfigures<other_configures...>{
public:
    static constexpr bool HasUnSpecifiedConfigure
        = EdgeConfigures<configure>::HasUnSpecifiedConfigure;
};

template<typename... configures>
class WeightedEdge:
        public ConfigureFilter<
                    EdgeConfigures,
                    true,/// find the parameter that are
                         /// contained in NodeConfigures
                    Edge,
                    configures...,
                    EndOfConfigure<>>::type{
private:
    using EdgeType =
        typename ConfigureFilter<
                    EdgeConfigures,
                    true,/// find the parameter that are
                         /// contained in NodeConfigures
                    Edge,
                    configures...,
                    EndOfConfigure<>>::type;
    using Configures =
        typename ConfigureFilter<
                    EdgeConfigures,
                    false,/// find the parameters that are
                          /// not contained in NodeConfigure
                    WeightedEdgeConfigures,
                    configures...,
                    EndOfConfigure<>>::type;
    static_assert(!Configures::HasUnSpecifiedConfigure,
                  "Undefined configuration");

public:
    /// configuration of nodes
    using LabelType  = typename EdgeType::LabelType;
    /// configuration of WeightedEdge
    using WeightType = typename Configures::WeightType;

private:
    const WeightType kWeight_;

public:
    WeightedEdge(const WeightType& weight,
                 const  LabelType& label)
                     :EdgeType(label),
                      kWeight_(weight){
        return;
    }

    template<class... Args>
    WeightedEdge(const  LabelType&    label,
                 const       Args&... args)
                     :EdgeType(args),
                      kWeight_(weight){
        return;
    }

    inline const WeightType& GetWeight() const{
        return this->kWeight_;
    }
    inline bool SameEdgeType(const WeightedEdge& e) const{
        return this->kWeight_ == e.kWeight_
            && EdgeType::SameEdgeType(e);
    }
};

#endif // _WEIGHTED_EDGE_HPP
