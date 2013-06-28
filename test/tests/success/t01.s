.data
global:
  .word 0
  .align 4
a:
  .word 0 : 10
  .align 4

.text

_start:
  JAL main

func:
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  LI $a0, 0
  MOVE $a2, $a0
  LI $a3, 0
  MOVE $a1, $a3
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)
  SUBI $sp, $sp, 4
  SW $a3, 0($sp)
  ADDI $sp, $sp, 4
  LW $a3, 0($sp)
  SUBI $sp, $sp, 4
  SUBI $sp, $sp, 4
  SW $a0, 0($sp)
  SLT $a0, $a3, $a2
  BEQZ $a0, .l2
 .l0
	#3 = arr[ i ]
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)
  ADDI $sp, $sp, 16
  SW $a3, 0($sp)
  SUBI $sp, $sp, 16
  SLE $a3, $a3, $a2
  BEQZ $a3, .l1
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)
  LI $a2, 1
  ADD $a2, $a1, $a2
  MOVE $a1, $a2
 .l1
  SUBI $sp, $sp, 4
  SW $a0, 0($sp)
  LI $a0, 1
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)
  ADDI $sp, $sp, 4
  LW $a2, 0($sp)
  SUBI $sp, $sp, 4
  ADD $a0, $a2, $a0
  MOVE $a2, $a0
  JAL print
 .l2
  SUBI $sp, $sp, 4
  SW $a3, 0($sp)
  ADDI $sp, $sp, 4
  LW $a3, 0($sp)
  SUBI $sp, $sp, 4
  BNEZ $a3, .l0
  MOVE $v0, $1
  JR $ra

main:
  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 1
  ADDI $sp, $sp, 16
  SW $a3, 0($sp)
  SUBI $sp, $sp, 16
  MOVE $a3, $a1
  SUBI $sp, $sp, 4
  SW $a0, 0($sp)
  LI $a0, 5
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 5
  ADD $a0, $a0, $a1
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  LI $a1, 5
  ADDI $sp, $sp, 48
  SW $a2, 0($sp)
  SUBI $sp, $sp, 48
  JAL func
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)
  MOVE $v0, $1
  JR $ra

