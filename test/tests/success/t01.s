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
  ADDI $sp, $sp, -8
  LI $5, 0
  SW $5, 4($sp)
  LI $5, 0
  SW $5, 0($sp)
	#c0 = 0
	i = #c0
	#c1 = 0
	sum = #c1
	#0 = i < len
	if #0 == 0 goto .l2
	.l0
	#1 = arr[ i ]
	#1 = #1 <= threshold
	if #1 == 0 goto .l1
	#c2 = 1
	#c2 = sum + #c2
	sum = #c2
	.l1
	#c3 = 1
	#c3 = i + #c3
	i = #c3
	callN 
	.l2
	if #0 != 0 goto .l0
	return sum
main:
  ADDI $sp, $sp, -8
  LI $5, 0
  SW $5, 4($sp)
  LI $5, 0
  SW $5, 0($sp)
	#c4 = 1
	global = #c4
	#c5 = 5
	#c6 = 5
	#c5 = #c5 + #c6
	#c7 = 5
	callN 
	return #3
