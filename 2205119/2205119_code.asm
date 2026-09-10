format ELF executable 4

entry start

segment readable writeable

buffer db '0000000000',10


segment readable executable

println:
push eax
push ebx
push ecx
push edx
push esi
push edi
sub esp,32
test eax,eax
jns print_positive
push eax
sub esp,1
mov byte [esp],'-'
mov eax,4
mov ebx,1
mov ecx,esp
mov edx,1
int 0x80
add esp,1
pop eax
neg eax
print_positive:
mov ebx,10
lea esi,[esp+31]
mov byte [esi],10
dec esi
cmp eax,0
jne convert_loop
mov byte [esi],'0'
dec esi
jmp finish_convert
convert_loop:
xor edx,edx
div ebx
add dl,'0'
mov [esi],dl
dec esi
test eax,eax
jnz convert_loop
finish_convert:
inc esi
lea edx,[esp+32]
sub edx,esi
mov eax,4
mov ebx,1
mov ecx,esi
int 0x80
add esp,32
pop edi
pop esi
pop edx
pop ecx
pop ebx
pop eax
ret

start:

call func_main
mov eax,1
xor ebx,ebx
int 0x80

func_func:
push ebp
mov ebp,esp
sub esp,4
mov eax,[ebp+8]
push eax
mov eax,0
pop ebx
cmp ebx,eax
je L2
mov eax,0
jmp L3
L2:
mov eax,1
L3:
cmp eax,0
je L1
mov eax,0
jmp L0
L1:
mov eax,[ebp+8]
mov [ebp-4],eax
mov eax,[ebp+8]
push eax
mov eax,1
pop ebx
sub ebx,eax
mov eax,ebx
push eax
call func_func
add esp,4
push eax
mov eax,[ebp-4]
pop ebx
add ebx,eax
mov eax,ebx
jmp L0
L0:
add esp,4
pop ebp
ret
func_func2:
push ebp
mov ebp,esp
sub esp,4
mov eax,[ebp+8]
push eax
mov eax,0
pop ebx
cmp ebx,eax
je L6
mov eax,0
jmp L7
L6:
mov eax,1
L7:
cmp eax,0
je L5
mov eax,0
jmp L4
L5:
mov eax,[ebp+8]
mov [ebp-4],eax
mov eax,[ebp+8]
push eax
mov eax,1
pop ebx
sub ebx,eax
mov eax,ebx
push eax
call func_func
add esp,4
push eax
mov eax,[ebp-4]
pop ebx
add ebx,eax
mov eax,ebx
jmp L4
L4:
add esp,4
pop ebp
ret
func_main:
push ebp
mov ebp,esp
sub esp,4
mov eax,7
push eax
call func_func
add esp,4
mov [ebp-4],eax
mov eax,[ebp-4]
call println
mov eax,0
jmp L8
L8:
add esp,4
pop ebp
ret
