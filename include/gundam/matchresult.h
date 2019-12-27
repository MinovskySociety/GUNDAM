#ifndef _MATCHRESULT_H
#define _MATCHRESULT_H
#include <ostream>
// single match_result is ptr->ptr
template <typename PatternVertexIDType, typename TargetVertexIDType,
          typename MatchResultContainer>
void MatchResultToFile(const MatchResultContainer &r, std::ostream &out) {
  size_t total_match_num = r.size();
  std::map<PatternVertexIDType, int> vertex_id_to_index;
  std::vector<PatternVertexIDType> index_to_vertex_id;
  for (const auto &single_match_result : r) {
    for (const auto &single_pair : single_match_result) {
      index_to_vertex_id.push_back(single_pair.first->id());
      vertex_id_to_index.insert(
          std::make_pair(single_pair.first->id(),
                         static_cast<int>(index_to_vertex_id.size() - 1)));
      std::cout << static_cast<int>(index_to_vertex_id.size() - 1) << " "
                << single_pair.first->id() << std::endl;
    }
    break;
  }
  out << "match_id";
  for (int i = 0; i < index_to_vertex_id.size(); i++) {
    out << "," << index_to_vertex_id[i];
  }
  out << std::endl;
  int now_match_id = 0;
  for (const auto &single_match_result : r) {
    std::vector<TargetVertexIDType> match_target_result(
        index_to_vertex_id.size());
    for (const auto &single_pair : single_match_result) {
      std::cout << vertex_id_to_index[single_pair.first->id()] << " "
                << single_pair.first->id() << " " << single_pair.second->id()
                << std::endl;
      int now_index = vertex_id_to_index[single_pair.first->id()];
      match_target_result[now_index] = single_pair.second->id();
    }
    std::cout << std::endl;
    out << (++now_match_id);
    for (int i = 0; i < match_target_result.size(); i++) {
      out << "," << match_target_result[i];
    }
    out << std::endl;
  }
}

#endif