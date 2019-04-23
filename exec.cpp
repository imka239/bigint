/*
    @author dzhiblavi
 */

#include <iostream>
#include <vector>
#include <list>
#include <cstdio>
#include <stack>
#include <random>
#include <typeinfo>
#include <ctime>
#include <chrono>
#include "big_integer.hpp"

size_t execute_raw()
{
    big_integer a, b;
    std::string as, bs;
    char op;
    size_t cnt;
    std::cin >> cnt >> op;
    for (size_t i = 0; i < cnt; ++i)
    {
        std::cin >> a >> b;
        switch (op)
        {
            case '+':
                a += b;
                break;
            case '-':
                a -= b;
                break;
            case '*':
                a *= b;
                break;
            case '/':
                a /= b;
                break;
            case '%':
                a %= b;
                break;
            default:
                break;
        }
    }
    return cnt;
}

size_t execute_out()
{
    big_integer a, b;
    std::string as, bs;
    char op;
    size_t cnt;
    std::cin >> cnt >> op;
    for (size_t i = 0; i < cnt; ++i)
    {
        std::cin >> as >> bs;
        a = big_integer(as);
        b = big_integer(bs);
        switch (op) {
            case '+':
                std::cout << a + b << '\n';
                break;
            case '-':
                std::cout << a - b << '\n';
                break;
            case '*':
                std::cout << a * b << '\n';
                break;
            case '/':
                std::cout << a / b << '\n';
                break;
            case '%':
                std::cout << a % b << '\n';
                break;
            default:
                break;
        }
    }
    return cnt;
}

int main()
{
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);
    std::ios::sync_with_stdio(false);
    std::string mode;
    std::cin >> mode;
    std::cerr.precision(12);
    auto ts = std::chrono::high_resolution_clock::now();
    size_t cnt;
    if (mode == "out")
        cnt = execute_out();
    else
        cnt = execute_raw();
    auto te = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> rm = te - ts;
    char c;
    std::cin >> c;
    if (c == 't')
      std::cerr << "CPP\t: " << std::fixed << rm.count() / cnt << '\n';
    return 0;
}
