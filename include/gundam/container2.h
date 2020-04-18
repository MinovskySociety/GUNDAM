#ifndef _CONTAINER2_H
#define _CONTAINER2_H

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

namespace GUNDAM {

template <typename KeyType, typename ValueType>
class VectorDict {
 private:
  using InnerContainerType = std::vector<std::pair<KeyType, ValueType>>;

 public:
  using iterator = typename InnerContainerType::iterator;

  using const_iterator = typename InnerContainerType::const_iterator;

  using value_type = typename InnerContainerType::value_type;

 public:
  size_t Count() const { return container_.size(); }

  template <class... VArgs>
  std::pair<iterator, bool> Insert(const KeyType& key, VArgs&&... vargs) {
    for (auto it = container_.begin(); it != container_.end(); ++it) {
      if (it->first == key) return std::make_pair(it, false);
    }
    container_.emplace_back(key, ValueType(std::forward<VArgs>(vargs)...));
    return make_pair(container_.end() - 1, true);
  }

  iterator Find(const KeyType& key) {
    for (auto it = container_.begin(); it != container_.end(); ++it) {
      if (it->first == key) return it;
    }
    return container_.end();
  }

  const_iterator Find(const KeyType& key) const {
    for (auto it = container_.cbegin(); it != container_.cend(); ++it) {
      if (it->first == key) return it;
    }
    return container_.cend();
  }

  bool Erase(const KeyType& key) {
    for (auto it = container_.begin(); it != container_.end(); ++it) {
      if (it->first == key) {
        container_.erase(it);
        return true;
      }
    }
    return false;
  }

  void Clear() { container_.clear(); }

  iterator begin() noexcept { return container_.begin(); }

  const_iterator begin() const noexcept { return container_.begin(); }

  const_iterator cbegin() const noexcept { return container_.cbegin(); }

  iterator end() noexcept { return container_.end(); }

  const_iterator end() const noexcept { return container_.cend(); }

  const_iterator cend() const noexcept { return container_.cend(); }

 private:
  InnerContainerType container_;
};

template <typename KeyType, typename ValueType>
class MapDict {
 private:
  using InnerContainerType = std::map<KeyType, ValueType>;

 public:
  using iterator = typename InnerContainerType::iterator;

  using const_iterator = typename InnerContainerType::const_iterator;

  using value_type = typename InnerContainerType::value_type;

 public:
  size_t Count() const { return container_.size(); }

  std::pair<iterator, bool> Insert(const KeyType& key, const ValueType& value) {
    return container_.emplace(key, value);
  }

  iterator Find(const KeyType& key) { return container_.find(key); }

  const_iterator Find(const KeyType& key) const { return container_.find(key); }

  bool Erase(const KeyType& key) {
    auto it = container_.find(key);
    if (it == container_.end()) return false;
    container_.erase(it);
    return true;
  }

  iterator Erase(iterator it) { return container_.erase(it); }

  iterator Erase(const_iterator it) { return container_.erase(it); }

  void Clear() { container_.clear(); }

  iterator begin() noexcept { return container_.begin(); }

  const_iterator begin() const noexcept { return container_.begin(); }

  const_iterator cbegin() const noexcept { return container_.cbegin(); }

  iterator end() noexcept { return container_.end(); }

  const_iterator end() const noexcept { return container_.cend(); }

  const_iterator cend() const noexcept { return container_.cend(); }

 private:
  InnerContainerType container_;
};

template <class ValueType>
class SortedVectorSet {
 private:
  using InnerContainerType = std::vector<ValueType>;

 public:
  using iterator = typename InnerContainerType::iterator;

  using const_iterator = typename InnerContainerType::const_iterator;

  using value_type = typename InnerContainerType::value_type;

 public:
  size_t Count() const { return container_.size(); }

  template <class... VArgs>
  std::pair<iterator, bool> Insert(VArgs&&... vargs) {
    ValueType value(std::forward<VArgs>(vargs)...);
    auto iter = std::lower_bound(container_.begin(), container_.end(), value);
    if (iter != container_.end() && *iter == value)
      return std::make_pair(iter, false);

    return std::make_pair(container_.emplace(iter, std::move(value)), true);
  }

  iterator Find(const ValueType& value) {
    auto iter = std::lower_bound(container_.begin(), container_.end(), value);
    if (iter == container_.end() || *iter != value) return container_.end();
    return iter;
  }

  const_iterator Find(const ValueType& value) const {
    auto iter = std::lower_bound(container_.cbegin(), container_.cend(), value);
    if (iter == container_.cend() || *iter != value) return container_.cend();
    return iter;
  }

  size_t Erase(const ValueType& value) {
    auto iter = Find(value);
    if (iter == container_.end()) return 0;
    container_.erase(iter);
    return 1;
  }

