#ifndef _GUNDAM_TOOL_TOPOLOGICAL_STAR_STAR_COVER_BY_PATH_H
#define _GUNDAM_TOOL_TOPOLOGICAL_STAR_STAR_COVER_BY_PATH_H

#include "gundam/type_getter/vertex_handle.h"
#include "gundam/tool/connected.h"
#include "gundam/tool/topological/path/is_path.h"
#include "gundam/tool/topological/star/is_star.h"

namespace GUNDAM {

/* ################################################# *
 * ## return false if:                            ## *
 * ##   1, input star is not a star               ## *
 * ##   2, any pattern in path_set is not a path  ## *
 * ##   3, path_set is empty                      ## *
 * ##   4, any of pathes in input star cannot     ## *
 * ##      be covered by the input path_set       ## *
 * ## else, return true                           ## *
 * ################################################# */
template<typename StarGraphType,
         typename PathGraphType>
bool StarCoverByPath(const StarGraphType&  star,
         const std::vector<PathGraphType>& path_set) {
  static constexpr bool bidirectional = true;
  const auto [end_vertex_set,
          central_vertex_handle] = StarEndPoints<bidirectional>(star);
  if (end_vertex_set.empty()) {
    assert(!central_vertex_handle);
    return false;
  }
  /* ########################## *
   * ##  do some thing here  ## *
   * ########################## */

  // wenzhi:  hint:
  //   use MatchUsingMatch with given partial_match, 
  //   see "Helper method3", or other helper methods
  //   in doc/prog_doc/match_using_match.md
  assert(false);
  return true;
}

};

#endif // _GUNDAM_TOOL_TOPOLOGICAL_STAR_STAR_COVER_BY_PATH_H