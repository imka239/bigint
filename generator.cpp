/**
 * @author dzhiblavi
 */

#include <iostream>
#include <random>
#include <string>

std::string _gen_number(size_t len)
{
    static std::mt19937 gen((int)time(0));
    std::string ret;
    int sgn = std::uniform_int_distribution<int>(0, 99)(gen);
    if (sgn % 2)
    {
//         ret += "-";
    }
    ret += char('0' + std::uniform_int_distribution<int>(1, 9)(gen));
    for (size_t i = 1; i < len; ++i)
    {
        ret += ('0' + std::uniform_int_distribution<int>(0, 9)(gen));
    }
    return ret;
}

int main()
{
    size_t lena, lenb, cnt;
    std::cin >> cnt >> lena >> lenb;
    for (size_t i = 0; i < cnt; ++i)
    {
        std::cout << _gen_number(lena) << '\n' << _gen_number(lenb) << '\n';
    }
    return 0;
}


