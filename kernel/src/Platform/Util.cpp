/*
    * Util.cpp
    * Platform utility routines
    * Copyright (c) Daniel Hammer, Limine Contributors (via Limine C++ example)
*/

// Halt and catch fire function.
void hcf()
{
    for (;;)
    {
#if defined(__x86_64__)
        asm("hlt");
#elif defined(__aarch64__) || defined(__riscv)
        asm("wfi");
#elif defined(__loongarch64)
        asm("idle 0");
#endif
    }
}