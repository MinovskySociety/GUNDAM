#ifndef _GUNDAM_UTIL_UTIL_H
#define _GUNDAM_UTIL_UTIL_H

namespace GUNDAM {

enum class EdgeDirection : uint8_t { 
  kIn    = 0, 
  kOut   = 1, 
  kInOut = 2
};

namespace execution {

class sequenced_policy;
class  parallel_policy;

};

}; // GUNDAM


#endif // _GUNDAM_UTIL_UTIL_H