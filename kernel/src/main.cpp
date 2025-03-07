/*
    * main.cpp
    * Kernel entry point
    * Copyright (c) 2025 Daniel Hammer, Limine Contributors (via Limine C++ example)
*/

#include <cstdint>
#include <cstddef>
#include <limine.h>

#include <Hal/GDT.hpp>
#include <Terminal/terminal.hpp>
#include <Libraries/string.hpp>
#include <Efi/UEFI.hpp>
#include <Common/Panic.hpp>
#include <Memory/Memmap.hpp>
#include <Memory/Heap.hpp>

#include <CppLib/Stream.hpp>
#include <CppLib/Vector.hpp>

#include <Platform/Limine.hpp>
#include <Platform/Util.hpp>

using namespace Kt;

namespace Memory {
    HeapAllocator* g_heap;
    uint64_t HHDMBase;
};

KernelOutStream kout;
KernelErrorStream kerr;

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kmain() {
    // Ensure the bootloader actually understands our base revision (see spec).
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    // Ensure we got a framebuffer.
    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    
    // Fetch the first framebuffer.
    limine_framebuffer *framebuffer{framebuffer_request.response->framebuffers[0]};

    // Initialize the terminal
    Kt::Initialize((uint32_t*)framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch, framebuffer->red_mask_size, framebuffer->red_mask_shift,
                    framebuffer->green_mask_size, framebuffer->green_mask_shift,
                    framebuffer->blue_mask_size, framebuffer->blue_mask_shift);

#if defined (__x86_64__)
    Hal::PrepareGDT();
    Hal::BridgeLoadGDT();
#endif

    uint64_t hhdm_offset = hhdm_request.response->offset;

    kout << "[Mem] HHDM offset: 0x" << base::hex << hhdm_offset << newline;

    Memory::HHDMBase = hhdm_offset;

    if (!system_table_request. response || !system_table_request.response->address) {
        kout << "[Mp] Firmware type: PC BIOS" << newline;
    } else {
        kout << "[Mp] Firmware type: EFI" << newline;
        kout << "[Efi] EFI system table at 0x" << base::hex << (uint64_t)system_table_request.response->address << newline;
    }

    if (memmap_request.response != nullptr) {
        kout << "[Mem] Creating Allocator for system conventional memory" << newline;

        auto result = Memory::Scan(memmap_request.response);
        auto allocator = Memory::HeapAllocator(result);

        Memory::g_heap = &allocator;
    } else {
        Panic("Guru Meditation Error: System memory map missing!", System::Registers{});
    }

    hcf();
}
