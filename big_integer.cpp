#include "big_integer.h"

#include <string>
#include <algorithm>
#include <iostream>

const dig ten = 10;
const dig ten_in_pow_eight = 100000000;


big_integer::big_integer(int32_t num) : _sign(num < 0) {
    if (num  == INT32_MIN) {
        _data.push_back(UINT32_MAX / 2 + 1);
    } else  if (num) {
        _data.push_back((dig) ((num > 0) ? num : -(num)));
    }
}

big_integer::big_integer(dig num) : _sign(false) {
    if (num)
        _data.push_back(num);
}



big_integer::big_integer(std::string const &s) {
    size_t ind = (s[0] == '-' || s[0] =='+' ? 1 : 0);
    while (ind + 8 < s.size()) {
        *this *= ten_in_pow_eight;
        dig num = 0;
        for (int i = 0; i < 8; i++) {
            num *= 10;
            num += s[ind + i] - '0';
        }
        *this += num;
        ind += 8;
    }

    dig num = 0;
    dig ten_in_pow_ind = 1;
    for (size_t i = ind; i < s.size(); i++) {
        num *= 10;
        num += s[i] - '0';
        ten_in_pow_ind *= 10;
    }
    *this *= ten_in_pow_ind;
    *this += num;
    _sign = ((s[0] == '-') && (!_data.empty()));
}

big_integer& big_integer::operator+=(const big_integer &that) {
    if (_sign == that._sign) {
        _add(that, 0);
    } else {
        _sign ^= 1;
        (*this) -= that;
        _sign ^= 1;
    }
    if (_data.empty()) {
        _sign = false;
    }
    return *this;
}

