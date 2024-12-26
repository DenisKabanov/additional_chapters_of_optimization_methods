#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

struct Item {
    int64_t weight;
    int64_t price;
    int64_t count;
};

int main() {
    std::ios_base::sync_with_stdio(false); // отключение режима синхронизации stdio (использование printf и cout одновременно) для увеличения производительности cout и cin в несколько раз
    std::cin.tie(nullptr); // отвязывание буфера cin от cout (позволяет не делать вывод при запросе пользовательских данных ~ ускоряет работу ввода/вывода, но требует ручной очистки буфера cout перед вызовом cin, например с помощью endl)

    int64_t items_count; // размер последовательности (число предметов для рюкзака)
    int64_t knapsack_max_weight; // размер (вместимость) рюкзака
    std::cin >> items_count >> knapsack_max_weight; // считываем данные из входного потока и записываем в переменные
    std::vector<Item> items(items_count); // создание вектора заданной длины под веса предметов
    for (int64_t i = 0; i < items_count; ++i) { // идём по размеру созданного вектора (числу предметов)
        std::cin >> items[i].weight >> items[i].price >> items[i].count; // считываем данные из входного потока и записываем в вектор на позицию i
    }

    // пересобираем возможные предметы
    std::vector<Item> expanded_items;
    for (const Item& item : items) {
        int64_t count = item.count;
        int64_t k = 1;

        while (count > 0) {
            int64_t take = std::min(k, count);
            int64_t weight_ = item.weight * take;
            int64_t price_ = item.price * take;

            expanded_items.emplace_back(Item{weight_, price_, 1});
            count -= take;
            k *= 2;
        }
    }

    // динамическим программированием ищем максимальную стоимость рюкзака
    std::vector<int64_t> DP(knapsack_max_weight + 1);
    for (const Item& item : expanded_items) {
        for (int64_t weight = knapsack_max_weight; weight >= item.weight; --weight) {
            DP[weight] = std::max(DP[weight], DP[weight - item.weight] + item.price);
        }
    }
    std::cout << DP[knapsack_max_weight];

    return 0;
}
