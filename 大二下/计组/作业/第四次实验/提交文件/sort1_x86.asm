.486
.model flat, stdcall
option casemap :none

include D:\masm32\include\windows.inc
include D:\masm32\include\masm32.inc
include D:\masm32\include\kernel32.inc
include D:\masm32\macros\macros.asm

includelib D:\masm32\lib\masm32.lib
includelib D:\masm32\lib\kernel32.lib

.data
array dd 8, 1, 5, 2, 7, 9, 6, 4, 3, 10

.code
start:

    ; 冒泡排序，降序
    mov ecx, 9              ; 外层循环次数

outer_loop:
    push ecx
    mov esi, 0              ; 数组下标偏移
    mov ecx, 9              ; 内层比较次数

inner_loop:
    mov eax, array[esi]
    mov ebx, array[esi+4]

    ; 降序：如果前一个 < 后一个，则交换
    cmp eax, ebx
    jge no_swap

    mov array[esi], ebx
    mov array[esi+4], eax

no_swap:
    add esi, 4
    loop inner_loop

    pop ecx
    loop outer_loop

    ; 输出排序结果
    print chr$("Sorted result descending:", 13, 10)

    mov ecx, 10
    mov esi, 0

print_loop:
    push ecx

    mov eax, array[esi]
    print str$(eax)
    print chr$(" ")

    add esi, 4
    pop ecx
    loop print_loop

    print chr$(13, 10)

    exit

end start