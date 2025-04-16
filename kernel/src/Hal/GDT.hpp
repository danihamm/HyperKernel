/*
    * gdt.hpp
    * Intel Global Descriptor Table
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>

using namespace std;

namespace Hal {
    class GDTEntry {
        public:
            uint16_t LimitLow;
            uint16_t BaseLow;
            uint8_t BaseMiddle;
            uint8_t AccessByte;
            uint8_t GranularityByte;
            uint8_t BaseHigh;        
        }__attribute__((packed));
        
        struct BasicGDT {
            GDTEntry Null;        
            GDTEntry KernelCode;        
            GDTEntry KernelData;        
            GDTEntry UserCode;        
            GDTEntry UserData;
        
            // Task State Segment
            GDTEntry TSS;
        }__attribute__((packed));
        
        struct GDTPointer {
            uint16_t Size;
            uint64_t GDTAddress;
        }__attribute__((packed));
        
        void BridgeLoadGDT();
        void PrepareGDT();
};