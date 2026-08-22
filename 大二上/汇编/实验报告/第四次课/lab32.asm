data segment
    array db 31H, 21H, 42H, 52H, 87H, 23H, 98H, 01H
    up db 0
    equa db 0
    down db 0

    up_str db 'up: $'
    equa_str db 'equa: $'
    down_str db 'down: $'
    crlf    db 0DH, 0AH, '$'
data ends

code segment
    assume cs:code, ds:data
start:
    mov ax, data
    mov ds, ax

    mov cx, 8
    mov si, 0

loop_compare:
    mov al, array[si]
    cmp al, 42H

    ja count_up
    je count_equa
    jb count_down

count_up:
    inc up
    jmp next

count_equa:
    inc equa
    jmp next

count_down:
    inc down
    jmp next

next:
    inc si
    loop loop_compare

    mov ah, 09H
    lea dx, up_str
    int 21H

    mov ah, 02H
    mov dl, up
    add dl, '0'
    int 21H

    lea dx, crlf
    mov ah, 09H
    int 21H
    
    lea dx, equa_str
    mov ah, 09H
    int 21H

    mov ah, 02H
    mov dl, equa
    add dl, '0'
    int 21H

    lea dx, crlf
    mov ah, 09H
    int 21H
    
    lea dx, down_str
    int 21H

    mov ah, 02H
    mov dl, down
    add dl, '0'
    int 21H

    lea dx, crlf
    mov ah, 09H
    int 21H
    
    mov ah, 4CH
    int 21H
code ends

end start