;file ps.as
;sample source code

.entry Next
.extern wNumber
STR:	.asciz	"aBcd" ^
MAIN:	add	$3,$5,$9 
LOOP:	ori	$99,-5,$2
	la 	vall
	jmp	Next
Next:	move	$20,$4
LIST:	.db 	6,-9
	bgt	$4,$2,END
	la 	K
	sw	$0,4,$100000000000000000000000000000000000000000000000000000000000000000000000000000000000000
	bne 	$31,$9,wNumber 
	gall	vall
	jmp	$4
	la	wNumber
.extern		vall
	.dh	number
K:	.dw	31,-12
END:	stop 
.entry	K
