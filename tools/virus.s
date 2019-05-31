
section .text

global start

start:

  push rbp
  push rsp
  mov rbp, rsp

  push rax
  push rdi
  push rsi
  push rdx

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

  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax
  pop rax

  pop rdx
  pop rsi
  pop rdi
  pop rax

  pop rsp
  pop rbp
