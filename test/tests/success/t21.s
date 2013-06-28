.data
b:
  .word 0
  .align 4

.text

_start:
  JAL main

main:
#Function Prolog  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  LI $a0, 4
  MOVE $a1, $a0
  LI $a2, 12
  MOVE $a3, $a2
  MOVE $a3, $a1