  iterator Erase(iterator pos) { return container_.erase(pos); }

  iterator Erase(const_iterator pos) { return container_.erase(pos); }

  void Clear() { container_.clear(); }

  iterator begin() noexcept { return container_.begin(); }

  const_iterator begin() const noexcept { return container_.begin(); }

  const_iterator cbegin() const noexcept { return container_.cbegin(); }

  iterator end() noexcept { return container_.end(); }

  const_iterator end() const noexcept { return container_.cend(); }

  const_iterator cend() const noexcept { return container_.cend(); }

 private:
  InnerContainerType container_;
};

template <class KeyType, class ValueType>
class SortedVectorDict {
 private:
  using InnerContainerType = std::vector<std::pair<KeyType, ValueType>>;

  static bool Compare(const std::pair<KeyType, ValueType>& pair,
                      const KeyType& key) {
    return pair.first < key;
  }

 public:
  using value_type = typename InnerContainerType::value_type;

  using reference = typename InnerContainerType::reference;

  using const_reference = typename InnerContainerType::const_reference;

  using pointer = typename InnerContainerType::pointer;

  using const_pointer = typename InnerContainerType::const_pointer;

  using iterator = typename InnerContainerType::iterator;

  using const_iterator = typename InnerContainerType::const_iterator;

 public:
  size_t Count() const { return container_.size(); }

  template <class... VArgs>
  std::pair<iterator, bool> Insert(const KeyType& key, VArgs&&... vargs) {
    auto iter =
        std::lower_bound(container_.begin(), container_.end(), key, Compare);
    if (iter != container_.end() && iter->first == key)
      return std::make_pair(iter, false);

    return std::make_pair(
        container_.emplace(iter, key, ValueType(std::forward<VArgs>(vargs)...)),
        true);
  }

  iterator Find(const KeyType& key) {
    auto iter =
        std::lower_bound(container_.begin(), container_.end(), key, Compare);
    if (iter == container_.end() || iter->first != key) return container_.end();
    return iter;
  }

  const_iterator Find(const KeyType& key) const {
    auto iter =
        std::lower_bound(container_.cbegin(), container_.cend(), key, Compare);
    if (iter == container_.cend() || iter->first != key)
      return container_.cend();
    return iter;
  }

  size_t Erase(const KeyType& key) {
    auto iter = Find(key);
    if (iter == container_.end()) return 0;
    container_.erase(iter);
    return 1;
  }

  iterator Erase(iterator pos) { return container_.erase(pos); }

  iterator Erase(const_iterator pos) { return container_.erase(pos); }

  void Clear() { container_.clear(); }

  iterator begin() noexcept { return container_.begin(); }

  const_iterator begin() const noexcept { return container_.begin(); }

  const_iterator cbegin() const noexcept { return container_.cbegin(); }

  iterator end() noexcept { return container_.end(); }

  const_iterator end() const noexcept { return container_.cend(); }

  const_iterator cend() const noexcept { return container_.cend(); }

 private:
  InnerContainerType container_;
};

template <class ValueType>
class PosKeyVector {
 private:
  using InnerContainerType = std::vector<ValueType>;

 public:
  using iterator = typename InnerContainerType::iterator;

  using const_iterator = typename InnerContainerType::const_iterator;

  using value_type = typename InnerContainerType::value_type;

 public:
  PosKeyVector() = default;

  PosKeyVector(size_t size) { container_.reserve(size); }

  size_t Count() const { return container_.size(); }

  template <class... VArgs>
  iterator Insert(VArgs&&... vargs) {
    container_.emplace_back(std::forward<VArgs>(vargs)...);
    return container_.end() - 1;
  }

  iterator Find(size_t pos) {
    assert(pos < container_.size());
    return container_.begin() + pos;
  }

  const_iterator Find(size_t pos) const {
    assert(pos < container_.size());
    return container_.cbegin() + pos;
  }

  void Clear() { container_.clear(); }

  iterator begin() noexcept { return container_.begin(); }

  const_iterator begin() const noexcept { return container_.begin(); }

  const_iterator cbegin() const noexcept { return container_.cbegin(); }

  iterator end() noexcept { return container_.end(); }

  const_iterator end() const noexcept { return container_.cend(); }

  const_iterator cend() const noexcept { return container_.cend(); }

  size_t GetPos(const const_iterator& it) const {
    return static_cast<size_t>(it - container_.cbegin());
  }

  size_t GetPos(const ValueType* p) const {
    return static_cast<size_t>(p - &*container_.cbegin());
  }

 private:
  InnerContainerType container_;
};

}  // namespace GUNDAM

#endif  // _CONTAINER_H
