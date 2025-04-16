/*
    * Util.hpp
    * Declarations of platform utility routines
    * Copyright (c) Daniel Hammer, Limine Contributors (via Limine C++ example)
*/

#pragma once

// Halt and catch fire function.
namespace Hal {
    __attribute__((noreturn)) void Halt();
};