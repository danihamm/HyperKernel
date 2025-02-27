#include <cstdint>
#include <cstddef>
#include <limine.h>

#include <Hal/GDT.hpp>
#include <KernelTerminal/terminal.hpp>
#include <Libraries/string.hpp>
#include <Efi/UEFI.hpp>
#include <Common/Panic.hpp>
#include <Memory/Memmap.hpp>
#include <Memory/PageAllocator.hpp>

using namespace Kt;

namespace Memory {
    PageAllocator* KernelPFA;
    uint64_t HHDMBase;
};

KernelOutStream kout;
KernelErrorStream kerr;

// Set the base revision to 3, this is recommended as this is the latest
// base revision described by the Limine boot protocol specification.
// See specification for further info.

namespace {

__attribute__((used, section(".limine_requests")))
volatile LIMINE_BASE_REVISION(3);

}

// The Limine requests can be placed anywhere, but it is important that
// the compiler does not optimise them away, so, usually, they should
// be made volatile or equivalent, _and_ they should be accessed at least
// once or marked as used with the "used" attribute as done here.

namespace {

__attribute__((used, section(".limine_requests")))
volatile limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".limine_requests")))
volatile limine_efi_system_table_request system_table_request = {
    .id = LIMINE_EFI_SYSTEM_TABLE_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".limine_requests")))
volatile limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST,
    .revision = 0,
    .response = nullptr
};

__attribute__((used, section(".limine_requests")))
volatile limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0,
    .response = nullptr
};

}

// Finally, define the start and end markers for the Limine requests.
// These can also be moved anywhere, to any .cpp file, as seen fit.

namespace {

__attribute__((used, section(".limine_requests_start")))
volatile LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
volatile LIMINE_REQUESTS_END_MARKER;

}

// GCC and Clang reserve the right to generate calls to the following
// 4 functions even if they are not directly called.
// Implement them as the C specification mandates.
// DO NOT remove or rename these functions, or stuff will eventually break!
// They CAN be moved to a different .cpp file.

extern "C" {

void *memcpy(void *dest, const void *src, std::size_t n) {
    std::uint8_t *pdest = static_cast<std::uint8_t *>(dest);
    const std::uint8_t *psrc = static_cast<const std::uint8_t *>(src);

    for (std::size_t i = 0; i < n; i++) {
        pdest[i] = psrc[i];
    }

    return dest;
}

void *memset(void *s, int c, std::size_t n) {
    std::uint8_t *p = static_cast<std::uint8_t *>(s);

    for (std::size_t i = 0; i < n; i++) {
        p[i] = static_cast<uint8_t>(c);
    }

    return s;
}

void *memmove(void *dest, const void *src, std::size_t n) {
    std::uint8_t *pdest = static_cast<std::uint8_t *>(dest);
    const std::uint8_t *psrc = static_cast<const std::uint8_t *>(src);

    if (src > dest) {
        for (std::size_t i = 0; i < n; i++) {
            pdest[i] = psrc[i];
        }
    } else if (src < dest) {
        for (std::size_t i = n; i > 0; i--) {
            pdest[i-1] = psrc[i-1];
        }
    }

    return dest;
}

int memcmp(const void *s1, const void *s2, std::size_t n) {
    const std::uint8_t *p1 = static_cast<const std::uint8_t *>(s1);
    const std::uint8_t *p2 = static_cast<const std::uint8_t *>(s2);

    for (std::size_t i = 0; i < n; i++) {
        if (p1[i] != p2[i]) {
            return p1[i] < p2[i] ? -1 : 1;
        }
    }

    return 0;
}

}

// Halt and catch fire function.
namespace {

void hcf() {
    for (;;) {
#if defined (__x86_64__)
        asm ("hlt");
#elif defined (__aarch64__) || defined (__riscv)
        asm ("wfi");
#elif defined (__loongarch64)
        asm ("idle 0");
#endif
    }
}

}

// The following stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should not be removed, unless you know what you are doing.
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { hcf(); }
    void *__dso_handle;
}

// Extern declarations for global constructors array.
extern void (*__init_array[])();
extern void (*__init_array_end[])();

// The following will be our kernel's entry point.
// If renaming kmain() to something else, make sure to change the
// linker script accordingly.
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

    // RGB lines
    for (std::size_t i = 500; i < 800; i++) {
        volatile std::uint32_t *fb_ptr = static_cast<volatile std::uint32_t *>(framebuffer->address);
        fb_ptr[i * (framebuffer->pitch / 4) + (i - 5*5)] = 0xFF0000; // Red
        fb_ptr[i * (framebuffer->pitch / 4) + (i - 10*5)] = 0x00FF00; // Green
        fb_ptr[i * (framebuffer->pitch / 4) + (i - 15*5)] = 0x0000FF; // Blue

        // fb_ptr[i * (framebuffer->pitch / 4) + (i - 20*5)] = 0xFF0000; // Red
        // fb_ptr[i * (framebuffer->pitch / 4) + (i - 25*5)] = 0x00FF00; // Green
        // fb_ptr[i * (framebuffer->pitch / 4) + (i - 30*5)] = 0x0000FF; // Blue

        // fb_ptr[i * (framebuffer->pitch / 4) + (i - 35*5)] = 0xFF0000; // Red
        // fb_ptr[i * (framebuffer->pitch / 4) + (i - 40*5)] = 0x00FF00; // Green
        // fb_ptr[i * (framebuffer->pitch / 4) + (i - 45*5)] = 0x0000FF; // Blue
    }

    uint64_t hhdm_offset = hhdm_request.response->offset;

    kout << "[Mem] HHDM offset: 0x" << base::hex << hhdm_offset << newline;

    Memory::HHDMBase = hhdm_offset;

    if (!system_table_request. response || !system_table_request.response->address) {
        kerr << "[Efi] EFI System Table not supported" << newline;
    } else {
        kout << "[Efi] EFI system table at 0x" << base::hex << (uint64_t)system_table_request.response->address << newline;
    }

    if (memmap_request.response != nullptr) {
        auto result = Memory::Scan(memmap_request.response);
        auto allocator = Memory::PageAllocator(result);

        kout << "[Mem] Creating PageAllocator for system conventional memory" << newline;
        Memory::KernelPFA = &allocator;
    } else {
        Panic("Guru Meditation Error: System memory map missing!", System::Registers{});
    }

    
    hcf();
}
