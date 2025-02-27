#pragma once
#include <cstdint>
#include <cstddef>

#include <Libraries/string.hpp>

namespace Kt
{
    constexpr char newline = '\n';

    namespace screen
    {
        constexpr const char *clear = "\033[2J";
        constexpr const char *cursor_reset = "\033[H";
    };

    void Initialize(std::uint32_t *framebuffer, std::size_t width, std::size_t height, std::size_t pitch,
                    std::uint8_t red_mask_size, std::uint8_t red_mask_shift,
                    std::uint8_t green_mask_size, std::uint8_t green_mask_shift,
                    std::uint8_t blue_mask_size, std::uint8_t blue_mask_shift);
    void Putchar(char c);
    void Print(const char *text);

    enum base
    {
        oct = 8,
        dec = 10,
        hex = 16
    };

    inline base base_custom(int custom)
    {
        return (base)custom;
    }

    class KernelOutStream
    {
    public:
        base streamBaseType = base::dec;

        // C++ streaming operator like cout
        friend KernelOutStream &operator<<(KernelOutStream &t, const char *string)
        {
            Print(string);
            return t;
        }

        // C++ streaming operator like cout
        friend KernelOutStream &operator<<(KernelOutStream &t, const char chr)
        {
            Putchar(chr);
            return t;
        }

        friend KernelOutStream &operator<<(KernelOutStream &t, int number)
        {
            Print(Lib::int2basestr(number, t.streamBaseType));

            return t;
        }

        friend KernelOutStream &operator<<(KernelOutStream &t, std::uint32_t number)
        {
            Print(Lib::uint2basestr(number, t.streamBaseType));

            return t;
        }

        friend KernelOutStream &operator<<(KernelOutStream &t, std::uint64_t number)
        {
            Print(Lib::u64_2_basestr(number, t.streamBaseType));

            return t;
        }

        friend KernelOutStream &operator<<(KernelOutStream &t, base newBase)
        {
            t.streamBaseType = newBase;

            return t;
        }
    };

    // This will be on the kernel entry point's stack. Which is totally fine since we don't ever exit from that function
};

extern Kt::KernelOutStream kout;

namespace Kt
{
    class KernelErrorStream
    {
    public:
        template <typename T>
        friend KernelErrorStream &operator<<(KernelErrorStream &t, T value)
        {
            kout << "\e[0;31m" << value << "\e[0m";

            return t;
        }
    };
};

extern Kt::KernelErrorStream kerr;