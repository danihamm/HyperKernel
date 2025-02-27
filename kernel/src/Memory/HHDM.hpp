#pragma once
#include <cstdint>

namespace Memory {
    extern std::uint64_t HHDMBase;

    inline uint64_t HHDM(uint64_t address) {
        return HHDMBase + address;
    }

    inline uint64_t HHDM(void* address) {
        return HHDMBase + (uint64_t)address;
    }

    inline uint64_t SubHHDM(uint64_t address) {
        return address - HHDMBase;
    }

    inline uint64_t SubHHDM(void* address) {
        return SubHHDM((uint64_t)address);
    }

    inline uint64_t IsHDDMVirtAddr(uint64_t address) {
        if (address > HHDMBase) {
            return true;
        }

        return false;
    }
};