#include <ctime>
#include <iostream>
#include <string>

#include "gundam/large_graph.h"
#include "gundam/geneator.h"
#include "gundam/util.h"

#include "gtest/gtest.h"

TEST(TestGUNDAM, IDGEN) {
  using namespace GUNDAM;

  using G = GUNDAM::LargeGraph<uint64_t, uint32_t, std::string, uint64_t,
                               uint32_t, std::string>;
  G g;
  time(0);
  SimpleArithmeticIDGenerator<typename G::EdgeType::IDType> edge_id_gen;
  for (int i = 1; i <= 10; i++) g.AddVertex(i, 0);
  for (int i = 1; i <= 10; i++) {
    for (int j = i + 1; j <= 10; j++) {
      int id = rand() % 10000000;
      g.AddEdge(i, j, 0, id);
      edge_id_gen.UseID(id);
    }
  }
  bool res = g.AddEdge(1, 1, 1, edge_id_gen.GetID()).second;
  ASSERT_EQ(res, 1);
}