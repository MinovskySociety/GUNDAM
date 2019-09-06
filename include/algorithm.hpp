#ifndef _ALGORITHM_HPP
#define _ALGORITHM_HPP

#include <set>
#include <queue>

#include "graph.hpp"

namespace algorithm{
    template<template <typename...> class GraphType, typename... configures>
    void bfs(const GraphType<configures...>& graph,
             const typename
                   GraphType<configures...>::VertexType* const start_vertex,
             std::ostream& out_stream){
        static_assert(std::is_base_of<GUNDAM::Graph    <configures...>,
                                              GraphType<configures...>>::value,
                     "Illegal input parameter, should be a graph");

        using VertexType = typename GraphType<configures...>::VertexType;
        using VertexPtr  = const VertexType*;

        std:: set <VertexPtr> visited;
        std::queue<VertexPtr> vertex_queue;

        vertex_queue.push(start_vertex);

        while (!vertex_queue.empty()){
            VertexPtr now_vertex = vertex_queue.front();
            vertex_queue.pop();
            out_stream<<"now visiting vertex:"<<now_vertex->id()<<std::endl;
            out_stream<<"output edge labels:"<<std::endl;
            for (auto it = now_vertex->edge_label_cbegin();
                     !it.IsDone();it++){
                out_stream<<it->to_string()<<std::endl;
            }
            out_stream<<"output edges:"<<std::endl;
            for (auto it = now_vertex->edge_cbegin();
                     !it.IsDone();it++){
                out_stream<<it->to_string()<<std::endl;
                out_stream<<"dst vertex:"<<std::endl;
                out_stream<<it->const_dst_ptr()
                              ->to_string()<<std::endl;
                out_stream<<"src vertex:"<<std::endl;
                out_stream<<it->const_src_ptr()
                              ->to_string()<<std::endl;
                out_stream<<"edge label:"<<std::endl;
                out_stream<<it->label()
                               .to_string()<<std::endl;
                out_stream<<"\toutput edges of dst vertex:"<<std::endl;
                for (auto eit = it->const_dst_ptr()->edge_cbegin();
                         !eit.IsDone();eit++){
                    out_stream<<"\t"<<eit->to_string()<<std::endl;
                }
            }
            out_stream<<"neighboring vertexes:"<<std::endl;
            for (auto it = now_vertex->vertex_cbegin();
                     !it.IsDone();it++){
                out_stream<<it->to_string()<<std::endl;
                if (visited.find(it.const_vertex_ptr()) == visited.end()){
                    visited.insert(it.const_vertex_ptr());
                    vertex_queue.emplace(it.const_vertex_ptr());
                }
            }
        }
        return;
    }

//    template<template <typename...> class GraphType, typename... configures>
//    void bfs(const GraphType<configures...>& graph,
//             const typename
//                   GraphType<configures...>::VertexType* const start_vertex,
//             std::ostream& out_stream){
//        static_assert(std::is_base_of<GUNDAM::Graph    <configures...>,
//                                              GraphType<configures...>>::value,
//                     "Illegal input parameter, should be a graph");
//
//        using VertexType = typename GraphType<configures...>::VertexType;
//        using VertexPtr  = const VertexType*;
//
//        std:: set <VertexPtr> visited;
//        std::queue<VertexPtr> vertex_queue;
//
//        vertex_queue.push(start_vertex);
//
//        while (!vertex_queue.empty()){
//            VertexPtr now_vertex = vertex_queue.front();
//            vertex_queue.pop();
//            for (auto it = now_vertex->vertex_cbegin();
//                     !it.IsDone();it++){
//                if (visited.find(it.const_vertex_ptr()) == visited.end()){
//                    visited.insert(it.const_vertex_ptr());
//                    vertex_queue.emplace(it.const_vertex_ptr());
//                }
//            }
//        }
//        return;
//    }
}

#endif // _ALGORITHM_HPP
