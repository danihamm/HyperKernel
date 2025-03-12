/*
    * PageFrameAllocator.hpp
    * Memory allocator for physical pages
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include "Memmap.hpp"

namespace Memory {
    class PageFrameAllocator {
        struct Page {
            std::size_t size;
            Page* next;
        };

        Page head{};

        LargestSection g_section;
public:
        PageFrameAllocator(LargestSection section);

        void* Allocate();
        void Free(void* ptr);
    };

    extern PageFrameAllocator* g_pfa;
};