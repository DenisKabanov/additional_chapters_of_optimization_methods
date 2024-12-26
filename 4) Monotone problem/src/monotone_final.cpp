#include <algorithm>
#include <iostream>
#include <optional>
#include <queue>
#include <tuple>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::optional;
using std::vector;

class Solver {
 private:
  int count_vectors;
  int dimension;
  vector<vector<int>> vectors_data;
  vector<std::pair<vector<int>, int>> vectors_sorted;

  bool check_order(const vector<int>& v_first,
                   const vector<int>& v_second) const {
    for (size_t i = 0; i < v_first.size(); ++i) {
      if (v_first[i] > v_second[i]) {
        return false;
      }
    }
    return true;
  }

  bool find_match(int vertex, const vector<vector<int>>& adj_list,
                  vector<bool>& visited, vector<optional<int>>& matching) {
    if (visited[vertex]) {
      return false;
    }
    visited[vertex] = true;
    for (int neighbor : adj_list[vertex]) {
      if (!matching[neighbor].has_value() ||
          find_match(matching[neighbor].value(), adj_list, visited, matching)) {
        matching[neighbor] = vertex;
        matching[vertex] = neighbor;
        return true;
      }
    }
    return false;
  }

  vector<optional<int>> find_matchings(const vector<vector<int>>& adj_list) {
    vector<optional<int>> matching(2 * count_vectors, std::nullopt);
    for (int vector_id = 0; vector_id < count_vectors; ++vector_id) {
      vector<bool> visited(count_vectors, false);
      find_match(vector_id, adj_list, visited, matching);
    }
    return matching;
  }

  vector<int> find_vertex_cover(const vector<vector<int>>& adj_list,
                                const vector<optional<int>>& matching) {
    vector<bool> visited(2 * count_vectors, false);
    std::queue<int> queue;

    for (int vector_id = 0; vector_id < count_vectors; ++vector_id) {
      if (!matching[vector_id].has_value()) {
        queue.push(vector_id);
      }
    }

    while (!queue.empty()) {
      int vector_id = queue.front();
      queue.pop();
      visited[vector_id] = true;
      for (int neighbor : adj_list[vector_id]) {
        if (!visited[neighbor]) {
          visited[neighbor] = true;
          if (matching[neighbor].has_value()) {
            queue.push(matching[neighbor].value());
          }
        }
      }
    }

    vector<int> vertex_cover;
    for (int vector_id = 0; vector_id < count_vectors; ++vector_id) {
      if (!visited[vector_id]) {
        vertex_cover.push_back(vector_id + 1);
      }
    }
    for (int neighbor = count_vectors; neighbor < 2 * count_vectors;
         ++neighbor) {
      if (visited[neighbor]) {
        vertex_cover.push_back(neighbor - count_vectors + 1);
      }
    }
    return vertex_cover;
  }

 public:
  Solver(){};

  ~Solver() {}

  void read_file() {
    cin >> count_vectors >> dimension;
    for (int i = 0; i < count_vectors; ++i) {
      vector<int> item(dimension + 1, 0);
      for (int j = 0; j <= dimension; ++j) {
        cin >> item[j];
      }
      vectors_data.push_back(item);
    }
  }

  void sort_vectors() {
    for (int i = 0; i < count_vectors; ++i) {
      vector<int> coords;
      for (int j = 0; j < dimension; ++j) {
        coords.push_back(vectors_data[i][j]);
      }
      vectors_sorted.emplace_back(coords, i);
    }
    std::sort(vectors_sorted.begin(), vectors_sorted.end());
  }

  void print_solution(vector<int>& vertex_cover) {
    cout << vertex_cover.size() << endl;
    for (int v : vertex_cover) {
      cout << v << " ";
    }
    cout << endl;
  }

  void solve_problem() {
    vector<vector<int>> adj_list(count_vectors);
    for (int i = 0; i < count_vectors; ++i) {
      for (int j = i + 1; j < count_vectors; ++j) {
        if (check_order(vectors_sorted[i].first, vectors_sorted[j].first) &&
            vectors_data[vectors_sorted[i].second][dimension] >
                vectors_data[vectors_sorted[j].second][dimension]) {
          adj_list[vectors_sorted[i].second].push_back(
              vectors_sorted[j].second + count_vectors);
        }
      }
    }

    vector<optional<int>> matching = find_matchings(adj_list);
    vector<int> vertex_cover = find_vertex_cover(adj_list, matching);
    print_solution(vertex_cover);
  }
};

int main() {
  Solver solver;
  solver.read_file();
  solver.sort_vectors();
  solver.solve_problem();
  return 0;
}