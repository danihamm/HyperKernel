/*
    * Spinlock.cpp
    * C++ Spinlock
    * Copyright (c) 2025 Daniel Hammer
*/

#include "Spinlock.hpp"

namespace kcp {
    void Spinlock::Acquire() {
        while (atomic_flag.test_and_set(std::memory_order_acquire));
    }

    void Spinlock::Release() {
        atomic_flag.clear(std::memory_order_release);
    }
};