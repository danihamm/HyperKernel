/*
    * PortIo.hpp
    * Inline Intel port IO functions
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>

namespace Io {
    // OSDev website has the parameters for out(b/w/l) in the opposite order in their example,
    // (port, then value), but the manpage on GNU ('man outb') has it in this order.

    inline void Out8(uint8_t value, uint16_t port) {
        asm ("outb %0, %1" : : "a"(value), "Nd"(port) : "memory");
    }

    inline void Out16(uint16_t value, uint16_t port) {
        asm ("outw %0, %1" : : "a"(value), "Nd"(port) : "memory");
    }

    inline void Out32(uint32_t value, uint16_t port) {
        asm ("outl %0, %1" : : "a"(value), "Nd"(port) : "memory");
    }

    /* TODO fix */
    // inline uint8_t In8(uint16_t port) {
    //     asm ("inb %0" : : "Nd"(port) : "memory");
    // }

    // inline uint16_t In16(uint16_t port) {
    //     asm ("inw %0" : : "Nd"(port) : "memory");
    // }

    // inline uint32_t In32(uint16_t port) {
    //     asm ("inl %0" : : "Nd"(port) : "memory");
    // }

    inline void IoPortWait() {
        Out8(0x80, 0);
    }
};