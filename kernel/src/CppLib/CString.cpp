/*
    * CString.cpp
    * C++ string tooling
    * Copyright (c) 2025 Daniel Hammer
*/

#include "CString.hpp"
#include "Stream.hpp"

#include <Libraries/Memory.hpp>
#include <Memory/Heap.hpp>

namespace kcp {
    vector<const char*> splitstr(const char* str, char delimiter) {
        vector<const char*> result;
        cstringstream current_stream = cstringstream();

        while (true) {
            if (*str == delimiter || *str == '\0') {
                auto cstr = current_stream.c_str();
                auto cstr_len = Lib::strlen(cstr);

                result.push_back((const char*)Memory::g_heap->Request(sizeof(char) * (cstr_len + 1)));

                memcpy((void*)result.at(result.size() - 1), cstr, cstr_len + 1);

                if (*str == '\0') {
                    break;
                }

                current_stream = cstringstream();
                str++;
                continue;
            }

            current_stream << (char)*str;
            str++;
        };
        
        return result;
    }
};