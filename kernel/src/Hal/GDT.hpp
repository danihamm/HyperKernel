/*
    * gdt.hpp
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>

using namespace std;

// __attribute__((packed)) is the GCC extensions way of telling the compiler to ensure that it doesn't mess with these structures or add packing bytes
// for optimization because that would easily result in a triple fault.

namespace Hal {
    class GDTEntry {
        public:
            // Base/Limit are obsolete in Long mode because segmentation is no longer used
            // Sadly we must still build and load the GDT during kernel init
        
            uint16_t LimitLow;
            uint16_t BaseLow;
            uint8_t BaseMiddle;
        
            // Determine which processor rings this segment can be used in
            uint8_t AccessByte;
            // Lower 4 bits are the higher 4 bits of limit
            uint8_t GranularityByte;
        
            uint8_t BaseHigh;
        
            // 16 + 16 + 8 + 8 = 48 bits
        }__attribute__((packed));
        
        struct BasicGDT {
            // Conventionally the first entry of the GDT has all values zeroed out.
            GDTEntry Null;
        
            // Kernel code segment descriptor
            GDTEntry KernelCode;
        
            // Kernel data segment descriptor
            GDTEntry KernelData;
        
            // UM code segment descriptor
            GDTEntry UserCode;
        
            // UM data segment descriptor
            GDTEntry UserData;
        
            // Task State Segment
            GDTEntry TSS;
        }__attribute__((packed));
        
        // Simple structure that tells the CPU the size of the GDT, and it's address
        struct GDTPointer {
            uint16_t Size;
            uint64_t GDTAddress;
        }__attribute__((packed));
        
        void BridgeLoadGDT();
        void PrepareGDT();
};