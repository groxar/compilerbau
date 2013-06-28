.data

.text

_start:
  JAL main

print:
#Function Prolog
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  MOVE $t0, $a0
  LI $t1, 2         # #0
  MUL $t1, $t1, $t0
  MOVE $v0, $t1
  LW $fp, 0($sp)
  LW $ra, 4($sp)
  ADDI $sp, $sp, 8
  JR $ra

