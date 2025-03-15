#pragma once
#include "Memmap.hpp"

namespace Memory {
    class HeapAllocator {
        struct Node {
            size_t size;
            Node* next;
        };

        struct Header {
            std::size_t size;
        }__attribute__((packed));

        Node head{};

        Header* GetHeader(void* block);
        void InsertToFreelist(void* ptr, std::size_t size);
        void InsertPageToFreelist();
        void InsertPagesToFreelist(std::size_t n);
    public:
        HeapAllocator();
        void* Request(size_t size);
        void* Realloc(void* ptr, size_t size);
        void Free(void *pagePtr);
        void Walk();
        size_t GetAllocatedBlockSize(void* ptr);
    };

    extern HeapAllocator* g_heap;
};