MAIN: blt $5,$26, loop
  .asciz "abcD00&\"	
 .dh 4,6,-5,1	
loop: jmp MAIN
 .extern label
.entry loop	
 la label
koop: .db -9,	55,0,77,-6,+9,7
bne $9,$6, koop
 call label
add $5,$8          ,$10
	move $8,$5
jmp loop
;not important

ori $9,-45,$6
la label
stop
