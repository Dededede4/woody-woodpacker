
section .text

global start
global main

start:
  call main
  ret

main:
  push rbp
  mov rbp, rsp

  sub rsp, 16  ; on se réserve 16 octets

  push 0x0A    ; les adresses de la stacks sont décroissantes
  push '.'
  push '.'
  push '.'
  push '.'
  push '.'
  push 'Y'
  push 'D'
  push 'O'
  push 'O'
  push 'W'
  push '.'
  push '.'
  push '.'
  push '.'


  mov rax, 1
  mov rdi, 1
  mov rsi, rsp      ; on affiche de la plus petite adresse à la plus grande
  mov rdx, 0x78     ; nombre de bits (??????)
  syscall           ; );

  mov rsp, rbp
  mov rax, 60       ; exit(
  mov rdi, 0        ;   EXIT_SUCCESS
  syscall           ; );
