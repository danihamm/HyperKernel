#pragma once
#include <limine.h>
#include <cstddef>

using namespace std;

namespace Memory {
    // Shared
    struct LargestSection {
        uint64_t address;
        size_t size;
    };

    LargestSection Scan(limine_memmap_response* mmap);
};