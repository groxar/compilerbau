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
  BEQZ $t1, .l0
  LI $t2, 12         # #1
  MOVE $t1, $t2     # i = #1
  B .l2
.l0:
  LI $t3, 10         # #2
  SEQ $t3, $t1, $t3
  SEQ $t4, $t4, $t3
  BEQZ $t4, .l1
  LI $t5, 0         # #4
  MOVE $t1, $t5     # i = #4
.l1:
.l2:
  MOVE $v0, $t1
  LI $v0, 10
  SYSCALL

