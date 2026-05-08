; ============================================
; Mini-C Compiler Output — x86 Assembly
; Generated automatically from Mini-C source
; ============================================

section .data
    x dd 0
    y dd 0
    c dd 0
    t3 dd 0
    fmt_int db "%d", 0
    fmt_float db "%f", 0
    fmt_char db "%c", 0
    fmt_str db "%s", 0
    newline db 10, 0

section .text
    global _main
    extern _printf


_main:
    push ebp
    mov ebp, esp
    mov eax, 42
    mov [x], eax
    mov eax, 100
    mov [y], eax
    mov eax, ['A']
    mov [c], eax
    push "Value of x: "
    call _printf
    add esp, 4
    mov eax, [x]
    push eax
    push fmt_int
    call _printf
    add esp, 8
    push newline
    call _printf
    add esp, 4
    push "Sum: "
    call _printf
    add esp, 4
    mov eax, [x]
    mov ebx, [y]
    add eax, ebx
    mov [t3], eax
    mov eax, [t3]
    push eax
    push fmt_int
    call _printf
    add esp, 8
    push newline
    call _printf
    add esp, 4
    push "Char: "
    call _printf
    add esp, 4
    mov eax, [c]
    push eax
    push fmt_char
    call _printf
    add esp, 8
    push newline
    call _printf
    add esp, 4
    push "Hello World!\n"
    call _printf
    add esp, 4
    mov eax, 0
    mov esp, ebp
    pop ebp
    ret
    mov esp, ebp
    pop ebp
    ret

