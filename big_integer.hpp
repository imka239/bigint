/*
    author dzhiblavi
 */

#ifndef big_integer_hpp
#define big_integer_hpp

#include <string>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>
#include <stdint.h>
#include "_core_arithmetics.hpp"

#define SAFE_VECTOR

#ifdef STD_VECTOR
#include <vector>
using std::vector;
#endif
#ifdef SAFE_VECTOR
#include "vector.hpp"
#endif
#ifdef __UNSAFE_VECTOR__
#include "__unsafe_vector.hpp"
using __unsafe::vector;
#endif

class big_integer
{
public:
    using digit_t = uint64_t;
    using digit_ptr = digit_t*;
    using const_ptr = digit_t const*;
    static const size_t DIGIT_SIZE = sizeof (digit_t);

private:
    vector<digit_t>_data;
    bool _sgn = false;

    static big_integer _karat_mul(big_integer const&, big_integer const&);
    static int _compare(const_ptr, const_ptr, size_t, size_t);
    digit_t _read_digit(const std::string&, size_t, size_t) noexcept;
    big_integer &_apply_bitwise(big_integer const& bi, digit_t (*f)(digit_t, digit_t));
    big_integer &_naive_mul(big_integer const&);

    void _normalize();
    big_integer _higher(size_t) const;
    big_integer _lower(size_t) const;
    big_integer &_shift_left(size_t);
    big_integer &_shift_right(size_t);

    void _to_moved_repr();
    void _to_signed_repr();

public:
    big_integer() noexcept = default;
    big_integer(int64_t);
    big_integer(const big_integer&) = default;
    big_integer(big_integer&&) noexcept;
    explicit big_integer(const std::string&);
    static big_integer from_unsigned_long(uint64_t);

    big_integer& operator=(const big_integer&);
    big_integer& operator=(big_integer&&) noexcept;

    void swap(big_integer&) noexcept;
    ~big_integer() noexcept = default;
    bool is_zero() const noexcept;
    big_integer& div_mod(uint64_t, uint64_t&);

    big_integer& operator+=(const big_integer&);
    big_integer& operator-=(const big_integer&);
    big_integer& operator*=(const big_integer&);
    big_integer& operator/=(const big_integer&);
    big_integer& operator%=(const big_integer&);

    big_integer& operator++();
    const big_integer operator++(int);
    big_integer& operator--();
    const big_integer operator--(int);

    big_integer operator-() const;
    big_integer operator+() const;
    big_integer operator~() const;

    big_integer& operator<<=(size_t);
    big_integer& operator>>=(size_t);
    big_integer& operator&=(const big_integer&);
    big_integer& operator|=(const big_integer&);
    big_integer& operator^=(const big_integer&);

    friend bool operator==(const big_integer&, const big_integer&);
    friend bool operator!=(const big_integer&, const big_integer&);
    friend bool operator<(const big_integer&, const big_integer&);
    friend bool operator>(const big_integer&, const big_integer&);
    friend bool operator<=(const big_integer&, const big_integer&);
    friend bool operator>=(const big_integer&, const big_integer&);

    friend big_integer operator+(big_integer, const big_integer&);
    friend big_integer operator-(big_integer, const big_integer&);
    friend big_integer operator*(big_integer, const big_integer&);
    friend big_integer operator/(big_integer, const big_integer&);
    friend big_integer operator%(big_integer, const big_integer&);

    friend big_integer operator>>(big_integer, size_t);
    friend big_integer operator<<(big_integer, size_t);
    friend big_integer operator&(big_integer, const big_integer&);
    friend big_integer operator|(big_integer, const big_integer&);
    friend big_integer operator^(big_integer, const big_integer&);

    friend std::string to_string(const big_integer&);
    friend std::ostream& operator<<(std::ostream&, const big_integer&);
    friend std::istream& operator>>(std::istream&, big_integer&);
};

#endif /* big_integer_hpp */
