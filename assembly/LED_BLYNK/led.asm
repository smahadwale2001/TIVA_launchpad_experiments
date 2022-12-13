;.global main;, SVC_Handler

main:
	b mov_ex;

mov_ex:
	mov R0, #23h;
here_m:
	b here_m;
