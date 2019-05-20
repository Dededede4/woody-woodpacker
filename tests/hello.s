
section .text

global start
global main

start:
  call main
  ret

msg: db '....WOODY.....', 0x0a
main:
  push rbp
  mov rbp, rsp
  mov rax, 1
  mov rdi, 1
  mov rsi, msg 
  mov rdx, 0x0f     ; nombre de bits (??????)
  syscall           ; );
  mov rsp, rbp
  mov rax, 60       ; exit(
  mov rdi, 0        ;   EXIT_SUCCESS
  syscall           ; );
