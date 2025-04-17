[bits 64]
section .text
global GetCR3
global LoadCR3

GetCR3:
    mov rax, cr3
    ret

LoadCR3:
    mov cr3, rdi
    ret