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

  LI $t0, 12         # #0
  MOVE $t1, $t0     # i = #0
  LI $t2, 12         # #1
  SEQ $t2, $t1, $t2
  LI $t3, 13         # #2
  SEQ $t3, $t1, $t3
  OR  $t2, $t2, $t3
  LI $t4, 15         # #3
  SGE $t4, $t1, $t4
  LI $t5, 17         # #4
  SLE $t5, $t1, $t5
  AND $t4, $t4, $t5
  OR  $t2, $t2, $t4
  BEQZ $t2, .l0
  LI $t6, 0         # #5
  MOVE $t1, $t6     # i = #5
  B .l1
.l0:
  LI $t7, 1         # #6
  MOVE $t1, $t7     # i = #6
.l1:
  MOVE $v0, $t1
  LI $v0, 10
  SYSCALL

