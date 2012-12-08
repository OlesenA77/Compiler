;;;Assembly output of mtp
	extern read
	extern write
	section .data
	section .bss
	section .text
	global main
main:
	push ebp
	mov ebp, esp
	sub esp, 4	;variable flog
	sub esp, 4	;variable blarg
	;; Write Statement
	mov eax, dword [ebp - 4]
	sub esp, 4
	mov [ebp - 12], eax
	sub esp, 4
	mov dword [ebp - 16], 1
	call write
	add esp, 8
	;; Write Statement
	mov eax, dword [ebp - 4]
	sub esp, 4
	mov [ebp - 12], eax
	mov eax, dword [ebp - 8]
	sub esp, 4
	mov [ebp - 16], eax
	sub esp, 4
	mov dword [ebp - 20], 2
	call write
	add esp, 12
	mov esp, ebp
	pop ebp
	ret