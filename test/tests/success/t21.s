.data
b:
  .word 0
  .align 4

.text

_start:
  JAL main

main:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  LI $t0, 2         ##0
  LI $t1, 1         ##1
  LI $t2, 1         ##2
  LI $t3, 1         ##3
  LI $t4, 1         ##4
  MOVE $a0, $t1 #para 0
  MOVE $a1, $t2 #para 1
  MOVE $a2, $t3 #para 2
  MOVE $a3, $t4 #para 3
  SUBI $sp, $sp, 4
  SW $t0, 0($sp)  #push #0 in memory
  SUBI $sp, $sp, 4
  SW $t1, 0($sp)  #push #1 in memory
  SUBI $sp, $sp, 4
  SW $t2, 0($sp)  #push #2 in memory
  SUBI $sp, $sp, 4
  SW $t3, 0($sp)  #push #3 in memory
  SUBI $sp, $sp, 4
  SW $t4, 0($sp)  #push #4 in memory
  JAL foo
  MOVE $t5, $v0 #return value
  LW $t6, 16($sp)  #load #0 from memory in reg
  ADD $t6, $t6, $t5
  MOVE $t7, $t6
  LI $v0, 10
  SYSCALL

foo:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  MOVE $t8, $a0
  MOVE $t9, $a1
  MOVE $t0, $a2
  MOVE $t1, $a3
  ADD $t2, $t8, $t9
  ADD $t2, $t2, $t0
  ADD $t2, $t2, $t1
  MOVE $t9, $t2
  MOVE $v0, $t9
  LW $fp, 0($sp)
  LW $ra, 4($sp)
  ADDI $sp, $sp, 8
  JR $ra

