#include "Memmap.hpp"
#include <Terminal/terminal.hpp>
#include <Common/Panic.hpp>

#include "Heap.hpp"

using namespace Kt;

namespace Memory {
    LargestSection Scan(limine_memmap_response* mmap) {
        LargestSection currentLargestSection{};

        for (size_t i = 0; i < mmap->entry_count; i++) {
            auto entry = mmap->entries[i];

            if (entry->base == 0) {
                continue;
            }
            
            if (entry->type == LIMINE_MEMMAP_USABLE) {
                kout << "[Mem] Found conventional memory section (size = " << base::dec << entry->length << " bytes, address = 0x" << base::hex << (uint64_t)entry->base << ")" << newline;                
                
                if (entry->length > currentLargestSection.size) {
                    currentLargestSection = {
                        .address = (uint64_t)entry->base,
                        .size = entry->length
                    };
                }
            }
        }

        [[unlikely]] if (currentLargestSection.address == 0) {
            Panic("Couldn't find a usable memory section.", System::Registers{});
        }

        return currentLargestSection;

        // PageAllocator pa(currentLargestSection);

        // uint64_t alloc1 = (uint64_t)pa.Allocate();
        // uint64_t alloc2 = (uint64_t)pa.Allocate();
        // pa.Free((void *)alloc2);
        // uint64_t alloc3 = (uint64_t)pa.Allocate();

        // kout << "0x" << base::hex << alloc1 << "\n";
        // kout << "0x" << base::hex << alloc2 << "\n";
        // kout << "0x" << base::hex << alloc3 << "\n";
    }
};