big_integer& big_integer::operator-=(const big_integer &that) {
    if (_sign == that._sign) {
        int32_t flag = _compare(that);
        if (flag == 0) {
            _sign = false;
            _data.resize(0);
        } else {
            if (flag < 0) {
                big_integer tmp(that);
                tmp -= *this;
                tmp._sign ^= 1;
                std::swap(*this, tmp);
            } else {
                *this = _subtract(that);
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
    size_t sz = that._sz();

    big_integer ans;

    //std::vector<dig> answer(_sz() + that._sz());
    for (size_t i = 0; i < sz; i++) {
        ans._add(_mul(that._data[i]), i);
    }

    ans._delete_zero();
    ans._sign = (ans != 0) && (_sign ^ that._sign);
    std::swap(ans, *this);
    return (*this);
}

big_integer& big_integer::operator/=(const big_integer &that) {
    big_integer remind = 0;
    _div_on_bigint(that, remind);
    return (*this);
}

big_integer& big_integer::operator%=(const big_integer &that) {
    big_integer remind = 0;
    _div_on_bigint(that, remind);
    *this = remind;
    return *this;
}

big_integer& big_integer::operator&=(const big_integer &that) {
    big_integer ans(that);
    size_t sz = std::max(_sz(), that._sz());
    ans._data.resize(sz); _data.resize(sz);
    _delete_minus(); ans._delete_minus();
    for (size_t i = 0; i < sz; i++) {
        _data[i] = _data[i] & ans._data[i];
    }
    _sign = _sign & that._sign;
    _delete_zero();
    _append_minus();
    return (*this);
}



big_integer& big_integer::operator|=(const big_integer &that) {
    big_integer ans(that);
    size_t sz = std::max(_sz(), that._sz());
    ans._data.resize(sz); _data.resize(sz);
    _delete_minus(); ans._delete_minus();
    for (size_t i = 0; i < sz; i++) {
        _data[i] = _data[i] | ans._data[i];
    }
    _sign = _sign | that._sign;
    _delete_zero();
    _append_minus();
    return (*this);
}

big_integer& big_integer::operator^=(const big_integer &that) {
    big_integer ans(that);
    size_t sz = std::max(_sz(), that._sz());
    ans._data.resize(sz); _data.resize(sz);
    _delete_minus(); ans._delete_minus();
    for (size_t i = 0; i < sz; i++) {
        _data[i] = _data[i] ^ ans._data[i];
    }
    _sign = _sign ^ that._sign;
    _delete_zero();
    _append_minus();
    return (*this);
}

big_integer& big_integer::operator<<=(size_t sz) {
    dig mod = sz % 32;
    dig div = sz / 32;
    *this *= (((dig) 1) << mod);
    _shift_left(div);
    _delete_zero();
    return (*this);
}


big_integer& big_integer::operator>>=(size_t sz) {
    dig mod = sz % 32;
    dig div = sz / 32;
    *this /= (((dig) 1) << mod);
    _shift_right(div);
    _delete_zero();
    if (_sign) {
        (*this)-=1;
    }
    return (*this);
}

big_integer operator>>(big_integer a, size_t sz) {
    a >>= sz;
    return a;
}

big_integer operator<<(big_integer a, size_t sz) {
    a <<= sz;
    return a;
}

big_integer operator^(big_integer a, const big_integer &b) {
    a ^= b;
    return a;
}

big_integer operator|(big_integer a, const big_integer &b) {
    a |= b;
    return a;
}

big_integer operator&(big_integer a, const big_integer &b) {
    a &= b;
    return a;
}

big_integer operator%(big_integer a, const big_integer &b) {
    a %= b;
    return a;
}

big_integer operator/(big_integer a, const big_integer &b) {
    a /= b;
    return a;
}

big_integer operator+(big_integer a, const big_integer &b) {
    a += b;
    return a;
}

big_integer operator-(big_integer a, const big_integer &b) {
    a -= b;
    return a;
}

big_integer operator*(big_integer a, const big_integer &b) {
    a *= b;
    return a;
}

bool operator==(const big_integer &a, const big_integer &b) {
    if (a._sign != b._sign) {

        return false;
    }
    return a._compare(b) == 0;
}

bool operator!=(const big_integer &a, const big_integer &b) {
    return !(a == b);
}

bool operator<(const big_integer &a, const big_integer &b) {
    if (a._sign != b._sign) {
        return (a._sign > b._sign);
    }
    int32_t fl = a._compare(b);
    return (fl != 0) && (a._sign ^ (fl == -1));
}

bool operator<=(const big_integer &a, const big_integer &b) {
    return !(a > b);
}

bool operator>(const big_integer &a, const big_integer &b) {
    if (a._sign != b._sign) {
        return (a._sign < b._sign);
    }
    int32_t fl = a._compare(b);
    return (fl != 0) && (a._sign ^ (fl == 1));
}

bool operator>=(const big_integer &a, const big_integer &b) {
    return !(a < b);
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
    big_integer changeable(a);
    std::string ans = "";
    while (!changeable.is_zero()) {
        dig remainder = 0;
        changeable._div_on_dig(ten, remainder);
        ans += std::to_string(remainder);
    }
    ans += ((a._sign == 1) ? "-" : "");
    std::reverse(ans.begin(), ans.end());
    return ans;
}

bool big_integer::is_zero() const {
    return _data.empty() && !_sign;
}

void big_integer::_delete_minus() {
    if (!_sign || is_zero()) {
        return;
    }
    for (size_t i = 0; i < _sz(); i++) {
        _data[i] = ~_data[i];
    }
    _sign = false;
    (*this) += 1;
    _sign = true;
}

void big_integer::_append_minus() {
    if (!_sign || is_zero()) {
        return;
    }
    _sign = false;
    (*this) -= 1;
    _sign = true;
    for (size_t i = 0; i < _sz(); i++) {
        _data[i] = ~_data[i];
    }
}

std::istream& operator>>(std::istream& os, big_integer& bi) {
    std::string s;
    std::cin >> s;
    bi = big_integer(s);
    return os;
}

std::ostream& operator<<(std::ostream& os, big_integer const& bi) {
    return os << to_string(bi);
}