/*
    * stream.hpp
    * Streaming
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstddef>
#include <cstdint>

using namespace std;

// Kernel C++ Platform
namespace kcp {
    enum base
    {
        oct = 8,
        dec = 10,
        hex = 16
    };

    class cstringstream {
        const char* string;
        std::size_t size;

        base current_base = base::dec;

    public:
        cstringstream();
        ~cstringstream();
        
        cstringstream& operator<<(char c);
        cstringstream& operator<<(char* str);
        cstringstream& operator<<(const char* str);

        cstringstream& operator<<(int num);
        cstringstream& operator<<(uint32_t val);
        cstringstream& operator<<(uint64_t val);

        cstringstream& operator<<(base nb);

        const char* cstr();
    };
};