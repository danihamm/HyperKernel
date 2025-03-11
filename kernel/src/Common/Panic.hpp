#pragma once
#include <Platform/Registers.hpp>
#include <Terminal/Terminal.hpp>

void Panic(const char *meditationString, System::PanicFrame* frame);