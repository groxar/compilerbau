.data

.text

_start:
  JAL main

main:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  LI $t0, 0         # #0
  MOVE $t1, $t0     # i = #0
  LI $t2, 0         # #1
  MOVE $t3, $t2     # j = #1
  LI $t4, 5         # #2
  SLT $t4, $t1, $t4
  BEQZ $t4, .l5
.l0:
  LI $t5, 0         # #3
  MOVE $t3, $t5     # j = #3
  LI $t6, 4         # #4
  SLT $t6, $t3, $t6
  BEQZ $t6, .l4
.l1:
  LI $t7, 1         # #5
  ADD $t7, $t3, $t7
  MOVE $t3, $t7     # j = #5
  LI $t8, 2         # #6
  SEQ $t8, $t3, $t8
  BEQZ $t8, .l2
  LI $t9, 1         # #7
  MOVE $t1, $t9     # i = #7
  B .l3
.l2:
  SUBI $sp, $sp, 4
  SW $t0, 0($sp)  #push #0 in memory
  LI $t0, 2         # #8
  MOVE $t1, $t0     # i = #8
.l3:
.l4:
  BNEZ $t6, .l1
  SUBI $sp, $sp, 4
  SW $t1, 0($sp)  #push i in memory
  LI $t1, 1         # #9
  SUBI $sp, $sp, 4
  SW $t2, 0($sp)  #push #1 in memory
  LW $t2, 4($sp)  #load i from memory in reg
  ADD $t1, $t2, $t1
  MOVE $t2, $t1     # i = #9
.l5:
  BNEZ $t4, .l0
  MOVE $v0, $t2
  LI $v0, 10
  SYSCALL

