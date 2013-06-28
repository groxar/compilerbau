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
  MOVE $t1, $t0     # a = #0
  LI $t2, 13         # #1
  MOVE $t3, $t2     # b = #1
  ADD $t4, $t1, $t1
  ADD $t4, $t4, $t3
  MUL $t5, $t3, $t1
  ADD $t4, $t4, $t5
  MOVE $t3, $t4     # b = #2
  MOVE $t6, $t3     # i = b
  MOVE $v0, $t6
  LI $v0, 10
  SYSCALL

