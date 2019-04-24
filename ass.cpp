//
// Created by imka on 23.04.19.
//

#include <algorithm>
#include <cstring>
#include "big_integer.h"


dig big_integer::_get_i(size_t ind) const {
    if (ind < _data.size()) {
        return _data[ind];
    } else {
        return 0;
    }
}

void big_integer::_add(const big_integer& that, const size_t place) {
    bool carry = false;
    size_t sz = std::max(_data.size(), (that._data.size() + place));
    _data.resize(sz);
    for (size_t i = 0; i < sz - place; i++) {
        dig bi1 = this->_get_i(i + place);
        dig bi2 = that._get_i(i);
        __asm__ (
        "xor %%rdx, %%rdx;"
        "add %%rbx, %%rax;"
        "adc $0, %%rdx;"
        "add %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (_data[i + place]), "=d" (carry)
        : "a" (bi1), "b" (bi2), "c" (carry)
        );
    }

    if (carry) {
        _data.push_back((dig) carry);
    }

    _delete_zero();
}

big_integer big_integer::_subtract(const big_integer & that) const {
    big_integer ans;
    ans._sign = this->_sign;
    bool carry = false;
    size_t sz = _data.size();
    ans._data.reserve(sz);
    for (size_t i = 0; i < sz; i++) {
        dig bi1 = this->_get_i(i);
        dig bi2 = that._get_i(i);
        dig d = 0;
        __asm__ (
        "xor %%rdx, %%rdx;"
        "sbb %%rbx, %%rax;"
        "adc $0, %%rdx;"
        "sbb %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (d), "=d" (carry)
        : "a" (bi1), "b" (bi2), "c" (carry)
        );
        ans._data.push_back(d);
    }

    ans._delete_zero();
    return ans;
}

int32_t big_integer::_compare(const big_integer &that) const {
    if (_data.size() != that._data.size()) {
        return ((that._data.size() > _data.size()) ? -1 : 1);
    }
    if (_data.empty()) {
        return 0;
    }
    for (size_t i = _data.size() - 1; i >= 0; i--) {
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
    dig carry = 0;
    size_t sz = _data.size();

    big_integer ans;
    ans._sign = this->_sign;
    ans._data.reserve(sz + 1);

    for (size_t i = 0; i < sz; i++) {
        dig bi1 = _get_i(i);
        dig d;
        __asm__ (
        "mul %%rbx;"
        "add %%rcx, %%rax;"
        "adc $0, %%rdx;"
        : "=a" (d), "=d" (carry)
        : "a" (bi1), "b" (val), "c" (carry)
        );
        ans._data.push_back(d);
    }
    if (carry != 0) {
        ans._data.push_back(carry);
    }

    ans._delete_zero();
    return ans;
}

std::pair<big_integer, dig> big_integer::_div_on_dig(dig val) const {
    dig carry = 0;
    size_t sz = _data.size();

    big_integer ans;
    ans._sign = this->_sign;

    for (size_t i = 0; i < sz; i++) {
        dig d1 = _get_i(sz - i - 1);
        dig d;
        __asm__ (
        "div %%rbx;"
        : "=a" (d), "=d" (carry)
        : "a" (d1), "b" (val), "d" (carry)
        );
        ans._data.push_back(d);
    }
    std::reverse(ans._data.begin(), ans._data.end());
    ans._delete_zero();
    return std::make_pair(ans, carry);
}

std::pair<big_integer, big_integer> big_integer::_div_on_bigint(const big_integer &that) const {
    if (that._data.empty()) {
        throw new error_t();
    }
    if (that._data.size() == 1) {
        auto ans = this->_div_on_dig(that._data[0]);
        ans.first._sign = (ans.first != 0 && (_sign ^ that._sign));
        big_integer answer = big_integer(ans.second);
        answer._sign = (answer != 0 && _sign);
        return std::make_pair(ans.first, answer);
    }

    if (this->_compare(that) < 0) {
        return std::make_pair(big_integer(), *this);
    }
    big_integer a(*this);
    big_integer b(that);
    a._sign = false; b._sign = false;

    dig r = _normalizer(a, b);
    size_t n = b._data.size();
    size_t m = a._data.size() - n;
    big_integer ans;
    ans._data.reserve(m);
    big_integer betta_in_pow_j(b);
    betta_in_pow_j._shift_left(m);

    if (betta_in_pow_j <= a) {
        ans._data.push_back(1);
        a -= betta_in_pow_j;
    }

    betta_in_pow_j._shift_right(1);

    for (size_t i = m - 1; i >= 0; betta_in_pow_j._shift_right(1), i--) {
        big_integer q;
        q._data.push_back(a._get_i(n + i - 1));
        q._data.push_back(a._get_i(n + i));
        q = q._div_on_dig(b._get_i(n - 1)).first;
        if (q._data.size() > 1) {
            q._data.resize(0);
            q._data.push_back(UINT64_MAX);
        }

        a -= betta_in_pow_j * q;
        while(a < 0) {
            q -= 1;
            a += betta_in_pow_j;
        }
        ans._data.push_back(q._data[0]);
        if (i == 0) {
            break;
        }
    }
    std::reverse(ans._data.begin(), ans._data.end());
    ans._delete_zero();
    ans._sign = (ans != 0 && (_sign ^ that._sign));
    a /= r;
    a._sign = (a != 0 && _sign);
    return std::make_pair(ans, b);
}

void big_integer::_delete_zero() {
    size_t i = _data.size();
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
    _data.resize(_data.size() + n);
    memmove(_data.data() + n, _data.data(), sizeof(dig) * (_data.size() - n));
    memset(_data.data(), 0, sizeof(dig) * n);
}


void big_integer::_shift_right(size_t n) {
    if (n == 0) {
        return;
    }
    memmove(_data.data(), _data.data() + n, sizeof(dig) * (_data.size() - n));
    _data.resize(_data.size() - n);
}

dig big_integer::_normalizer(big_integer &a, big_integer &b) const {
    big_integer x = big_integer(1);
    x._shift_left(1);
    x = x._div_on_dig(b._data[b._data.size() - 1] + 1).first;
    dig ans = x._data[0];
    a *= ans;
    b *= ans;
    return ans;
}