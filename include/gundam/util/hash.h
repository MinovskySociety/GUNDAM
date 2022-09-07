#ifndef _GUNDAM_UTIL_HASH_H
#define _GUNDAM_UTIL_HASH_H

namespace GUNDAM {

struct v_handle_hash {
  template <typename Handle>
  size_t operator() (const Handle& h) const {
      return std::hash<unsigned>{}(h->id());
  }
};

};

#endif // _GUNDAM_UTIL_HASH_H