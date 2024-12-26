// Реализация очереди с поддержкой максимального элемента
#pragma once // директива препроцессора, предназначенная для предотвращения многократного включения одного и того же заголовочного файла

#include <cassert> // для отлова логических багов
#include "max_stack.hpp"

// Очередь с поддержкой поиска максимального элемента за O(1).
template <typename T> // шаблон для работы с разными типами данных
class MaxQueue {
    private:
        MaxStack<T> stack_head; // стек головы
        MaxStack<T> stack_tail; // стек хвоста


    public:
        // Конструктор класса.
        MaxQueue() {}

        // Деструктор класса.
        ~MaxQueue() {}

        // Метод для добавления элемента в конец очереди.
        void push(T value) {
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
                // if (stack_tail.empty()) { // если ещё и стек хвоста пуст
                //     throw std::runtime_error("Head and tail stacks are empty!"); // выкидываем ошибку
                // }
                assert(!stack_tail.empty()); // если ещё и стек хвоста пуст — выкидываем ошибку

                this->move_elements(); // перемещаем элементы из хвоста в голову
            }
            stack_head.pop(); // удаляем первый элемент очереди (верхний в стеке головы)
        }

        // Метод, возвращающий первый элемент очереди (FIFO).
        T top() {
            if (stack_head.empty()) { // если стек головы пуст
                // if (stack_tail.empty()) { // если ещё и стек хвоста пуст
                //     throw std::runtime_error("Head and tail stacks are empty!"); // выкидываем ошибку
                // }
                assert(!stack_tail.empty()); // если ещё и стек хвоста пуст — выкидываем ошибку

                this->move_elements(); // перемещаем элементы из хвоста в голову
            }
            return stack_head.top(); // возвращаем первый элемент очереди (верхний в стеке головы)
        }

        // Метод для возвращения максимального элемента очереди.
        T get_max() {
            if (stack_head.empty()) { // если стек головы пуст
                // if (stack_tail.empty()) { // если ещё и стек хвоста пуст
                //     throw std::runtime_error("Head and tail stacks are empty!"); // выкидываем ошибку
                // }
                assert(!stack_tail.empty()); // если ещё и стек хвоста пуст — выкидываем ошибку

                this->move_elements(); // перемещаем элементы из хвоста в голову
            }

            T max_value = stack_head.get_max(); // берём максимальное значение у стека головы
            if (!stack_tail.empty()) // если есть ещё стек хвоста
                max_value = std::max(max_value, stack_tail.get_max()); // находим максимальный элемент из головы и хвоста

            return max_value; // возвращаем максимальный элемент из стеков
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
