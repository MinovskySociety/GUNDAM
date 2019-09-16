#include <iostream>
#include  <fstream>

//#include "gundam/label.hpp"
//#include "gundam/node.hpp"
//#include "gundam/attribute_node.hpp"
//#include "gundam/edge.hpp"
//#include "gundam/weighted_edge.hpp"

#include "gundam/vertex.hpp"
//#include "gundam/configures.hpp"
#include "gundam/graph_configure.hpp"
#include "gundam/graph.hpp"
#include "gundam/algorithm.hpp"

#include "test.hpp"


int main(){
//    std::cout << "Hello world!" << std::endl;
//    Label<> trivialLabel;
//    Label<uint8_t> L1(1);
//    Label<uint8_t> L2(1);
//    Label<uint8_t> L3(2);
//    Label<uint64_t> LX(2);
////    LabelWithWildcard<int,0> L4(1);
////    LabelWithWildcard<uint8_t,19> L5(1);
////    LabelWithWildcard<uint8_t,2> L6(2);
////    LabelWithWildcard<uint8_t> L7(std::numeric_limits<uint8_t>::max());
//
//    std::cout<<"L1 can match to L2:"<<L1.CanMatchTo(L2)<<std::endl;
//    std::cout<<"L2 can match to L1:"<<L2.CanMatchTo(L1)<<std::endl;
//    std::cout<<"L1 can match to L3:"<<L1.CanMatchTo(L3)<<std::endl;
//    std::cout<<"L3 can match to L1:"<<L3.CanMatchTo(L1)<<std::endl;
////    std::cout<<"L4 can match to L5:"<<L4.CanMatchTo(L5)<<std::endl;
////    std::cout<<"L5 can match to L4:"<<L5.CanMatchTo(L4)<<std::endl;
////    std::cout<<"L4 can match to L1:"<<L4.CanMatchTo(L1)<<std::endl;
////    std::cout<<"L1 can match to L4:"<<L1.CanMatchTo(L4)<<std::endl;
////    std::cout<<"L7 can match to L6:"<<L7.CanMatchTo(L6)<<std::endl;
////    std::cout<<"L6 can match to L7:"<<L6.CanMatchTo(L7)<<std::endl;
//
////    Node<Label<uint32_t>> n1(1);
//
////    Label<int> d(1);
//
////    Node<SetIDType   <uint32_t>,
////         SetLabelType<Label<uint64_t> >> n1(2,LX);
//
////    Node<SetIDType   <uint32_t>,
////         Label<uint64_t> > n1(2,LX);
//
////    AttributeNode<//SetAttributeConst<true>,
////                         SetAttributeIsConst<true>,
////                         SetAttributeKeyType<uint8_t>,
////                         SetIDType<uint32_t>> n1(2,L1);
//
//    Node<>                              n0(1,L1);
//
//    Node<SetLabelType<Label<uint64_t>>> n1(1,LX);
//
//    Node<SetIDType<uint8_t>>            n2(1,L1);
//
//    Node<SetIDType<uint8_t>,
//         SetLabelType<Label<uint64_t>>> n3(1,LX);
//
//    Node<SetLabelType<Label<uint64_t>>,
//         SetIDType<uint8_t>>            n4(1,LX);
//
////    Node<SetIDType<uint8_t>,
////         Label<uint64_t>> n4E(1,LX);
//
//    AttributeNode<SetAttributeKeyType<uint8_t>>  n5(1,L1);
//
//    AttributeNode<SetLabelType<Label<uint64_t>>> n6(1,LX);
//
//    AttributeNode<SetAttributeKeyType<uint8_t>,
//                  SetLabelType<Label<uint64_t>>> n7(1,LX);
//
//    AttributeNode<SetLabelType<Label<uint64_t>>,
//                  SetAttributeKeyType<uint8_t>>  n8(1,LX);
//
////    AttributeNode<SetLabelType<Label<uint8_t>>> n3(n2);
//    Edge<SetLabelType<Label<uint8_t>>> e1(L1);
////    WeightedEdge<SetLabelType<Label<uint8_t>>,
////                SetWeightType<uint16_t>> e2(L1,6);
////
////    WeightedEdge<SetLabelType<Label<uint8_t>>,
////                SetWeightType<uint16_t>> e3(e2);
////    Node<SetIDType<uint32_t>> n1(1,L1);
//
////    Node<NodeConfigure<NodeConfigureType::LabelType,Label<>>,
////         NodeConfigure<NodeConfigureType::IDType,uint32_t>> n2(2,L1);
////
////    Node<NodeConfigure<NodeConfigureType::IDType,uint64_t>> n3(2,L1);
////
////    Node<NodeConfigure<NodeConfigureType::LabelType,Label<uint64_t>>,
////         NodeConfigure<NodeConfigureType::IDType,uint32_t>> n4(2,LX);
////
////    Node<NodeConfigure<NodeConfigureType::IDType,uint32_t>,
////         NodeConfigure<NodeConfigureType::IDType,uint32_t>> n5(2,L1);
//
////    Graph<SetNodeIDType<std::string>,
////          SetNodeAttributeType<std::string>,
////          SetEdgeLabelType<std::string>,
////          SetAllowDuplicateEdge<true>> G;
////
////    G.AddNode(...)
////    ...
////    G.AddEdge(...)
////    ...
////
////    G::VertexType* const v1 = G.GetNode("admin");///EdgeLabelIterator
////    for (auto edge_label_iter = v1.GetEdgeLabelIterator();
////             !edge_label_iter.IsDone();
////              edge_label_iter.ToNext()){
////        std::cout<<edge_label_iter.GetEdgeLabel()<<std::endl;
////    }
////    for (auto vertex_iter = v1.GetVertexIterator("visit");
////             !vertex_iter.IsDone();
////              vertex_iter.ToNext()){
////        std::cout<<(*vertex_iter).ToString()<<std::endl;
////    }
////
////    return 0;
//
    using GraphType
        = GUNDAM::Graph<>;
//        = GUNDAM::Graph<SetVertexIDType<uint32_t>,
//                        SetVertexLabelType<Label<uint32_t>>,
//                        SetVertexAttributeType<std::string>,
//                        SetVertexAttributeIsConst<true>,
//                        SetEdgeLabelType<Label<uint32_t>>>;

    GraphType G;

    GraphType G2(G);

    std::ifstream vertex_file("..\\..\\..\\..\\benchmark\\graph1\\vertex.txt");
    std::ifstream   edge_file("..\\..\\..\\..\\benchmark\\graph1\\edge.txt");

    test::LoadGraph(G,vertex_file,
                        edge_file);

    const typename GraphType::VertexType* const vertex = G.const_vertex_ptr(1);
    assert(vertex != nullptr);

    algorithm::bfs(G,vertex,std::cout);

    std::cout<<"end-of-program"<<std::endl;
////    for (auto it = vertex->edge_label_cbegin();!it.IsDone();it++){
////        std::cout<<"edge label is:"<<*it<<std::endl;
////    }
////    for (auto it = vertex->vertex_cbegin();!it.IsDone();it++){
////        std::cout<<"vertex id is:"       <<it->id()       <<std::endl;
////        std::cout<<"vertex label is:"    <<it->label()    <<std::endl;
////        std::cout<<"vertex attribute is:"<<it->attribute()<<std::endl;
////        for (auto edge_label_iter = it->edge_label_cbegin();
////                 !edge_label_iter.IsDone();edge_label_iter++){
////            ...
////        }
////    }
}

