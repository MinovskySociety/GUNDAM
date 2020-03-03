#ifndef _ITERATOR2_H
#define _ITERATOR2_H

#include <cassert>
#include <type_traits>
#include <utility>

namespace GUNDAM {

template <class IteratorType,
          class ValueType = typename IteratorType::value_type>
class DefaultCast {
 public:
  ValueType &operator()(IteratorType &it) { return *it; }
};

template <class IteratorType, class ValueType = typename std::remove_pointer<
                                  typename IteratorType::value_type>::type>
class PointerCast {
 public:
  ValueType &operator()(IteratorType &it) { return *(*it); }
};

template <class IteratorType,
          class ValueType = typename IteratorType::value_type::second_type>
class PairSecondCast {
 public:
  ValueType &operator()(IteratorType &it) { return it->second; }
};

template <class IteratorType,
          class ValueType = typename std::remove_pointer<
              typename IteratorType::value_type::second_type>::type>
class PairSecondPointerCast {
 public:
  ValueType &operator()(IteratorType &it) const { return *(it->second); }
};

template <class IteratorType, class ValueType,
          class Cast = DefaultCast<IteratorType>>
class GIterator {
 public:
  GIterator() = default;

  template <class IterBegin, class IterEnd>
  GIterator(IterBegin &&begin, IterEnd &&end)
      : it_{std::forward<IterBegin>(begin)}, end_{std::forward<IterEnd>(end)} {}

  GIterator(const GIterator &) = default;

  GIterator(GIterator &&) = default;

  GIterator &operator=(const GIterator &) = default;

  GIterator &operator=(GIterator &&) = default;

  ~GIterator() = default;

  ValueType &operator*() { return Cast()(it_); }

  const ValueType operator*() const { return Cast()(it_); }

  ValueType *operator->() { return &Cast()(it_); }

  const ValueType *operator->() const { return &Cast()(it_); }

  operator ValueType *() { return &Cast()(it_); }

  operator const ValueType *() const { return &Cast()(it_); }

  GIterator operator++() {
    /// prefix
    assert(!this->IsDone());
    ++it_;
    return *this;
  }

  GIterator operator++(int) {
    /// postfix
    assert(!this->IsDone());
    GIterator tmp(*this);
    //++(*this);
    ++it_;
    return tmp;
  }

  bool IsDone() const { return it_ == end_; }

 private:
  IteratorType it_;
  IteratorType end_;
};

}  // namespace GUNDAM

#endif  // _ITERATOR_H