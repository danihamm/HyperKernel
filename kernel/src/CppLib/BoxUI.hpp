/*
    * BoxUI.hpp
    * Copyright (c) 2025 Daniel Hammer
*/

#pragma once

// ANSI escape codes for color and formatting
#define BOXUI_ANSI_RESET        "\033[0m"
#define BOXUI_ANSI_BOLD         "\033[1m"
#define BOXUI_ANSI_RED_BG       "\033[41m"
#define BOXUI_ANSI_WHITE_FG     "\033[97m"
#define BOXUI_H                 "─"
#define BOXUI_V                 "│"
#define BOXUI_TL                "┌"
#define BOXUI_TR                "┐"
#define BOXUI_BL                "└"
#define BOXUI_BR                "┘"

constexpr int BOXUI_LABEL_WIDTH = 28;

// Print a boxed line with padding and optional bold
inline void PrintBoxedLine(auto& out, const char* content, int width, bool bold = false, const char* bg = nullptr) {
    out << BOXUI_V << " ";
    int len = 0;
    if (bold) out << BOXUI_ANSI_BOLD;
    for (const char* p = content; *p && len < width - 6; ++p, ++len) out << *p;
    if (bold) {
        out << BOXUI_ANSI_RESET;
        if (bg) out << bg << BOXUI_ANSI_WHITE_FG;
    }
    for (int i = len; i < width - 4; ++i) out << " ";
    out << " " << BOXUI_V << "\n";
}

// Print a boxed separator line
inline void PrintBoxedSeparator(auto& out, int width) {
    out << BOXUI_V << " ";
    for (int i = 0; i < width - 4; ++i) out << BOXUI_H;
    out << " " << BOXUI_V << "\n";
}

// Print a boxed line with a label and a hex value, aligned
template<typename T>
inline void PrintBoxedHex(auto& out, const char* label, T value, int width) {
    out << BOXUI_V << " ";
    int len = 0;
    for (const char* p = label; *p; ++p, ++len) out << *p;
    for (; len < BOXUI_LABEL_WIDTH; ++len) out << " ";
    out << ": 0x";
    len += 4 + (BOXUI_LABEL_WIDTH - len);
    bool started = false;
    int hexLen = 0;
    for (int i = (sizeof(T) * 2) - 1; i >= 0; --i) {
        char c = ((value >> (i * 4)) & 0xF);
        if (c || started || i == 0) {
            started = true;
            out << (char)(c < 10 ? '0' + c : 'A' + (c - 10));
            ++hexLen;
        }
    }
    len += hexLen;
    for (int i = len; i < width - 4; ++i) out << " ";
    out << " " << BOXUI_V << "\n";
}

// Print a boxed line with a label and a decimal value, aligned
template<typename T>
inline void PrintBoxedDec(auto& out, const char* label, T value, int width) {
    out << BOXUI_V << " ";
    int len = 0;
    for (const char* p = label; *p; ++p, ++len) out << *p;
    for (; len < BOXUI_LABEL_WIDTH; ++len) out << " ";
    out << ": ";
    len += 2 + (BOXUI_LABEL_WIDTH - len);
    char buf[32];
    int idx = 0;
    T v = value;
    if (v == 0) {
        buf[idx++] = '0';
    } else {
        char tmp[32];
        int t = 0;
        while (v > 0 && t < 31) {
            tmp[t++] = '0' + (v % 10);
            v /= 10;
        }
        for (int i = t - 1; i >= 0; --i) buf[idx++] = tmp[i];
    }
    buf[idx] = 0;
    for (int i = 0; i < idx; ++i, ++len) out << buf[i];
    for (int i = len; i < width - 4; ++i) out << " ";
    out << " " << BOXUI_V << "\n";
}