/*
    * memory.hpp
    * Memory function signatures
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <cstdint>

// Signatures, defined in main.cpp, probably will move sometime soon
extern "C"
{
    void *memcpy(void *dest, const void *src, std::size_t n);
    void *memset(void *s, int c, std::size_t n);
    void *memmove(void *dest, const void *src, std::size_t n);
    int memcmp(const void *s1, const void *s2, std::size_t n);    
}