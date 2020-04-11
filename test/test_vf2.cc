#include "gundam/large_graph.h"
#include "gundam/small_graph.h"
//#include "gundam/graph.h"
#include "gundam/label.h"
#include "gundam/vf2.h"

#include <cstdint>
#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <cstring>
#include <vector>

#include "gtest/gtest.h"

#include <ctime>
inline uint64_t GetTime() { return clock() * 1000 / CLOCKS_PER_SEC; }

template <typename Ptr1, typename Ptr2>
bool LabelEqual2(const Ptr1& a, const Ptr2& b) {
  return a->label() == b->label();
}

template<class QueryGraph, class TargetGraph>
void TestVF2_1() {
  using VertexLabelType = typename QueryGraph::VertexType::LabelType;
  using EdgeLabelType = typename TargetGraph::EdgeType::LabelType;

  using QueryVertexConstPtr = typename QueryGraph::VertexConstPtr;  
  using TargetVertexConstPtr = typename TargetGraph::VertexConstPtr;  

  using QueryEdgeConstPtr = typename QueryGraph::EdgeConstPtr;  
  using TargetEdgeConstPtr = typename TargetGraph::EdgeConstPtr;  

  QueryGraph query;
  TargetGraph target;  

  // query
  query.AddVertex(1, VertexLabelType(0));
  query.AddVertex(2, VertexLabelType(1));
  query.AddVertex(3, VertexLabelType(0));
  query.AddEdge(1, 2, EdgeLabelType(1), 1);
  query.AddEdge(3, 2, EdgeLabelType(1), 2);
  //query.AddEdge(3, 1, EdgeLabelType(1), 3);

  // target
  target.AddVertex(1, VertexLabelType(0));
  target.AddVertex(2, VertexLabelType(1));
  target.AddVertex(3, VertexLabelType(0));
  target.AddEdge(1, 2, EdgeLabelType(1), 1);
  target.AddEdge(3, 2, EdgeLabelType(1), 2);
  target.AddEdge(3, 1, EdgeLabelType(1), 3);

  std::vector<std::map<QueryVertexConstPtr, TargetVertexConstPtr>> match_result;
  int count =
      GUNDAM::VF2<GUNDAM::MatchSemantics::kIsomorphism>(query, target, -1, match_result);

  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
  ASSERT_EQ(count, 2);

  count = GUNDAM::VF2<GUNDAM::MatchSemantics::kIsomorphism>(
      query, target, LabelEqual2<QueryVertexConstPtr, TargetVertexConstPtr>,
      LabelEqual2<QueryEdgeConstPtr, TargetEdgeConstPtr>, -1, match_result);

  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
  ASSERT_EQ(count, 2);
}

TEST(TestGUNDAM, VF2_1) {
  using namespace GUNDAM;
  
  //using G = Graph<>;
  //TestVF2_1<G>();

  using LG = LargeGraph<uint32_t, uint32_t, std::string, uint32_t, uint32_t, std::string>;  
  //TestVF2_1<LG>();

  using SG = SmallGraph<uint32_t, uint32_t, uint32_t, uint32_t>;
  TestVF2_1<LG, LG>();
  TestVF2_1<SG, LG>();
}

template <typename Ptr1, typename Ptr2>
bool LabelEqualCustom(const Ptr1& a, const Ptr2& b) {
  return *(uint32_t*)&a->label() == static_cast<uint32_t>(stoi(b->label()));
}

template <class QueryGraph, class TargetGraph>
void TestVF2_2() {
  using namespace GUNDAM;

  QueryGraph query;
  TargetGraph target;

  // query
  ASSERT_TRUE(query.AddVertex(1, 0).second);
  ASSERT_TRUE(query.AddVertex(2, 1).second);
  ASSERT_TRUE(query.AddVertex(3, 0).second);
  ASSERT_TRUE(query.AddEdge(1, 2, 1, 1).second);
  ASSERT_TRUE(query.AddEdge(3, 2, 1, 2).second);

  // target
  ASSERT_TRUE(target.AddVertex(1, "0").second);
  ASSERT_TRUE(target.AddVertex(2, "1").second);
  ASSERT_TRUE(target.AddVertex(3, "0").second);
  ASSERT_TRUE(target.AddEdge(1, 2, std::string("1"), 1).second);
  ASSERT_TRUE(target.AddEdge(3, 2, std::string("1"), 2).second);
  ASSERT_TRUE(target.AddEdge(3, 1, std::string("1"), 3).second);

  std::vector<std::map<typename QueryGraph::VertexConstPtr,
                       typename TargetGraph::VertexConstPtr>>
      match_result;
  int count = VF2<MatchSemantics::kIsomorphism>(
      query, target,
      LabelEqualCustom<typename QueryGraph::VertexConstPtr,
                       typename TargetGraph::VertexConstPtr>,
      LabelEqualCustom<typename QueryGraph::EdgeConstPtr,
                       typename TargetGraph::EdgeConstPtr>,
      -1,
      match_result);

  ASSERT_EQ(count, 2);
  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match: " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
}

