#ifndef MATCH_SEMANTICS
#define MATCH_SEMANTICS

namespace GUNDAM {

enum MatchSemantics : uint8_t {
   kIsomorphism,
  kHomomorphism,
        kSimulation,
    kDualSimulation,
  kStrongSimulation
};

enum MatchAlgorithm : bool {
  kVf2,
  kDagDp
};

};

#endif // MATCH_SEMANTICS