        ;; 6502 test assembly file

fred	equ 99
tom	equ 100
dick	= 27
	
	org $8000

main:	jmp start
	dc.b	'Hello', 0

start:	brk

