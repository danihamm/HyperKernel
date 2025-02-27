#pragma once
#include <cstdint>
#include <cstddef>

namespace Lib {
    inline int strlen(const char *string) {
        int c = 0;

        while (*string != '\0') {
            string++;
            c++;            
        }

        return c;
    }

    char *int2basestr(int num, size_t radix);
    char *u64_2_basestr(uint64_t num, size_t radix);
    char *uint2basestr(uint32_t num, size_t radix);
}