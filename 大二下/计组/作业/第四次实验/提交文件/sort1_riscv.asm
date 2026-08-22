# sort1_riscv.asm
# RISC-V 降序排序
# 原始数据：8,1,5,2,7,9,6,4,3,10
# 排序结果：10,9,8,7,6,5,4,3,2,1

main:
    # 初始化数据
    ori t0, zero, 8
    sw  t0, 0(zero)
    ori t0, zero, 1
    sw  t0, 4(zero)
    ori t0, zero, 5
    sw  t0, 8(zero)
    ori t0, zero, 2
    sw  t0, 12(zero)
    ori t0, zero, 7
    sw  t0, 16(zero)
    ori t0, zero, 9
    sw  t0, 20(zero)
    ori t0, zero, 6
    sw  t0, 24(zero)
    ori t0, zero, 4
    sw  t0, 28(zero)
    ori t0, zero, 3
    sw  t0, 32(zero)
    ori t0, zero, 10
    sw  t0, 36(zero)

    ori s0, zero, 0       # i = 0
    ori s2, zero, 9       # 外层循环次数

outer_loop:
    slt t0, s0, s2        # i < 9 ?
    beq t0, zero, finish

    ori s1, zero, 0       # j = 0
    sub s3, s2, s0        # 内层次数 = 9 - i

inner_loop:
    slt t0, s1, s3        # j < 9-i ?
    beq t0, zero, next_outer

    slli t1, s1, 2        # t1 = j * 4
    lw   t2, 0(t1)        # 当前元素
    lw   t3, 4(t1)        # 后一个元素

    # 降序：当前元素 < 后一个元素，则交换
    slt t4, t2, t3
    beq t4, zero, no_swap

    sw t3, 0(t1)
    sw t2, 4(t1)

no_swap:
    addi s1, s1, 1
    jal zero, inner_loop

next_outer:
    addi s0, s0, 1
    jal zero, outer_loop

finish:
    jal zero, finish