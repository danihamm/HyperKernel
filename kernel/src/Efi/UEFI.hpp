/*
    * UEFI.hpp
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>

namespace Efi {
    typedef void* EFI_HANDLE;

    struct TableHeader {
        std::uint64_t Signature;
        std::uint32_t Revision;
        std::uint32_t HeaderSize;
        std::uint32_t CRC32;
        std::uint32_t Reserved;
    }__attribute__((packed));

    struct SystemTable {
        TableHeader Header;
        void* FirmwareVendor; // Pointer to a CHAR16 string of the fw vendor name string
        std::uint32_t FirmwareRevision;
        
        EFI_HANDLE ConsoleInHandle;
        void* ConIn;
        EFI_HANDLE ConsoleOutHandle;
        void* ConOut;

        EFI_HANDLE StandardErrorHandle;
        void* StdErr;

        // Jackpot
        void *RuntimeServices;

        void *BootServices;

        std::uint64_t NumberOfTableEntries;

        void *ConfigurationTable;
    };
};