
section .text

global start

start:

  push rbp
  push rsp
  mov rbp, rsp

  push rax
  push rdi
  push rsi
  push rsp
  push rdx
  push rcx

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

  mov rax, 0x4242424242424242
  mov rcx, 0x4343434343434343 ; longueur à déchifrer
  mov rdx, 0x4444444444444444 ; la clef de déchiffrement

  add rcx, rax

while:
  xor [rax], dl
  ror rdx, 8
  inc rax
  cmp rax, rcx  
  jnz while

pop rcx
pop rdx
pop rsp
pop rsi
pop rdi 
pop rax

  pop rsp
  pop rbp
