#ifdef _LABEL_CONFIGURE_HPP
#define _LABEL_CONFIGURE_HPP

#include <assert.h>

enum class LabelConfigureType:uint8_t{
    UnderlieData
};

class LabelConfigures{
protected:
    inline static const bool SpecifiedUnderlieType() const{
        return false;
    }
};

template<LabelConfigureType::UnderlieDataType,
         typename      tLabelUnderlieDataType,
         typename... OtherConfigures>
class LabelConfigures: protected LabelConfigures<OtherConfigures>{
protected:
    static_assert(!this->SpecifiedUnderlieType(),
                  "replicate configuration of label underline data type");

    using LabelUnderlieType = tLabelUnderlieType;

    inline static const bool SpecifiedUnderlieType() const override{
        return false;
    }
};
#endif //  _LABEL_CONFIGURE_HPP
