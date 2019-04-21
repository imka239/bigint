/*
    author dzhiblavi
 */

#include <tgmath.h>
#include "_core_arithmetics.hpp"

namespace _core {
    __uint128_t msl;
    uint64_t l;
    uint64_t d_normal;
    const __uint128_t t64 = __uint128_t(1) << 64;

    uint64_t high(__uint128_t x) {
        return x >> 64;
    }

    uint64_t low(__uint128_t x) {
        return x;
    }

    uint64_t sll(__uint128_t x, size_t i) {
        return x << i;
    }

    uint64_t srl(__uint128_t x, size_t i) {
        return x >> i;
    }

    void set_constant_divisor(uint64_t d) {
        l = (uint64_t) std::log2(d) + 1;
        msl = (t64 * ((__uint128_t(1) << l) - d) - 1) / d;
        d_normal = sll(d, 64 - l);
    }

    uint64_t divd(__uint128_t n, uint64_t d, uint64_t& rm) {
        if (n < d) {
            rm = n;
            return 0UL;
        }
        uint64_t n2 = sll(high(n), 64 - l) + srl(low(n), l);
        uint64_t n10 = sll(low(n), 64 - l);
        int64_t mn10 = -((int64_t) n10 < 0L);
        uint64_t n_adj = n10 + (mn10 & (d_normal - t64));
        uint64_t q1 = n2 + high(msl * (n2 - mn10) + n_adj);
        __int128 dr = n - t64 * d + (t64 - 1 - q1) * d;
        uint64_t __ret = high(dr) - (t64 - 1 - q1) + t64;
        rm = n - __ret * d;
        return __ret;
    }

    uint64_t _fast_short_div(uint64_t *p, uint64_t x, size_t size) {
        set_constant_divisor(x);
        __uint128_t carry = 0UL;
        uint64_t rm;
        for (size_t i = size; i-- > 0;) {
            carry = (carry << 64) + (__uint128_t) p[i];
            p[i] = divd(carry, x, rm);
            carry = rm;
        }
        return (uint64_t) carry;
    }

    uint64_t _pow10(size_t i) {
        static uint64_t _pw[19];
        if (_pw[i])
            return _pw[i];
        return _pw[i] = (uint64_t) std::pow(10, i);
    }
}
