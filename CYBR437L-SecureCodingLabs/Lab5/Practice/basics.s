.text
.globl main

main:
	movl $5, %eax # put 5 into eax
	addl $3, %eax # add 3 to 5 = 8
	subl $2, %eax # subtract: 8 - 2 = 6
	ret	      # return with result in eax

.section .note.GNU-stack,"",@progbits
