#include <cstdint>
#include <iostream>
#include <string>

#include "gtest/gtest.h"

#include "gundam/graph.h"
#include "gundam/large_graph.h"

TEST(TestGUNDAM, GoogleTest) { ASSERT_TRUE(true); }

TEST(TestGUNDAM, LargeGraph) {
  using namespace GUNDAM;

  /*  using G0 =
        Graph<SetVertexIDType<uint64_t>, SetVertexLabelType<uint32_t>,
              SetVertexAttributeKeyType<std::string>, SetEdgeIDType<uint64_t>,
              SetEdgeLabelType<uint32_t>, SetEdgeAttributeKeyType<std::string>>;
   */

  using G1 = LargeGraph<uint64_t, uint32_t, std::string, uint64_t, uint32_t,
                        std::string>;

  G1 g;
  bool res;

  G1::VertexPtr v1;
  std::tie(v1, res) = g.AddVertex(1, 1);
  ASSERT_TRUE(res);
  ASSERT_TRUE(v1);

  ASSERT_TRUE(g.AddVertex(2, 2).second);
  ASSERT_TRUE(g.AddVertex(3, 3).second);
  ASSERT_TRUE(g.AddVertex(4, 4).second);
  ASSERT_TRUE(g.AddVertex(5, 5).second);

  G1::VertexPtr v5;
  std::tie(v5, res) = g.AddVertex(5, 100);
  ASSERT_FALSE(res);
  ASSERT_TRUE(v5);
  ASSERT_EQ(5, v5->label());

  ASSERT_TRUE(g.AddVertex(41, 1).second);
  ASSERT_TRUE(g.AddVertex(31, 1).second);
  ASSERT_TRUE(g.AddVertex(32, 2).second);
  ASSERT_TRUE(g.AddVertex(21, 1).second);
  ASSERT_TRUE(g.AddVertex(22, 2).second);
  ASSERT_TRUE(g.AddVertex(23, 3).second);
  ASSERT_TRUE(g.AddVertex(11, 1).second);
  ASSERT_TRUE(g.AddVertex(12, 2).second);
  ASSERT_TRUE(g.AddVertex(13, 3).second);
  ASSERT_TRUE(g.AddVertex(14, 4).second);

  ASSERT_EQ(15, g.CountVertex());

  auto v1a = g.FindVertex(1);
  ASSERT_EQ(v1, v1a);

  G1::VertexPtr v2 = g.FindVertex(2);
  ASSERT_TRUE(v2);
  ASSERT_EQ(2, v2->label());
  G1::VertexConstPtr v2c = g.FindConstVertex(2);
  ASSERT_TRUE(v2c);
  ASSERT_EQ(v2, v2c);

  {
    G1::VertexIterator it;
    ASSERT_TRUE(it.IsDone());

    int count = 0;
    for (it = g.VertexBegin(); !it.IsDone(); ++it) {
      std::cout << it->id() << " " << it->label() << std::endl;
      ++count;
    }
    ASSERT_EQ(count, g.CountVertex());
  }

  {
    G1::VertexConstIterator it;
    ASSERT_TRUE(it.IsDone());

    int count = 0;
    for (it = g.VertexCBegin(); !it.IsDone(); it++) {
      std::cout << it->id() << " " << it->label() << std::endl;
      ++count;
    }
    ASSERT_EQ(count, g.CountVertex());
  }

  {
    G1::VertexConstIterator it;
    ASSERT_TRUE(it.IsDone());

    int count = 0;
    for (it = g.VertexCBegin(1); !it.IsDone(); it++) {
      std::cout << it->id() << " " << it->label() << std::endl;
      ++count;
    }
    ASSERT_EQ(count, g.CountVertex(1));
  }

  {
    G1::VertexConstIterator it;
    ASSERT_TRUE(it.IsDone());

    int count = 0;
    for (it = g.VertexCBegin(2); !it.IsDone(); it++) {
      std::cout << it->id() << " " << it->label() << std::endl;
      ++count;
    }
    ASSERT_EQ(count, g.CountVertex(2));
  }

  ASSERT_TRUE(g.AddEdge(3, 13, 1, 1).second);
  ASSERT_TRUE(g.AddEdge(3, 23, 1, 2).second);
  ASSERT_TRUE(g.AddEdge(13, 3, 1, 3).second);
  ASSERT_TRUE(g.AddEdge(13, 23, 1, 4).second);
  ASSERT_TRUE(g.AddEdge(23, 3, 1, 5).second);
  ASSERT_TRUE(g.AddEdge(23, 13, 1, 6).second);

  ASSERT_TRUE(g.AddEdge(4, 14, 1, 7).second);
  ASSERT_TRUE(g.AddEdge(14, 4, 1, 8).second);

  ASSERT_TRUE(g.AddEdge(5, 5, 1, 9).second);

  ASSERT_TRUE(g.AddEdge(3, 4, 2, 10).second);
  ASSERT_TRUE(g.AddEdge(3, 14, 2, 11).second);
  ASSERT_TRUE(g.AddEdge(13, 4, 2, 12).second);
  ASSERT_TRUE(g.AddEdge(13, 14, 2, 13).second);
  ASSERT_TRUE(g.AddEdge(23, 4, 2, 14).second);
  ASSERT_TRUE(g.AddEdge(23, 14, 2, 15).second);

  {
    G1::EdgeIterator it;
    ASSERT_TRUE(it.IsDone());

    int count = 0;
    for (it = g.EdgeBegin(); !it.IsDone(); ++it) {
      std::cout << it->id() << " " << it->label() << " " << it->src_ptr()->id()
                << " " << it->dst_ptr()->id() << std::endl;
      ++count;
    }
    ASSERT_EQ(count, g.CountEdge());
  }

  {
    G1::EdgeConstIterator it;
    ASSERT_TRUE(it.IsDone());

    int count = 0;
    for (it = g.EdgeCBegin(); !it.IsDone(); ++it) {
      std::cout << it->id() << " " << it->label() << " " << it->src_ptr()->id()
                << " " << it->dst_ptr()->id() << std::endl;
      ++count;
    }
    ASSERT_EQ(count, g.CountEdge());
  }

  g.EraseEdge(5);
  g.EraseEdge(9);
  g.EraseEdge(7);

  g.EraseVertex(3);

  G1 t(g);

  g = t;
}
