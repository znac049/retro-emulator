        ;; 6502 test assembly file

	org $8000

main:	jmp start
	dc.b	'Hello', 0

start:	brk

