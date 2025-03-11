/*
    * MemoryAllocator.cpp
    * Heap memory allocator
    * Copyright (c) 2025 Daniel Hammer
*/

#include "Heap.hpp"
#include "Memmap.hpp"
#include "HHDM.hpp"

#include <Libraries/Memory.hpp>
#include <Terminal/Terminal.hpp>
#include <Common/Panic.hpp>
#include <Platform/Util.hpp>

#include <Common/Panic.hpp>

namespace Memory
{
    constexpr size_t metadata_size = 8; // 8 bytes
    
    HeapAllocator::HeapAllocator(LargestSection section)
    {
        kout << "HeapAllocator: constructor called" << Kt::newline;
    }

    void* HeapAllocator::Request(size_t size) {
        return nullptr;
    }

    void* HeapAllocator::Realloc(void* ptr, size_t size) {
        return nullptr;
    }

    void HeapAllocator::Free(void* ptr) {

    }
    
    // Traverses the Allocator's linked list for debugging
    void HeapAllocator::Walk() {
        // Node* current = {head.next};
        // size_t i{0};

        // while (current != nullptr) {
        //     kout << "Block " << base::dec << i << " {" << current->size << " bytes & address 0x" << base::hex << (uint64_t)current << "}" << Kt::newline;
        //     current = current->next;
        //     i++;
        // }
    }

};