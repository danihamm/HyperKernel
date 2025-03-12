/*
    * PageFrameAllocator.hpp
    * Memory allocator for physical pages
    * Copyright (c) 2025 Daniel Hammer
*/

#include "PageFrameAllocator.hpp"
#include "HHDM.hpp"

namespace Memory {
    PageFrameAllocator::PageFrameAllocator(LargestSection section) {
        /* we need the virtual address rather than the physical address, so we call the helper */
        g_section = LargestSection{
            .address = HHDM(section.address),
            .size = section.size
        };

        head.next = (Page*)g_section.address;
        head.next->size = section.size;
        head.next->next = nullptr;
    }

    void* PageFrameAllocator::Allocate() {
        Page* current = head.next;
        Page* prev = &head;

        while (current != nullptr) {
            if (current->size >= 0x1000) {
                uint64_t current_addr = (uint64_t)current;
                uint64_t current_end_addr = current_addr + current->size;
                uint64_t new_size = current->size - 0x1000;

                if (new_size == 0) {
                    prev->next = current->next;
                    return (void*)current;
                }
                else
                {
                    current->size -= 0x1000;
                    return (void*)current_end_addr - 0x1000;
                }
            }

            prev = current;
            current = current->next;
        }

        return nullptr;
    }

    void PageFrameAllocator::Free(void* ptr) {
        auto prev_next = head.next;
        head.next = (Page*)ptr;

        head.next->next = prev_next;
        head.next->size = 0x1000;
    }
};