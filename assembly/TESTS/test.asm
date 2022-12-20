	.global main
	.thumb
	.text

main:
	MOVW R0,#0000h;
	MOVT R0,#9000h;
	MOVW R1,#0000h;
	MOVT R1,#9000h;
	ADDS R2,R1,R0;

while:
	b while
