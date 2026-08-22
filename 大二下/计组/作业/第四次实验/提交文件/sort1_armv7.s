.global _start

_start:
    ldr r4, =array      // r4 = 数组首地址
    mov r0, #9          // 外层循环次数 i = 9

outer_loop:
    cmp r0, #0
    beq stop

    mov r1, #0          // j = 0
    mov r2, r0          // 内层比较次数

inner_loop:
    cmp r1, r2
    beq next_outer

    lsl r3, r1, #2      // r3 = j * 4
    add r5, r4, r3      // r5 = array[j] 地址

    ldr r6, [r5]        // r6 = array[j]
    ldr r7, [r5, #4]    // r7 = array[j+1]

    // 降序：如果 array[j] < array[j+1]，交换
    cmp r6, r7
    bge no_swap

    str r7, [r5]
    str r6, [r5, #4]

no_swap:
    add r1, r1, #1
    b inner_loop

next_outer:
    sub r0, r0, #1
    b outer_loop

stop:
    b stop

.data
array:
    .word 8, 1, 5, 2, 7, 9, 6, 4, 3, 10