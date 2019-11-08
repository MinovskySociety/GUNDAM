#ifndef _UTIL_H
#define _UTIL_H

#include "graph.h"

#include <iostream>

namespace GUNDAM {

template <template <typename...> class GraphType, typename... configures>
void PrintGraph(const GraphType<configures...>& graph) {
  std::cout << "vertex:\n";
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    std::cout << it->id() << " " << it->label() << std::endl;
  }
  std::cout << "edge:\n";
  for (auto it = graph.VertexCBegin(); !it.IsDone(); it++) {
    for (auto edge_it = graph.FindConstVertex(it->id())->OutEdgeCBegin();
         !edge_it.IsDone(); edge_it++) {
      std::cout << edge_it->const_src_ptr()->id() << " "
                << edge_it->const_dst_ptr()->id() << " " << edge_it->label()
                << std::endl;
    }
  }
}
}  // namespace GUNDAM

#endif