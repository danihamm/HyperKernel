#pragma once
#include <cstdint>
#include <Terminal/Terminal.hpp>

namespace Memory::VMM {
    struct PageTableEntry {
        std::uint8_t Present : 1;
        std::uint8_t Writable : 1;
        std::uint8_t Supervisor : 1;
        std::uint8_t WriteThrough : 1;
        std::uint8_t CacheDisabled : 1;
        std::uint8_t Accessed : 1;
        std::uint8_t Ignore : 1;
        std::uint8_t LargerPages : 1;
        std::uint8_t PageSize : 1;
        std::uint8_t Available : 3;
        std::uint64_t Address : 52;
    };

    struct PageTableEntry40Bit {
        std::uint8_t Present : 1;
        std::uint8_t Writable : 1;
        std::uint8_t Supervisor : 1;
        std::uint8_t WriteThrough : 1;
        std::uint8_t CacheDisabled : 1;
        std::uint8_t Accessed : 1;
        std::uint8_t Ignore : 1;
        std::uint8_t LargerPages : 1;
        std::uint8_t PageSize : 1;
        std::uint8_t Available : 3;
        std::uint64_t Address : 40;
        std::uint8_t AvailableHigh : 7;
        std::uint8_t PK : 4;
        std::uint8_t NX : 1;
    };

    
    struct PageTable {
        PageTableEntry entries[512];
    } __attribute__((packed)) __attribute__((aligned(0x1000)));

    struct VirtualAddress {
        std::uint64_t address;

        VirtualAddress(std::uint64_t newAddress) {
            if (newAddress % 0x1000 != 0) {
                Kt::KernelLogStream(Kt::WARNING, "VMM") << "VirtualAddress object created with non-aligned value.";
            }

            address = newAddress;
        }

        uint64_t GetL4Index() {
            return (address >> 39) & 0x1ff;
        }

        uint64_t GetL3Index() {
            return (address >> 30) & 0x1ff;
        }

        uint64_t GetL2Index() {
            return (address >> 21) & 0x1ff;
        }

        uint64_t GetPageIndex() {
            return (address >> 12) & 0x1ff;
        }

        uint64_t GetIndex(size_t level) {
            if (level == 4)
                return GetL4Index();

            else if (level == 3)
                return GetL3Index();

            else if (level == 2)
                return GetL2Index();

            else if (level == 1)
                return GetPageIndex();
        }
    };

    class Paging {
        PageTable* PML4{};

        PageTable* HandleLevel(VirtualAddress virtualAddress, PageTable* table, size_t level);
public:
        Paging();
        void Init(std::uint64_t kernelBaseVirt, std::uint64_t kernelSize);
        void Map(std::uint64_t physicalAddress, std::uint64_t virtualAddress);
        static std::uint64_t GetPhysAddr(std::uint64_t PML4, std::uint64_t virtualAddress, bool use40BitL1 = false);
        std::uint64_t GetPhysAddr(std::uint64_t virtualAddress);
    };

    extern "C" uint64_t GetCR3();
    extern "C" void LoadCR3(PageTable* PML4);
};