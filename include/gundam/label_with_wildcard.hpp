#ifndef _LABEL_WITH_WILDCARD_HPP
#define _LABEL_WITH_WILDCARD_HPP

#include <type_traits>
#include <assert.h>

#include "define.hpp"
#include "label.hpp"

template<typename... LabelUnderlieType>
class LabelWithWildcard;

template<typename    Arg,
         typename... Args>
class LabelWithWildcard<Arg,Args...>{
private:
    /// only allows zero or two parameters
    /// trick compiler, since the template parameter
    /// must be used
    static_assert(!std::is_object<Arg>::value,
                  "Illegal parameter number");
    static_assert( std::is_object<Arg>::value,
                  "Illegal parameter number");
};

template<>
class LabelWithWildcard<>:public Label<>{
private:
    const bool kIsWildcard_;

public:
    LabelWithWildcard(bool is_wildcard)
             :Label<>(),
              kIsWildcard_(is_wildcard){
        return;
    }

    inline bool CanMatchTo(const LabelWithWildcard& label) const{
        if (this->kIsWildCard_
        || !label.kIsWildcard_)
            return true;/// can be easily verified
        return false;
    }
};

template<typename LabelUnderlieType,
         LabelUnderlieType tWildcardValue>
class LabelWithWildcard<LabelUnderlieType,tWildcardValue>
          :public Label<LabelUnderlieType>{
private:
    static_assert(std::is_object<LabelUnderlieType>::value,
                  "Illegal underlie label type");
    static constexpr kWildcardValue_ = tWildcardValue;

public:
    LabelWithWildcard(const LabelUnderlieType& value)
                     :Label<LabelUnderlieType>(value){
        return;
    }

    inline bool operator ==(const Label<LabelUnderlieType>& label) const{
        return this->kLable_ == label.kLable_;
    }
    inline bool CanMatchTo(const Label<LabelUnderlieType>& label) const{
        return this->kLable_ == label.kLable_;
    }

    inline bool CanMatchTo(const LabelWithWildcard& label) const{
        if (this->kIsWildCard_
        || !label.kIsWildcard_)
            return true;/// can be easily verified
        return false;
    }
};

template<typename LabelUnderlieType>
class LabelWithWildcard: public Label<LabelUnderlieType>{
private:
    static constexpr LabelUnderlieType  wildcard
                   = LabelUnderlieType::wildcardValue;

    using LabelType = Label<LabelUnderlieType>;

///ToDo:
/// add static_assert to check whether the
/// LabelUnderlieType Specified wildcard value
/// code sample:
//    template <typename T>
//    class HasFoo {
//        template <typename C>
//            static int call_foo (int (C::*)(float));
//        template <typename C>
//            static std::true_type has_foo (decltype(call_foo(&C::foo)));
//        template <typename C>
//            static std::false_type has_foo (...);
//    public:
//        static constexpr bool value = decltype(has_foo<T>(0)){};
//    };

public:
    using LabelType::Label;

    inline bool CanMatchTo(const LabelWithWildcard<LabelUnderlieType>& label) const{
        if (this->kLable_ == this->wildcard)
            return true;
        return LabelType::CanMatchTo(label);
    }
};

#endif // _LABEL_WITH_WILDCARD_HPP
