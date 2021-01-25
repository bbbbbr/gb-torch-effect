; Modified verLCDn os serial.s from GBDK 2020 4.0
;
; Does not include default serial_IO handler that is not
; possible to remove with stock GBDK 2020 4.0
;
; Standard GBDK 4.0 Serial Link / LCD handler will not be
; linked unless any of it's functions are called/used.


	.globl	.int

	.area	_HEADER_LCD (ABS)

	.org	0x48		; LCD (Install LCD handler at interrupt address)
.custom_int_LCD_vec:
	JP .custom_int_LCD  ; Call main LCD interrupt handler 
;	PUSH	AF
;	PUSH	HL
;	LD	HL,#.custom_int_0x58   ; Load LCD chained interrupt table
;	JP	.int            ; Call main interrupt handler


;	.area   _GSINIT

	;; Clear LCD global variables
;	XOR A
;	LD  HL,#.start_custom_LCD_globals
;	LD  C,#(.end_custom_LCD_globals - .start_custom_LCD_globals)
;	RST 0x28


	.area	_BASE

.custom_int_LCD:
	PUSH	AF
	PUSH	HL
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	LD		A, #0xC1
	LDH		(_LCDC_REG+0),a  ; Turn BG/Window off
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	RES		0, A
;	SET		0, A
	LDH		(_LCDC_REG+0),a  ; Turn BG/Window back on
	POP		HL
	POP		AF
	RETI


