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
  LI $t2, 10         # #1
  SLT $t2, $t1, $t2
  BEQZ $t2, .l1
.l0:
  LI $t3, 1         # #2
  ADD $t3, $t1, $t3
  MOVE $t1, $t3     # i = #2
.l1:
  BNEZ $t2, .l0
  LI $t4, 0         # #3
  MOVE $t1, $t4     # i = #3
  MOVE $v0, $t1
  LI $v0, 10
  SYSCALL

