section .data
result dd 0

section .text
global SumSquares

SumSquares:
push ebp
mov ebp, esp

mov ecx, [ebp+8]
mov esi, [ebp+12]
xor eax, eax

walkthrough:
lodsd
mul eax
add eax, [result]
mov [result], eax
loop walkthrough

mov eax, [result]
pop ebp
ret
