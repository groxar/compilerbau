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
#Function Prolog  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  LI $a0, 0
  MOVE $a1, $a0
  LI $a2, 0
  MOVE $a3, $a2
  SUBI $sp, $sp, 4
  SW $a0, 0($sp)  #push reg in memory
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)  #push reg in memory
  SLT $a1, $a1, $a0
  BEQZ $a1, .l2
 .l0
	#3 = arr[ i ]
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)  #push reg in memory
  SUBI $sp, $sp, 4
  SW $a3, 0($sp)  #push reg in memory
  SLE $a3, $a3, $a2
  BEQZ $a3, .l1
  SUBI $sp, $sp, 4
  SW $a0, 0($sp)  #push reg in memory
  LI $a0, 1
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)  #push reg in memory
  LW $a1, 4($sp) #load variable from mem in reg
  ADD $a0, $a1, $a0
  MOVE $a1, $a0
 .l1
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)  #push reg in memory
  LI $a2, 1
  SUBI $sp, $sp, 4
  SW $a3, 0($sp)  #push reg in memory
  LW $a3, 4($sp) #load variable from mem in reg
  ADD $a2, $a3, $a2
  MOVE $a3, $a2
  JAL print
 .l2
  SUBI $sp, $sp, 4
  SW $a0, 0($sp)  #push reg in memory
  LW $a0, 4($sp) #load variable from mem in reg
  BNEZ $a0, .l0
  MOVE $v0, $1
  JR $ra

main:
#Function Prolog  SUBI $sp, $sp, 8
  SW $ra, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  SW $a1, 24($sp) #store changes in memory
  LI $a1, 1
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)  #push reg in memory
  MOVE $a2, $a1
  SW $a3, 36($sp) #store changes in memory
  LI $a3, 5
  SW $a0, 20($sp) #store changes in memory
  LI $a0, 5
  ADD $a3, $a3, $a0
  SUBI $sp, $sp, 4
  SW $a1, 0($sp)  #push reg in memory
  LI $a1, 5
  SUBI $sp, $sp, 4
  SW $a2, 0($sp)  #push reg in memory
  JAL func
  SUBI $sp, $sp, 4
  SW $a3, 0($sp)  #push reg in memory
  MOVE $v0, $3
  JR $ra

