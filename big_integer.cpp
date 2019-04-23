#include "big_integer.h"

#include <string>
#include <stdexcept>
#include <algorithm>

const dig ten = 10;

big_integer::big_integer(uint32_t num) : _sign(num <= 0) {
    if (num) {
        _data.push_back((dig) ((num < 0) ? num : -num));
    }
}

big_integer::big_integer(std::string const &s) : _sign(s[0] == '-') {
    size_t ind = (s[0] == '-' || s[0] =='+' ? 1 : 0);
    //todo to take 8 letters
    while (ind < s.size()) {
        *this *= ten;
        *this += big_integer(s[ind] - '0');
    }
    _sign = (_sign && (!_data.empty()));
}

big_integer& big_integer::operator+=(const big_integer &that) {
    if (_sign == that._sign) {
        this->_add(that, 0);
    } else {
        _sign ^= 1;
        (*this) -= that;
        _sign ^= 1;
    }
    return *this;
}

big_integer& big_integer::operator-=(const big_integer &that) {
    if (_sign == that._sign) {
        int32_t flag = this->_compare(that);
        if (flag == 0) {
            _sign = false;
            _data.resize(0);
        } else {
            if (flag < 0) {
                *this = that._subtract(*this);
                _sign = false;
            } else {
                *this = this->_subtract(that);
                _sign = true;
            }
        }
    } else {
        _sign ^= 1;
        (*this) += that;
        _sign ^= 1;
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
    if (a.is_zero()) {
        return "0";
    }
    //todo
    big_integer changeable(a);
    std::string ans = "";
    while (!changeable.is_zero()) {
        auto p = changeable._div(ten);
        ans += std::to_string((p.second));
        changeable = p.first;
    }
    ans += ((a._sign == 1) ? "-" : "");
    std::reverse(ans.begin(), ans.end());
    return ans;
}

bool big_integer::is_zero() const {
    return _data.empty() && !_sign;
}

