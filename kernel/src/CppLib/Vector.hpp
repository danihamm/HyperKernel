/*
    * vector.hpp
    * Kernel C++ platform: Vector library
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>
#include <Terminal/terminal.hpp>
#include <Memory/Heap.hpp>

namespace kcp
{
    template<typename T>
    class vector {
    protected:
        T* array;
        std::size_t sz;

    public:
        vector()
        {
            sz = 0;
            array = nullptr;
        }

        T operator[](std::int64_t position)
        {
            if (position > (sz + 1)) {
                kerr << "Vector out of bounds caught!" << Kt::newline;
                return T{};
            }
            else
            {
                return array[position];
            }
        }

        T at(std::int64_t position) {
            return this->operator[](position);
        }

        void push_back(T value) {
            array = (T *)Memory::g_allocator->Realloc(array, sz + 1);
            array[sz++] = value;
        }

        void test() {
            array = (T *)Memory::g_allocator->Realloc(array, sz + 1);
        }

        T& get_array()
        {
            return *array;
        }
    };
};