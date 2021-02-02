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
	PUSH	AF  ; 2
	PUSH	BC  ; 2
	PUSH	HL  ; 2

; == SCX offset
; * Even multiples of 8 = no correction
; * Every + 1 after starts -1 more left
;   Up to max of -7 left

;	ldh a, (_SCX_REG+0)
;	and a, #0x07
;	ld  hl, #jump_offset$
;wait_scx_offset$:
;	add a, (hl)
;	jp 	(hl)

;	nop
;	nop
;	nop
;	nop
;
;	nop
;	nop
;	nop
;	nop
;jump_offset$:

wait_mode_23$:
    ldh	a, (_STAT_REG+0)
	bit	1, a
	jr	z, wait_mode_23$		; Wait until mode 2/3   <-- optimize this (wait till mode 2 at least)

wait_mode_10$:
    ldh	a, (_STAT_REG+0)  ; 3
	bit	1, a
;	and a, #0x03
	jr	nz, wait_mode_10$		; On first pass -> Wait until mode 01


lcd_loop_start$:

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop


    nop
    nop
    nop
    nop
    nop

	nop
	nop
	nop
	nop
	nop

	LD		A, #0xE9
	LDH		(_LCDC_REG+0),a ; Swap BG Tile Map to Alt

	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop


	LD		A, #0xE1
	LDH		(_LCDC_REG+0),a ; Swap BG Tile Map to Main

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop
	nop
	nop

	nop
	nop
	nop
	nop
	nop

; == Test for Exit based on LY
	ldh	a, (_LY_REG+0)
	ld	hl, #_y_line_end
	sub	a, (hl)
	jp 	Z, lcd_isr_exit$			; Exit after ending line reached

	jp lcd_loop_start$			; No exit, stay in ISR another line



lcd_isr_exit$:

	LD		A, #0xE9
	LDH		(_LCDC_REG+0),a ; Swap BG Tile Map to Alt

	POP		HL
	POP		BC
	POP		AF
	RETI


