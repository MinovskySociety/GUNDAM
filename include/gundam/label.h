#ifndef _LABEL_HPP
#define _LABEL_HPP

#include <string>
#include <type_traits>
#include <assert.h>

#include "define.h"

template<typename... LabelUnderlieType>
class Label;

template<typename         Arg1,
         typename         Arg2,
         typename... OtherArgs>
class Label<Arg1,Arg2,OtherArgs...>{
private:
    /// only allows zero or one parameter
    /// trick compiler, same to static_assert(false)
    /// since the template parameter must be used
    static_assert(!std::is_object<Arg1>::value,
                  "Illegal parameter number");
    static_assert( std::is_object<Arg1>::value,
                  "Illegal parameter number");
};

template<>
class Label<>{
    /// trivial label, only have one value
public:
    using UnderlieType = void;

    Label(){
        return;
    }

    inline bool operator ==(const Label<>& label) const{
        return true;
    }
    inline bool CanMatchTo(const Label<>& label) const{
        return true;
    }
    inline std::string to_string() const{
        return "trivial label";
    }
};

template<typename LabelUnderlieType>
class Label<LabelUnderlieType>{
private:
    static_assert(std::is_object<LabelUnderlieType>::value,
                  "Illegal underlie label type");
protected:
    const LabelUnderlieType kLable_;

public:
    using UnderlieType = LabelUnderlieType;

    Label(const LabelUnderlieType& kLable_)
                          :kLable_(kLable_){
        return;
    }

    inline bool operator ==(const Label<LabelUnderlieType>& label) const{
        return this->kLable_ == label.kLable_;
    }
    inline bool CanMatchTo(const Label<LabelUnderlieType>& label) const{
        return this->kLable_ == label.kLable_;
    }
    inline std::string to_string() const{
        return std::to_string(this->kLable_);
    }
};
#endif // _LABEL_HPP