TEST(TestGUNDAM, VF2_2) {
  using namespace GUNDAM;

  //using QueryGraph =
  //    Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<Label<uint32_t>>,
  //          SetEdgeIDType<uint32_t>, SetEdgeLabelType<Label<uint32_t>>,
  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //          SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  //using TargetGraph =
  //    Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<std::string>,
  //          SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //          SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

  using QueryGraph = LargeGraph<uint32_t, Label<uint32_t>, std::string,
                                uint32_t, Label<uint32_t>, std::string>;

  using TargetGraph = LargeGraph<uint64_t, std::string, std::string, uint64_t,
                                 std::string, std::string>;  

  TestVF2_2<QueryGraph, TargetGraph>();
}

template <class QueryGraph, class TargetGraph>
void TestVF2_3() {
  using namespace GUNDAM;

  QueryGraph query;
  TargetGraph target;

  // query
  ASSERT_TRUE(query.AddVertex(1, 0).second);
  ASSERT_TRUE(query.AddVertex(2, 1).second);
  ASSERT_TRUE(query.AddVertex(3, 1).second);
  ASSERT_TRUE(query.AddEdge(1, 2, 1, 1).second);
  ASSERT_TRUE(query.AddEdge(1, 3, 1, 2).second);

  // target
  ASSERT_TRUE(target.AddVertex(1, "0").second);
  ASSERT_TRUE(target.AddVertex(2, "1").second);
  ASSERT_TRUE(target.AddVertex(3, "1").second);
  ASSERT_TRUE(target.AddEdge(1, 2, std::string("1"), 1).second);
  ASSERT_TRUE(target.AddEdge(1, 3, std::string("1"), 2).second);
  // ASSERT_TRUE(target.AddEdge(1, 2, std::string("1"), 3).second);

  std::vector<std::map<typename QueryGraph::VertexConstPtr,
                       typename TargetGraph::VertexConstPtr>>
      match_result;
  int count = VF2<MatchSemantics::kIsomorphism>(
      query, target,
      LabelEqualCustom<typename QueryGraph::VertexConstPtr,
                       typename TargetGraph::VertexConstPtr>,
      LabelEqualCustom<typename QueryGraph::EdgeConstPtr,
                       typename TargetGraph::EdgeConstPtr>,
      -1,
      match_result);

  ASSERT_EQ(count, 2);
  // ASSERT_EQ(count, 4);
  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match: " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
}

TEST(TestGUNDAM, VF2_3) {
  using namespace GUNDAM;

  //using QueryGraph =
  //    Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<Label<uint32_t>>,
  //          SetEdgeIDType<uint32_t>, SetEdgeLabelType<Label<uint32_t>>,
  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //          SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  //using TargetGraph =
  //    Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<std::string>,
  //          SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //          SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

  using QueryGraph = LargeGraph<uint32_t, Label<uint32_t>, std::string,
                                uint32_t, Label<uint32_t>, std::string>;

  using TargetGraph = LargeGraph<uint64_t, std::string, std::string, uint64_t,
                                 std::string, std::string>;  

  TestVF2_3<QueryGraph, TargetGraph>();
}


