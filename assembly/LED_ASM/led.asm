	.global main
	.thumb
	.text

main:
	b SRAM_CONF


MOV_EX:
	MOVW R0,#0E060h; CLOCK LOWER NIBBLE
	MOVT R0,#0400Fh; CLOCK UPPER NIBBLE
	MOVW R1,#00540h;
	MOVT R1,#00240h;
	MOV R0,#R1;

SRAM_CONF:
	LDR R0,RCC_ADDR ; LOAD RCC Register Address
	LDR R1,RCC_VAL  ; LOAD RCC Register Value for 40MHz EXT Oscillator
	STR R1,[R0]		; LOAD RCC register value to address
	LDR R0,SYSCTL_RCGCGPIO			;SET THE RCGC Register Address
	LDR R1,SYSCTL_RCGCGPIO_PORTF	;Set the RCGC register for clock gating of port f peripheral
	STR R1,[R0]		; Load value to Address
	LDR R0,PORTDIR	; SET R0 to GPIODIR of PORTF
	LDR R1,DIRPIN	; SET R1 to PIN1|PIN2|PIN3
	STR R1,[R0]		; SET GPIODIR
	LDR R0,DRIVE8R	; SET R0 for 8ma drive
	LDR R1,DIRPIN	; SET R1 to PIN1|PIN2|PIN3
	STR R1,[R0]		; SET Register to appropriate value
	LDR R0,DEN		; SET R0 for GPIODEN Address
	LDR R1,DIRPIN	; SET R1 to PIN1|PIN2|PIN3
	STR R1,[R0]		; SET GPIODEN Register to appropriate value
	LDR R0,PORTF	; LOAD PORTF Address
	LSL R2,R1,#2	; Shift by 2 for ADDR
	ADD R3,R2,R0	; PORTF GPIODATA
	MOV R4,#00h		; FOR LED OFF

led_blynk:
	STR R1,[R3]
	LDR R0,DELAY_TIME
	bl sDelay
	STR R4,[R3]
	LDR R0,DELAY_TIME
	bl sDelay
	b led_blynk


here_loop:
	b here_loop

sDelay:
	SUBS R0,#1
	BNE sDelay
	BX LR


RCC_ADDR:
	.word 0x400FE060
RCC_VAL:
	.word 0x02400540
SYSCTL_RCGCGPIO:
	.word 0x400FE608
SYSCTL_RCGCGPIO_PORTF:
	.word 0x00000020
PORTDIR:
	.word 0x40025400
DRIVE8R:
	.word 0x40025508
DEN:
	.word 0x4002551C
PORTF:
	.word 0x40025000
DIRPIN:
	.word 0x0000000E ;GPIO PIN1,PIN2,PIN3
DELAY_TIME:
	.word 0x0065B9AB;0x00000004;
	.end

	;SYSCTL_RCC_R -> 0x400FE060
	;0x02400540
	;SYSCTL_RCGCGPIO_R -> 0x400FE608
	;SYSCTL_RCGCGPIO_R5      0x00000020

	;
