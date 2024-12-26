// Реализация стека с поддержкой максимального элемента
#pragma once // директива препроцессора, предназначенная для предотвращения многократного включения одного и того же заголовочного файла

#include <iostream> // подключение стандартных библиотек для работы с потоками ввода-вывода
#include <vector> // для работы с векторами
#include <random> // для генерации случайных чисел
#include <tuple> // для возвращения нескольких значений из функции

std::tuple<size_t, int, std::vector<int>, std::vector<int>, std::vector<int>> read_knapsack_input(std::istream& input_stream = std::cin) {
    size_t items_count; // размер последовательности (число предметов для рюкзака)
    int knapsack_max_weight; // размер (вместимость) рюкзака
    input_stream >> items_count >> knapsack_max_weight; // считываем данные из входного потока и записываем в переменные

    std::vector<int> weights(items_count); // создание вектора заданной длины под веса предметов
    std::vector<int> prices(items_count); // создание вектора заданной длины под стоимость предметов
    std::vector<int> counts(items_count); // создание вектора заданной длины под доступное количество предметов
    for (size_t i = 0; i < items_count; ++i) { // идём по размеру созданного вектора (числу предметов)
        input_stream >> weights[i] >> prices[i] >> counts[i]; // считываем данные из входного потока и записываем в вектор на позицию i
    }
    return {items_count, knapsack_max_weight, weights, prices, counts};
}

void print_vector(const std::vector<int> &vector) {
    for (auto element: vector) // идём по элементам вектора
        std::cout << element << ' ';
    std::cout << std::endl;
}


// std::vector<int> ReadNumbers(std::istream& input_stream = std::cin) {
//     size_t sequence_length; // размер последовательности
//     input_stream >> sequence_length; // считываем данные из входного потока и записываем в переменную
//     std::vector<int> numbers(sequence_length); // создание вектора заданной длины
//     for (size_t i = 0; i < numbers.size(); ++i) { // идём по размеру созданного вектора
//         input_stream >> numbers[i]; // считываем данные из входного потока и записываем в вектор на позицию i
//     }
//     return numbers;
// }

// std::vector<std::string> Input() {
//     size_t rows;
//     std::cin >> rows;
//     std::vector<std::string> table;
//     table.reserve(rows);
//     for (size_t row = 0; row < rows; ++row) {
//         std::string line;
//         std::cin >> line;
//         table.push_back(line);
//     }
//     return table;
// }

// std::vector<int> Unique(std::vector<int> numbers) {
//     // here we sort a copy of given numbers,
//     // so that the user does not lose his data
//     std::sort(numbers.begin(), numbers.end());
//     numbers.erase(
//         std::unique(numbers.begin(), numbers.end()),
//         numbers.end());
//     return numbers;
// }