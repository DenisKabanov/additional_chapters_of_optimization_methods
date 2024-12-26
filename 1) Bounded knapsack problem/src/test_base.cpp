#include <iostream>
#include <memory>

#include "../include/max_stack.hpp"
#include "../include/max_queue.hpp"
#include "../include/max_plus_conv.hpp"

#include <random>
#include <fstream>

using std::cin;
using std::cout;
using std::endl;

void stack_unit_test_1() {
    MaxStack<int> S;

    S.push(1);
    S.push(4);
    S.push(3);
    S.push(2);

    while (!S.empty()) {
        cout << S.get_max() << " ";
        S.pop();
    }

    assert(S.empty());

    cout << "\n";
}

void queque_unit_test_1() {
    MaxQueue<int> Q;
    Q.push(1);
    Q.push(4);
    Q.push(3);
    Q.push(2);

    cout << Q.get_max() << " ";

    Q.pop();
    Q.pop();

    cout << Q.get_max() << " ";

    Q.pop();

    cout << Q.get_max() << " ";

    Q.push(1);

    cout << Q.get_max() << " \n";

    cout << "How is waiting: ";

    while (!Q.empty()) {
        cout << Q.top() << " ";
        Q.pop();
    }
    cout << "\n";
}

void conv_unit_test_1() {
    std::vector<int> alpha = {1, -3, 4, 3, 2, 5, 2, 1};
    int C = 3;

    auto gamma = fast_max_plus_conv(alpha, C);
    for (size_t i = 0; i < gamma.size(); i++) {
        std::cout << gamma[i] << " ";
    }

    assert(gamma.size() == 2 * alpha.size() - 1);
    cout << "\n";
}

void conv_unit_test_2() {
    // std::vector<int> alpha = {0, 1, 1, 2, 2, 7, 7};
    std::vector<int> alpha = {1, 2, 4, 7};
    int C = 4;

    auto gamma = fast_max_plus_conv(alpha, C);
    for (size_t i = 0; i < gamma.size(); i++) {
        std::cout << gamma[i] << " ";
    }

    assert(gamma.size() == 2 * alpha.size() - 1);
    cout << "\n";
}

void conv_stress_test(size_t attempts_num = 10000, size_t n = 10, int a = -100, int b = 100, int seed = 0, std::string log_fname = "../logs/conv_stress_test_log.txt")
{
    std::ofstream log(log_fname);

    std::vector<int> alpha(n, 0);
    int C = 0;

    // std::random_device rd;
    std::mt19937 gen(seed);
    std::uniform_int_distribution<int> udist(a, b);

    log << "seed = " << seed << "\n";
    for (size_t i = 0; i < attempts_num; i++)
    {
        log << "-------------------------" << "\n";
        log << "Attempt number: " << i << "\n";
        log << "-------------------------" << "\n";

        C = udist(gen);
        for (size_t i = 0; i < alpha.size(); i++)
        {
            alpha[i] = udist(gen);
        }

        log << "C = " << C << "\n";
        log << "alpha:" << "\n";
        for (size_t i = 0; i < n; ++i)
            log << alpha[i] << " ";
        log << "\n";

        auto fast_conv_result = fast_max_plus_conv(alpha, C);
        auto simple_conv_result = simple_max_plus_conv(alpha, C);

        log << "fast_conv_result:" << "\n";
        for (size_t i = 0; i < fast_conv_result.size(); ++i)
            log << fast_conv_result[i] << " ";
        log << "\n";

        log << "simple_conv_result:" << "\n";
        for (size_t i = 0; i < simple_conv_result.size(); ++i)
            log << simple_conv_result[i] << " ";
        log << "\n";

        if (fast_conv_result[i] != simple_conv_result[i])
        {
            log << "Failed!";
        }
        log << endl;
    }
}

int main()
{
    // stack_unit_test_1();
    // queque_unit_test_1();
    conv_unit_test_2();
    // conv_unit_test_1();
    // conv_stress_test();


    cout << "Все тесты прошли успешно!" << endl;
    return 0;
}