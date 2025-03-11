/*
    * main.cpp
    * Kernel entry point
    * Copyright (c) 2025 Daniel Hammer, Limine Contributors (via Limine C++ example)
*/

#include <cstdint>
#include <cstddef>
#include <limine.h>

#include <Hal/GDT.hpp>
#include <Terminal/Terminal.hpp>
#include <Libraries/String.hpp>
#include <Efi/UEFI.hpp>
#include <Common/Panic.hpp>
#include <Memory/Memmap.hpp>
#include <Memory/Heap.hpp>
#include <Memory/HHDM.hpp>

#include <CppLib/Stream.hpp>
#include <CppLib/Vector.hpp>
#include <CppLib/CString.hpp>

#include <Platform/Limine.hpp>
#include <Platform/Util.hpp>

#include <Libraries/Memory.hpp>

#include <Hal/IDT.hpp>

using namespace Kt;

namespace Memory {
    HeapAllocator* g_heap;
    uint64_t HHDMBase;
};

KernelOutStream kout{};
KernelErrorStream kerr{};

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

extern "C" void kmain() {
    if (LIMINE_BASE_REVISION_SUPPORTED == false) {
        hcf();
    }

    // Call global constructors.
    for (std::size_t i = 0; &__init_array[i] != __init_array_end; i++) {
        __init_array[i]();
    }

    if (framebuffer_request.response == nullptr
     || framebuffer_request.response->framebuffer_count < 1) {
        hcf();
    }
    
    limine_framebuffer *framebuffer{framebuffer_request.response->framebuffers[0]};

    Kt::Initialize(
        (uint32_t*)framebuffer->address,
        framebuffer->width,
        framebuffer->height,
        framebuffer->pitch,
        framebuffer->red_mask_size,
        framebuffer->red_mask_shift,
        framebuffer->green_mask_size,
        framebuffer->green_mask_shift,
        framebuffer->blue_mask_size,
        framebuffer->blue_mask_shift
    );

    kout << "OS220 research kernel project" << "\n" << "Written by Daniel Hammer" << "\n" << "\n";

#if defined (__x86_64__)
    Hal::PrepareGDT();
    Hal::BridgeLoadGDT();
#endif

    uint64_t hhdm_offset = hhdm_request.response->offset;
    Memory::HHDMBase = hhdm_offset;

    if (memmap_request.response != nullptr) {
        kout << "MemoryManagement: Creating global HeapAllocator" << newline;

        auto result = Memory::Scan(memmap_request.response);
        auto allocator = Memory::HeapAllocator(result);

        Memory::g_heap = &allocator;
    } else {
        Panic("Guru Meditation Error: System memory map missing!", nullptr);
    }

#if defined (__x86_64__)
    Hal::IDTInitialize();
#endif

    int*a = (int*)0xffffff;
    *a=10;
    hcf();
}
