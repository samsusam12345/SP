section .data
	array db 35, 12, 64, 23, 101, 68, 89, 53, 27, 95, 3, 16; входной массив
	array_size equ 12; размер массива
	mask equ 17; маска 010001
	count db 0
	output db '%i', 0xA, 0x0

section .text
	global main; объявление точки входа
	extern printf; функция вывода из библиотеки
;точка входа
main:
	mov esi, array; ссылка на массив
	mov ecx, array_size; размер массива
	xor eax, eax; зануление eax 
	xor ebx, ebx; зануление ebx
;проход по элементам массива
walkthrough:
lodsb; читает один байт из регистра esi в регистр eax
xor eax, mask
and eax, mask
cmp eax, mask; если после выполнения предыдущих операций xor и and выполняется равенство, то число подходит
JNE false; если равенство не выполняется, переход на метку false
inc ebx; если равенство выполняется, то увеличиваем значение ebx на 1

;переход к следующему элементу массива
false:
loop walkthrough; уменьшает на 1 ecx, если после этого он не равен 0, то переход на метку walkthrough

exit:
push ebx
push output
call printf
add esp, 8

mov eax, 1;
mov ebx, 0
int 0x80
