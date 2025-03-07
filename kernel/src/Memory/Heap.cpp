/*
    * MemoryAllocator.cpp
    * Heap memory allocator
    * Copyright (c) 2025 Daniel Hammer
*/

#include "Heap.hpp"
#include "Memmap.hpp"
#include "HHDM.hpp"

#include <Libraries/Memory.hpp>
#include <Terminal/terminal.hpp>

namespace Memory
{
    HeapAllocator::HeapAllocator(LargestSection section)
    {
        g_section = section;

        Block *current_ref{&head};
        Block *start_block_ptr {(Block *)HHDM(section.address)};
        Block& start_block = *start_block_ptr;

        start_block = Block {
            .size = section.size,
            .next = nullptr
        };

        *start_block_ptr = start_block;

        current_ref->next = start_block_ptr;

        kout << "[Mem] HeapAllocator object created for memory section starting at 0x" << base::hex << section.address << Kt::newline;
    }

    void* HeapAllocator::Request(size_t size, bool phys) {                
        Block* current_block_ptr{head.next};
        Block* current_prev_ptr{(Block *)&head};

        while (true) {
            if (current_block_ptr == nullptr) {
                kerr << "A HeapAllocator was forced to return a Null pointer. (1)" << Kt::newline;  
                return nullptr;              
            }

            if (current_prev_ptr == nullptr) {
                kerr << "An HeapAllocator was forced to return a Null pointer. (2)" << Kt::newline;
                return nullptr;
            }

            size_t descriptor_size = 8;
            size_t total_size = size + descriptor_size;

            if (current_block_ptr->size >= total_size) {
                Block block_copy = *current_block_ptr;
                BlockDescriptor* descriptor {(BlockDescriptor *)current_block_ptr};
                descriptor->block_size = size;
                
                void* block_start = (void *)(uint64_t)current_block_ptr + descriptor_size;

                if (!block_copy.size) {
                    current_prev_ptr->next = block_copy.next;
                }
                else
                {
                    block_copy.size -= total_size;

                    current_prev_ptr->next = (Block *)(uint64_t)descriptor + total_size;
                    *current_prev_ptr->next = block_copy;
                }

                if (phys) {
                    return (void *)SubHHDM(current_block_ptr);
                }

                return (void *)block_start;
            } else {
                if (current_block_ptr->next != nullptr) {
                    current_prev_ptr = current_block_ptr;
                    current_block_ptr = current_block_ptr->next;

                    continue;
                } else {
                    kerr << "An HeapAllocator was forced to return a Null pointer. (3)" << Kt::newline;
                    return nullptr;
                }
            }

            if (current_block_ptr != nullptr) {
                if (current_block_ptr->next == nullptr) {
                    kerr << "An HeapAllocator was forced to return a Null pointer. (4)" << Kt::newline;
                    return nullptr;
                }

                current_block_ptr = current_block_ptr->next;
            } else
            {
                kerr << "An HeapAllocator was forced to return a Null pointer. (5)" << Kt::newline;
                return nullptr;
            }
        }

        return nullptr;
    }

    void* HeapAllocator::Realloc(void* ptr, size_t size) {
        void *new_block = this->Request(size);

        if (ptr == nullptr) {
            return new_block;
        }

        if (new_block == nullptr) {
            kerr << "HeapAllocator: Reallocation failed due to memory starvation" << Kt::newline;
            return nullptr;
        }

        BlockDescriptor* descriptor = (BlockDescriptor*)((uint64_t)ptr - 8);

        memcpy(new_block, ptr, descriptor->block_size);

        this->Free(ptr);

        return new_block;
    }

    void HeapAllocator::Free(void *pagePtr) {
        if (!IsHDDMVirtAddr((uint64_t)pagePtr)) {
            pagePtr = (void *)HHDM(pagePtr);
        }

        BlockDescriptor* descriptor_ptr = (BlockDescriptor*)((uint64_t)pagePtr - 8);
        BlockDescriptor descriptor = *descriptor_ptr;

        Block* fmrNext = head.next;
        Block* blockPtr = (Block *)pagePtr;

        // Merge the descriptor with the block
        Block* base = (Block *)((uint64_t)blockPtr - 8);
        head.next = base;

        // Set the full size
        base->size = descriptor.block_size + 8;

        // Attach the merged block to the structure
        head.next->next = fmrNext;
    }
    
    // Traverses the Allocator's linked list for debugging
    void HeapAllocator::Walk() {
        Block* current = {head.next};
        size_t i{0};

        while (current != nullptr) {
            kout << "Block " << base::dec << i << " {" << current->size << " bytes & address 0x" << base::hex << (uint64_t)current << "}" << Kt::newline;
            current = current->next;
            i++;
        }
    }
};