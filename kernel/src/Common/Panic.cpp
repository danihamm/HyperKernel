#include "Panic.hpp"

void Panic(const char *meditationString, System::PanicFrame* frame) {
    kerr << "\nGuru Meditation" << "\n" << "\n";
    
    kerr << "\t" << "MeditationString: " << meditationString << Kt::newline;

#if defined (__x86_64__)
    if (frame != nullptr) {
        if (frame->InterruptVector == 0xE) { // In case of #PF the CPU pushes an error code to the frame
            auto pf_frame = (System::PageFaultPanicFrame*)frame;
            frame = (System::PanicFrame*)&pf_frame->IP;

            kerr << "\t" << "InterruptVector: " << "0x" << base::hex << 0xE << "\n";

            /* Page fault error details */
            kerr << "\t" << "PageFaultPresent: " << base::dec << pf_frame->PageFaultError.Present << "\n";
            kerr << "\t" << "PageFaultWrite: " << base::dec << pf_frame->PageFaultError.Write << "\n";
            kerr << "\t" << "PageFaultUser: " << base::dec << pf_frame->PageFaultError.User << "\n";
            kerr << "\t" << "PageFaultReservedWrite: " << base::dec << pf_frame->PageFaultError.ReservedWrite << "\n";
            kerr << "\t" << "PageFaultInstructionFetch: " << base::dec << pf_frame->PageFaultError.InstructionFetch << "\n";
            kerr << "\t" << "PageFaultProtectionKey: " << base::dec << pf_frame->PageFaultError.ProtectionKey << "\n";
            kerr << "\t" << "PageFaultShadowStack: " << base::dec << pf_frame->PageFaultError.ShadowStack << "\n";
            kerr << "\t" << "PageFaultSGX: " << base::dec << pf_frame->PageFaultError.ShadowStack << "\n";
        }
        else
        {
            kerr << "\t" << "InterruptVector: " << "0x" << base::hex << frame->InterruptVector << "\n";
        }

        kerr << "\t" << "InstructionPointer: " << "0x" << base::hex << frame->IP << "\n";
        kerr << "\t" << "CodeSegment: " << "0x" << base::hex << frame->CS << "\n";
        kerr << "\t" << "Flags: " << "0x" << base::hex << frame->Flags << "\n";
        kerr << "\t" << "StackPointer: " << "0x" << base::hex << frame->SP << "\n";   
        kerr << "\t" << "StackSegment: " << "0x" << base::hex << frame->SS << "\n";   
    }
#endif
    while (true) {
        #if defined (__x86_64__)
        asm ("cli");
        asm ("hlt");
        #elif defined (__aarch64__) || defined (__riscv)
        asm ("wfi");
        #elif defined (__loongarch64)
        asm ("idle 0");
        #endif
    }
}