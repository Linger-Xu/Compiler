.data
newline: .asciiz "\n"
.text
j label_12_main #main entry
#->procedure
Proc_com:
sw $ra, -4($t4) #填写返回地址
j com_begin #过程入口
com_begin:
#->assign
addi $t0, $zero, 36 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
move $t2, $t0 #save ac
addi $t0, $zero, 28 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($t2) #var assign : store value
#<- assign
#->while
label_0_While:
addi $t0, $zero, 36 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
addi $t0, $zero, 108 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 2 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
add $t0, $t0, $t1 #op +
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
addi $t0, $zero, 32 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
sub $t0, $t0, $t1 #op -
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
sub $t0, $t0, $t1 #op <
blt $t0, $0, label_1_true #br if true
li $t0, 0 #false case
j label_2_false #必定跳过下一条指令
label_1_true:
li $t0, 1 #true case
label_2_false:
beq $t0, $0, label_11_EndWhile # jump out while 
# while : jump out while 
#->assign
addi $t0, $zero, 112 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 1 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
add $t0, $t0, $t1 #op +
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
addi $t0, $zero, 32 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
sub $t0, $t0, $t1 #op -
addi $t1, $zero, 1 ##t1为数组下界
sub $t0, $t0, $t1 ##下标减去下界
li $t1, 4 #地址偏移乘4
mult $t0, $t1 #乘4的偏移
mflo $t0 #将低32为载入T0
addi $t0, $t0, 28 ##求出数组变量的偏移，即数组里的这个元素相对于基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
move $t2, $t0 #save ac
addi $t0, $zero, 36 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($t2) #var assign : store value
#<- assign
#->if
addi $t0, $zero, 32 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 2 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
sub $t0, $t0, $t1 #op <
blt $t0, $0, label_3_true #br if true
li $t0, 0 #false case
j label_4_false #必定跳过下一条指令
label_3_true:
li $t0, 1 #true case
label_4_false:
beq $t0, $0, label_9_Else #跳转到else分支
#->assign
addi $t0, $zero, 40 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
move $t2, $t0 #save ac
li $t0, 1 #载入常量到T0
sw $t0, 0($t2) #var assign : store value
#<- assign
#->while
label_5_While:
addi $t0, $zero, 40 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
addi $t0, $zero, 112 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 1 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
add $t0, $t0, $t1 #op +
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
sub $t0, $t0, $t1 #op <
blt $t0, $0, label_6_true #br if true
li $t0, 0 #false case
j label_7_false #必定跳过下一条指令
label_6_true:
li $t0, 1 #true case
label_7_false:
beq $t0, $0, label_8_EndWhile # jump out while 
# while : jump out while 
#-> write
addi $t0, $zero, 40 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
addi $t1, $zero, 1 ##t1为数组下界
sub $t0, $t0, $t1 ##下标减去下界
li $t1, 4 #地址偏移乘4
mult $t0, $t1 #乘4的偏移
mflo $t0 #将低32为载入T0
addi $t0, $t0, 28 ##求出数组变量的偏移，即数组里的这个元素相对于基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
li $v0,1
move $a0,$t0
syscall
li $v0, 4       # 设置系统调用编号为4（打印字符串）
la $a0, newline # 加载换行符的地址到$a0寄存器
syscall         # 调用系统调用
#<- write
#->assign
addi $t0, $zero, 40 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
move $t2, $t0 #save ac
addi $t0, $zero, 40 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 1 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
add $t0, $t0, $t1 #op +
sw $t0, 0($t2) #var assign : store value
#<- assign
j label_5_While #jump to start
# return to condition exp
label_8_EndWhile:
#<-while
j label_10_EndIf #jmp to end
label_9_Else:
#-------------------> Call Start
addi $t0, $zero, 36 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 1 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
add $t0, $t0, $t1 #op +
sw $t0, -28($sp) #为下一层存传入的参数
addi $t0, $zero, 32 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 1 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
sub $t0, $t0, $t1 #op -
sw $t0, -32($sp) #为下一层存传入的参数
sw $t4, 0($sp) #存储动态链条地址
li $t5, 1 #T5此时存层数
sw $t5, -8($t4) #T5存入活动记录
sw $t0, -12($sp) #存T0数值到栈
sw $t1, -16($sp) #存T1数值到栈
sw $t2, -20($sp) #存T2数值到栈
li $t5, 44 #T5此时displayoff表偏移
sw $t5, -24($sp) #存displayOff到栈
sub $t5, $sp, $t5 #T5存display表绝对地址
move $t4, $sp #将新生成记录的栈底存储T4
addi $sp, $sp, -52 #申请ACSIZE大小的活动记录空间,SP指向栈顶


