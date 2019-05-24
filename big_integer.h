#ifndef BIG_INTEGER_H
#define BIG_INTEGER_H

#include <string>
#include <vector>
#include "mega_vector.h"

using dig = uint32_t;

struct big_integer {
    big_integer() = default;
    big_integer(const big_integer &) = default;
    big_integer(int32_t);
    big_integer(dig);
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
    big_integer& operator<<=(size_t);
    friend big_integer operator<<(big_integer, size_t);
    friend std::ostream& operator<<(std::ostream& os, big_integer const&);
    friend std::istream& operator>>(std::istream& os, big_integer& bi);

    big_integer& operator>>=(size_t);
    friend big_integer operator>>(big_integer, size_t);

    friend std::string to_string(const big_integer&);
    bool is_zero() const;

private:
    mega_vector<dig> _data;
    bool _sign = false;

    size_t _sz() const;
    dig _get_i(size_t) const;
    void _add(const big_integer&, size_t);
    big_integer& _subtract(const big_integer&);
    big_integer _mul(dig) const;
    big_integer& _div_on_dig(dig, dig&);
    big_integer& _div_on_bigint(const big_integer &that, big_integer&);
    int32_t _compare(const big_integer &that) const;
    dig _normalizer(big_integer&, big_integer&) const;
    void _delete_zero();
    void _delete_minus();
    void _append_minus();
    void _shift_left(size_t);
    void _shift_right(size_t);
};

#endif // BIG_INTEGER_H