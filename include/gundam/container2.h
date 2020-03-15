#ifndef _CONTAINER2_H
#define _CONTAINER2_H

#include <algorithm>
#include <map>
#include <utility>
#include <vector>

namespace GUNDAM {

template <typename KeyType, typename ValueType>
class Vector {
 private:
  using InnerContainerType = std::vector<std::pair<KeyType, ValueType>>;

 public:
  using iterator = typename InnerContainerType::iterator;

  using const_iterator = typename InnerContainerType::const_iterator;

  using value_type = typename InnerContainerType::value_type;

 public:
  size_t Count() const { return container_.size(); }

  std::pair<iterator, bool> Insert(const KeyType& key, const ValueType& value) {
    for (auto it = container_.begin(); it != container_.end(); ++it) {
      if (it->first == key) return std::make_pair(it, false);
    }
    container_.emplace_back(key, value);
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
class Map {
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


}  // namespace GUNDAM

#endif  // _CONTAINER_H
