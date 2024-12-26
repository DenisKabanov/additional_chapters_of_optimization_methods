// вариант без reverse — стал чуть быстрее (на 1 ms), но всё равно не проходит по time limit

#include <iostream>
#include <stack>
#include <vector>
#include <cassert> // для отлова логических багов
#include <algorithm>

using std::cin;
using std::cout;
using std::endl;
using std::vector;


struct Item {
    int64_t weight; // вес предмета
    int64_t price; // стоимость предмета
    int pos; // каким по счёту был предмет при считывании данных
};


class Solution { // класс для хранения информации о каждом решении
    public:
        int64_t weight = 0; // вес предметов в решении
        int64_t price = 0; // стоимость предметов в решении
        vector<int> items_ids; // id предметов в решении

        // Конструктор класса
        Solution() {
            weight = 0;
            price = 0;
            items_ids = vector<int>();
        }

        // Конструктор класса
        Solution(int64_t weight_, int64_t price_, vector<int>& items_ids_) {
            weight = weight_;
            price = price_;
            items_ids = items_ids_;
        }

        void update_solution(int64_t weight_new, int64_t price_new, vector<int>& items_ids_new) {
            weight = weight_new;
            price = price_new;
            items_ids = items_ids_new;
        }

        void print_solution() {
            if (items_ids.size() == 0) {
                cout << 0;
            } else {
                cout << items_ids.size() << endl;
                sort(items_ids.begin(), items_ids.end()); // сортируем id-шники от наименьшего к наибольшему
                for (int id : items_ids) cout << id << " "; // выводим id-шники
            }
            cout << endl;
        }

        friend bool operator== (const Solution& lhs, const Solution& rhs)
        {
            return (lhs.price == rhs.price) && (lhs.weight == rhs.weight) && (lhs.items_ids == rhs.items_ids);
        }
};


void generate_solutions(const vector<Item>& data, vector<Solution>& solutions, int64_t bound) {
    int items_count = data.size(); // число предметов
    // int total_combinations = std::pow(2, items_count); // общее количество комбинаций (2^число_предметов)
    int total_combinations = 1 << items_count; // общее количество комбинаций — это 2^n

    int64_t solution_weight; // вес решения
    int64_t solution_price; // стоимость решения
    vector<int> solution_items; // вектор предметов для каждого решения

    for (int mask = 0; mask < total_combinations; ++mask) {
        solution_weight = 0; // зануляем изначальный вес для каждого решения
        solution_price = 0; // зануляем изначальную стоимость для каждого решения
        solution_items.clear(); // очищаем вектор id-шников
        for (int i = 0; i < items_count; ++i) {
            if (mask & (1 << i)) { // если установлен i-й бит в маске — берём i-ый предмет в рассматриваемое решение
                solution_weight += data[i].weight;
                solution_price += data[i].price;
                solution_items.push_back(data[i].pos);
            }
        }

        // cout << "sol weight: " << solution_weight << ", sol price: " << solution_price << endl;

        if (solution_weight > bound) { // если вес решения превысил максимальную границу
            continue; // переходим к следующему решению
        }

        int position = static_cast<int>(solutions.size()) - 1; // с какой позиции начинать проверку на помещение решения
        if (position < 1) { // если ещё не сформировалось начало вектора решений
            solutions.emplace_back(Solution(solution_weight, solution_price, solution_items)); // просто добавляем вектор в решения (под позицией 0 идёт решение без каких-либо предметов, под 1 — первое встреченное решение)
            continue; // переходим к следующему решению
        }

        while (position > 0) { // пока не прошли по доступным для изменения позициям решений
            if (solutions[position].weight < solution_weight) { // если новое решение тяжелее того, что рассматривается на позиции
                solutions.emplace_back(Solution(solution_weight, solution_price, solution_items)); // добавляем найденное решение в конец вектора с решениями
                break;
            } else if (solutions[position].weight == solution_weight) { // если вес полученного решения равен весу решения на позиции
                if (solutions[position].price >= solution_price) { // и при этом уступает в стоимости решению, схожему по весу
                    break; // не добавляем данный элемент
                } else { // если же стоимость нового решения с таким весом выше
                    solutions[position].update_solution(solution_weight, solution_price, solution_items); // перезаписываем им решение на этой позиции
                    break;
                }
            } else if (solutions[position - 1].weight < solution_weight) { // если вес текущего решения меньше решения на позиции, но больше чем предшественника данной позиции
                solutions.insert(solutions.begin() + position, Solution(solution_weight, solution_price, solution_items)); // вставляем решение на текущую позицию, сдвигая вперёд решение на данной позиции
                break;
            } else {
                position--; // переходим к рассмотрению более ранней позиции
            }
        }
    }
}


template <typename T>
void print_items_vector(vector<T>& vector) {
    for (auto item : vector) {
        cout << "weight: " << item.weight << ", price: " << item.price << ", pos: " << item.pos << " " << endl;
    }
}

