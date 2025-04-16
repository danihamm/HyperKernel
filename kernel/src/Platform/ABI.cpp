/*
    * Itanium.cpp
    * Definitions for the Itanium C++ ABI
    * Copyright (c) Daniel Hammer, Limine Contributors (via Limine C++ example)
*/

#include <Platform/Util.hpp>

// The following stubs are required by the Itanium C++ ABI (the one we use,
// regardless of the "Itanium" nomenclature).
// Like the memory functions above, these stubs can be moved to a different .cpp file,
// but should not be removed, unless you know what you are doing.
extern "C" {
    int __cxa_atexit(void (*)(void *), void *, void *) { return 0; }
    void __cxa_pure_virtual() { Hal::Halt(); }
    void *__dso_handle;
}
