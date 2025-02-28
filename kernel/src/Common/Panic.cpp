#include "Panic.hpp"

void Panic(const char *meditationString, System::Registers registers) {
    kerr << "=========== Kernel panic ===========" << Kt::newline;
    kerr << meditationString << Kt::newline;

    while (true) {
        #if defined (__x86_64__)
        asm ("cli");
        asm ("hlt");
        #elif defined (__aarch64__) || defined (__riscv)
        asm ("wfi");
        #elif defined (__loongarch64)
        asm ("idle 0");
        #endif
    }
}