#include "Paging.hpp"
#include <Memory/PageFrameAllocator.hpp>
#include <Common/Panic.hpp>
#include <Memory/HHDM.hpp>

namespace Memory::VMM {
    extern "C" uint64_t KernelStartSymbol;
    
    std::uint64_t GetPhysKernelAddress(std::uint64_t virtualAddress) {
        return Paging::GetPhysAddr(GetCR3(), (std::uint64_t)virtualAddress, true);
    }

    Paging::Paging() {
        PML4 = (PageTable*)SubHHDM((PageTable*)Memory::g_pfa->AllocateZeroed());
    }

    void Paging::Init(std::uint64_t kernelBaseVirt, std::uint64_t kernelSize) {
        // Map kernel
        Kt::KernelLogStream(Kt::DEBUG, "VMM") << "Paging::Init called with kernelBaseVirt as 0x" << base::hex << kernelBaseVirt << " and kernelSize as " << base::dec << kernelSize;

        for (std::uint64_t pageAddr = kernelBaseVirt; pageAddr < (kernelBaseVirt + kernelSize); pageAddr += 0x1000) {
            Kt::KernelLogStream(Kt::DEBUG, "VMM") << "Mapping 0x" << base::hex << GetPhysKernelAddress(pageAddr) << " to 0x" << pageAddr;
            Map(GetPhysKernelAddress(pageAddr), pageAddr);
        }

        // TODO map HHDM
        // TODO map ACPI tables

        LoadCR3((PageTable*)&PML4);
    }

    PageTable* Paging::HandleLevel(VirtualAddress virtualAddress, PageTable* table, const size_t level) {
        PageTableEntry* entry = (PageTableEntry*)Memory::HHDM(&table->entries[virtualAddress.GetIndex(level)]);
        
        entry->Present = true;
        entry->Writable = true;

        uint64_t downLevelAddr = Memory::SubHHDM((uint64_t)Memory::g_pfa->AllocateZeroed());

        entry->Address = downLevelAddr >> 12;

        return (PageTable*)downLevelAddr;
    }

    void Paging::Map(std::uint64_t physicalAddress, std::uint64_t virtualAddress) {
        if (virtualAddress % 0x1000 != 0 || physicalAddress % 0x1000 != 0) {
            Panic("Value that isn't page-aligned passed as address to Paging::Map!", nullptr);
        }

        VirtualAddress virtualAddressObj(virtualAddress);

        auto PML3 = HandleLevel(virtualAddressObj, PML4, 4);
        auto PML2 = HandleLevel(virtualAddressObj, PML3, 3);
        auto PML1 = HandleLevel(virtualAddressObj, PML2, 2);

        PageTableEntry* pageEntry = (PageTableEntry*)Memory::HHDM(&PML1->entries[virtualAddressObj.GetPageIndex()]);

        pageEntry->Present = true;
        pageEntry->Writable = true;

        pageEntry->Address = virtualAddress >> 12;
    }

    std::uint64_t Paging::GetPhysAddr(std::uint64_t pml4, std::uint64_t virtualAddress, bool use40BitL1) {
        VirtualAddress virtualAddressObj(virtualAddress);
        
        PageTable* pml4Virt = (PageTable*)HHDM(pml4);

        PageTableEntry* pml4_entry = &pml4Virt->entries[virtualAddressObj.GetL4Index()];

        PageTable* pml3 = (PageTable*)HHDM(pml4_entry->Address << 12);
        PageTableEntry* pml3_entry = &pml3->entries[virtualAddressObj.GetL3Index()];

        PageTable* pml2 = (PageTable*)HHDM(pml3_entry->Address << 12);
        PageTableEntry* pml2_entry = &pml2->entries[virtualAddressObj.GetL2Index()];

        PageTable* pml1 = (PageTable*)HHDM(pml2_entry->Address << 12);

        if (use40BitL1 == true) {
            PageTableEntry40Bit* pml1_entry = (PageTableEntry40Bit*)&pml1->entries[virtualAddressObj.GetPageIndex()];
            return (uint64_t)pml1_entry->Address << 12;
        }

        PageTableEntry* pml1_entry = &pml1->entries[virtualAddressObj.GetPageIndex()];
        return (uint64_t)pml1_entry->Address << 12;
    }

    std::uint64_t Paging::GetPhysAddr(std::uint64_t virtualAddress) {
        return GetPhysAddr((std::uint64_t)PML4, virtualAddress, false);
    }
};