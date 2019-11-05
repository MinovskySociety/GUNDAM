#include "gundam/csvgraph.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(TestGUNDAM, CSVGraph) {
  using namespace GUNDAM;
    char e_file[1000]="C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_e.csv";
    char v_file[1000]="C:\\Users\\Administrator\\Desktop\\buaa\\dmine-pro\\testdata\\liantong_n.csv";
    using GraphType = GUNDAM::Graph<>;
    using VertexType = typename GraphType::VertexType;
    using   EdgeType = typename GraphType::EdgeType;
    using VertexIDType            = typename VertexType::IDType;
    using VertexLabelType         = typename VertexType::LabelType;
    using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
    using EdgeLabelType         = typename EdgeType::LabelType;
    using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
    using VertexPtr  =  typename GraphType::VertexPtr;
    GraphType test_graph;
    GUNDAM::ReadCSVGraph(test_graph,v_file,e_file);
    GUNDAM::OutputGraph(test_graph);
    std::cout<<"Output end!\n";
  ASSERT_TRUE(true);
}