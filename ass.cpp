//
// Created by imka on 23.04.19.
//

#include <algorithm>
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
    bool carry = false;
    size_t sz = _data.size();
    ans._data.resize(sz);
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
        return (that._data.size() > _data.size() ? -1 : 1);
    }
    for (size_t i = _data.size() - 1; i >= 0; i--) {
        if (_data[i] != that._data[i]) {
            return (that._data[i] > _data[i] ? -1 : 1);
        }
    }
    return 0;
}

big_integer big_integer::_mul(dig val) const {
    dig carry = 0;
    size_t sz = _data.size();

    big_integer ans;
    ans._data.resize(sz + 1);

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

std::pair<big_integer, dig> big_integer::_div(dig val) const {
    dig carry = 0;
    size_t sz = _data.size();

    big_integer ans;

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