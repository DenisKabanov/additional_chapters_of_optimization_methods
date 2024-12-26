// Реализация (max, +) свёртки
#pragma once // директива препроцессора, предназначенная для предотвращения многократного включения одного и того же заголовочного файла

#include <cassert> // для отлова логических багов
#include <limits> // для работы с максимальными и минимальными значениями типов
#include <vector> // для работы с векторами
#include "max_queue.hpp"

// using std::vector;

std::vector<int> fast_max_plus_conv(const std::vector<int> &alpha, int C) {
    size_t n = alpha.size(); // размер входного вектора (size_t ~ беззнаковый целочисленный тип)
    int shift = (n - 1) * C; // смещение за каждый шаг свёртки

    std::vector<int> gamma;
    gamma.reserve(n); // заранее резервируем память для вектора размера n
    
    MaxQueue<int> Q; // очередь

    // пока окна (последовательности alpha и beta) не сравняются ~ не пройдём по всему вектору alpha
    for (int alpha_i : alpha) { // идём по элементам вектора alpha
        shift -= C; // обновляем смещение
        Q.push(alpha_i + shift);
        gamma.push_back(Q.get_max() - shift);
    }

    Q.pop();
    shift = C;

    while (!Q.empty()) {
        shift += C;
        gamma.push_back(Q.get_max() + shift);
        Q.pop();
    }

    return gamma;
}


std::vector<int> simple_max_plus_conv(const std::vector<int> &alpha, int C){
    size_t n = alpha.size(); // размер входного вектора (size_t ~ unsigned int)
    
    std::vector<int> gamma(2 * n - 1, std::numeric_limits<int>::min());

    for (size_t k = 0; k < gamma.size(); ++k) {
        for (size_t i = 0; i <= k; ++i) {
            if (i < n && k < n + i)
                gamma[k] = std::max(gamma[k], alpha[i] + C * static_cast<int>(k - i));
        }
    }

    return gamma;
}


int get_max_element(const std::vector<int> &values) {
    int max_element = values[0]; // изначально считаем, что первый элемент самый большой
    for(size_t i = 1; i < values.size(); ++i) { //start iterating from the second element
        if(values[i] > max_element)
            max_element = values[i];
    }
    return max_element;
}