void print_solutions_vector(vector<Solution>& vector) {
    cout << "================" << endl;
    for (auto item : vector) {
        cout << "solution weight: " << item.weight << ", solution price: "  << item.price << " " << endl;
        cout << "Items: ";
        for (auto id : item.items_ids) {
            cout << id << " ";
        }
        cout << endl << "-----------------" << endl;
    }
}


class MaxStack {
    private:
        std::stack<Solution> stack_data; // обычный стек
        std::stack<Solution> stack_max; // стек, хранящий максимальные значения


    public:
        // Конструктор класса.
        MaxStack() {}

        // Деструктор класса.
        ~MaxStack() {}

        // Метод для добавления элемента наверх стека.
        void push(Solution value) {
            stack_data.push(value); // добавляем значение в стек
            if (stack_max.empty() || value.price >= stack_max.top().price) { // если stack_max пуст или новое значение больше или равно текущему максимуму, добавляем его в stack_max
                stack_max.push(value); // добавляем значение в стек максимумов
            }
        }

        // Метод для удаления элемента с верхушки стека.
        void pop() {
            assert(!stack_data.empty()); // если стек с данными пуст — выкидываем ошибку

            if (stack_data.top() == stack_max.top()) { // если верхний элемент основного стека равен верхнему элементу stack_max, удаляем его из stack_max
                stack_max.pop(); // удаляем верхний элемент стека с максимумами
            }
            stack_data.pop(); // удаляем верхний элемент стека с данными
        }

        // Метод для получения верхнего элемента стека.
        Solution top() const {
            assert(!stack_data.empty()); // если стек с данными пуст — выкидываем ошибку

            return stack_data.top(); // возвращаем верхушку стека
        }

        // Метод для получения максимального элемента стека.
        Solution get_max() const {
            assert(!stack_max.empty()); // если стек с максимумами пуст — выкидываем ошибку

            return stack_max.top(); // возвращаем верхушку стека максимумов
        }

        // Метод для получения размера стека.
        size_t get_size() const {
            return stack_data.size();
        }

        // Метод для проверки пустоты стека.
        bool empty() const {
            return stack_data.empty();
        }
};


class MaxQueue {
    private:
        MaxStack stack_head; // стек головы
        MaxStack stack_tail; // стек хвоста

    public:
        // Конструктор класса.
        MaxQueue() {}

        // Деструктор класса.
        ~MaxQueue() {}

        // Метод для добавления элемента в конец очереди.
        void push(Solution value) {
            stack_tail.push(value);
        }

        // Метод для перенесения элементов из стека хвоста в стек головы (был верхним в стеке хвоста — стал нижним в стеке головы, правило FIFO соблюдается).
        void move_elements() {
            while (!stack_tail.empty()) { // пока стек хвоста не пустой — копируем элементы из него в стек головы
                stack_head.push(stack_tail.top()); // добавляем в стек головы верхний элемент стека хвоста
                stack_tail.pop(); // удаляем добавленный элемент из стека хвоста
            }
        }

        // Метод, удаляющий первый элемент очереди (FIFO).
        void pop() {
            if (stack_head.empty()) { // если стек головы пуст
                assert(!stack_tail.empty()); // если ещё и стек хвоста пуст — выкидываем ошибку

                this->move_elements(); // перемещаем элементы из хвоста в голову
            }
            stack_head.pop(); // удаляем первый элемент очереди (верхний в стеке головы)
        }

        // Метод, возвращающий первый элемент очереди (FIFO).
        Solution top() {
            if (stack_head.empty()) { // если стек головы пуст
                assert(!stack_tail.empty()); // если ещё и стек хвоста пуст — выкидываем ошибку

                this->move_elements(); // перемещаем элементы из хвоста в голову
            }
            return stack_head.top(); // возвращаем первый элемент очереди (верхний в стеке головы)
        }

        // Метод для возвращения максимального элемента очереди.
        Solution get_max() {
            if (stack_head.empty()) { // если стек головы пуст
                assert(!stack_tail.empty()); // если ещё и стек хвоста пуст — выкидываем ошибку

                this->move_elements(); // перемещаем элементы из хвоста в голову
            }

            Solution max_value_head = stack_head.get_max(); // берём максимальное значение у стека головы
            if (!stack_tail.empty()) { // если есть ещё стек хвоста
                Solution max_value_tail = stack_tail.get_max(); // находим максимальный элемент из головы и хвоста
                if (max_value_tail.price > max_value_head.price) {
                    return max_value_tail; // возвращаем максимальный элемент из стеков
                }
            }
            return max_value_head;
        }

        // Метод для получения размера очереди.
        size_t get_size() const {
            return stack_head.get_size() + stack_tail.get_size();
        }

        // Метод для проверки пустоты очереди.
        bool empty() const {
            return stack_head.empty() && stack_tail.empty(); // проверяем, пусты ли оба стека
        }
};


