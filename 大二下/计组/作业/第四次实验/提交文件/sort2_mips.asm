# sort2_mips.asm
# MIPS升序排序程序
# 原始数据：8,1,5,2,7,9,6,4,3,10
# 排序结果：1,2,3,4,5,6,7,8,9,10

main:
    # 初始化数据到数据存储器 0,4,8,...,36
    addi $t0, $zero, 8
    sw   $t0, 0($zero)
    addi $t0, $zero, 1
    sw   $t0, 4($zero)
    addi $t0, $zero, 5
    sw   $t0, 8($zero)
    addi $t0, $zero, 2
    sw   $t0, 12($zero)
    addi $t0, $zero, 7
    sw   $t0, 16($zero)
    addi $t0, $zero, 9
    sw   $t0, 20($zero)
    addi $t0, $zero, 6
    sw   $t0, 24($zero)
    addi $t0, $zero, 4
    sw   $t0, 28($zero)
    addi $t0, $zero, 3
    sw   $t0, 32($zero)
    addi $t0, $zero, 10
    sw   $t0, 36($zero)

    # 冒泡排序：升序
    addi $s0, $zero, 0      # i = 0
    addi $s2, $zero, 9      # 外层循环次数 9

outer_loop:
    slt  $t0, $s0, $s2      # i < 9 ?
    beq  $t0, $zero, finish

    addi $s1, $zero, 0      # j = 0
    sub  $s3, $s2, $s0      # 内层比较次数 = 9 - i

inner_loop:
    slt  $t0, $s1, $s3      # j < 9-i ?
    beq  $t0, $zero, next_outer

    sll  $t1, $s1, 2        # t1 = j * 4
    lw   $t2, 0($t1)        # 当前元素
    lw   $t3, 4($t1)        # 后一个元素

    # 升序：如果 当前元素 > 后一个元素，则交换
    slt  $t4, $t3, $t2
    beq  $t4, $zero, no_swap

    sw   $t3, 0($t1)
    sw   $t2, 4($t1)

no_swap:
    addi $s1, $s1, 1
    j    inner_loop

next_outer:
    addi $s0, $s0, 1
    j    outer_loop

finish:
    addi $v0, $zero, 10
    syscall
