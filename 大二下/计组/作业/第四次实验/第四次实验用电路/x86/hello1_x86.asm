.486
.model flat, stdcall
option casemap :none   		
include D:\masm32\include\windows.inc
include D:\masm32\include\masm32.inc
include D:\masm32\include\kernel32.inc
include D:\masm32\macros\macros.asm
includelib D:\masm32\lib\masm32.lib
includelib D:\masm32\lib\kernel32.lib

.code
start:
    print "Hello World!"
    exit
end start