#ifndef _CONFIGURES_HPP
#define _CONFIGURES_HPP

#include "define.hpp"

enum class StoreData:bool{
      OutEdge,
    InOutEdge
};

enum class DataStructure:bool{
    AdjacencyMatrix,
    AdjacencyList
};

enum class ContainerType:uint8_t{
    List,   /// for graph with few  elements (Edge types or associated Vertexes)
    Vector, /// for static graph
    Array,  /// for static graph that can statically set the maximum size of neighbors
    Map,    /// for graph with lots elements (Edge types or associated Vertexes)
    Set,    ///
/// unimplemented feature
    Adaptive/// use list when this vertex have few  elements (Edge types or associated Vertexes)
            /// use map  when this vertex have lots elements (Edge types or associated Vertexes)
            /// for dynamic graph with non-balanced elements distribution
};

enum class SortType:bool{
    /// how the vertexes connected by the same type of edge
    /// are sorted
    None, /// not sorted
    ByID  /// sorted by ID of the vertex(or edge)
};

enum class IteratorType:uint8_t{
    UnspecifiedEdgeLabel,
      SpecifiedEdgeLabel,
      SingleElement
};
#endif // _CONFIGURES_HPP
