
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



; Must make the update in mode 2?
;  if mode 3 hits before its updated then it doesn't take effect until...
;
;

.custom_int_LCD:
	PUSH	AF
	PUSH	BC

lcd_line_handle$:							

wait_mode_23$:
    ldh	a, (_STAT_REG+0)
	bit	1, a
	jr	z, wait_mode_23$		; Wait until mode 2/3   <-- optimize this (wait till mode 2 at least)
	bit	0, a
	jr	z, wait_mode_23$		; Wait until mode 2/3    <-- wait till mode 3 at least

	LD		A, #0xE0
	LDH		(_LCDC_REG+0),a ; BG & Window = ON

wait_mode_10$:
    ldh	a, (_STAT_REG+0)
	bit	1, a
	jr	nz, wait_mode_10$		; Wait until mode 01


wait_mode_23_b$:
    ldh	a, (_STAT_REG+0)
	bit	1, a
	jr	z, wait_mode_23_b$		; Wait until mode 2/3


	LD		A, #0xE1
	LDH		(_LCDC_REG+0),a ; Turn BG & Window = ON


wait_mode_10_b$:
    ldh	a, (_STAT_REG+0)
	bit	1, a
	jr	nz, wait_mode_10_b$		; Wait until mode 01



	ldh	a, (_LY_REG+0)
	sub	a, #96
	jp  nc, lcd_isr_exit$		; Exit after ending line reached
	
	jp lcd_line_handle$


lcd_isr_exit$:

	LD		A, #0xE1
	LDH		(_LCDC_REG+0),a ; Turn BG & Window = ON

	POP		BC
	POP		AF
	RETI

