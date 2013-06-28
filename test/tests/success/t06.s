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
  LI $t1, 4
  MUL $t1, $t1, $t0
  ADD $t1, $t2, $t1
  LW $t1, 0($t1)   # global[#0]
  LI $t3, 0         # #2
  MOVE $t1, $t3     # #1 = #2
  LI $t4, 0         # #3
  MOVE $t5, $t4     # i = #3
  LI $t6, 10         # #4
  SLT $t6, $t5, $t6
  BEQZ $t6, .l1
.l0:
  LI $t7, 4
  MUL $t7, $t7, $t5
  ADD $t7, $t2, $t7
  LW $t7, 0($t7)   # global[i]
  MOVE $t7, $t5     # #5 = i
  LI $t8, 1         # #6
  ADD $t8, $t5, $t8
  MOVE $t5, $t8     # i = #6
.l1:
  BNEZ $t6, .l0
  LI $t9, 12         # #7
  ADD $t9, $t5, $t9
  MOVE $v0, $t9
  LI $v0, 10
  SYSCALL

