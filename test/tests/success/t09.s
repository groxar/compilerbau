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
.l0:
  LI $t2, 2         # #1
  SLL $t2, $t1, $t2
  MOVE $t1, $t2     # i = #1
  LI $t3, 1024         # #2
  SLT $t3, $t1, $t3
  LI $t4, 512         # #3
  SGT $t4, $t1, $t4
  AND $t3, $t3, $t4
  BNEZ $t3, .l0
  MOVE $v0, $t1
  LI $v0, 10
  SYSCALL