lw $t5, 0($t4) #动态链取上层记录初始地址
lw $t6, -24($t5) #取上层记录display表偏移地址
sub $t5, $t5, $t6 #T5存算出的上层display表绝对地址,T5=T5-T6
lw $t6, 0($t5) #T6此时上层display表中第i层内容
sw $t6, -44($t4) #这里T4是本层起始地址，从这计算的偏移,本层对应i层存入对应上层display表第i个数据
sw $t4, -48($t4) #还有本层起始地址也要填入display表
jal Proc_com #转向子程序
lw $t0, -12($t4) #恢复T0
lw $t1, -16($t4) #恢复T1
lw $t2, -20($t4) #恢复T2
addi $sp, $t4, 0 #恢复SP
lw $t4, 0($t4) #恢复T4
#<------------------- Call End
label_10_EndIf:
#<- if
#->assign
addi $t0, $zero, 36 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
move $t2, $t0 #save ac
addi $t0, $zero, 36 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 4 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, 0($sp) #存临时数据
li $t7, 4 #
sub $sp, $sp, $t7 #sp栈顶向下-4
li $t0, 1 #载入常量到T0
lw $t1, 4($sp) #载入数据
li $t7, 4 #
add $sp, $sp, $t7 #sp栈顶向上+4恢复
move $t7, $t1 #把t1放到t7里
move $t1, $t0 #把t0放到t1里
move $t0, $t7 #把t7放到t0里
add $t0, $t0, $t1 #op +
sw $t0, 0($t2) #var assign : store value
#<- assign
j label_0_While #jump to start
# return to condition exp
label_11_EndWhile:
#<-while
lw $t2, -4($t4) # fetch return address
jr $t2 #回到上一级过程
#<-procedure
label_12_main:
addi $t4, $sp, 0 #主过程活动记录生成！！！
li $t1, 116 #Main程序display偏移
sw $t1, -24($sp) #Main程序存displayoff偏移
sub $t1, $sp, $t1 #Main程序display表绝对偏移地址
sw $sp, 0($t1) #存储Main程序display表第一个元素，即他自己
addi $sp, $sp, -120 #申请Main程序栈空间
#-> read
li $v0,5
syscall
addi $t0, $zero, 108 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
li $t1, 4 #
sw $v0, 0($sp) #保存读入的值在栈中
sub $sp, $sp, $t1 #read语句开辟栈存储读入的值
lw $t1, 4($sp) # 栈中取read的数
sw $t1, 0($t0) # var read : store value
li $t1, 4 #
add $sp, $sp, $t1 #read语句开辟栈存储读入的值
#<- read
#-> read
li $v0,5
syscall
addi $t0, $zero, 112 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
li $t1, 4 #
sw $v0, 0($sp) #保存读入的值在栈中
sub $sp, $sp, $t1 #read语句开辟栈存储读入的值
lw $t1, 4($sp) # 栈中取read的数
sw $t1, 0($t0) # var read : store value
li $t1, 4 #
add $sp, $sp, $t1 #read语句开辟栈存储读入的值
#<- read
#-------------------> Call Start
li $t0, 1 #载入常量到T0
sw $t0, -28($sp) #为下一层存传入的参数
addi $t0, $zero, 112 ##t0里存相对于自己AR基址的偏移
add $t6, $t0, $zero ##先把t0的值传到t6里，防止后面冲突
lw $t0, -24($t4) ##把display偏移值放到t0里，4*6
addi $t1, $t0, 0 ##找到display里的记录相对于基址的偏移
sub $t1, $t4, $t1 ##求出记录的绝对偏移,栈是倒长的
lw $t0, 0($t1) ##取出记录里的内容
sub $t0, $t0, $t6 ##计算绝对偏移，t0里存的是绝对地址
lw $t0, 0($t0) #load id value
sw $t0, -32($sp) #为下一层存传入的参数
sw $t4, 0($sp) #存储动态链条地址
li $t5, 1 #T5此时存层数
sw $t5, -8($t4) #T5存入活动记录
sw $t0, -12($sp) #存T0数值到栈
sw $t1, -16($sp) #存T1数值到栈
sw $t2, -20($sp) #存T2数值到栈
li $t5, 44 #T5此时displayoff表偏移
sw $t5, -24($sp) #存displayOff到栈
sub $t5, $sp, $t5 #T5存display表绝对地址
move $t4, $sp #将新生成记录的栈底存储T4
addi $sp, $sp, -52 #申请ACSIZE大小的活动记录空间,SP指向栈顶


lw $t5, 0($t4) #动态链取上层记录初始地址
lw $t6, -24($t5) #取上层记录display表偏移地址
sub $t5, $t5, $t6 #T5存算出的上层display表绝对地址,T5=T5-T6
lw $t6, 0($t5) #T6此时上层display表中第i层内容
sw $t6, -44($t4) #这里T4是本层起始地址，从这计算的偏移,本层对应i层存入对应上层display表第i个数据
sw $t4, -48($t4) #还有本层起始地址也要填入display表
jal Proc_com #转向子程序
lw $t0, -12($t4) #恢复T0
lw $t1, -16($t4) #恢复T1
lw $t2, -20($t4) #恢复T2
addi $sp, $t4, 0 #恢复SP
lw $t4, 0($t4) #恢复T4
#<------------------- Call End
li $v0, 10 #退出程序
syscall