int main() {
    std::ios_base::sync_with_stdio(false); // отключение режима синхронизации stdio (использование printf и cout одновременно) для увеличения производительности cout и cin в несколько раз
    std::cin.tie(nullptr); // отвязывание буфера cin от cout (позволяет не делать вывод при запросе пользовательских данных ~ ускоряет работу ввода/вывода, но требует ручной очистки буфера cout перед вызовом cin, например с помощью endl)

    // считываем входные данные
    int count_items;
    int64_t left_bound, right_bound;
    cin >> count_items >> left_bound >> right_bound;
    vector<Item> items(count_items); // создание вектора под предметы
    for (int i = 0; i < count_items; ++i) {
        cin >> items[i].weight >> items[i].price;
        items[i].pos = i + 1; // запоминаем каким по счёту пришёл предмет (+1 из-за нумерации с 1)
    }


    // border-case — есть всего один предмет
    // vector<int> dummy; // фиктивный вектор для инициализации лучшего решения
    // Solution best_sol(0, 0, dummy); // объект, хранящий лучшее решение
    Solution best_sol; // объект, хранящий лучшее решение
    if (count_items == 1) {
        if ((left_bound <= items[0].weight) && (items[0].weight <= right_bound)) {
            vector<int> sol_ = {items[0].pos};
            best_sol.update_solution(items[0].weight, items[0].price, sol_);
        }
        best_sol.print_solution();

        return 0;
    }


    // сортируем входные данные по весу предметов от наименьшего к наибольшему
    std::sort(items.begin(), items.end(), [](auto &left, auto &right) {
        return left.weight < right.weight;
    });


    // делим данные на две половины
    int middle = count_items / 2; // индекс среднего элемента (middle идёт в правую часть)
    vector<Item> items_left(items.begin(), items.begin() + middle);
    vector<Item> items_right(items.begin() + middle, items.end());
    // print_items_vector(items);
    // cout << "left: " << endl;
    // print_items_vector(items_left);
    // cout << "right: ";
    // print_items_vector(items_right);

    // считаем возможные решения на половинах
    vector<Solution> solutions_left, solutions_right;
    solutions_left.reserve(1 << solutions_left.size()); // сразу резервируем память под элементы
    solutions_left.reserve(1 << solutions_right.size()); // сразу резервируем память под элементы
    generate_solutions(items_left, solutions_left, right_bound);
    generate_solutions(items_right, solutions_right, right_bound);
    // std::reverse(solutions_right.begin(), solutions_right.end()); // переворачиваем вектор вторых решений
    // cout << "solutions left: " << solutions_left.size() << endl;
    // print_solutions_vector(solutions_left);
    // cout << "solutions right: " << solutions_right.size() << endl;
    // print_solutions_vector(solutions_right);


    // комбинируем решения из обоих половин
    MaxQueue Queue; // очередь для решений из левой половины
    // int left_sol_id = 0;
    int left_sol_id = static_cast<int>(solutions_left.size()) - 1;
    int64_t consumed_weight = 0; // вес, занятый правым решением
    for (Solution& right_sol : solutions_right) {
        consumed_weight = right_sol.weight;

        while (left_sol_id > -1) { // пока не прошли по всему левому вектору решений
            if (solutions_left[left_sol_id].weight + consumed_weight >= left_bound) {
                if (solutions_left[left_sol_id].weight + consumed_weight <= right_bound) {
                    Queue.push(solutions_left[left_sol_id]); // добавляем решение в очередь
                }
                left_sol_id--; // идём к следующему левому решению 
            } else {
                break;
            }
        }

        // while ((!Queue.empty()) && (Queue.top().weight + consumed_weight < left_bound)) { // удаляем элементы из очереди, ответы с которыми не входят в область [left_bound, right_bound]
        //     Queue.pop();
        // }

        while ((!Queue.empty()) && (Queue.top().weight + consumed_weight > right_bound)) { // удаляем элементы из очереди, ответы с которыми не входят в область [left_bound, right_bound]
            Queue.pop();
        }

        Solution best_left_sol;
        if (!Queue.empty()) {
            best_left_sol = Queue.get_max(); // берём лучшее решение на промежутке
        }

        if ((best_left_sol.weight + consumed_weight >= left_bound) && (best_left_sol.weight + consumed_weight <= right_bound) && (best_left_sol.price + right_sol.price > best_sol.price)) { // если решение подходит по границам и оно лучше текущего лучшего ответа
            vector<int> items_ids = best_left_sol.items_ids;
            items_ids.insert(items_ids.end(), right_sol.items_ids.begin(), right_sol.items_ids.end());
            best_sol.update_solution(best_left_sol.weight + consumed_weight, best_left_sol.price + right_sol.price, items_ids); // обновляем лучшее решение
        }
    }
    best_sol.print_solution();

    return 0;
}
