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
  SW $31, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  SUBI $sp, $sp, 4
  LI $5, 0
  SW $5, 0($sp)

  SUBI $sp, $sp, 4
  LI $5, 0
  SW $5, 0($sp)

OP
	if #0 == 0 goto .l2
	.l0
	#1 = arr[ i ]
OP
	if #1 == 0 goto .l1
  SUBI $sp, $sp, 4
  LI $5, 1
  SW $5, 0($sp)
OP

	.l1
  SUBI $sp, $sp, 4
  LI $5, 1
  SW $5, 0($sp)
OP

	callN 
	.l2
	if #0 != 0 goto .l0
	return sum
main:
  SUBI $sp, $sp, 8
  SW $31, 4($sp)
  SW $fp, 0($sp)
  MOVE $fp, $sp

  SUBI $sp, $sp, 4
  LI $5, 1
  SW $5, 0($sp)

  SUBI $sp, $sp, 4
  LI $5, 5
  SW $5, 0($sp)
  SUBI $sp, $sp, 4
  LI $5, 5
  SW $5, 0($sp)
OP
  SUBI $sp, $sp, 4
  LI $5, 5
  SW $5, 0($sp)
	callN 
	return #6
