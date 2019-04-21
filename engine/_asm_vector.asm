global _asm_short_add
global _asm_short_div
global _asm_short_sub
global _asm_sub
global _asm_add
global _asm_mul

_asm_short_add:
    test rdx, rdx
    jz .pre
    lea rax, [rdi + 8 * rdx]
    xor r8, r8
    clc
.wl:
    add [rdi], rsi
    mov rsi, 0
    setc sil
    test rsi, rsi
    jz .pre
    lea rdi, [rdi + 8]
    cmp rdi, rax
    jne .wl
    mov rax, rsi
    ret
.pre:
    xor rax, rax
    ret

_asm_short_div:
    lea rcx, [rdi - 8]
    lea rdi, [rdi + 8 * rdx - 8]
    xor rdx, rdx
.wl:
    mov rax, [rdi]
    div rsi
    mov [rdi], rax
    lea rdi, [rdi - 8]
    cmp rdi, rcx
    jne .wl
    mov rax, rdx
    ret

_asm_short_sub:
    test rdx, rdx
    jz .pre
    lea rax, [rdi + 8 * rdx]
    clc
.wl:
    sub [rdi], rsi
    setc sil
    test rsi, rsi
    jz .re
    lea rdi, [rdi + 8]
    cmp rdi, rax
    jne .wl
.re:
    mov rax, rsi
    ret
.pre:
    xor rax, rax
    ret

_asm_sub:
    test rdx, rdx
    jz .re
    clc
.wl:
    mov rax, [rsi]
    sbb [rdi], rax
    lea rdi, [rdi + 8]
    lea rsi, [rsi + 8]
    dec rdx
    jnz .wl
.re:
    mov rax, 0
    setc al
    ret

_asm_add:
    test rdx, rdx
    jz .re
    clc
.wl:
    mov rax, [rsi]
    adc rax, [rdi]
    mov [rdi], rax
    lea rdi, [rdi + 8]
    lea rsi, [rsi + 8]
    dec rdx
    jnz .wl
.re:
    mov rax, 0
    setc al
    ret

;r10 ovefrlow
;rsi - current first argument pointer
;r9 - current second argument pointer
;rcx - current first argument leftsize
;rbx - current second argument leftsize
;rdi - current result pointer
;r11 - result pointer
; [rdi] = [rsi] * [rdx], |[rsi]| = rcx, |[rdx]| = r8     <-   input pointers
; [rdi] = [rsi] * [r9],  |[rsi]| = rcx, |[r9]| = rbx     <-   rntm pointers
_asm_mul:
    push rbx
    xor r10, r10 ; zero overflow
    mov r9, rdx ; set r9 to second argument pointer
    mov r11, rdi ; set r11 to result pointer
    mov rbx, r8 ; set rbx to second argument size
    push r9
.for:
    mov rax, [rsi] ; rax = [rsi]
    mul qword[r9] ; rdx|rax = [rsi] * [r9] - cur first * cur second
    add rax, r10 ; rax += r10 - overflow
    adc rdx, 0 ; rdx is ovfl in future
    add rax, [rdi] ; rax += [rdi] - old value
    adc rdx, 0 ; rdx ovfl
    mov [rdi], rax ; [rdi] = rax - result
    mov r10, rdx ; save overflow
    lea r9, [r9 + 8] ; move second arg pointer
    lea rdi, [rdi + 8] ; move current result pointer
    dec rbx
    jnz .for ; if second argument is not over, repeat
.wl:
    test r10, r10
    jz .cont
    add [rdi], rdx
    mov r10, 0
    setc r10b
    jmp .wl
.cont:
    dec rcx
    jz .re ; if first argument is over, we're done, else we go to next digit in first argument
    xor r10, r10 ; zero overflow
    lea r11, [r11 + 8] ; increase base result pointer
    mov rdi, r11 ; set new result pointer
    mov r9, [rsp]
    lea rsi, [rsi + 8] ; move first argument pointer
    mov rbx, r8 ; set second argument length to initial value
    jmp .for ; go with pushing r9
.re:
    pop r9
    pop rbx
    xor rax, rax
    ret
