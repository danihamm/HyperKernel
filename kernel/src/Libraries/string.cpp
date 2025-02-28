#include "string.hpp"
#include <cstdint>
#include <cstddef>

using namespace std;

namespace Lib
{
    char output[1024];

    // TODO make this buffer safe
    char *int2basestr(int num, size_t radix)
    {
        char * str = (char *)&output;

        if (!num) {
            return (char *)"0";
        }

        char base[] = "0123456789ABCDEFGH"; // IJKLMN ....
        if (radix < 2 || radix > (sizeof(base) - 1))
        { // radix supported?
            str[0] = '\0';
            return str;
        }

        int si = 0;
        if (num < 0)
        {
            str[si++] = '-';
            num = -num;
        }
        char tmp[256];
        int rdi = -1;
        while (num)
        {
            tmp[++rdi] = base[num % radix];
            num /= radix;
        }
        while (rdi >= 0)
            str[si++] = tmp[rdi--];
        str[si] = '\0';
        return str;
    }

    char *u64_2_basestr(uint64_t num, size_t radix)
    {
        char * str = (char *)&output;

        if (!num) {
            return (char *)"0";
        }

        char base[] = "0123456789ABCDEFGH"; // IJKLMN ....

        if (radix < 2 || radix > (sizeof(base) - 1))
        { // radix supported?
            str[0] = '\0';
            return str;
        }

        int si = 0;

        char tmp[256];
        int rdi = -1;
        while (num)
        {
            tmp[++rdi] = base[num % radix];
            num /= radix;
        }
        while (rdi >= 0)
            str[si++] = tmp[rdi--];
        str[si] = '\0';
        return str;
    }

    char *uint2basestr(uint32_t num, size_t radix)
    {
        return u64_2_basestr((uint64_t)num, radix);
    }
}