.data
global:
  .word 0
  .align 4
a:
  .word 0 : 10
  .align 4

.text

_start:
  JAL main

func:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  MOVE $t0, $a0
  MOVE $t1, $a1
  MOVE $t2, $a2
  LI $t3, 0         # #0
  MOVE $t4, $t3     # i = #0
  LI $t5, 0         # #1
  MOVE $t6, $t5     # sum = #1
  SLT $t7, $t4, $t1
  BEQZ $t7, .l2
.l0:
  LI $t8, 4
  MUL $t8, $t8, $t4
  ADD $t8, $t0, $t8
  LW $t8, 0($t8)   # arr[i]
  SLE $t8, $t8, $t2
  BEQZ $t8, .l1
  LI $t9, 1         # #4
  ADD $t9, $t6, $t9
  MOVE $t6, $t9     # sum = #4
.l1:
  SUBI $sp, $sp, 4
  SW $t0, 0($sp)  #push arr in memory
  LI $t0, 1         # #5
  ADD $t0, $t4, $t0
  MOVE $t4, $t0     # i = #5
.l2:
  BNEZ $t7, .l0
  MOVE $v0, $t6
  LW $fp, 0($sp)
  LW $ra, 4($sp)
  ADDI $sp, $sp, 8
  JR $ra

main:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  SUBI $sp, $sp, 4
  SW $t1, 0($sp)  #push len in memory
  LI $t1, 1         # #6
  LA $t2, global
  SUBI $sp, $sp, 4
  SW $t2, 0($sp)  #push threshold in memory
  MOVE $t2, $t1     # global = #6
  SUBI $sp, $sp, 4
  SW $t3, 0($sp)  #push #0 in memory
  LI $t3, 5         # #7
  SUBI $sp, $sp, 4
  SW $t4, 0($sp)  #push i in memory
  LI $t4, 5         # #8
  ADD $t3, $t3, $t4
  SUBI $sp, $sp, 4
  SW $t5, 0($sp)  #push #1 in memory
  LI $t5, 5         # #9
  LA $t6, a
  SUBI $sp, $sp, 4
  SW $t6, 0($sp)  #push sum in memory
  MOVE $a0, $t6 #para 0
  MOVE $a1, $t3 #para 1
  MOVE $a2, $t5 #para 2
  SUBI $sp, $sp, 4
  SW $t0, 0($sp)  #push #5 in memory
  SUBI $sp, $sp, 4
  SW $t1, 0($sp)  #push #6 in memory
  SUBI $sp, $sp, 4
  SW $t2, 0($sp)  #push global in memory
  SUBI $sp, $sp, 4
  SW $t3, 0($sp)  #push #7 in memory
  SUBI $sp, $sp, 4
  SW $t4, 0($sp)  #push #8 in memory
  SUBI $sp, $sp, 4
  SW $t5, 0($sp)  #push #9 in memory
  SUBI $sp, $sp, 4
  SW $t6, 0($sp)  #push a in memory
  SUBI $sp, $sp, 4
  SW $t7, 0($sp)  #push #2 in memory
  SUBI $sp, $sp, 4
  SW $t8, 0($sp)  #push #3 in memory
  SUBI $sp, $sp, 4
  SW $t9, 0($sp)  #push #4 in memory
  JAL func
  MOVE $t7, $v0 #return value
  MOVE $v0, $t7
  LI $v0, 10
  SYSCALL

