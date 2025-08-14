/*
    * Time.hpp
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once
#include <CppLib/CString.hpp>

namespace Timekeeping {
    const CString Months[] = {
        nullptr,
        "January",
        "February",
        "March",
        "April",
        "May",
        "June",
        "July",
        "August",
        "September",
        "October",
        "November",
        "December"
    };

    void Init(uint16_t Year, uint8_t Month, uint8_t Day, uint8_t Hour, uint8_t Minute, uint8_t Second);
};
