#include <iostream>
#include <string>
#include <vector>

int main()
{
    size_t cnt_lines;
    std::cin >> cnt_lines;
    std::vector<std::string> a(cnt_lines), b(cnt_lines);
    for (size_t i = 0; i < cnt_lines; ++i)
    {
        std::cin >> a[i];
    }
    for (size_t i = 0; i < cnt_lines; ++i)
    {
        std::cin >> b[i];
    }
    size_t cnt = 0;
    for (size_t i = 0; i < cnt_lines; ++i)
    {
        cnt += (a[i] != b[i]);
    }
    std::cout << cnt << std::endl;
    return 0;
}
