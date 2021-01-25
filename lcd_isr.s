
; From SameBoy
; Each line is 456 cycles. Without scrolling, sprites or a window:
; Mode 2 - 80  cycles / OAM Transfer
; Mode 3 - 172 cycles / Rendering
; Mode 0 - 204 cycles / HBlank
; 
; Mode 1 is VBlank

	.area	_HEADER_LCD (ABS)

	.org	0x48		; LCD (Install LCD handler at interrupt address)
.custom_int_LCD_vec:
	JP .custom_int_LCD  ; Call main LCD interrupt handler 

	.area	_BASE

.custom_int_LCD:
	PUSH	AF
;	PUSH	BC

	NOP
	NOP
	NOP
	NOP
	NOP



;	ld  a, #167
;	ldh	(_WX_REG+0),a  ; Move window off-screen to turn off

;	ld	a, #0x07
;	ld  a, #53
;	ld  a, (_LY_REG+0)
;	ldh	(_WX_REG+0),a  ; Move window to middle of screen

;	ld	a, #0xc1
;	ldh	(_LCDC_REG+0),a  ; Turn off window



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


	LD		A, #0xE1
	LDH		(_LCDC_REG+0),a  ; Turn BG/Window on


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

	ld	a, #0xE0
	LDH		(_LCDC_REG+0),a  ; Turn BG/Window Off

;1$:
;    ldh a, (_STAT_REG+0)
;	bit  1,a
;	jr nz, 1$
;     and #0x03
;	    jr nz, 1$

;	ld	a, #0xE0
;	RES		0, A
;	LDH		(_LCDC_REG+0),a  ; Turn BG/Window Off


;	ld	a, #0xE1
;	ldh	(_LCDC_REG+0),a  ; Turn window back on

;	SET		5, A
;	ldh	(_LCDC_REG+0),a  ; Turn window back on

;	ld  a, #106
;	ldh	(_WX_REG+0),a  ; Move window to right edge of screen


;1$:
;    ldh a, (_STAT_REG+0)
;	bit  1,a
;	jr nz, 1$
;     and #0x03
;	    jr nz, 1$

;	LD		A, #0xE1
;	LDH		(_LCDC_REG+0),a  ; Turn BG/Window off

;	POP		BC
	POP		AF
	RETI


