/*
    * vector.hpp
    * Kernel C++ platform: Vector library
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>
#include <Terminal/Terminal.hpp>
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

        T& operator[](std::size_t position)
        {
            if (position > (sz - 1)) {
                kerr << "Vector out of bounds caught!" << Kt::newline;
                
                return at(0);
            }
            else
            {
                return array[position];
            }
        }

        T& at(std::int64_t position) {
            return this->operator[](position);
        }

        size_t push_back(T value) {
            size_t _sz = sz;

            array = (T *)Memory::g_heap->Realloc(array, sizeof(T) * (sz + 1));
            array[sz++] = value;

            return _sz;
        }

        T* get_array()
        {
            return array;
        }

        size_t size() {
            return sz;
        }
    };
};