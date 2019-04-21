/*
    author dzhiblavi
 */

#ifndef _core_arithmetics
#define _core_arithmetics

#include <stdint.h>
#include <cstddef>

namespace _core {
    extern "C" {
        uint64_t _asm_add(uint64_t *, uint64_t const *, size_t);
        uint64_t _asm_sub(uint64_t *, uint64_t const *, size_t);
        uint64_t _asm_mul(uint64_t *, uint64_t const *, uint64_t const *, size_t, size_t);
        uint64_t _asm_short_add(uint64_t *, uint64_t, size_t);
        uint64_t _asm_short_sub(uint64_t *, uint64_t, size_t);
    }
    uint64_t _fast_short_div(uint64_t *, uint64_t, size_t);
    uint64_t _pow10(size_t);
}

#endif /* _core_arithmetics.hpp */
