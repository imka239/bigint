//
// Created by imka on 23.04.19.
//

#include <algorithm>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include "big_integer.h"


size_t big_integer::_sz() const {
    return _data.size();
}

dig big_integer::_get_i(size_t ind) const {
    if (ind < _sz()) {
        return _data[ind];
    } else {
        return 0;
    }
}

void big_integer::_add(const big_integer& that, const size_t place) {
    size_t carry = 0;
    size_t sz = std::max(_sz(), (that._sz() + place));
    _data.resize(sz);
    for (size_t i = 0; i < sz - place; i++) {
        dig bi1 = _get_i(i + place);
        dig bi2 = that._get_i(i);
        uint64_t d = bi2;
        d = d + bi1 + carry;
        _data[i + place] = d;
        carry = d >> 32;
    }
    _data.push_back((dig) carry);
    _delete_zero();
}

big_integer big_integer::_subtract(const big_integer & that) const {
    big_integer ans;
    ans._sign = _sign;
    size_t carry = 0;
    size_t sz = _data.size();
    ans._data.reserve(sz);
    for (size_t i = 0; i < sz; i++) {
        dig bi1 = _get_i(i);
        dig bi2 = that._get_i(i);
        ans._data.push_back(bi1 - bi2 - carry);
        size_t newcarry = 0;
        if (bi1 < bi2 + carry) {
            newcarry = 1;
        }
        carry = newcarry;
    }

    ans._delete_zero();
    return ans;
}

int32_t big_integer::_compare(const big_integer &that) const {
    if (_sz() != that._sz()) {
        return ((that._sz() > _sz()) ? -1 : 1);
    }
    if (_data.empty()) {
        return 0;
    }
    for (size_t i = _sz() - 1; i >= 0; i--) {
        if (_data[i] != that._data[i]) {
            return ((that._data[i] > _data[i]) ? -1 : 1);
        }
        if (i == 0) {
            break;
        }
    }
    return 0;
}

big_integer big_integer::_mul(dig val) const {
    uint64_t carry = 0;
    size_t sz = _sz();
    big_integer ans;
    ans._sign = _sign;
    ans._data.reserve(sz + 1);
    for (size_t i = 0; i < sz; i++) {
        dig bi1 = _get_i(i);
        uint64_t d = bi1;
        d *= val;
        d += carry;
        carry = (d >> 32);
        ans._data.push_back(d);
    }
    ans._data.push_back(carry);
    ans._delete_zero();
    return ans;
}

big_integer& big_integer::_div_on_dig(dig val, dig& rm) {
    uint64_t carry = 0;
    size_t sz = _sz();
    big_integer ans;
    ans._sign = _sign;
    for (size_t i = 0; i < sz; i++) {
        dig d1 = _get_i(sz - i - 1);
        uint64_t d = (carry << 32) + d1;
        carry = d % val;
        ans._data.push_back(d / val);
    }
    std::reverse(ans._data.begin(), ans._data.end());
    ans._delete_zero();
    rm = carry;
    return *this = ans;
}

big_integer& big_integer::_div_on_bigint(const big_integer &that, big_integer& remind) {
    if (that._data.empty()) {
        throw std::runtime_error("division by zero");
    }
    if (that._sz() == 1) {
        dig remainder = 0;
        bool sign = _sign;
        _div_on_dig(that._data[0], remainder);
        _sign = (*this != 0 && (_sign ^ that._sign));
        if (remainder == 0) {
            remind = 0;
            return *this;
        }
        remind = remainder;
        remind._sign = (remind != 0 && sign);
        return *this;
    }
    if (_compare(that) < 0) {
        remind = *this;
        *this = 0;
        return *this;
    }
    big_integer a(*this);
    big_integer b(that);
    a._sign = false; b._sign = false;

    dig r = _normalizer(a, b);
    size_t n = b._sz();
    size_t m = a._sz() - n;
    big_integer ans;
    ans._data.reserve(m);
    big_integer betta_in_pow_j(b);
    betta_in_pow_j._shift_left(m);

    if (betta_in_pow_j <= a) {
        ans._data.push_back(1);
        a -= betta_in_pow_j;
    }
    betta_in_pow_j._shift_right(1);
    for (size_t i = m; i-- > 0; betta_in_pow_j._shift_right(1)) {
        big_integer q;
        q._data.push_back(a._get_i(n + i - 1));
        q._data.push_back(a._get_i(n + i));
        dig remainder = 0;
        q._div_on_dig(b._get_i(n - 1), remainder);
        if (q._sz() > 1) {
            q._data.resize(0);
            q._data.push_back(UINT32_MAX);
        }
        a -= betta_in_pow_j * q;
        while(a < 0) {
            q -= 1;
            a += betta_in_pow_j;
        }
        if (!q._data.empty())
            ans._data.push_back(q._data[0]);
        else
            ans._data.push_back(0);
        if (i == 0) {
            break;
        }
    }
    std::reverse(ans._data.begin(), ans._data.end());
    ans._delete_zero();
    ans._sign = (ans != 0 && (_sign ^ that._sign));
    a /= r;
    a._sign = (a != 0 && _sign);
    remind = a;
    *this = ans;
    return *this;
}

void big_integer::_delete_zero() {
    size_t i = _sz();
    while (i > 0 && _data[i - 1] == 0) {
        i--;
    }
    _data.resize(i);
    if (_data.empty()) {
        _sign = false;
    }
}

void big_integer::_shift_left(size_t n) {
    if (n == 0) {
        return;
    }
    _data.resize(_sz() + n);
    memmove(_data.data() + n, _data.data(), sizeof(dig) * (_sz() - n));
    memset(_data.data(), 0, sizeof(dig) * n);
}

void big_integer::_shift_right(size_t n) {
    if (n == 0) {
        return;
    }
    memmove(_data.data(), _data.data() + n, sizeof(dig) * (_sz() - n));
    _data.resize(_sz() - n);
}

dig big_integer::_normalizer(big_integer &a, big_integer &b) const {
    big_integer x = big_integer(1);
    x._shift_left(1);
    dig remainder = 0;
    x._div_on_dig(b._data[b._sz() - 1] + 1, remainder);
    dig ans = x._data[0];
    a *= ans;
    b *= ans;
    return ans;
}
