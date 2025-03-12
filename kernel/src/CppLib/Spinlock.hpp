/*
    * Spinlock.cpp
    * C++ Spinlock
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <atomic>

namespace kcp {
    class Spinlock {
        std::atomic_flag atomic_flag{ATOMIC_FLAG_INIT};
    public:
        void Aquire();
        void Release();
    };
};