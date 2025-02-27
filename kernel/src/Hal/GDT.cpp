/*
    * gdt.hpp
*/

#include "GDT.hpp"
#include "../KernelTerminal/terminal.hpp"

// Limine loads a GDT of course, (CS = 0x28) but we will need to make a TSS someday... therefore we load our own now

namespace Hal {
    using namespace Kt;

    GDTPointer gdtPointer{};
    BasicGDT kernelGDT{};
    
    void PrepareGDT() {
        kout << "[Hal] GDT at " << base::hex << (uint64_t)&kernelGDT << "\n";
        kernelGDT = {
            // Code segment offset 0x08
            // Data segment offset 0x10
                
            // Not sure if having LimitLow set to 0xFFFF for the Null segment is kosher
            {0xFFFF, 0, 0, 0x00, 0x00, 0},
    
            // Kernel code/data
            {0xFFFF, 0, 0, 0x9A, 0xA0, 0},
            {0xFFFF, 0, 0, 0x92, 0xA0, 0},
    
            // User code/data
            {0xFFFF, 0, 0, 0x9A, 0xA0, 0},
            {0xFFFF, 0, 0, 0x92, 0xA0, 0},
    
            // One day this will point to our actual TSS
            {
                // Limit = sizeof(TSS) - 1
                0,
    
                // Base = &TSS
                0,
                0,
                
                // Access byte = 0xFA
                0xFA,
    
                // Granularity = 0x00
                0x00,
    
                0x0
            }
        };
    
        gdtPointer = GDTPointer{
            .Size = sizeof(kernelGDT) - 1,
            .GDTAddress = (uint64_t)&kernelGDT
        };
    }
    
    // Helpers implemented in gdt.asm
    extern "C" void LoadGDT(GDTPointer *gdtPointer);
    extern "C" void ReloadSegments();
    
    void BridgeLoadGDT() {
        // Puts the GDT pointer structure into the GDTR
        kout << "[Hal] Setting GDTR" << Kt::newline;
        LoadGDT(&gdtPointer);
    
        kout << "[Hal] Reloading segments" << Kt::newline;
        ReloadSegments();
    }
};