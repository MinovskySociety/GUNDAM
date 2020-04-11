#ifndef _POINTER_H
#define _POINTER_H

#include <type_traits>

namespace GUNDAM {
template <class ContentType, bool is_const>
class GPointer {
 public:
  GPointer() : is_null_(true){};

  GPointer(const GPointer &b) : is_null_{b.is_null_} {
    if (!is_null_) {
      v_ = b.v_;
    }
  };

  GPointer(GPointer &&b) : is_null_{std::move(b.is_null_)} {
    if (!is_null_) {
      v_ = std::move(b.v_);
    }
  }

  GPointer(const ContentType &v) : v_(v), is_null_(false) {}

  GPointer(typename std::conditional<is_const, const ContentType *,
                                     ContentType *>::type p) {
    if (p) {
      is_null_ = false;
      v_ = *p;
    } else {
      is_null_ = true;
    }
  }  

  GPointer &operator=(const GPointer &b) {
    is_null_ = b.is_null_;
    if (!is_null_) {
      v_ = b.v_;
    }
    return *this;
  }  

  GPointer &operator=(GPointer &&b) {
    is_null_ = std::move(b.is_null_);
    if (!is_null_) {
      v_ = std::move(b.v_);
    }
    return *this;
  }  

  GPointer &operator=(typename std::conditional<is_const, const ContentType *,
                                                ContentType *>::type *p) {
    if (p) {
      is_null_ = false;
      v_ = *p;
    } else {
      is_null_ = true;
    }
    return *this;
  }

  ~GPointer(){};

  
  typename std::conditional<is_const, const ContentType &, ContentType &>::type
  operator*() {
    assert(!IsNull());
    return v_;
  }

  const ContentType &operator*() const {
    assert(!IsNull());
    return v_;
  }

  typename std::conditional<is_const, const ContentType *, ContentType *>::type
  operator->() {
    assert(!IsNull());
    return &v_;
  }

  const ContentType *operator->() const {
    assert(!IsNull());
    return &v_;
  }

  bool operator==(const GPointer &b) const {
    if (IsNull()) {
      return b.IsNull();
    } else if (b.IsNull()) {
      return false;
    } else {
      return v_ == b.v_;
    }
  }

  bool operator<(const GPointer &b) const {
    if (IsNull() || b.IsNull()) {
      return false;
    } else {
      return v_ < b.v_;
    }
  }

  bool IsNull() const { return is_null_; }

  operator bool() const { return !IsNull(); };

 private:
  ContentType v_;
  bool is_null_;
};
}  // namespace GUNDAM

#endif