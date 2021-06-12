; From SameBoy
; Each line is 456 cycles. Without scrolling, sprites or a window:
; Mode 2 - 80  cycles / OAM Transfer
; Mode 3 - 172 cycles / Rendering
; Mode 0 - 204 cycles / HBlank
; 
; Mode 1 is VBlank

	.area	_HEADER_LCD (ABS)

	.org	0x48			; LCD (Install LCD handler at interrupt address)
.custom_int_LCD_vec:
	JP .custom_int_LCD  	; Call main LCD interrupt handler 

	.area	_BASE



.custom_int_LCD:
	PUSH	AF  ; 2
	PUSH	BC  ; 2
	PUSH	HL  ; 2
							; == Prep for WX Right-side Rounded Window Update
							; Loads address at *p_x_end in BC
	ld	hl, #_p_x_end+0
	ld	c, (hl)
	inc hl
	ld	b, (hl)
	ld	a, (bc)
								
	ldh	(_WX_REG+0), a		; == Load Window location for first line
	inc	c        			; Move pointer to next LUT entry (LUT is 128 byte aligned, don't need a 16 bit add

							; Then This is performed below for each scanline + inc c
							; 	ld	a, (bc)
							;	ldh	(_WX_REG+0),a   
							; 	inc	c


							; Try to line main loop up with start of Mode 2 (Search Objs)

							; Per-Scanline Modes: 2 (Search Objs) -> 3 (Render) -> 0 (H-Blank)
							;
							; == Since end of Mode 3 -> 0 timing can fluctuate with
							;    extra cycles when sprites are fetched, use the (hoepfully)
							;    more consistant mode 0 -> 2 transition to align instead

	ld h, #0x03 			; Pre-load mask into A
wait_mode_10$:				; Wait for H-Blank (Mode 0)
    ldh	a, (_STAT_REG+0)
	and a, h				; Mask to 0x03
	jr	nz, wait_mode_10$	; Exit loop on Mode 3 (Render) -> 0 (H-Blank) transition

wait_mode_23$:
    ldh	a, (_STAT_REG+0)
	and a, h				; Mask to 0x03
	jr	z, wait_mode_23$	; Exit loop on Mode 0 (H-Blank) -> 2 (Search Objs) transition


							; Should now be ~ Mode 2 (Search Objs)
lcd_loop_start$:

	ld	a, #0xEF
	ldh	(_LCDC_REG+0),a		; Swap BG Tile Map to Alt
;;	ld	a, #0x00			; White
;	ld	a, #0xFF			; Black
;	ldh	(_BGP_REG+0),a		; Set Background Pal to new color at start of row

	nop
	nop
	nop
	nop
	nop


	nop
	nop
	nop
	nop
	nop						; ~ Mode 3 (Render) begins

	nop
	nop
	nop
	nop
	nop

	nop
							; One less NOP before the swap causes
							; a gap to show and requires another
							; column of masking sprites.
							;
							; One more NOP and the scx % 8 behavior
							; bumps out into the spotlight area.
							;
							; Timing appears a little more sensitive
							; on MGB hardware than CGB/AGS-CGB.

							; == End of left edge BG masking

	ld 	a, #0xE7
	ldh	(_LCDC_REG+0), a	; Swap BG Tile Map to Main
;	ld	a, #0xE4
;	ldh	(_BGP_REG+0), a		; Set Background Pal to Normal

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
	nop						; ~ Mode 0 (HBlank) begins
	nop
	nop
							; == WX Right-side Rounded Window Update
							;
							; (After exit test so that WX update should be in hblank)
							; Update WX each scanline from the circle LUT
							; BC is pre-loaded with the first LUT address on entering the ISR
							; 44 cycles total
	ld	a, (bc)  			; Load WX for current scanline from LUT (bc is pre-loaded)
	ldh	(_WX_REG+0), a
	inc	c        			; Move pointer to next LUT entry (LUT is 128 byte aligned, don't need a 16 bit add

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
	jp 	Z, lcd_isr_exit$	; Exit after ending Y line reached


	jp lcd_loop_start$		; Otherwise, stay in ISR for another line



lcd_isr_exit$:

	ld	A, #0xEF
	ldh	 (_LCDC_REG+0), a	; Swap BG Tile Map to Alt
;	ld	a, #0xFF
;	ldh	(_BGP_REG+0), a		; Set Background Pal to Normal

	POP		HL
	POP		BC
	POP		AF
	RETI



