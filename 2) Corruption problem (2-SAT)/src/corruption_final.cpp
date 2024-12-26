#include <iostream>
#include <numeric>
#include <queue>
#include <stack>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::queue;
using std::stack;
using std::vector;

class ImplicationGraph {
 public:
  int num_literals;
  int num_vertices;
  vector<vector<int>> adj_list;
  vector<vector<int>> adj_list_T;

  explicit ImplicationGraph(int Value) {
    num_literals = Value;
    num_vertices = Value * 2;
    adj_list.resize(num_vertices, vector<int>(0));
    adj_list_T.resize(num_vertices, vector<int>(0));
  }

  void add_edge(int vertex_f, int vertex_s) {
    adj_list[vertex_f].emplace_back(vertex_s);
    adj_list_T[vertex_s].emplace_back(vertex_f);
  }

  void DFS(int vertex, vector<bool>& visited, stack<int>& Stack) {
    visited[vertex] = true;
    for (auto v_next : adj_list[vertex]) {
      if (!visited[v_next]) {
        DFS(v_next, visited, Stack);
      }
    }

    Stack.push(vertex);
  }

  void DFS_backward(int vertex, vector<int>& scc, int scc_number) {
    scc[vertex] = scc_number;
    for (auto v_next : adj_list_T[vertex]) {
      if (!scc[v_next]) {
        DFS_backward(v_next, scc, scc_number);
      }
    }
  }

  void find_scc() {
    vector<int> dummy_neighbor(num_vertices);
    std::iota(dummy_neighbor.begin(), dummy_neighbor.end(), 0);

    vector<bool> visited(num_vertices, false);
    stack<int> topological_order;
    for (auto i : dummy_neighbor) {
      if (!visited[i]) {
        DFS(i, visited, topological_order);
      }
    }

    vector<int> scc(num_vertices, 0);
    int scc_counter = 1;
    while (!topological_order.empty()) {
      int vertex = topological_order.top();

      topological_order.pop();
      if (!scc[vertex]) {
        DFS_backward(vertex, scc, scc_counter);
        scc_counter++;
      }
    }

    bool good_solution = true;
    for (int i = 0; i < num_literals; ++i) {
      if (scc[i] == scc[i + num_literals]) {
        good_solution = false;
        break;
      }
    }

    int v_start = 0;
    int v_finish = v_start + num_literals;
    std::fill(visited.begin(), visited.end(), 0);
    queue<int> Queue;
    Queue.push(v_start);
    visited[v_start] = true;

    while (!Queue.empty()) {
      int current = Queue.front();
      Queue.pop();

      if (current == v_finish) {
        good_solution = false;
        break;
      }

      for (int neighbor : adj_list[current]) {
        if (!visited[neighbor]) {
          visited[neighbor] = true;
          Queue.push(neighbor);
        }
      }
    }

    if (good_solution) {
      cout << "yes" << endl;
    } else {
      cout << "no" << endl;
    }
  }
};

int main() {
  int count_participants, count_judges;
  cin >> count_participants >> count_judges;

  ImplicationGraph graph(count_participants);
  for (int i_ = 0; i_ < count_judges; ++i_) {
    int v_f, v_s, not_v_f, not_v_s;
    cin >> v_f >> v_s;

    if (v_f < 0) {
      not_v_f = std::abs(v_f);
      v_f = std::abs(v_f) + count_participants;
    } else {
      not_v_f = v_f + count_participants;
    }

    if (v_s < 0) {
      not_v_s = std::abs(v_s);
      v_s = std::abs(v_s) + count_participants;
    } else {
      not_v_s = v_s + count_participants;
    }
    graph.add_edge(not_v_f - 1, v_s - 1);
    graph.add_edge(not_v_s - 1, v_f - 1);
  }

  graph.find_scc();

  return 0;
}
