#include "PageAllocator.hpp"
#include "Memmap.hpp"
#include "HHDM.hpp"

#include <KernelTerminal/terminal.hpp>

namespace Memory
{
#if defined(__x86_64__) || defined(__aarch64__)
#define PAGE_SIZE 4096
#else
#error Sorry, Unimplemented!
#endif

    PageAllocator::PageAllocator(LargestSection section)
    {
        g_section = section;

        size_t pages = g_section.size / PAGE_SIZE;
        Block *current_ref{&head};
        Block *start_block_ptr {(Block *)HHDM(section.address)};
        Block& start_block = *start_block_ptr;

        start_block = Block {
            .size = section.size,
            .next = nullptr
        };

        *start_block_ptr = start_block;

        current_ref->next = start_block_ptr;

        kout << "[Mem] PageAllocator created for memory section starting at 0x" << Kt::base::hex << section.address << Kt::newline;
    }

    void* PageAllocator::RequestPage(bool phys) {        
        bool found_block = false;
        
        Block* current_block_ptr{head.next};
        Block* current_prev_ptr{(Block *)&head};

        while (!found_block) {
            if (current_block_ptr == nullptr) {
                kerr << "A PageAllocator was forced to return a Null pointer. (1)" << Kt::newline;  
                return nullptr;              
            }

            if (current_prev_ptr == nullptr) {
                kerr << "A PageAllocator was forced to return a Null pointer. (2)" << Kt::newline;
                return nullptr;
            }
            
            if (current_block_ptr->size >= 0x1000) { // 0x1000 == 4096
                current_block_ptr->size -= 0x1000;

                if (!current_block_ptr->size) {
                    current_prev_ptr->next = current_block_ptr->next;
                    return (void *)current_block_ptr;
                }

                Block block_copy = *current_block_ptr;

                current_prev_ptr->next = (Block *)(uint64_t)current_block_ptr + 0x1000;
                *current_prev_ptr->next = block_copy;

                if (phys) {
                    return (void *)SubHHDM(current_block_ptr);
                }

                return (void *)current_block_ptr;
            } else {
                if (current_block_ptr->next != nullptr) {
                    current_prev_ptr = current_block_ptr;
                    current_block_ptr = current_block_ptr->next;
                } else {
                    kerr << "A PageAllocator was forced to return a Null pointer. (3)" << Kt::newline;
                    return nullptr;
                }
            }

            if (current_block_ptr != nullptr) {
                if (current_block_ptr->next == nullptr) {
                    kerr << "A PageAllocator was forced to return a Null pointer. (4)" << Kt::newline;
                    return nullptr;
                }

                current_block_ptr = current_block_ptr->next;
            } else
            {
                kerr << "A PageAllocator was forced to return a Null pointer. (5)" << Kt::newline;
                return nullptr;
            }
        }

        return nullptr;
    }

    void PageAllocator::Free(void *pagePtr) {
        if (!IsHDDMVirtAddr((uint64_t)pagePtr)) {
            pagePtr = (void *)HHDM(pagePtr);
        }

        Block* fmrNext = head.next;

        Block* blockPtr = (Block *)pagePtr;

        head.next = blockPtr;
        head.next->size = 0x1000;

        head.next->next = fmrNext;
    }

    void PageAllocator::Stress() {
        // ~410 MiB
        for (size_t i = 0; i < 100000; i++) {
            kout << "[Mem] Stress allocation " << Kt::base::dec << i << ": " << "0x" << Kt::base::hex << (uint64_t)Memory::KernelPFA->RequestPage(false) << Kt::newline;
        }
    }
    
    // Traverses the PageAllocator's linked list for debugging
    void PageAllocator::Walk() {
        Block* current = {head.next};
        size_t i{0};

        while (current != nullptr) {
            kout << "Block " << Kt::base::dec << i << " {" << current->size << " bytes & address 0x" << Kt::base::hex << (uint64_t)current << "}" << Kt::newline;
            current = current->next;
            i++;
        }
    }
};