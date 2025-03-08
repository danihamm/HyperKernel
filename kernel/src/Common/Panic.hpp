#pragma once
#include <Platform/Registers.hpp>
#include <Terminal/terminal.hpp>

void Panic(const char *meditationString, System::PanicFrame* frame);