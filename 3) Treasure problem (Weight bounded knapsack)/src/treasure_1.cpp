// задуска хорошая, но только если у нас нет нижнего ограничения на L, так как при умном обходе в generate_solutions алгоритм удаляет 
// явно не оптимальные решения, но которые при данном условии могут всё же понадобиться

#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>

using std::cin;
using std::cout;
using std::endl;
using std::vector;

struct Item {
    int64_t weight; // вес предмета
    int64_t price; // стоимость предмета
    int pos; // каким по счёту был предмет при считывании данных
};


class Solution {
    public:
        int64_t weight; // вес предметов в решении
        int64_t price; // стоимость предметов в решении
        vector<int> items_ids; // id предметов в решении

        // Конструктор класса
        Solution(int64_t weight_, int64_t price_, vector<int>& items_ids_) {
            weight = weight_;
            price = price_;
            items_ids = items_ids_;
        }
};


class BestSolution { // структура для хранения данных о лучшем решении
    public:
        int64_t weight = 0; // вес предметов в лучшем решении
        int64_t price = 0; // стоимость предметов в лучшем решении
        vector<int> items_ids; // id предметов лучшего решения

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
};


void generate_solutions(const vector<Item>& data, vector<Solution>& solutions, int64_t bound) {
    int items_count = data.size(); // число предметов
    // int total_combinations = std::pow(2, items_count); // общее количество комбинаций (2^число_предметов)
    int total_combinations = 1 << items_count; // общее количество комбинаций — это 2^n

    int64_t solution_weight; // вес решения
    int64_t solution_price; // стоимость решения
    vector<int> solution_items; // вектор предметов для каждого решения

    int64_t solution_prev_weight = -1; // вес предыдущего решения
    int64_t solution_prev_price = -1; // стоимость предыдущего решения

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
        // cout << "prev weight: " << solution_prev_weight << ", prev price: " << solution_prev_price << endl;

        if (solution_weight > bound) { // если вес решения превысил максимальную границу
            continue; // переходим к следующему решению
        }

        if ((solution_prev_weight < solution_weight) && (solution_prev_price < solution_price)) { // если новое тяжелее и дороже предыдущего
            solutions.emplace_back(Solution(solution_weight, solution_price, solution_items)); // добавляем найденное решение в конец вектора с решениями
        } else if ((solution_prev_weight <= solution_weight) && (solution_prev_price >= solution_price)) { // если предыдущее решение по всему лучше (или равно) текущего
            continue; // текущее отбрасываем (переходи на следующую итерацию цикла)
        } else if ((solution_prev_weight >= solution_weight) && (solution_prev_price <= solution_price)) { // если предыдущее решение тяжелее или столько же весит, но при этом стоит меньше
            while (solution_prev_weight >= solution_weight) { // удаляем предыдущие решения, если они не легче текущего, а стоимость их меньше
                solutions.pop_back(); // удаляем последнее решение
                if (solutions.size() > 0) { // усли есть ещё решения — сравниваем их
                    solution_prev_weight = solutions.back().weight; // обновляем вес последнего решения
                    solution_prev_price = solutions.back().price; // обновляем стоимость последнего решения
                } else { // если таким образом удалились все решения
                    solution_prev_weight = -1; // обновляем вес последнего решения
                    solution_prev_price = -1; // обновляем стоимость последнего решения
                }
            }
            solutions.emplace_back(Solution(solution_weight, solution_price, solution_items)); // добавляем найденное решение в конец вектора с решениями
        } else if ((solution_prev_weight > solution_weight) && (solution_prev_price > solution_price)) { // если предыдущее решение тяжелее и при этом стоит больше — нужно впихнуть текущее на определённую позицию
            int position = std::max(static_cast<int>(solutions.size()) - 2, 0); // с какой позиции начинать проверку на помещение решения
            bool position_not_found = true; // флаг, что нужно искать позицию дальше

            while ((position >= 0) && position_not_found) { // пока не нашли место куда поставить решение
                int64_t solution_pos_weight = solutions[position].weight; // вес решения на рассматриваемой позиции
                int64_t solution_pos_price = solutions[position].price; // стоимость решения на рассматриваемой позиции

                if ((solution_pos_weight <= solution_weight) && (solution_pos_price >= solution_price)) { // если решение на рассматриваемой позиции оказалось лучше рассматриваемого
                    position = -1; // ставим флаги, чтобы такое решение не добавлялось
                    position_not_found = false; // ставим флаги, чтобы такое решение не добавлялось
                } else if ((solution_pos_weight < solution_weight) && (solution_pos_price < solution_price)) { // если решение на рассматриваемой позиции и по весу и по цене меньше
                    position_not_found = false; // ставим флаг, что нашли нужную позицию для вставки
                } else if ((solution_pos_weight >= solution_weight) && (solution_pos_price <= solution_price)) { // если решение на рассматриваемой позиции по весу тяжелее или такое же и стоит меньше
                    solutions.erase(solutions.begin() + position); // удаляем данный элемент
                    position--; // переходим на следующую позицию
                } else if ((solution_pos_weight > solution_weight) && (solution_pos_price > solution_price)) { // если решение на рассматриваемой позиции все ещё с большим весом и стоимостью, чем наше
                    position--;
                }
            }

            if ((position < 0) && position_not_found) { // если все решения и тяжелее и дороже, либо мы их удалили
                solutions.insert(solutions.begin(), Solution(solution_weight, solution_price, solution_items)); // вставляем решение на нулевую позицию
            } else if (position >= 0) { // усли нашли позицию для вставки
                solutions.insert(solutions.begin() + position, Solution(solution_weight, solution_price, solution_items)); // вставляем решение
            }
        }
        
        // обновляем предыдущее решение для следующей итерации
        solution_prev_weight = solutions.back().weight;
        solution_prev_price = solutions.back().price;
        // if (solutions.size() > 0) {
        //     solution_prev_weight = solutions.back().weight;
        //     solution_prev_price = solutions.back().price;
        // } else {
        //     solution_prev_weight = 0;
        //     solution_prev_price = 0;
        // }
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
    BestSolution best_sol; // объект, хранящий лучшее решение
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
    int middle = count_items / 2; // индекс среднего элемента (идёт в правую часть)
    vector<Item> items_left(items.begin(), items.begin() + middle);
    vector<Item> items_right(items.begin() + middle, items.end());
    print_items_vector(items);
    cout << "left: " << endl;
    print_items_vector(items_left);
    cout << "right: ";
    print_items_vector(items_right);

    // считаем возможные решения на половинах
    vector<Solution> solutions_left, solutions_right;
    generate_solutions(items_left, solutions_left, right_bound);
    generate_solutions(items_right, solutions_right, right_bound);
    cout << "solutions left: " << solutions_left.size() << endl;
    print_solutions_vector(solutions_left);
    cout << "solutions right: " << solutions_right.size() << endl;
    print_solutions_vector(solutions_right);


    // комбинируем решения из обеих половин
    int solution_left_id = 0; // id для итерации по левым решениям (умно подгоняется из-за отсортированных левого и правого массива решений по весу и стоимости)
    for (Solution solution_right : solutions_right) { // идём по решениям второй половины
        // ищем оптимальное решение из левой части
        if (solution_right.weight + solutions_left[solution_left_id].weight <= right_bound) { // если текущая комбинация левого и правого решения допустима
            while (solution_left_id < solutions_left.size() - 1) { // пока не проверены все левые решения или мы не вышли (-1 из-за нумерации с нуля)
                if (solution_right.weight + solutions_left[solution_left_id + 1].weight <= right_bound) { // если правое решение улучшается левым
                    solution_left_id++; // переходим к следующему решению в левом массиве (они отсортированы от меньшего к большему, поэтому итерируемся, пока не найдём оптимальный для решения из правого массива)
                } else {
                    break;
                }
            }
        } else { // если комбинации недопустимы - ищем допустимую (всегда есть решение с весом равным нулю)
            while (solution_right.weight + solutions_left[solution_left_id].weight > right_bound) { // пока не выполнится ограничение по весу комбинации решений
                solution_left_id--;
            }
        }

        if ((solution_right.price + solutions_left[solution_left_id].price > best_sol.price) && (solution_right.weight + solutions_left[solution_left_id].weight >= left_bound)) { // если комбинация левого и правого решения оказалась лучшей и подходит и по левому ограничению
            vector<int> items_ids = solutions_left[solution_left_id].items_ids;
            items_ids.insert(items_ids.end(), solution_right.items_ids.begin(), solution_right.items_ids.end());
            best_sol.update_solution(solution_right.weight + solutions_left[solution_left_id].weight, solution_right.price + solutions_left[solution_left_id].price, items_ids); // обновляем лучшее решение
        }
    }

    best_sol.print_solution();

    return 0;
}
