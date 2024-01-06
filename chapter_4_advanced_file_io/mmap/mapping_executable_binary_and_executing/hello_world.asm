;; This is MacOS specific assembly as the system call number changes based on the platform
;; The actual assembly was written following this: https://youtu.be/sFYFuBzu9Ow?t=7001
;; But it has been adapted to macos system call by looking at the nasm macho64 example 
;; macho64 example: https://github.com/guilhermecaldas/nasm-examples-macho64/blob/master/src/hello.asm

BITS 64
%define SYS_WRITE 0x2000004
%define SYS_EXIT 0x2000001
%define STDOUT 1

_start:
    ;; write system call
    mov rax, SYS_WRITE
    mov rdi, STDOUT
    lea rsi, [rel hello]
    mov rdx, hello_size
    syscall

    ;; exit system call
    mov rax, SYS_EXIT
    mov rdi, 0
    syscall

    ret 


;; 10 is a new line char
hello: db "Hello from Bipin. mmap() and exec assembly..? Hmm... sound like JIT compilation .. Cool!!", 10
;; subtract current address to hello to get the size of hello message
hello_size: equ $ - hello
