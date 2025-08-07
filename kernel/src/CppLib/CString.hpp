/*
    * CString.cpp
    * C++ string tooling
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <CppLib/Vector.hpp>

typedef const char* CString;

namespace kcp {
    vector<const char*> splitstr(CString str, char delimiter);
};