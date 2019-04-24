#include "big_integer.h"

#include <string>
#include <algorithm>

const dig ten = 10;

big_integer::big_integer(int64_t num) : _sign(num < 0) {
    if (num) {
        _data.push_back((dig) ((num > 0) ? num : -num));
    }
}



big_integer::big_integer(std::string const &s) {
    size_t ind = (s[0] == '-' || s[0] =='+' ? 1 : 0);
    //todo to take 8 letters
    while (ind < s.size()) {
        *this *= ten;
        *this += big_integer(s[ind++] - '0');
    }
    _sign = ((s[0] == '-') && (!_data.empty()));
}

big_integer& big_integer::operator+=(const big_integer &that) {
    if (_sign == that._sign) {
        this->_add(that, 0);
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
        int32_t flag = this->_compare(that);
        if (flag == 0) {
            _sign = false;
            _data.resize(0);
        } else {
            if (flag < 0) {
                *this = that._subtract(*this);
                _sign ^= 1;
            } else {
                *this = this->_subtract(that);
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
    size_t sz = _data.size();

    big_integer ans;

    for (size_t i = 0; i < sz; i++) {
        ans._add(that._mul(_data[i]), i);
    }

    ans._delete_zero();
    ans._sign = (ans != 0) && (_sign ^ that._sign);
    *this = ans;
    return (*this);
}

big_integer& big_integer::operator/=(const big_integer &that) {
    *this = this->_div_on_bigint(that).first;
    return (*this);
}

big_integer& big_integer::operator%=(const big_integer &that) {
    *this = this->_div_on_bigint(that).second;
    return (*this);
}

big_integer& big_integer::operator&=(const big_integer &that) {
    big_integer ans(that);
    size_t sz = std::max(_data.size(), that._data.size());
    ans._data.resize(sz); _data.resize(sz);
    _delete_minus(); ans._delete_minus();
    for (size_t i = 0; i < sz; i++) {
        this->_data[i] = this->_data[i] & ans._data[i];
    }
    _sign = _sign & that._sign;
    _delete_zero();
    _append_minus();
    return (*this);
}



big_integer& big_integer::operator|=(const big_integer &that) {
    big_integer ans(that);
    size_t sz = std::max(_data.size(), that._data.size());
    ans._data.resize(sz); _data.resize(sz);
    _delete_minus(); ans._delete_minus();
    for (size_t i = 0; i < sz; i++) {
        this->_data[i] = this->_data[i] | ans._data[i];
    }
    _sign = _sign | that._sign;
    _delete_zero();
    _append_minus();
    return (*this);
}

big_integer& big_integer::operator^=(const big_integer &that) {
    big_integer ans(that);
    size_t sz = std::max(_data.size(), that._data.size());
    ans._data.resize(sz); _data.resize(sz);
    _delete_minus(); ans._delete_minus();
    for (size_t i = 0; i < sz; i++) {
        this->_data[i] = this->_data[i] ^ ans._data[i];
    }
    _sign = _sign ^ that._sign;
    _delete_zero();
    _append_minus();
    return (*this);
}

big_integer& big_integer::operator<<=(size_t sz) {
    dig mod = sz % 64;
    dig div = sz / 64;
    *this *= (((dig) 1) << mod);
    _shift_left(div);
    _delete_zero();
    return (*this);
}


big_integer& big_integer::operator>>=(size_t sz) {
    dig mod = sz % 64;
    dig div = sz / 64;
    *this /= (((dig)1) << mod);
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
        auto p = changeable._div_on_dig(ten);
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

void big_integer::_delete_minus() {
    if (!_sign || is_zero()) {
        return;
    }
    for (size_t &i : _data) {
        i = ~i;
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
    for (size_t &i : _data) {
        i = ~i;
    }
}