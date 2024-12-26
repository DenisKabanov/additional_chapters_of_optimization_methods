// На первой строке натуральное число n (1 ≤ n ≤ 300) — количество групп вещей и натуральное число s (s ≤ 105) — размер рюкзака. 
// Следующие n строк содержат тройки натуральных чисел w_i, c_i, k_i, что означает, что в очередную группу входят k_i одинаковых вещей, каждая веса w_i и стоимости c_i. При этом 1 ≤ w_i, c_i, k_i ≤ 10^9.

#include <iostream> // подключение стандартных библиотек для работы с потоками ввода-вывода
#include "../include/utils.hpp" // функции-утилиты
#include "../include/max_plus_conv.hpp" // (max, +) свёртка

using std::vector;
using std::cout;
using std::endl;

int main() {
    std::ios_base::sync_with_stdio(false); // отключение режима синхронизации stdio (использование printf и cout одновременно) для увеличения производительности cout и cin в несколько раз
    std::cin.tie(nullptr); // отвязывание буфера cin от cout (позволяет не делать вывод при запросе пользовательских данных ~ ускоряет работу ввода/вывода, но требует ручной очистки буфера cout перед вызовом cin, например с помощью endl)

    auto [items_count, knapsack_max_weight, weights, prices, counts] = read_knapsack_input();
    int DP[items_count][knapsack_max_weight+1] = {0}; // создание двумерного массива для динамического программирования (knapsack_max_weight+1, чтобы включить и максимальный вес)

    // int items_count = 2;
    // int knapsack_max_weight = 13;
    // std::vector<int> weights = {0, 3, 0};
    // std::vector<int> prices = {0, 4, 0};
    // std::vector<int> counts = {0, 2, 0};
    // int DP[items_count][knapsack_max_weight+1] = {0}; // создание двумерного массива для динамического программирования (knapsack_max_weight+1, чтобы включить и максимальный вес)


    // DP[0][0] = 0;
    // DP[0][1] = 0;
    // DP[0][2] = 1;
    // DP[0][3] = 1;
    // DP[0][4] = 1;
    // DP[0][5] = 2;
    // DP[0][6] = 2;
    // DP[0][7] = 4;
    // DP[0][8] = 4;
    // DP[0][9] = 4;
    // DP[0][10] = 7;
    // DP[0][11] = 7;
    // DP[0][12] = 9;
    // DP[0][13] = 9;

    // заполняем первую строку матрицы динамического программирования
    for (int j = 0; j < knapsack_max_weight + 1; ++j) {
        int weight_i = weights[0]; // вес 1-го предмета
        int price_i = prices[0]; // стоимость 1-го предмета
        int max_count_i = std::min(static_cast<int>(knapsack_max_weight / weight_i), counts[0]); // сколько максимум можно взять 1-ых предметов при лимите на вместимость рюкзака

        for (int k = 0; k <= max_count_i; ++k)
            if (weight_i * k <= j)
                DP[0][j] = price_i * k;
    }

    // заполняем оставшуюся часть матрицы динамического программирования
    for (int i = 1; i < items_count; ++i) { // идём по количеству предметов, начиная с 1, так как нулевой уже заполнен
        int weight_i = weights[i]; // вес i-го предмета
        int price_i = prices[i]; // стоимость i-го предмета
        int max_count_i = std::min(static_cast<int>(knapsack_max_weight / weight_i), counts[i]); // сколько максимум можно взять i-ых предметов при лимите на вместимость рюкзака

        std::vector<int> r_division(weight_i); // вектор возможных остатков r при делении W на weights[i], то есть W = weights[i] * count_i + r
        std::iota(r_division.begin(), r_division.end(), 0); // заполняем возможными остатками, начиная с нуля и до веса предмета - 1
        
        for (int r: r_division) { // идём по значениям остатка
            std::vector<int> counts_i(static_cast<int>((knapsack_max_weight - r) / weight_i) + 1); // вектор возможного количества взятия предмета i (без учёта ограничения, а +1 нужен из-за нумерации с нуля)
            std::iota(counts_i.begin(), counts_i.end(), 0); // заполняем вектор значениями от нуля до максимального количества с учётом фиксированного остатка
            
            std::cout << "counts_i: ";
            print_vector(counts_i);

            std::vector<int> DP_prev(counts_i.size()); // вектор с элементами предыдущего шага для свёртки на текущем шаге
            for(size_t j=0; j < counts_i.size(); ++j)
                DP_prev[j] = DP[i-1][weight_i * counts_i[j] + r]; // заполняем вектор нужными элементами с предыдущего шага

            std::cout << "DP_prev: ";
            print_vector(DP_prev);

            vector<int> conv = fast_max_plus_conv(DP_prev, price_i); // считаем свёртку для определения оптимального количества предмета i

            std::cout << "conv: ";
            print_vector(conv);

            int prev_diff = 0;
            int diff = 0;
            int count_i = 0; // сколько раз взяли i-ый предмет
            for(int j = 0; j < counts_i.size(); ++j) {
                diff = conv[j] - DP_prev[j];
                if (diff - prev_diff > 0)
                    count_i += 1;
                prev_diff = diff;

                // int count_i = static_cast<int>(std::ceil((float)(conv[j] - DP_prev[j]) / price_i));
                std::cout << "count_i: " << count_i << endl;
                // if (j <= max_count_i) {
                if (count_i <= max_count_i) { // пока не вышли за пределы возможного взятия предметов
                    DP[i][weight_i * counts_i[j] + r] = conv[j];
                } else { // если мы пытаемся взять предмет i больше раз, чем он у нас есть — пересчитываем так, если бы у нас этого предмета больше не было
                    // int steps_back = 0; // сколько можно сэкономить веса, не уменьшая стоимость
                    // while ((DP[i][weight_i * max_count_i + r - (steps_back + 1)] == conv[max_count_i]) && (steps_back < r)) // пока можем идти назад по таблице, уменьшая вес, но не стоимость (не можемо откатиться дальше остатка)
                    //     steps_back += 1;
                    // cout << "steps_back: " << steps_back << endl;

                    conv[j] = conv[j-1];
                    // DP[i][weight_i * counts_i[j] + r] = conv[j] + DP[i-1][(count_i - max_count_i) * weight_i + r];
                    // DP[i][weight_i * counts_i[j] + r] = std::max(conv[j], DP_prev[j]);

                    DP[i][weight_i * counts_i[j] + r] = std::max(DP[i][weight_i * counts_i[j] + r - 1], DP_prev[j]);
                }
            }

            std::cout << "DP: ";
            for (auto &column : DP[i]) {
                std::cout << column << " ";
            }
            std::cout << std::endl;
            cout << "=======" << endl;
        }
    }

    for (auto &row : DP) {
        for (auto &column : row) {
            std::cout << column << " ";
        }
        std::cout << std::endl;
    }

    DP[items_count-1][knapsack_max_weight] = std::max(DP[items_count-1][knapsack_max_weight], DP[items_count-1][knapsack_max_weight-1]);
    std::cout << DP[items_count-1][knapsack_max_weight]; // выводим ответ на задачу
    return 0;
}