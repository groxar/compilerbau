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

  LI $t0, 1         # #0
  MOVE $t1, $t0     # b = #0
  MOVE $t2, $t1     # a = b
  LI $t3, 1         # #1
  SEQ $t3, $t2, $t3
  LI $t4, 1         # #2
  SEQ $t4, $t1, $t4
  OR  $t3, $t3, $t4
  MOVE $t5, $t3     # i = #1
  MOVE $v0, $t5
  LI $v0, 10
  SYSCALL

