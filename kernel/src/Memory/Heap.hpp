#pragma once
#include "Memmap.hpp"

namespace Memory {
    class Allocator {
        LargestSection g_section;

        struct Block {
            size_t size;
            Block* next;
        };

        struct BlockDescriptor {
            size_t block_size;
        };

        Block head;
    public:
        Allocator(LargestSection section);
        void* Request(size_t size, bool phys = false);
        void* Realloc(void* ptr, size_t size);
        void Free(void *pagePtr);

        void Stress();
        void Walk();
    };

    extern Allocator* g_allocator;
};