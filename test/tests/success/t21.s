.data

.text

_start:
  JAL main

main:
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  LI $a0, 2
  LI $a1, 2
  ADD $a0, $a0, $a1
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 2
  ADD $a0, $a0, $a1
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 3
  REM $a0, $a0, $a1
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 1
  ADD $a0, $a0, $a1
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 6
  MUL $a0, $a0, $a1
