/*
    * vector.hpp
    * Kernel C++ platform: Vector library
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>
#include <Terminal/Terminal.hpp>

#include <Memory/Heap.hpp>

#include <Memory/PageFrameAllocator.hpp>
#include <Common/Panic.hpp>

#include <initializer_list>

namespace kcp
{
    template<typename T>
    class vector {
    protected:
        T* array;
        std::size_t sz;
        std::size_t capacity;

    public:
        vector()
        {
            sz = 0;
            array = nullptr;
        }

        vector(std::initializer_list<T> initList) {
            sz = 0;
            array = nullptr;

            for (auto itr = initList.begin(); itr != initList.end(); itr++) {
                T item = *itr;
                push_back(item);
            }
        }

        T& operator[](std::size_t position)
        {
            if (position > (capacity - 1)) {
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

            if (capacity < sz + 1) {
                array = (T *)Memory::g_heap->Realloc(array, sizeof(T) * (sz + 1));
                capacity++;
            }

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