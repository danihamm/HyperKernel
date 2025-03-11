#pragma once
#include <cstdint>

using namespace std;

#if defined (__x86_64__)
namespace System {
    struct PanicFrame {
        uint64_t IP;
        uint64_t CS;
        uint64_t Flags;
        uint64_t SP;
        uint64_t SS;

        uint8_t InterruptVector;
    }__attribute__((packed));

    struct PageFaultErrorCode {
        uint8_t Present : 1;
        uint8_t Write : 1;
        uint8_t User : 1;
        uint8_t ReservedWrite : 1;
        uint8_t InstructionFetch : 1;
        uint8_t ProtectionKey : 1;
        uint8_t ShadowStack : 1;
        uint8_t SGX : 1;

        uint64_t Reserved : 56;
    }__attribute__((packed));

    struct PageFaultPanicFrame {
        PageFaultErrorCode PageFaultError;
        uint64_t IP;
        uint64_t CS;
        uint64_t Flags;
        uint64_t SP;
        uint64_t SS;

        uint8_t InterruptVector;
    }__attribute__((packed));

    struct GPFPanicFrame {
        uint64_t GeneralProtectionFaultError;
        uint64_t IP;
        uint64_t CS;
        uint64_t Flags;
        uint64_t SP;
        uint64_t SS;

        uint8_t InterruptVector;
    }__attribute__((packed));

};
#endif