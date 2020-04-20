#ifndef _GENEATOR_H
#define _GENEATOR_H

#include <type_traits>

namespace GUNDAM {

template <class IDType>
class VoidIDGenerator {
 public:
  IDType GetID() {}

  void UseID(const IDType& id) {}
};

template <class IDType, class = typename std::enable_if<
                            std::is_arithmetic<IDType>::value, bool>::type>
class SimpleArithmeticIDGenerator {
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

 private:
  IDType used_max_ = 0;
};

}  // namespace GUNDAM
#endif