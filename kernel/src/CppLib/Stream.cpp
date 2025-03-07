/*
    * stream.cpp
    * String stream/string writer
    * Copyright (c) 2025 Daniel Hammer
*/

#include "Stream.hpp"
#include <Memory/Heap.hpp>
#include <Terminal/terminal.hpp>
#include <Libraries/string.hpp>

kcp::cstringstream::cstringstream()
{
    this->string = nullptr;
    this->size = 0;
}

kcp::cstringstream::~cstringstream()
{
    delete this->string;
}

kcp::cstringstream& kcp::cstringstream::operator<<(char c) {
    this->string = (const char *)Memory::g_heap->Realloc((void *)this->string, this->size + 1);

    if (!this->string)
    {
        kerr << "Streaming failed due to failed allocation" << Kt::newline;
        return *this;
    }
    
    char* ref = (char *)&string[size];
    *ref = c;

    ref++;
    *ref = '\0';

    this->size++;

    return *this;
}

kcp::cstringstream& kcp::cstringstream::operator<<(char* str) {
    while (*str != '\0')
    {
        *this << *str;
        str++;
    }

    return *this;
}

kcp::cstringstream& kcp::cstringstream::operator<<(int num) {
    char* out_str = Lib::int2basestr(num, current_base);

    *this << out_str;
    return *this;
}

kcp::cstringstream& kcp::cstringstream::operator<<(uint32_t val) {
    char* out_str = Lib::uint2basestr(val, current_base);

    *this << out_str;
    return *this;
}

kcp::cstringstream& kcp::cstringstream::operator<<(uint64_t val) {
    char* out_str = Lib::u64_2_basestr(val, current_base);

    *this << out_str;
    return *this;
}

kcp::cstringstream& kcp::cstringstream::operator<<(base nb)
{
    current_base = nb;
}

const char* kcp::cstringstream::cstr() {
    return this->string;
}