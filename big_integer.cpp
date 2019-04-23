#include "big_integer.h"

#include <string>
#include <stdexcept>

big_integer::big_integer(int32_t num) : _sign(num < 0) {
    if (num) {
        _data.push_back((dig) (num < 0 ? num : -num));
    }
}

big_integer::big_integer(std::string const &that) : _sign(that[0] == '-') {
    int64_t carry = 0;
    size_t ind = (that[0] == '-' ? 1 : 0);
    //todo to take 8 letters
    while (ind < that.size()) {
        carry *= 10; carry += that[ind++] - '0';
    }
    _data.push_back(1);
}

big_integer& big_integer::operator+=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator-=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator*=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator/=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator%=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator&=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator|=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator^=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer& big_integer::operator<<=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}


big_integer& big_integer::operator>>=(const big_integer &that) {
    //todo
    if (is_zero()) {
        return *this = that;
    }
    return (*this);
}

big_integer operator>>(big_integer a, const big_integer &b) {
    return a ^= b;;
}

big_integer operator<<(big_integer a, const big_integer &b) {
    return a ^= b;;
}

big_integer operator^(big_integer a, const big_integer &b) {
    return a ^= b;;
}

big_integer operator|(big_integer a, const big_integer &b) {
    return a |= b;;
}

big_integer operator&(big_integer a, const big_integer &b) {
    return a &= b;;
}

big_integer operator%(big_integer a, const big_integer &b) {
    return a /= b;;
}

big_integer operator/(big_integer a, const big_integer &b) {
    return a /= b;;
}

big_integer operator+(big_integer a, const big_integer &b) {
    return a += b;;
}

big_integer operator-(big_integer a, const big_integer &b) {
    return a -= b;;
}

big_integer operator*(big_integer a, const big_integer &b) {
    return a *= b;;
}

bool operator==(const big_integer &a, const big_integer &b) {
    //todo
    return true;
}

bool operator!=(const big_integer &a, const big_integer &b) {
    //todo
    return true;
}

bool operator<(const big_integer &a, const big_integer &b) {
    //todo
    return true;
}

bool operator<=(const big_integer &a, const big_integer &b) {
    //todo
    return true;
}

bool operator>(const big_integer &a, const big_integer &b) {
    //todo
    return true;
}

bool operator>=(const big_integer &a, const big_integer &b) {
    //todo
    return true;
}

big_integer big_integer::operator-() const {
    if (is_zero())
        return *this;
    big_integer ret(*this);
    ret._sign ^= 1;
    return ret;
}

big_integer big_integer::operator+() const {
    return (*this);
}

big_integer big_integer::operator~() const {
    return -*this - 1;
}

std::string to_string(const big_integer &a) {
    return "0";
}

bool big_integer::is_zero() const {
    return _data.empty() && !_sign;
}