template <class QueryGraph, class TargetGraph>
void TestVF2Speed1(int times_outer, int times_inner) {
  using namespace GUNDAM;

  QueryGraph query;
  TargetGraph target;

  // query
  ASSERT_TRUE(query.AddVertex(1, 0).second);
  ASSERT_TRUE(query.AddVertex(2, 2).second);
  ASSERT_TRUE(query.AddVertex(3, 3).second);
  ASSERT_TRUE(query.AddVertex(4, 2).second);
  ASSERT_TRUE(query.AddVertex(5, 3).second);
  ASSERT_TRUE(query.AddEdge(1, 2, 1, 1).second);
  ASSERT_TRUE(query.AddEdge(1, 4, 1, 2).second);
  ASSERT_TRUE(query.AddEdge(3, 2, 2, 3).second);
  ASSERT_TRUE(query.AddEdge(5, 4, 2, 4).second);

  // target
  ASSERT_TRUE(target.AddVertex(1, 0).second);
  ASSERT_TRUE(target.AddVertex(2, 2).second);
  ASSERT_TRUE(target.AddVertex(3, 3).second);
  ASSERT_TRUE(target.AddVertex(4, 2).second);
  ASSERT_TRUE(target.AddVertex(5, 3).second);
  ASSERT_TRUE(target.AddVertex(6, 0).second);
  ASSERT_TRUE(target.AddVertex(7, 2).second);
  ASSERT_TRUE(target.AddVertex(8, 3).second);
  ASSERT_TRUE(target.AddVertex(9, 2).second);
  ASSERT_TRUE(target.AddVertex(10, 3).second);
  uint64_t eid = 0;
  ASSERT_TRUE(target.AddEdge(1, 2, 1, ++eid).second);
  ASSERT_TRUE(target.AddEdge(1, 4, 1, ++eid).second);
  ASSERT_TRUE(target.AddEdge(1, 7, 1, ++eid).second);
  ASSERT_TRUE(target.AddEdge(1, 9, 1, ++eid).second);
  ASSERT_TRUE(target.AddEdge(3, 2, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(5, 4, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(3, 4, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(5, 2, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(8, 7, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(10, 9, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(8, 9, 2, ++eid).second);
  ASSERT_TRUE(target.AddEdge(10, 7, 2, ++eid).second);

  std::vector<std::map<typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr>>
      match_result1;

  std::vector<std::vector<
      std::pair<typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr>>>
      match_result2;
  
  uint64_t start, end;

  for (int j = 0; j < times_outer; j++) {
    std::cout << "         Default: ";
    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int count = VF2(query, target, -1, match_result1);

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << end - start << " ms" << std::endl;

    std::cout << "       Recursive: ";
    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = _vf2::VF2_Recursive<MatchSemantics::kIsomorphism>(
          query, target,
          _vf2::LabelEqual<typename QueryGraph::VertexConstPtr,
                           typename TargetGraph::VertexConstPtr>(),
          _vf2::LabelEqual<typename QueryGraph::EdgeConstPtr,
                           typename TargetGraph::EdgeConstPtr>(),
          std::bind(_vf2::MatchCallbackSaveResult<
                        typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr,
                        std::vector<std::map<typename QueryGraph::VertexConstPtr,
                                             typename TargetGraph::VertexConstPtr>>>,
                    std::placeholders::_1, &max_result, &match_result1));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << end - start << " ms" << std::endl;

    std::cout << "    NonRecursive: ";
    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = _vf2::VF2_NonRecursive<MatchSemantics::kIsomorphism>(
          query, target,
          _vf2::LabelEqual<typename QueryGraph::VertexConstPtr,
                           typename TargetGraph::VertexConstPtr>(),
          _vf2::LabelEqual<typename QueryGraph::EdgeConstPtr,
                           typename TargetGraph::EdgeConstPtr>(),
          std::bind(_vf2::MatchCallbackSaveResult<
                        typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr,
                        std::vector<std::map<typename QueryGraph::VertexConstPtr,
                                             typename TargetGraph::VertexConstPtr>>>,
                    std::placeholders::_1, &max_result, &match_result1));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int count = VF2<MatchSemantics::kIsomorphism>(query, target, 20);

      ASSERT_EQ(20, count);
    }
    end = GetTime();
    std::cout << "           Limit: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int count =
          VF2<MatchSemantics::kIsomorphism>(query, target, -1, match_result2);

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "   Vector Result: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = VF2<MatchSemantics::kIsomorphism>(
          query, target,
          std::bind(_vf2::MatchCallbackSaveResult1<
                        typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr,
                        std::vector<std::map<typename QueryGraph::VertexConstPtr,
                                             typename TargetGraph::VertexConstPtr>>>,
                    std::placeholders::_1, &max_result, &match_result1));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "Custom Callback1: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = VF2<MatchSemantics::kIsomorphism>(
          query, target,
          std::bind(
              _vf2::MatchCallbackSaveResult2<
                  typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr,
                  std::vector<
                      std::vector<std::pair<typename QueryGraph::VertexConstPtr,
                                            typename TargetGraph::VertexConstPtr>>>>,
              std::placeholders::_1, &max_result, &match_result2));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "Custom Callback2: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < times_inner; i++) {
      int count = VF2<MatchSemantics::kIsomorphism>(
          query, target,
          LabelEqual2<typename QueryGraph::VertexConstPtr, typename TargetGraph::VertexConstPtr>,
          LabelEqual2<typename QueryGraph::EdgeConstPtr, typename TargetGraph::EdgeConstPtr>, -1,
          match_result1);

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "  Custom Compare: " << end - start << " ms" << std::endl;

    std::cout << std::endl;
  }
}

TEST(TestGUNDAM, VF2_Speed_1) {
  using namespace GUNDAM;

  //using QueryGraph1 =
  //    Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint32_t>,
  //          SetEdgeIDType<uint32_t>, SetEdgeLabelType<uint32_t>,
  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //          SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

  //using TargetGraph1 =
  //    Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<uint32_t>,
  //          SetEdgeIDType<uint64_t>, SetEdgeLabelType<uint32_t>,
  //          SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
  //          SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

  using QLG = LargeGraph<uint32_t, uint32_t, std::string, uint32_t,
                                 uint32_t, std::string>; 

  using QSG = SmallGraph<uint32_t, uint32_t, uint32_t, uint32_t>;

  using TLG = LargeGraph<uint64_t, uint32_t, std::string, uint64_t,
                                  uint32_t, std::string>;
  
  TestVF2Speed1<QLG, TLG>(1, 10000);
  TestVF2Speed1<QSG, TLG>(1, 10000);  
}