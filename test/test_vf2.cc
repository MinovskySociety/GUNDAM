#include "gundam/vf2.h"

#include <functional>
#include <iostream>
#include <list>
#include <map>
#include <vector>

#include "gtest/gtest.h"

#ifdef WIN32
#include <windows.h>
inline uint64_t GetTime() { return GetTickCount64(); }
#else
#include <sys/time.h>
inline uint64_t GetTime() {
  timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000 + t.tv_usec / 1000;
}
#endif

template <typename Ptr1, typename Ptr2>
bool LabelEqual(const Ptr1& a, const Ptr2& b) {
  return a->label() == b->label();
}

TEST(TestGUNDAM, VF2_1) {
  using namespace GUNDAM;
  using GraphType = GUNDAM::Graph<>;
  using VertexType = typename GraphType::VertexType;
  using EdgeType = typename GraphType::EdgeType;
  using VertexIDType = typename VertexType::IDType;
  using VertexLabelType = typename VertexType::LabelType;
  using EdgeLabelType = typename EdgeType::LabelType;
  using VertexConstPtr = typename GraphType::VertexConstPtr;
  using EdgeConstPtr = typename GraphType::EdgeConstPtr;

  GraphType query, target;
  VertexIDType query_id = 1, target_id = 1;

  // query
  query.AddVertex(1, VertexLabelType(0));
  query.AddVertex(2, VertexLabelType(1));
  query.AddVertex(3, VertexLabelType(0));
  query.AddEdge(1, 2, EdgeLabelType(1), 1);
  query.AddEdge(3, 2, EdgeLabelType(1), 2);
  // query.AddEdge(3,1,EdgeLabelType(1));

  // target
  target.AddVertex(1, VertexLabelType(0));
  target.AddVertex(2, VertexLabelType(1));
  target.AddVertex(3, VertexLabelType(0));
  target.AddEdge(1, 2, EdgeLabelType(1), 1);
  target.AddEdge(3, 2, EdgeLabelType(1), 2);
  target.AddEdge(3, 1, EdgeLabelType(1), 3);

  std::vector<std::map<VertexConstPtr, VertexConstPtr>> match_result;
  int count =
      VF2<MatchSemantics::kIsomorphism>(query, target, -1, match_result);

  for (size_t i = 0; i < match_result.size(); i++) {
    std::cout << "match " << i << std::endl;
    for (const auto& mapping : match_result[i]) {
      std::cout << " " << mapping.first->id() << " " << mapping.second->id()
                << std::endl;
    }
  }
  std::cout << "count: " << match_result.size() << std::endl;
  ASSERT_EQ(count, 2);

  count = VF2<MatchSemantics::kIsomorphism>(
      query, target, LabelEqual<VertexConstPtr, VertexConstPtr>,
      LabelEqual<EdgeConstPtr, EdgeConstPtr>, -1, match_result);

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

template <typename Ptr1, typename Ptr2>
bool LabelEqualCustom(const Ptr1& a, const Ptr2& b) {
  return *(uint32_t*)&a->label() == static_cast<uint32_t>(stoi(b->label()));
}

TEST(TestGUNDAM, VF2_2) {
  using namespace GUNDAM;
  using QueryGraph =
      Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<Label<uint32_t>>,
            SetEdgeIDType<uint32_t>, SetEdgeLabelType<Label<uint32_t>>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  using TargetGraph =
      Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<std::string>,
            SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

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

  std::vector<std::map<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>>
      match_result;
  int count = VF2<MatchSemantics::kIsomorphism>(
      query, target,
      LabelEqualCustom<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>,
      LabelEqualCustom<QueryGraph::EdgeConstPtr, TargetGraph::EdgeConstPtr>, -1,
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

TEST(TestGUNDAM, VF2_3) {
  using namespace GUNDAM;
  using QueryGraph =
      Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<Label<uint32_t>>,
            SetEdgeIDType<uint32_t>, SetEdgeLabelType<Label<uint32_t>>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<true>, SetEdgeHasAttribute<true>>;

  using TargetGraph =
      Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<std::string>,
            SetEdgeIDType<uint64_t>, SetEdgeLabelType<std::string>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

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

  std::vector<std::map<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>>
      match_result;
  int count = VF2<MatchSemantics::kIsomorphism>(
      query, target,
      LabelEqualCustom<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>,
      LabelEqualCustom<QueryGraph::EdgeConstPtr, TargetGraph::EdgeConstPtr>, -1,
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

TEST(TestGUNDAM, VF2_Speed_1) {
  using namespace GUNDAM;
  using QueryGraph =
      Graph<SetVertexIDType<uint32_t>, SetVertexLabelType<uint32_t>,
            SetEdgeIDType<uint32_t>, SetEdgeLabelType<uint32_t>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

  using TargetGraph =
      Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<uint32_t>,
            SetEdgeIDType<uint64_t>, SetEdgeLabelType<uint32_t>,
            SetAllowMultipleEdge<true>, SetAllowDuplicateEdge<true>,
            SetVertexHasAttribute<false>, SetEdgeHasAttribute<false>>;

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

  std::vector<std::map<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>>
      match_result1;

  std::vector<std::vector<
      std::pair<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>>>
      match_result2;

  const int TIMES_OUTER = 10;
  const int TIMES_INNER = 10000;
  uint64_t start, end;

  for (int j = 0; j < TIMES_OUTER; j++) {
    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int count =
          VF2<MatchSemantics::kIsomorphism>(query, target, -1, match_result1);

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "         Default: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = _vf2::VF2_Recursive<MatchSemantics::kIsomorphism>(
          query, target,
          std::bind(_vf2::MatchCallbackSaveResult<
                        QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr,
                        std::vector<std::map<QueryGraph::VertexConstPtr,
                                             TargetGraph::VertexConstPtr>>>,
                    std::placeholders::_1, &max_result, &match_result1));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "       Recursive: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = _vf2::VF2_NonRecursive<MatchSemantics::kIsomorphism>(
          query, target,
          std::bind(_vf2::MatchCallbackSaveResult<
                        QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr,
                        std::vector<std::map<QueryGraph::VertexConstPtr,
                                             TargetGraph::VertexConstPtr>>>,
                    std::placeholders::_1, &max_result, &match_result1));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "    NonRecursive: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int count = VF2<MatchSemantics::kIsomorphism>(query, target, 20);

      ASSERT_EQ(20, count);
    }
    end = GetTime();
    std::cout << "           Limit: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int count =
          VF2<MatchSemantics::kIsomorphism>(query, target, -1, match_result2);

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "   Vector Result: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = VF2<MatchSemantics::kIsomorphism>(
          query, target,
          std::bind(_vf2::MatchCallbackSaveResult1<
                        QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr,
                        std::vector<std::map<QueryGraph::VertexConstPtr,
                                             TargetGraph::VertexConstPtr>>>,
                    std::placeholders::_1, &max_result, &match_result1));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "Custom Callback1: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int max_result = -1;
      match_result1.clear();

      int count = VF2<MatchSemantics::kIsomorphism>(
          query, target,
          std::bind(
              _vf2::MatchCallbackSaveResult2<
                  QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr,
                  std::vector<
                      std::vector<std::pair<QueryGraph::VertexConstPtr,
                                            TargetGraph::VertexConstPtr>>>>,
              std::placeholders::_1, &max_result, &match_result2));

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "Custom Callback2: " << end - start << " ms" << std::endl;

    start = GetTime();
    for (int i = 0; i < TIMES_INNER; i++) {
      int count = VF2<MatchSemantics::kIsomorphism>(
          query, target,
          LabelEqual<QueryGraph::VertexConstPtr, TargetGraph::VertexConstPtr>,
          LabelEqual<QueryGraph::EdgeConstPtr, TargetGraph::EdgeConstPtr>, -1,
          match_result1);

      ASSERT_EQ(40, count);
    }
    end = GetTime();
    std::cout << "  Custom Compare: " << end - start << " ms" << std::endl;

    std::cout << std::endl;
  }
}