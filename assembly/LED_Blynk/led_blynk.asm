.global main

main:
	b mov_ex;

mov_ex:
	mov R0,#23;
here_m:
	b here_m;
