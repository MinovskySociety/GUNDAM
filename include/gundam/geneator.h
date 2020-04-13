#ifndef _GENEATOR_H
#define _GENEATOR_H
#include <type_traits>
template <typename IDType,
          typename std::enable_if<std::is_arithmetic<IDType>::value,
                                  bool>::type = false>
class SimpleArithmeticIDEmptyGenerator {
 private:
 public:
  IDType GetID() {}

  void UseID(const IDType& id) {}    
};

template <typename IDType,
          typename std::enable_if<std::is_arithmetic<IDType>::value,
                                  bool>::type = false>
class SimpleArithmeticIDGenerator {
 private:
  IDType used_max_ = 0;

 public:
  IDType GetID() {
    this->used_max_ = this->used_max_ + 1;
    return used_max_;
  }

  void UseID(const IDType& id) {
    if (id > this->used_max_) {
      this->used_max_ = id;
    }
  }
};
#endif