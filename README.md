# OS220
Research kernel in C++

### Implemented (kernel)
- Global Descriptor Table
- Output via Flanterm by mintsuki
- Memory map scanning
- Kernel-mode heap allocator

### In progress (kernel)
- ISR handling (interrupts)
- Virtual memory paging
- UEFI runtime service support
- ACPI & AML support

### Implemented (kernel C++ library)
- new/delete operators via kernel heap
- String stream (`cstringstream`)
- Console output stream (`kout`, `kerr`)

### In progress (kernel C++ library)
- Vector/array class
- String class