/*
    * Alloc.cpp
    * C++ new/delete operator
    * Copyright (c) 2025 Daniel Hammer
*/

#include <cstddef>
#include <Memory/Heap.hpp>

void* operator new(std::size_t size)
{
    return Memory::g_heap->Request(size);
}

void operator delete(void* block)
{
    Memory::g_heap->Free(block);
}

void operator delete(void* block, long unsigned int)
{
    Memory::g_heap->Free(block);
}