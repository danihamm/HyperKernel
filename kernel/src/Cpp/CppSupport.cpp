/*
    * C++ new/delete operator
    * Copyright (c) 2025 Daniel Hammer
*/

#include <cstddef>
#include <Memory/MemoryAllocator.hpp>

void* operator new(std::size_t size)
{
    return Memory::g_allocator->Request(size);
}

void operator delete(void* block)
{
    Memory::g_allocator->Free(block);
}