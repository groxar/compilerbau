.data
tz:
  .word 0 : 20
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

  LI $t0, 2         # #0
  LI $t1, 4
  MUL $t1, $t1, $t0
  LA $t2, tz
  ADD $t1, $t2, $t1
  LW $t1, 0($t1)   # tz[#0]
  LI $t3, 23         # #2
  MOVE $t1, $t3     # #1 = #2
  LI $t4, 2         # #3
  LI $t5, 2         # #4
  LI $t6, 4
  MUL $t6, $t6, $t5
  ADD $t6, $t2, $t6
  LW $t6, 0($t6)   # tz[#4]
  ADD $t4, $t4, $t6
  LI $v0, 10
  SYSCALL

foo:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  MOVE $t7, $a0
  MOVE $t8, $a1
  MOVE $t9, $a2
  SUBI $sp, $sp, 4
  SW $t0, 0($sp)  #push #0 in memory
  MOVE $t0, $a3
  SUBI $sp, $sp, 4
  SW $t1, 0($sp)  #push #1 in memory
  ADD $t1, $t7, $t8
  ADD $t1, $t1, $t9
  ADD $t1, $t1, $t0
  MOVE $t8, $t1     # b = #6
  MOVE $v0, $t8
  LW $fp, 0($sp)
  LW $ra, 4($sp)
  ADDI $sp, $sp, 8
  JR $ra

