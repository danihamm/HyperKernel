#pragma once
#include "Memmap.hpp"

namespace Memory {
    class PageAllocator {
        LargestSection g_section;

        struct Block {
            size_t size;
            Block* next;
        };

        Block head;
    public:
        PageAllocator(LargestSection section);
        void* RequestPage(bool phys = false);
        void Free(void *pagePtr);

        void Stress();
        void Walk();
    };

    extern PageAllocator* KernelPFA;
};