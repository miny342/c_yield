.intel_syntax noprefix
.globl get
get:
    # callee regを退避
    mov rdx, [rdi]
    mov [rdx], rbx
    mov [rdx+8], rsp
    mov [rdx+16], rbp
    mov [rdx+24], r12
    mov [rdx+32], r13
    mov [rdx+40], r14
    mov [rdx+48], r15

    # 中断していた callee regを復元
    mov rdx, [rdi+8]
    mov rbx, [rdx]
    mov rsp, [rdx+8]
    mov rbp, [rdx+16]
    mov r12, [rdx+24]
    mov r13, [rdx+32]
    mov r14, [rdx+40]
    mov r15, [rdx+48]

    # Contextのjumpaddrに飛ぶ
    jmp [rdi+16]
    ud2

.globl yield
yield:
    # 次に再開するときのjumpaddrを設定
    pop rdx
    mov [rdi+16], rdx

    # 中断として callee save regを退避
    mov rdx, [rdi+8]
    mov [rdx], rbx
    mov [rdx+8], rsp
    mov [rdx+16], rbp
    mov [rdx+24], r12
    mov [rdx+32], r13
    mov [rdx+40], r14
    mov [rdx+48], r15

    # callee save regを復元
    mov rdx, [rdi]
    mov rbx, [rdx]
    mov rsp, [rdx+8]
    mov rbp, [rdx+16]
    mov r12, [rdx+24]
    mov r13, [rdx+32]
    mov r14, [rdx+40]
    mov r15, [rdx+48]

    # 返り値はyieldの第二引数
    mov rax, rsi
    ret

