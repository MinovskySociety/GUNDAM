#ifndef _ATTRIBUTE_HPP
#define _ATTRIBUTE_HPP

#include <assert.h>

#include "define.h"

template<typename KeyType>
class Attribute{
 private:
  static_assert(std::is_object<KeyType>::value,
               "Illegal Attribute Key Type");

  class AbstractValue{
   public:
    virtual ~AbstractValue() = 0;
  };

  template<typename ConcreteDataType_>
  class ConcreteValue : public AbstractValue{
   private:
    ConcreteDataType_ value_;

   public:
    ConcreteValue(const ConcreteDataType_& value)
                                   :value_(value){
      return;
    }
    virtual ~ConcreteValue() override{
      return;
    }
    inline const ConcreteDataType_& const_value() const{
      return this->value_;
    }
    inline ConcreteDataType_& value(){
      return this->value_;
    }
    inline void set_value(const ConcreteDataType_& value){
      return this->value_ = value;
    }
  };

  const KeyType kKey_;
  AbstractValue* const value_ptr_;

 public:
  Attribute(const KeyType& key):kKey_(key),
                           value_ptr_(nullptr){
    return;
  }

  ~Attribute(){
    delete this->value_ptr_;
    return;
  }

  inline const KeyType& key() const{
    return this->kKey_;
  }

  inline bool operator<(const Attribute& attribute) const{
    return this->kKey_ < attribute.kKey_;
  }

  inline bool operator>(const Attribute& attribute) const{
    return this->kKey_ > attribute.kKey_;
  }

  inline bool operator==(const Attribute& attribute) const{
    return this->kKey_ == attribute.kKey_;
  }

  inline bool operator!=(const Attribute& attribute) const{
    return this->kKey_ != attribute.kKey_;
  }

  template<typename ConcreteDataType_>
  inline void set_value(const ConcreteDataType_& value){
    if (this->value_ptr_== nullptr){
      this->value_ptr_ = new ConcreteValue<ConcreteDataType_>(value);
      return;
    }
    static_cast<ConcreteValue<
                ConcreteDataType_>*
               >(this->value_ptr_)->set_value(value);
    return;
  }

  template<typename ConcreteDataType_>
  inline ConcreteDataType_& value(){
    return static_cast<ConcreteValue<
                       ConcreteDataType_>*
                      >(this->value_ptr_)->value();
  }

  template<typename ConcreteDataType_>
  inline const ConcreteDataType_& const_value() const{
    return static_cast<ConcreteValue<
                       ConcreteDataType_>*
                      >(this->value_ptr_)->const_value();
  }
};
#endif // _ATTRIBUTE_HPP
