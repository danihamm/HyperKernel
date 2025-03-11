# OS220
Research project aiming to create a modern kernel and operating system in C++

![OS220 handling a page fault exception in the kernel](https://i.imgur.com/P0Qz350.png)

### Kernel
- [x] Global Descriptor Table
- [x] Output via Flanterm by mintsuki
- [x] Memory map scanning
- [x] Kernel-mode heap allocator
- [x] ISR handling (interrupts)
- [x] Exception handling
- [ ] Virtual memory paging
- [ ] UEFI runtime service support
- [ ] ACPI & AML support
- [ ] PCI-e
- [ ] HPET timer
- [ ] Object manager
- [ ] Ramdisk
- [ ] Scheduler

### Kernel C++ library
- [x] new/delete operators
- [x] String stream (`cstringstream`)
- [x] Console output stream via `operator<<` (`kout`, `kerr`)
- [x] Vector/array class
- [ ] String class