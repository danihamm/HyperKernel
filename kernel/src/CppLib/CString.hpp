/*
    * CString.cpp
    * C++ string tooling
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <CppLib/Vector.hpp>

namespace kcp {
    vector<const char*> splitstr(const char* str, char delimiter);
};