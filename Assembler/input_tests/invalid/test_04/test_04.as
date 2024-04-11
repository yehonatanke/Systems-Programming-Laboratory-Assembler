.data 3,3,3,4,r,r,r
move 1 1 1 1
.entry ANOTHER_LIST2
.extern Y
.define sz=4
.define qw = 9

BEGINNING:	mov r4, ANOTHER_LIST2[sz]  
    	mov r3,r1
	mov #+3,   Y[5]   
	mov     #-3333, ANOTHER_LIST2[sz]  
	mov r4, ANOTHER_LIST2[3]  
	mov r4, ANOTHER_LIST2[2] 
	mov #1011 , ANOTHER_LIST2[2]
LOOP2:	jmp Y
	mcr  m_mcr2
		cmp K,#sz
		bne Y
	endmcr 
		prn #-7
		prn r6
		mov STR[7],STR[4]
		sub r4,r6 
	m_mcr2  
L3:     inc L5
.entry  LOOP2
		bne LOOP2
		mov #-3  ,  r6
		cmp  #qw		,   #89
		add #-300,r4
		sub #3,STR[qw]
		not r2
		clr  r4
		inc r4 
		dec r4 
		jmp END2
		bne  LOOP2
		red  r6
		prn  r2
		jsr r5
		rts 
		hlt
		
		mov  #18 ,Y
		add  #qw ,STR[7]
		sub  #-3 ,r4
		mov  ANOTHER_LIST2,Y
		add  ANOTHER_LIST2,STR[7]
		sub  Y,r4
		mov  STR[5],Y
		add  STR[7],STR[7]
		sub  STR[sz],r4
		mov   r3,Y
		add   r4,STR[7]
		sub   r1,r4

		cmp  #18 ,Y
		cmp  #qw ,STR[7]
		cmp  #-3 ,r4
		cmp  #-1 ,#sz
		cmp  ANOTHER_LIST2,Y
		cmp  ANOTHER_LIST2,STR[7]
		cmp  Y,r4
		cmp  Y,#-567
		cmp  STR[5],Y
		cmp  STR[7],STR[7]
		cmp  STR[sz],r4
		cmp  STR[sz],#3072
		cmp   r3,Y
		cmp   r4,STR[7]
		cmp   r1,r4
		cmp   r1,#3333


		not  Y
		clr  STR[5]
		inc  r5
		dec  r4

		lea  STR ,Y
		lea  STR ,STR[7]
		lea  STR ,r4
		lea  STR[sz] ,Y
		lea  STR[7] ,STR[7]
		lea  STR[7],r4

		jmp Y
		bne r5

		prn   Y
		prn   STR[7]
		prn   r4
		prn   #3333


		lea LOOP2 , r1
END2:    hlt
.define  len2 = 6
STR:	.string "mnopqr"
ANOTHER_LIST2:	.data 8,-7, len2
LIST3:	.data len2,-7, len2, sz
K:		.data 44
.extern L5
