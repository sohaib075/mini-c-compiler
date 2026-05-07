; ============================================
; Mini-C Compiler Output — x86 Assembly
; Generated automatically from Mini-C source
; ============================================

section .data
    t1 dd 0
    n dd 0
    t2 dd 0
    t4 dd 0
    x dd 0
    a dd 0
    t5 dd 0
    sum dd 0
    i dd 0
    t9 dd 0
    t11 dd 0
    t13 dd 0
    t14 dd 0
    t16 dd 0
    fmt_int db "%d", 0
    fmt_float db "%f", 0
    fmt_char db "%c", 0
    fmt_str db "%s", 0
    newline db 10, 0

section .text
    global _main
    extern _printf


_abs_val:
    push ebp
    mov ebp, esp
    mov eax, [n]
    mov ebx, 0
    cmp eax, ebx
    setl al
    movzx eax, al
    mov [t1], eax
    mov eax, [t1]
    cmp eax, 0
    je L0
    mov eax, [n]
    neg eax
    mov [t2], eax
    mov eax, [t2]
    mov esp, ebp
    pop ebp
    ret
L0:
    mov eax, [n]
    mov esp, ebp
    pop ebp
    ret
    mov esp, ebp
    pop ebp
    ret


_main:
    push ebp
    mov ebp, esp
    mov eax, 7
    neg eax
    mov [t4], eax
    mov eax, [t4]
    mov [x], eax
    mov eax, [x]
    push eax
    call _abs_val
    add esp, 4
    mov [t5], eax
    mov eax, [t5]
    mov [a], eax
    push "Absolute: "
    call _printf
    add esp, 4
    mov eax, [a]
    push eax
    push fmt_int
    call _printf
    add esp, 8
    push newline
    call _printf
    add esp, 4
    mov eax, 0
    mov [sum], eax
    mov eax, 0
    mov [i], eax
L1:
    mov eax, [i]
    mov ebx, 10
    cmp eax, ebx
    setl al
    movzx eax, al
    mov [t9], eax
    mov eax, [t9]
    cmp eax, 0
    je L2
    mov eax, [i]
    cdq
    mov ebx, 2
    idiv ebx
    mov [t11], edx
    mov eax, [t11]
    mov ebx, 0
    cmp eax, ebx
    sete al
    movzx eax, al
    mov [t13], eax
    mov eax, [t13]
    cmp eax, 0
    je L3
    mov eax, [sum]
    mov ebx, [i]
    add eax, ebx
    mov [t14], eax
    mov eax, [t14]
    mov [sum], eax
L3:
    mov eax, [i]
    mov ebx, 1
    add eax, ebx
    mov [t16], eax
    mov eax, [t16]
    mov [i], eax
    jmp L1
L2:
    push "Sum of evens: "
    call _printf
    add esp, 4
    mov eax, [sum]
    push eax
    push fmt_int
    call _printf
    add esp, 8
    push newline
    call _printf
    add esp, 4
    mov eax, 0
    mov esp, ebp
    pop ebp
    ret
    mov esp, ebp
    pop ebp
    ret

