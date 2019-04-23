#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>

using dig = uint64_t;

struct big_integer {
    big_integer() = default;
    big_integer(const big_integer &) = default;
    big_integer(int32_t);
    big_integer(std::string const&);
    ~big_integer() = default;

    big_integer& operator=(const big_integer&) = default;

    big_integer& operator+=(const big_integer&);
    friend big_integer operator+(big_integer, const big_integer&);

    big_integer& operator-=(const big_integer&);
    friend big_integer operator-(big_integer, const big_integer&);

    big_integer& operator*=(const big_integer&);
    friend big_integer operator*(big_integer, const big_integer&);

    big_integer& operator/=(const big_integer&);
    friend big_integer operator/(big_integer, const big_integer&);

    big_integer& operator%=(const big_integer&);
    friend big_integer operator%(big_integer, const big_integer&);

    friend bool operator==(const big_integer&, const big_integer&);
    friend bool operator!=(const big_integer&, const big_integer&);

    friend bool operator<(const big_integer&, const big_integer&);
    friend bool operator<=(const big_integer&, const big_integer&);

    friend bool operator>(const big_integer&, const big_integer&);
    friend bool operator>=(const big_integer&, const big_integer&);

    big_integer operator-() const;
    big_integer operator+() const;
    big_integer operator~() const;

    big_integer& operator&=(const big_integer&);
    friend big_integer operator&(big_integer, const big_integer&);
    big_integer& operator|=(const big_integer&);
    friend big_integer operator|(big_integer, const big_integer&);
    big_integer& operator^=(const big_integer&);
    friend big_integer operator^(big_integer, const big_integer&);
    big_integer& operator<<=(const big_integer&);
    friend big_integer operator<<(big_integer, const big_integer&);
    big_integer& operator>>=(const big_integer&);
    friend big_integer operator>>(big_integer, const big_integer&);

    friend std::string to_string(const big_integer&);
    bool is_zero() const;

private:
    std::vector<dig> _data;
    bool _sign = false;
};

#endif // BIG_INTEGER_H
