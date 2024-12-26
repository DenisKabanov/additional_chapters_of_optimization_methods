#include <iostream>
#include <vector>
#include <stack>
#include <queue>
#include <numeric>

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::stack;
using std::queue;

class ImplicationGraph {
    public:
        int num_literals; // число уникальных литералов (без учёта отрицаний)
        int num_vertices; // количество вершин
        // vector<vector<int>> adj; // матрица смежности
        // vector<vector<int>> adj_T; // транспонированная матрица смежности
        vector<vector<int>> adj_list;
        vector<vector<int>> adj_list_T;


        // Конструктор графа
        explicit ImplicationGraph(int V) {
            num_literals = V;
            num_vertices = V * 2; // число вершин умножается на 2 из-за необходимости держать дополнительные литералы "отрицания" (их номер в матрице смежности определяется как i + num_literals)
            // adj.resize(num_vertices, vector<int>(num_vertices, 0)); // инициализация матрицы смежности
            // adj_T.resize(num_vertices, vector<int>(num_vertices, 0)); // инициализация транспонированной матрицы смежности

            adj_list.resize(num_vertices, vector<int>(0)); // инициализация списка смежности
            adj_list_T.resize(num_vertices, vector<int>(0)); // инициализация транспонированного списка смежности
        }


        void add_edge(int v_1, int v_2) {
            // adj[v_1][v_2] = 1; // добавление ребра от v_1 до v_2
            // adj_T[v_2][v_1] = 1; // добавление транспонированного ребра от v_1 до v_2

            adj_list[v_1].emplace_back(v_2);
            adj_list_T[v_2].emplace_back(v_1);
        }


        void DFS(int v, vector<bool>& visited, stack<int>& Stack) {
            visited[v] = true; // ставим флаг, что вершина v посещена (но ещё не рассмотрены все соседи)
            for (auto v_next : adj_list[v]) {
                if (!visited[v_next]) { // если вершина ещё не посещена
                    DFS(v_next, visited, Stack);
                }
            }

            Stack.push(v); // Помещаем вершину в стек после завершения
        }


        void DFS_backward(int v, vector<int>& scc, int scc_number) {
            scc[v] = scc_number; // присваиваем вершине определённый номер компоненты связности
            for (auto v_next : adj_list_T[v]) {
                if (!scc[v_next]) { // если соседняя вершина ещё не посещена (не присвоен номер компоненты связности, отличный от нуля)
                    DFS_backward(v_next, scc, scc_number);
                }
            }
        }


        void solve_2SAT() {
            // первый проход: заполняем стек с порядком завершения
            vector<int> dummy_neighbor(num_vertices);
            std::iota(dummy_neighbor.begin(), dummy_neighbor.end(), 0); // связываем dummy вершину со всеми вершинами графа
            vector<bool> visited(num_vertices, false); // вектор для отслеживания посещённых вершин (0 — вершина не посещена, 1 — посещена, но не рассмотрены всесоседи, 2 — посещена и рассмотрены все соседи)
            stack<int> topological_order;
            for (auto i : dummy_neighbor) { // идём по соседям dummy вершины
                if (!visited[i]) { // если вершина ещё не посещена
                    DFS(i, visited, topological_order);
                }
            }

            // второй проход: обходим транспонированный граф
            vector<int> scc(num_vertices, 0); // вектор, говорящий к какой сильной компоненте связности относится вершина i
            int scc_counter = 1; // счётчик для компонент связности
            while (!topological_order.empty()) {
                int v = topological_order.top();
                // cout << "Cheking vertex: " << v << endl;
                topological_order.pop(); // удаляем рассматриваемую вершину из стека
                if (!scc[v]) {
                    DFS_backward(v, scc, scc_counter);
                    scc_counter++;
                }
            }


            // проверка, что литералы и их отрицания лежат в разных компонентах связности
            bool good_solution = true; // флаг, решаема ли 2-SAT задача
            for (int i = 0; i < num_literals; ++i) {
                if (scc[i] == scc[i + num_literals]) { // если лежат в одной компоненте связности ==> 2-SAT не выполним
                    good_solution = false;
                    break;
                }
            }


            // проверка, что нет пути от от первого литерала до его отрицания (по условию задачи первый литерал должен быть 1, следовательно его отрицание не может идти после него в 2-SAT)
            int v_start = 0;
            int v_finish = v_start + num_literals;
            std::fill(visited.begin(), visited.end(), 0); // обновляем вектор отслеживания посещённых вершин
            queue<int> Queue; // Очередь для BFS
            Queue.push(v_start); // начинаем с стартовой вершины
            visited[v_start] = true;

            while (!Queue.empty()) {
                int current = Queue.front();
                Queue.pop();

                if (current == v_finish) { // если есть путь от проверяемого литерала до его отрицания
                    good_solution = false;
                    break;
                }

                for (int neighbor : adj_list[current]) {
                    if (!visited[neighbor]) { // если соседняя вершина не посещена
                        visited[neighbor] = true; // отмечаем её как посещённую
                        Queue.push(neighbor); // добавляем в очередь
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
    // считываем входные данные и заполняем граф импликации
    int count_participants, count_judges;
    cin >> count_participants >> count_judges;

    ImplicationGraph graph(count_participants);
    for (int i = 0; i < count_judges; ++i) {
        int v_1, v_2, not_v_1, not_v_2;
        cin >> v_1 >> v_2;

        // литералы "отрицания" идут после всех обычных литералов, то есть их номер определяется как i + count_participants, где count_participants ни что иное, как количество обычных литералов (участников ~ вершин графа без "отрицательных" литералов)
        if (v_1 < 0) {
            not_v_1 = std::abs(v_1);
            v_1 = std::abs(v_1) + count_participants;
        } else {
            not_v_1 = v_1 + count_participants;
        }

        if (v_2 < 0) {
            not_v_2 = std::abs(v_2);
            v_2 = std::abs(v_2) + count_participants;
        } else {
            not_v_2 = v_2 + count_participants;
        }
        graph.add_edge(not_v_1 - 1, v_2 - 1);
        graph.add_edge(not_v_2 - 1, v_1 - 1);
    }

    graph.solve_2SAT();

    return 0;
}
