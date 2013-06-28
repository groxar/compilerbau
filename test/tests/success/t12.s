.data

.text

_start:
  JAL main

add:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  MOVE $t0, $a0
  MOVE $t1, $a1
  ADD $t2, $t0, $t1
  MOVE $v0, $t2
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

  LI $t3, 1         # #1
  LI $t4, 2         # #2
  MOVE $a0, $t3 #para 0
  MOVE $a1, $t4 #para 1
  SUBI $sp, $sp, 4
  SW $t0, 0($sp)  #push i in memory
  SUBI $sp, $sp, 4
  SW $t1, 0($sp)  #push j in memory
  SUBI $sp, $sp, 4
  SW $t2, 0($sp)  #push #0 in memory
  SUBI $sp, $sp, 4
  SW $t3, 0($sp)  #push #1 in memory
  SUBI $sp, $sp, 4
  SW $t4, 0($sp)  #push #2 in memory
  JAL add
  MOVE $t5, $v0 #return value
  MOVE $v0, $t5
  LI $v0, 10
  SYSCALL