//     DAG <SetLabelType<LabelWithWildcard<uint8_t>>,
//          SetAllowDuplicateEdge<false>>      P;
//    Graph<SetIDType<std::string>,
//          SetEdgeAttributeType<std::string>> G;
//
//    Algorithm::MatchAlgorithm(P,G,matches);

//template<template <typename...> class GraphType0, typename... configures0,
//         template <typename...> class GraphType1, typename... configures1>
//void MatchAlgorithm(const GraphType0<configures0...> graphpattern,
//                    const GraphType1<configures1...> datagraph,
//                    Match<GraphType0<configures0...>,
//                          GraphType1<configures1...>>& matches){
//    using GraphPatternType = GraphType0<configures0...>;
//    using    DataGraphType = GraphType1<configures1...>;
//
//    using GraphPatternVertexType = GraphPatternType::VertexType;
//    using    DataGraphVertexType =    DataGraphType::VertexType;
//
//    static_assert(std::is_base_of<Graph<configure0...>,DataGraphType>,
//                 "data graph should be a graph");
//    static_assert(std::is_base_of< DAG <configure1...>,GraphPatternType>,
//                 "graph pattern should be a DAG");
//    static_assert(!GraphPatternType::AllowDuplicateEdge,
//                 "graph pattern does not allow duplicate edge");
//}

//#include <iostream>
//#include <typeinfo>
//
//class Base{
//public:
//    using testType  = int;
//    using testType2 = int64_t;
//
//    static constexpr bool testFunction(){
//        return true;
//    }
//};
//
//class Derive:public Base{
//public:
//    using testType = std::string;
//
////    static constexpr bool testFunction(){
////        return false;
////    }
//};
//
//int main(){
//    std::cout<<typeid( Base ::testType ).name()<<std::endl;
//    std::cout<<typeid( Base ::testType2).name()<<std::endl;
//    std::cout<<typeid(Derive::testType ).name()<<std::endl;
//    std::cout<<typeid(Derive::testType2).name()<<std::endl;
//
//    std::cout<< Base ::testFunction()<<std::endl;
//    std::cout<<Derive::testFunction()<<std::endl;
//}
