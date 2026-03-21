.globl main

.data
source:
    .word   3
    .word   1
    .word   4
    .word   1
    .word   5
    .word   9
    .word   0
dest:
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0
    .word   0

.text
fun:
    addi t0, a0, 1 # t0 = a0 +1
    sub t1, x0, a0 # t1 = -a0
    mul a0, t0, t1 # a0 = t0 * t1
    jr ra          #返回地址

main:
    # BEGIN PROLOGUE
    addi sp, sp, -20 #栈
    sw s0, 0(sp)     #
    sw s1, 4(sp)     #
    sw s2, 8(sp)     #
    sw s3, 12(sp)    #
    sw ra, 16(sp)    #
    # END PROLOGUE
    addi t0, x0, 0 # t0 = 0 k
    addi s0, x0, 0 # s0 = 0 sum
    la s1, source  # s1 = &source
    la s2, dest    # s2 = &dest
loop:
    slli s3, t0, 2   # s3 = t0 << 2 保证每次移动是4的倍数，指针k的位置
    add t1, s1, s3   # t1 = s1 + s3，
    lw t2, 0(t1)     # t2 = t1[0] source[k]
    beq t2, x0, exit # t2 == x0 退出
    add a0, x0, t2   # a0 = x0 + t2
    addi sp, sp, -8  # 栈分出空间
    sw t0, 0(sp)     # 函数调用前保存 t0(k) 和 t2(source[k])
    sw t2, 4(sp)     #
    jal fun
    lw t0, 0(sp)     # 函数调用后返回 t0 和 t2
    lw t2, 4(sp)     
    addi sp, sp, 8  # 栈复原
    add t2, x0, a0  # t2 = x0 + a0 
    add t3, s2, s3  # t3 = s2 + s3
    sw t2, 0(t3)    # t3[0] = t2
    add s0, s0, t2  #  s0 += t2 
    addi t0, t0, 1  #  t0 += 1 
    jal x0, loop    # 回到函数开始
exit:
    add a0, x0, s0
    # BEGIN EPILOGUE
    lw s0, 0(sp)
    lw s1, 4(sp)
    lw s2, 8(sp)
    lw s3, 12(sp)
    lw ra, 16(sp)
    addi sp, sp, 20
    # END EPILOGUE
    jr ra
