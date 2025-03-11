#pragma once
#include "Memmap.hpp"

namespace Memory {
    class HeapAllocator {
        LargestSection g_section;

        struct Node {
            size_t size;
            Node* next;
        };

        struct Metadata {
            size_t size;
        };

        Node head{0, 0};

    public:
        HeapAllocator(LargestSection section);
        void* Request(size_t size);
        void* Realloc(void* ptr, size_t size);
        void Free(void *pagePtr);
        void Walk();
    };

    extern HeapAllocator* g_heap;
};