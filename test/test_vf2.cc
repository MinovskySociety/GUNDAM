#include "gundam/vf2.h"

#include <iostream>

#include "gtest/gtest.h"

TEST(TestGUNDAM, VF2) {
  using namespace GUNDAM;
   using GraphType = GUNDAM::Graph<>;
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using VertexLabelUnderlieType = typename VertexType::LabelType::UnderlieType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using EdgeLabelUnderlieType = typename EdgeType::LabelType::UnderlieType;
  using VertexPtr = typename GraphType::VertexConstPtr;
  GraphType query, target;
  VertexIDType query_id = 1, target_id = 1;
  
  //query
  query.AddVertex(1,VertexLabelType(0));
  query.AddVertex(2,VertexLabelType(1));
  query.AddVertex(3,VertexLabelType(0));
  query.AddEdge(1,2,EdgeLabelType(1),1);
  query.AddEdge(3,2,EdgeLabelType(1),2);
  //query.AddEdge(3,1,EdgeLabelType(1));
  //target
  target.AddVertex(1,VertexLabelType(0));
  target.AddVertex(2,VertexLabelType(1));
  target.AddVertex(3,VertexLabelType(0));
  target.AddEdge(1,2,EdgeLabelType(1),1);
  target.AddEdge(3,2,EdgeLabelType(1),2);
  target.AddEdge(3,1,EdgeLabelType(1),3);
  
   std::vector<std::map<VertexPtr,VertexPtr>> match_result;
   GUNDAM::VF2<GUNDAM::MatchSemantics::kIsomorphism>(
       query,target,match_result
   );
   std::cout<<match_result.size()<<std::endl;
   if (!match_result.empty()){
       for (auto &it:match_result){
           std::cout<<"match:"<<std::endl;
           std::cout<<"query_id target_id:"<<std::endl;
           for (auto it1:it){
               std::cout<<it1.first->id()<<" "<<it1.second->id()<<std::endl;
           }
       }
   }
   
    return ;
  ASSERT_TRUE(true);
}