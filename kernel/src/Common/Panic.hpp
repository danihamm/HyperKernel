#pragma once
#include <System/Registers.hpp>
#include <KernelTerminal/terminal.hpp>

void Panic(const char *meditationString, System::Registers registers);